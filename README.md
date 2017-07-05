# StandardBMT
Standard BMT for Goldilocks and Goldilocks DBM 

# Purpose 
This program is for 
* comparing performance between Goldilocks DBMS client library (ODBC and ESQL)
* comparing performance between Goldilocks DBMS and DBM 


# Prerequisite
* Goldilocks DBMS and DBM must be installed 

# How to test 
* If you want to test DBMS, please run crt.sql using gsql. 
* If you want to test DBM, just run init_dbm.sh file. all data will be erased.  

# How to change dbms/dbm connection information 
* Just open Runner.cc and modify  constructor parameters . 

|| Position || Description || 
| 1 | User Name (DBMS) / Instance name (DBM ) |
| 2 | Password (DBMS) / Not used ( DBM ) | 
| 3 | DSN ( DBMS ) / TableNAme ( DBM ) | 


# How to run 
* just run Runner executable with appropriate parameters. 
* You can see help message if you set inaccuate parameters. 




