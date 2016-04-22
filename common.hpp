#define MAX_CHAR_COL_SIZE 100

EXEC SQL BEGIN DECLARE SECTION ;
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

EXEC SQL END DECLARE SECTION ;

