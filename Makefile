
INCLUDES+=-I${SUNDB_HOME}/include -I. -I${DBM_HOME}/include 
LIB_PATH=-L${SUNDB_HOME}/lib -L${DBM_HOME}/lib

CFLAGS=-g -W -Wall 

all : 
	gpec --include-path=. --output=gpec_bmt.cc  gpec_bmt.gc
	g++ -c ${CFLAGS}  ${INCLUDES} -o gpec_bmt.o gpec_bmt.cc
	g++ -c ${CFLAGS}  ${INCLUDES} -o odbc_bmt.o odbc_bmt.cc
	g++ -c ${CFLAGS}  ${INCLUDES} -o dbm_bmt.o dbm_bmt.cc
	g++ -c ${CFLAGS}  ${INCLUDES} Runner.cc -o Runner.o 
	g++ Runner.o gpec_bmt.o dbm_bmt.o  odbc_bmt.o -o Runner -lsundbesql -lsundba -lpthread -ldbm -lcmn -ldl -lm -lrt ${LIB_PATH}

clean : 
	rm -rf *.o gpec_bmt.cc Runner
