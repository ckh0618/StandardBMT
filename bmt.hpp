#pragma once 

#include <string>
#include <sundb.h>
#include <dbmAPI.h>

// Interface ...

class StandardRunner { 
  public : 
    virtual ~StandardRunner () {} 
    virtual int doInsert ( int aStartIndex, int aEndIndex ) = 0 ; 
    virtual int doUpdate ( int aStartIndex, int aEndIndex ) = 0 ;
    virtual int doDelete ( int aStartIndex, int aEndIndex ) = 0 ;
    virtual int doSelect ( int aStartIndex, int aEndIndex ) = 0 ; 
};  


class GPECRunner  : public StandardRunner { 
  typedef std::string string;
  public : 
     GPECRunner ( const string& aUser, const string& aPwd, const string& aDSN ) ;
     virtual ~GPECRunner () ;
     virtual int doInsert ( int aStartIndex, int aEndIndex )  ; 
     virtual int doUpdate ( int aStartIndex, int aEndIndex )  ; 
     virtual int doSelect ( int aStartIndex, int aEndIndex )  ; 
     virtual int doDelete ( int aStartIndex, int aEndIndex )  ; 
  private : 
     GPECRunner ( const GPECRunner& rhs ) ;
}; 


class DBMRunner : public StandardRunner { 
 typedef std::string string;
 public : 
   DBMRunner ( const string& aUser, const string& aPwd, const string& aDSN ) ; 
   virtual ~DBMRunner () ;
   virtual int doInsert ( int aStartIndex, int aEndIndex )  ; 
   virtual int doUpdate ( int aStartIndex, int aEndIndex )  ; 
   virtual int doSelect ( int aStartIndex, int aEndIndex )  ; 
   virtual int doDelete ( int aStartIndex, int aEndIndex )  ; 

  private : 
     DBMRunner ( const GPECRunner& rhs ) ;
     dbmHandle mHandle ; 

     #define MAX_CHAR_COL_SIZE 100
     typedef struct { 
       int c1 ; 
       char c2 [ MAX_CHAR_COL_SIZE ] ;
       char c3 [ MAX_CHAR_COL_SIZE ] ;
       char c4 [ MAX_CHAR_COL_SIZE ] ;
       char c5 [ MAX_CHAR_COL_SIZE ] ;
       char c6 [ MAX_CHAR_COL_SIZE ] ;
       char c7 [ MAX_CHAR_COL_SIZE ] ;
       char c8 [ MAX_CHAR_COL_SIZE ] ;
       int c9 ;
     } Record; 
     Record     mRecord; 
     std::string mTableName ;
}; 



class ODBCRunner  : public StandardRunner { 
  typedef std::string string;
  public : 
     ODBCRunner ( const string& aUser, const string& aPwd, const string& aDSN ) ;
     virtual ~ODBCRunner () ;
     virtual int doInsert ( int aStartIndex, int aEndIndex )  ; 
     virtual int doUpdate ( int aStartIndex, int aEndIndex )  ; 
     virtual int doSelect ( int aStartIndex, int aEndIndex )  ; 
     virtual int doDelete ( int aStartIndex, int aEndIndex )  ; 
  private : 
     ODBCRunner ( const ODBCRunner& rhs ) ;
     SQLHENV     mENV;
     SQLHDBC     mDBC;
     SQLHSTMT    mInsertStatement; 
     SQLHSTMT    mSelectStatement; 
     SQLHSTMT    mUpdateStatement; 
     SQLHSTMT    mDeleteStatement; 

     void PrepareAllStatement () ;
#define MAX_CHAR_COL_SIZE 100
     typedef struct { 
       int c1 ; 
       char c2 [ MAX_CHAR_COL_SIZE ] ;
       char c3 [ MAX_CHAR_COL_SIZE ] ;
       char c4 [ MAX_CHAR_COL_SIZE ] ;
       char c5 [ MAX_CHAR_COL_SIZE ] ;
       char c6 [ MAX_CHAR_COL_SIZE ] ;
       char c7 [ MAX_CHAR_COL_SIZE ] ;
       char c8 [ MAX_CHAR_COL_SIZE ] ;
       int c9 ;
     } Record; 

     typedef struct { 
       SQLLEN c1; 
       SQLLEN c2; 
       SQLLEN c3; 
       SQLLEN c4; 
       SQLLEN c5; 
       SQLLEN c6; 
       SQLLEN c7; 
       SQLLEN c8; 
       SQLLEN c9; 
     } RecordInd ; 
     Record     mRecord; 
     RecordInd  mRecordInd;
}; 

