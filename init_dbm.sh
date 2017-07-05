
rm -f $DBM_HOME/dic/*
rm -f $DBM_HOME/WAL/*
rm -f /dev/shm/* 


metaManager --silent << EOF > /dev/null
 initdb
 create instance $DBM_INSTANCE;
 quit;
EOF


metaManager --silent <<EOF 2>&1
conn ${DBM_INSTANCE} 
drop table test1 ;
create table test1 c1 int c2 char(100) c3 char(100) c4 char(100) c5 char(100) c6 char(100) c7 char(100) c8 char(100) c9 int init 1000000 extend 100000 max 10000000;
create index test1_idx1 on test1 c1 ;

