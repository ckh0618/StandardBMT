#include <iostream>
#include <sys/time.h>
#include "bmt.hpp"
#include <cassert>
#include <cstdio>
#include <vector>
#include <numeric>
#include <unistd.h>
#include <algorithm>





int getTimeDiff ( const struct timeval& aStart, 
    const struct timeval& aEnd ) { 

  return (aEnd.tv_sec - aStart.tv_sec) * 1000000  + 
    (aEnd.tv_usec - aStart.tv_usec ) ; 
} 

int printHelpMessage ( char **argv ) { 

  printf("%s -t [type] -o [operation_type] -s [start] -e [end] \n", argv[0] ) ;
  printf("type -> o : odbc  / g : gpec precompiler \n") ;
  printf("operation_type -> i : insert / s : select / d : delete / u : update\n" ) ;
  return 0 ;
} 

int main (int argc, char** argv) { 

  struct timeval sStartTime, sEndTime ; 
  std::vector <int> sStore;


  int param_opt ;

  char sOperation; 
  char sType; 
  int  sStart_arg = 0;
  int  sEnd_arg   = 0;
  int  sCheck = 0;
  StandardRunner *sRunner = NULL ;
 

  while ( (param_opt = getopt (argc, argv , "t:o:s:e:h" ) ) != -1 )  { 
    switch ( param_opt ) { 
      case 't' : 
        sType = optarg [0]  ;
        sCheck ++;
        break; 
      case 'o' : 
        sOperation = optarg[0];
        sCheck ++;
        break;

      case 's' : 
        sStart_arg = atoi (optarg);
        sCheck ++;
        break;

      case 'e' : 
        sEnd_arg = atoi (optarg);
        sCheck ++;
        break;

      case 'h' : 
        sCheck = 0;
        break;
    }
  } 

  if ( sCheck != 4 ) { 
    printHelpMessage (argv ) ;
    exit (0);
  } 


  switch ( sType ) { 
     case 'g' : 
       sRunner = new GPECRunner ( "test", "test", "" ) ;
     break; 
     case 'o' : 
       sRunner = new ODBCRunner ( "test", "test", "" ) ;
     break; 
     default : 
       printf("Invalid Type !!  [o : odbc] / [g : gpec] \n") ;
       exit ( -1 ) ;
  } 

  for ( int i = sStart_arg ; i < sEnd_arg + 1 ; ++i ) { 

    gettimeofday (&sStartTime, NULL ) ;

    switch ( sOperation ) { 
      case 'i' : 

        assert ( sRunner->doInsert ( i , i + 1 ) == 0 )  ;
        break;

      case 's' : 

        assert ( sRunner->doSelect ( i , i + 1 ) == 0 )  ;
        break;

      case 'd' : 

        assert ( sRunner->doDelete ( i , i + 1 ) == 0 )  ;
        break;

      case 'u' : 

        assert ( sRunner->doUpdate ( i , i + 1 ) == 0 )  ;
        break; 
    } 

    gettimeofday (&sEndTime, NULL ) ;

    int us = getTimeDiff ( sStartTime, sEndTime ) ;

    sStore.push_back ( us ) ;
  } 


  std::sort ( sStore.begin(), sStore.end());

  int sum = std::accumulate(sStore.begin(), sStore.end(), 0);
  int mean = sum / sStore.size();

  printf ("Min : %d Max : %d Mean : %d ", sStore[0] , sStore[sEnd_arg - sStart_arg - 1], mean );
  double tps = (double)(sEnd_arg - sStart_arg)  / ( (double)sum / 1000000 ) ;
  printf ("TPS : %lf\n", tps ) ;

  delete sRunner; 

  
  return 0; 
} 



