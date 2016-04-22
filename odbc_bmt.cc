#include <bmt.hpp>
#include <cassert>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

#define SUNDB_SQL_THROW( aLabel )               \
    goto aLabel;

#define SUNDB_SQL_TRY( aExpression )            \
    do                                          \
    {                                           \
        if( !(SQL_SUCCEEDED( aExpression ) ) )  \
        {                                       \
            goto SUNDB_FINISH_LABEL;            \
        }                                       \
    } while( 0 )


#define SUNDB_FINISH                           \
    goto SUNDB_FINISH_LABEL;                   \
    SUNDB_FINISH_LABEL:


#define INSERT_SQL (SQLCHAR*)"INSERT INTO TEST_01 VALUES (?,?,?,?,?,?,?,?,?,SYSDATE);"   
#define UPDATE_SQL (SQLCHAR*)"UPDATE TEST_01 SET c9 = c9 + 1 WHERE C1 = ?"   
#define DELETE_SQL  (SQLCHAR*)"DELETE FROM TEST_01 WHERE C1 = ? " 
#define SELECT_SQL (SQLCHAR*)"SELECT C1, C2, C3, C4, C5, C6, C7, C8 , C9 FROM TEST_01 WHERE C1 = ?"
                                     

static void PrintDiagnosticRecord( SQLSMALLINT aHandleType, SQLHANDLE aHandle )
{
    SQLCHAR       sSQLState[6];
    SQLINTEGER    sNaiveError;
    SQLSMALLINT   sTextLength;
    SQLCHAR       sMessageText[SQL_MAX_MESSAGE_LENGTH];
    SQLSMALLINT   sRecNumber = 1;
    SQLRETURN     sReturn;
    
    /* SQLGetDiagRec returns the currrent values that contains error, warning */
    while( 1 )
    {
        sReturn = SQLGetDiagRec( aHandleType,
                                 aHandle,
                                 sRecNumber,
                                 sSQLState,
                                 &sNaiveError,
                                 sMessageText,
                                 100,
                                 &sTextLength );

        if( sReturn == SQL_NO_DATA )
        {
            break;
        }

        SUNDB_SQL_TRY( sReturn );

        printf("\n=============================================\n" );
        printf("SQL_DIAG_SQLSTATE     : %s\n", sSQLState );
        printf("SQL_DIAG_NATIVE       : %d\n", sNaiveError );
        printf("SQL_DIAG_MESSAGE_TEXT : %s\n", sMessageText );
        printf("=============================================\n" );

        sRecNumber++;
    }

    return;

    SUNDB_FINISH;

    printf("SQLGetDiagRec failure.\n" );

    return;    
}


ODBCRunner :: ~ODBCRunner () {
 // ignore free handle... i think it would be ok because it is very shot term job. 
}

ODBCRunner :: ODBCRunner ( const string& aUser, 
                           const string& aPwd, 
                           const string& aDSN )  : mENV(NULL), mDBC (NULL) { 

  
  
  SQLRETURN   sRC = 0;
  
  // I think these apis never fail. 
  assert ( 0 == SQLAllocHandle ( SQL_HANDLE_ENV , NULL, &mENV ) )  ;
  assert ( 0 == SQLSetEnvAttr  ( mENV, SQL_ATTR_ODBC_VERSION , (SQLPOINTER)SQL_OV_ODBC3 , 0 ))  ; 
  assert ( 0 == SQLAllocHandle ( SQL_HANDLE_DBC, mENV, &mDBC )  ) ;
  
  sRC = SQLConnect ( mDBC, 
                    (SQLCHAR*)aDSN.c_str(), 
                    SQL_NTS, 
                    (SQLCHAR*)aUser.c_str(), 
                    SQL_NTS, 
                    (SQLCHAR*)aPwd.c_str(), 
                    SQL_NTS );

  if (sRC != SQL_SUCCESS ) { 
     PrintDiagnosticRecord ( SQL_HANDLE_DBC, mDBC ) ;
     exit (-1) ;
  } 

  // Prepare and bind for every statement 
  PrepareAllStatement () ;
}


