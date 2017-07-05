#include <bmt.hpp>
#include <cstdio>
#include <cstring> 
#include <dbmAPI.h>
 

DBMRunner :: DBMRunner ( const string& aUser, 
                         const string& aPwd, 
                         const string& aDSN) { 

  int sRC; 

  sRC = dbmInitHandle ( &mHandle, aUser.c_str() ) ;
  if ( sRC ) { 
    printf("Fail to connect DBM [%s] \n", aUser.c_str());
    exit (-1);
  }

  sRC = dbmPrepareTable ( &mHandle, aDSN.c_str () ) ;

  if ( sRC ) { 
    printf("Fail to prepare DBM table [%s] \n", aDSN.c_str());
    exit (-1);
  }

  mTableName = aDSN ;

}

DBMRunner :: ~ DBMRunner () { 
}

int DBMRunner :: doInsert ( int aStart,
                        int aEnd ) { 
  int sRC ; 

  while ( aStart < aEnd ) {
    mRecord.c1 = aStart; 
    std::sprintf(mRecord.c2 , "%010d", aStart ) ;
    std::sprintf(mRecord.c3 , "%010d", aStart ) ;
    std::sprintf(mRecord.c4 , "%010d", aStart ) ;
    std::sprintf(mRecord.c5 , "%010d", aStart ) ;
    std::sprintf(mRecord.c6 , "%010d", aStart ) ;
    std::sprintf(mRecord.c7 , "%010d", aStart ) ;
    std::sprintf(mRecord.c8 , "%010d", aStart ) ;
    mRecord.c9 = aStart; 

    sRC = dbmInsertRow ( &mHandle, mTableName.c_str(), &mRecord, sizeof ( mRecord ) ) ;
    
    if ( sRC ) { 
      printf ("dbmInsertFailed : [%d] \n", sRC ) ;
      return  -1 ;
    }

    sRC = dbmCommit ( &mHandle ) ;
    aStart ++ ;
  } 
  return 0 ;
}



int DBMRunner :: doUpdate ( int aStart, int aEnd ) { 
  int sRC ; 

  while ( aStart < aEnd ) {
    mRecord.c1 = aStart; 
    std::sprintf(mRecord.c2 , "%010d", aStart ) ;
    std::sprintf(mRecord.c3 , "%010d", aStart ) ;
    std::sprintf(mRecord.c4 , "%010d", aStart ) ;
    std::sprintf(mRecord.c5 , "%010d", aStart ) ;
    std::sprintf(mRecord.c6 , "%010d", aStart ) ;
    std::sprintf(mRecord.c7 , "%010d", aStart ) ;
    std::sprintf(mRecord.c8 , "%010d", aStart ) ;
    mRecord.c9 = aStart; 

    sRC = dbmUpdateRow ( &mHandle, mTableName.c_str(), &mRecord ) ;
    
    if ( sRC ) { 
      printf ("dbmUpdateFailed : [%d] \n", sRC ) ;
      return  -1 ;
    }

    sRC = dbmCommit ( &mHandle ) ;
    aStart ++ ;
  } 
  return 0 ;

}

int DBMRunner :: doSelect ( int aStart, int aEnd ) { 
  int sRC ; 

  while ( aStart < aEnd ) {
    mRecord.c1 = aStart; 

    sRC = dbmSelectRow ( &mHandle, mTableName.c_str(), &mRecord ) ;
    
    if ( sRC ) { 
      printf ("dbmSelectFailed : [%d] \n", sRC ) ;
      return  -1 ;
    }

    sRC = dbmCommit ( &mHandle ) ;
    aStart ++ ;
  } 
  return 0 ;

}

int DBMRunner :: doDelete ( int aStart, int aEnd ) { 
  int sRC ; 

  while ( aStart < aEnd ) {
    mRecord.c1 = aStart; 

    sRC = dbmDeleteRow ( &mHandle, mTableName.c_str(), &mRecord ) ;
    
    if ( sRC ) { 
      printf ("dbmSelectFailed : [%d] \n", sRC ) ;
      return  -1 ;
    }

    sRC = dbmCommit ( &mHandle ) ;
    aStart ++ ;
  } 
  return 0 ;

}