void ODBCRunner :: PrepareAllStatement ( ) { 
   SQLINTEGER sRC ;

   // Get Statement Handle 
   assert ( 0 == SQLAllocHandle ( SQL_HANDLE_STMT, mDBC, &mInsertStatement )) ;
   assert ( 0 == SQLAllocHandle ( SQL_HANDLE_STMT, mDBC, &mSelectStatement )) ;
   assert ( 0 == SQLAllocHandle ( SQL_HANDLE_STMT, mDBC, &mUpdateStatement )) ;
   assert ( 0 == SQLAllocHandle ( SQL_HANDLE_STMT, mDBC, &mDeleteStatement )) ;
   
   // prepare insert statement 
   sRC = SQLPrepare (mInsertStatement, 
                     INSERT_SQL, 
                     SQL_NTS ) ;

   if ( sRC != SQL_SUCCESS ) { 
     PrintDiagnosticRecord ( SQL_HANDLE_STMT, mInsertStatement ) ;
     exit ( -1 ) ;
   } 

   // prepare select statement 

   sRC = SQLPrepare (mSelectStatement, 
                     SELECT_SQL, 
                     SQL_NTS ) ;

   if ( sRC != SQL_SUCCESS ) { 
     PrintDiagnosticRecord ( SQL_HANDLE_STMT, mSelectStatement ) ;
     exit ( -1 ) ;
   } 

   // prepare update statement 

   sRC = SQLPrepare (mUpdateStatement, 
                     UPDATE_SQL, 
                     SQL_NTS ) ;

   if ( sRC != SQL_SUCCESS ) { 
     PrintDiagnosticRecord ( SQL_HANDLE_STMT, mUpdateStatement ) ;
     exit ( -1 ) ;
   } 

   // prepare delete statement 

   sRC = SQLPrepare (mDeleteStatement, 
                     DELETE_SQL, 
                     SQL_NTS ) ;

   if ( sRC != SQL_SUCCESS ) { 
     PrintDiagnosticRecord ( SQL_HANDLE_STMT, mDeleteStatement ) ;
     exit ( -1 ) ;
   } 


                     
   /**************************************************************
    Bind Params for insert statment 
   **************************************************************/
   sRC = SQLBindParameter ( mInsertStatement,     // Statement Handle 
                            1,                    // Parameter Number  
                            SQL_PARAM_INPUT,      // InputOutputType 
                            SQL_C_SLONG,          // ValueType 
                            SQL_INTEGER,          // ParameterType
                            0,                    // ColumnSize 
                            0,                    // DecimalDigit 
                            &mRecord.c1,          // ParameterValuePtr
                            sizeof(mRecord.c1),   // BufferLength 
                            &mRecordInd.c1        // Indicator or strlen 
                            ) ;                
   assert ( sRC  == SQL_SUCCESS ) ;

   sRC = SQLBindParameter ( mInsertStatement,     // Statement Handle 
                            2,                    // Parameter Number  
                            SQL_PARAM_INPUT,      // InputOutputType 
                            SQL_C_CHAR,           // ValueType 
                            SQL_VARCHAR,          // ParameterType
                            100,                  // ColumnSize 
                            0,                    // DecimalDigit 
                            &mRecord.c2,          // ParameterValuePtr
                            sizeof(mRecord.c2),   // BufferLength 
                            &mRecordInd.c2        // Indicator or strlen 
                            ) ;                
   assert ( sRC  == SQL_SUCCESS ) ;

   sRC = SQLBindParameter ( mInsertStatement,     // Statement Handle 
                            3,                    // Parameter Number  
                            SQL_PARAM_INPUT,      // InputOutputType 
                            SQL_C_CHAR,           // ValueType 
                            SQL_VARCHAR,          // ParameterType
                            100,                  // ColumnSize 
                            0,                    // DecimalDigit 
                            &mRecord.c3,          // ParameterValuePtr
                            sizeof(mRecord.c3),   // BufferLength 
                            &mRecordInd.c3        // Indicator or strlen 
                            ) ;                
   assert ( sRC  == SQL_SUCCESS ) ;

   sRC = SQLBindParameter ( mInsertStatement,     // Statement Handle 
                            4,                    // Parameter Number  
                            SQL_PARAM_INPUT,      // InputOutputType 
                            SQL_C_CHAR,           // ValueType 
                            SQL_VARCHAR,          // ParameterType
                            100,                  // ColumnSize 
                            0,                    // DecimalDigit 
                            &mRecord.c4,          // ParameterValuePtr
                            sizeof(mRecord.c4),   // BufferLength 
                            &mRecordInd.c4        // Indicator or strlen 
                            ) ;                
   assert ( sRC  == SQL_SUCCESS ) ;

   sRC = SQLBindParameter ( mInsertStatement,     // Statement Handle 
                            5,                    // Parameter Number  
                            SQL_PARAM_INPUT,      // InputOutputType 
                            SQL_C_CHAR,           // ValueType 
                            SQL_VARCHAR,          // ParameterType
                            100,                  // ColumnSize 
                            0,                    // DecimalDigit 
                            &mRecord.c5,          // ParameterValuePtr
                            sizeof(mRecord.c5),   // BufferLength 
                            &mRecordInd.c5        // Indicator or strlen 
                            ) ;                
   assert ( sRC  == SQL_SUCCESS ) ;


   sRC = SQLBindParameter ( mInsertStatement,     // Statement Handle 
                            6,                    // Parameter Number  
                            SQL_PARAM_INPUT,      // InputOutputType 
                            SQL_C_CHAR,           // ValueType 
                            SQL_VARCHAR,          // ParameterType
                            100,                  // ColumnSize 
                            0,                    // DecimalDigit 
                            &mRecord.c6,          // ParameterValuePtr
                            sizeof(mRecord.c6),   // BufferLength 
                            &mRecordInd.c6        // Indicator or strlen 
                            ) ;                
   assert ( sRC  == SQL_SUCCESS ) ;

   sRC = SQLBindParameter ( mInsertStatement,     // Statement Handle 
                            7,                    // Parameter Number  
                            SQL_PARAM_INPUT,      // InputOutputType 
                            SQL_C_CHAR,           // ValueType 
                            SQL_VARCHAR,          // ParameterType
                            100,                  // ColumnSize 
                            0,                    // DecimalDigit 
                            &mRecord.c7,          // ParameterValuePtr
                            sizeof(mRecord.c7),   // BufferLength 
                            &mRecordInd.c7        // Indicator or strlen 
                            ) ;                
   assert ( sRC  == SQL_SUCCESS ) ;


   sRC = SQLBindParameter ( mInsertStatement,     // Statement Handle 
                            8,                    // Parameter Number  
                            SQL_PARAM_INPUT,      // InputOutputType 
                            SQL_C_CHAR,           // ValueType 
                            SQL_VARCHAR,          // ParameterType
                            100,                  // ColumnSize 
                            0,                    // DecimalDigit 
                            &mRecord.c8,          // ParameterValuePtr
                            sizeof(mRecord.c8),   // BufferLength 
                            &mRecordInd.c8        // Indicator or strlen 
                            ) ;                
   assert ( sRC  == SQL_SUCCESS ) ;

 
   sRC = SQLBindParameter ( mInsertStatement,     // Statement Handle 
                            9,                    // Parameter Number  
                            SQL_PARAM_INPUT,      // InputOutputType 
                            SQL_C_SLONG,          // ValueType 
                            SQL_INTEGER,          // ParameterType
                            0,                    // ColumnSize 
                            0,                    // DecimalDigit 
                            &mRecord.c9,          // ParameterValuePtr
                            sizeof(mRecord.c9),   // BufferLength 
                            &mRecordInd.c9        // Indicator or strlen 
                            ) ;                
   assert ( sRC  == SQL_SUCCESS ) ;

   /**************************************************************
    Bind Params for update statment 
   **************************************************************/
   sRC = SQLBindParameter ( mUpdateStatement,     // Statement Handle 
                            1,                    // Parameter Number  
                            SQL_PARAM_INPUT,      // InputOutputType 
                            SQL_C_SLONG,          // ValueType 
                            SQL_INTEGER,          // ParameterType
                            0,                    // ColumnSize 
                            0,                    // DecimalDigit 
                            &mRecord.c1,          // ParameterValuePtr
                            sizeof(mRecord.c1),   // BufferLength 
                            &mRecordInd.c1        // Indicator or strlen 
                            ) ;                
   assert ( sRC  == SQL_SUCCESS ) ;

 
   /**************************************************************
    Bind Params for delete statment 
   **************************************************************/
   sRC = SQLBindParameter ( mDeleteStatement,     // Statement Handle 
                            1,                    // Parameter Number  
                            SQL_PARAM_INPUT,      // InputOutputType 
                            SQL_C_SLONG,          // ValueType 
                            SQL_INTEGER,          // ParameterType
                            0,                    // ColumnSize 
                            0,                    // DecimalDigit 
                            &mRecord.c1,          // ParameterValuePtr
                            sizeof(mRecord.c1),   // BufferLength 
                            &mRecordInd.c1        // Indicator or strlen 
                            ) ;                
   assert ( sRC  == SQL_SUCCESS ) ;

    
   /**************************************************************
    Bind Params and Bind Col  for select statment 
   **************************************************************/
   sRC = SQLBindParameter ( mSelectStatement,     // Statement Handle 
                            1,                    // Parameter Number  
                            SQL_PARAM_INPUT,      // InputOutputType 
                            SQL_C_SLONG,          // ValueType 
                            SQL_INTEGER,          // ParameterType
                            0,                    // ColumnSize 
                            0,                    // DecimalDigit 
                            &mRecord.c1,          // ParameterValuePtr
                            sizeof(mRecord.c1),   // BufferLength 
                            &mRecordInd.c1        // Indicator or strlen 
                            ) ;                
   assert ( sRC  == SQL_SUCCESS ) ;


   sRC = SQLBindCol ( mSelectStatement,   // StatementHandle 
                      1,                  // ColumnNumber 
                      SQL_C_SLONG,        // Target Type  
                      &mRecord.c1,        // Target Value Prt
                      sizeof(mRecord.c1), // Buffer Length 
                      &mRecordInd.c1      // Indicator 
                     );

   assert ( sRC  == SQL_SUCCESS ) ;
 
   sRC = SQLBindCol ( mSelectStatement,   // StatementHandle 
                      2,                  // ColumnNumber 
                      SQL_C_CHAR,        // Target Type  
                      &mRecord.c2,        // Target Value Prt
                      sizeof(mRecord.c2), // Buffer Length 
                      &mRecordInd.c2      // Indicator 
                     );

   assert ( sRC  == SQL_SUCCESS ) ;

   sRC = SQLBindCol ( mSelectStatement,   // StatementHandle 
                      3,                  // ColumnNumber 
                      SQL_C_CHAR,        // Target Type  
                      &mRecord.c3,        // Target Value Prt
                      sizeof(mRecord.c3), // Buffer Length 
                      &mRecordInd.c3      // Indicator 
                     );

   assert ( sRC  == SQL_SUCCESS ) ;

   sRC = SQLBindCol ( mSelectStatement,   // StatementHandle 
                      4,                  // ColumnNumber 
                      SQL_C_CHAR,        // Target Type  
                      &mRecord.c4,        // Target Value Prt
                      sizeof(mRecord.c4), // Buffer Length 
                      &mRecordInd.c4      // Indicator 
                     );

   assert ( sRC  == SQL_SUCCESS ) ;
   sRC = SQLBindCol ( mSelectStatement,   // StatementHandle 
                      5,                  // ColumnNumber 
                      SQL_C_CHAR,        // Target Type  
                      &mRecord.c5,        // Target Value Prt
                      sizeof(mRecord.c5), // Buffer Length 
                      &mRecordInd.c5      // Indicator 
                     );

   assert ( sRC  == SQL_SUCCESS ) ;

   sRC = SQLBindCol ( mSelectStatement,   // StatementHandle 
                      6,                  // ColumnNumber 
                      SQL_C_CHAR,        // Target Type  
                      &mRecord.c6,        // Target Value Prt
                      sizeof(mRecord.c6), // Buffer Length 
                      &mRecordInd.c6      // Indicator 
                     );

   assert ( sRC  == SQL_SUCCESS ) ;

   sRC = SQLBindCol ( mSelectStatement,   // StatementHandle 
                      7,                  // ColumnNumber 
                      SQL_C_CHAR,        // Target Type  
                      &mRecord.c7,        // Target Value Prt
                      sizeof(mRecord.c7), // Buffer Length 
                      &mRecordInd.c7      // Indicator 
                     );

   assert ( sRC  == SQL_SUCCESS ) ;

   sRC = SQLBindCol ( mSelectStatement,   // StatementHandle 
                      8,                  // ColumnNumber 
                      SQL_C_CHAR,        // Target Type  
                      &mRecord.c8,        // Target Value Prt
                      sizeof(mRecord.c8), // Buffer Length 
                      &mRecordInd.c8      // Indicator 
                     );

   assert ( sRC  == SQL_SUCCESS ) ;

   sRC = SQLBindCol ( mSelectStatement,   // StatementHandle 
                      9,                  // ColumnNumber 
                      SQL_C_CHAR,        // Target Type  
                      &mRecord.c9,        // Target Value Prt
                      sizeof(mRecord.c9), // Buffer Length 
                      &mRecordInd.c9      // Indicator 
                     );

   assert ( sRC  == SQL_SUCCESS ) ;

} 

int ODBCRunner :: doUpdate ( int aStartIndex , int aEndIndex ) { 
   SQLINTEGER sRC; 
   
   while ( aStartIndex < aEndIndex ) { 
     mRecord.c1 = aStartIndex; 
     sRC = SQLExecute ( mUpdateStatement ) ;
     
     if ( sRC != SQL_SUCCESS ) { 
       PrintDiagnosticRecord ( SQL_HANDLE_STMT, mUpdateStatement ) ;
       return -1;
     } 
     aStartIndex ++ ;
   } 
  
   return 0;

} 

int ODBCRunner :: doSelect ( int aStartIndex , int aEndIndex ) { 
   SQLINTEGER sRC; 
   
   while ( aStartIndex < aEndIndex ) { 
     mRecord.c1 = aStartIndex; 

     sRC = SQLExecute ( mSelectStatement ) ;
     
     if ( sRC != SQL_SUCCESS ) { 
       PrintDiagnosticRecord ( SQL_HANDLE_STMT, mSelectStatement ) ;
       return -1;
     } 

     while ( SQL_SUCCEEDED ( sRC = SQLFetch ( mSelectStatement ) ) ) { 
       // Do nothing
     } 
     sRC = SQLFreeStmt ( mSelectStatement, SQL_CLOSE) ;
     if ( sRC != SQL_SUCCESS ) { 
       PrintDiagnosticRecord ( SQL_HANDLE_STMT, mSelectStatement ) ;
       return -1;
     } 
     aStartIndex ++ ;
   } 
  
  return 0;
} 
int ODBCRunner :: doDelete ( int aStartIndex , int aEndIndex ) { 
   SQLINTEGER sRC; 
   
   while ( aStartIndex < aEndIndex ) { 
     mRecord.c1 = aStartIndex; 

     sRC = SQLExecute ( mDeleteStatement ) ;
     
     if ( sRC != SQL_SUCCESS ) { 
       PrintDiagnosticRecord ( SQL_HANDLE_STMT, mDeleteStatement ) ;
       return -1;
     } 
     aStartIndex ++ ;
   } 
  
  return 0;
} 

int ODBCRunner :: doInsert ( int aStartIndex, int aEndIndex ) { 
  
   SQLINTEGER sRC; 
   
   while ( aStartIndex < aEndIndex ) { 
     mRecord.c1 = aStartIndex; 
     mRecordInd.c2 = std::sprintf(mRecord.c2 , "%010d", aStartIndex ) ;
     mRecordInd.c3 = std::sprintf(mRecord.c3 , "%010d", aStartIndex ) ;
     mRecordInd.c4 = std::sprintf(mRecord.c4 , "%010d", aStartIndex ) ;
     mRecordInd.c5 = std::sprintf(mRecord.c5 , "%010d", aStartIndex ) ;
     mRecordInd.c6 = std::sprintf(mRecord.c6 , "%010d", aStartIndex ) ;
     mRecordInd.c7 = std::sprintf(mRecord.c7 , "%010d", aStartIndex ) ;
     mRecordInd.c8 = std::sprintf(mRecord.c8 , "%010d", aStartIndex ) ;
     mRecord.c9 = aStartIndex; 

     sRC = SQLExecute ( mInsertStatement ) ;
     
     if ( sRC != SQL_SUCCESS ) { 
       PrintDiagnosticRecord ( SQL_HANDLE_STMT, mInsertStatement ) ;
       return -1;
     } 
     aStartIndex ++ ;
   } 
  
   return 0;
} 

