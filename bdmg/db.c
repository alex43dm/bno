#define _SQL_OS_QNX32
#include<conio.h>
#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>
#include <ctype.h>
#include <stdarg.h>
/*
EXEC SQL INCLUDE SQLCA;
*/
#include "sqlca.h"
#ifdef __cplusplus
    extern "C" {
#endif
extern SQLCA sqlca;
extern SQLCA _fd_ *sqlcaptr;
extern short int _ESQL_Version4_;
extern short int _ESQL_OS_QNX32_;
#ifdef __cplusplus
    }
#endif
#include "sqlda.h"
#if defined( __WATCOMC__ )
#pragma pack(push,1)
#endif
static struct { 
        char        _sqlfar *userid;
        char        _sqlfar *password;
        short int   num;
        SQLCA       _fd_ * _sqlfar *sqlca;
        short int   _sqlfar *sqlpp_version;
        short int   _sqlfar *sqlpp_os;
}  __SQLV_db_1 = { SQLFARNULL, SQLFARNULL, 0, &sqlcaptr, &_ESQL_Version4_, &_ESQL_OS_QNX32_ };
#if defined( __WATCOMC__ )
#pragma pack(pop)
#endif
static char  __SQLV_db_2[] = "DBA";
static char  __SQLV_db_3[] = "SQL";
static char         *__SQL_ProgName = {"db"};
static a_sql_statement_number  __SQL_S1 = 0;
static struct {
    unsigned char sqldaid[8];
    long int sqldabc;
    short int sqln;
    short int sqld;
    SQLDA_VARIABLE sqlvar[6];
}  __SQLV_db_4
    = { {'S','Q','L','D','A',' ',' ',' '},
    sizeof(SQLDA)+(6-1)*sizeof(SQLDA_VARIABLE),
    6, 6, {
      { 480, 8, 0L, 0L, {0,{0}}},
      { 480, 8, 0L, 0L, {0,{0}}},
      { 496, 4, 0L, 0L, {0,{0}}},
      { 452, 1, 0L, 0L, {0,{0}}},
      { 452, 1, 0L, 0L, {0,{0}}},
      { 496, 4, 0L, 0L, {0,{0}}}
    } };

#include "sqldef.h"


class DB{ 

/*
EXEC SQL BEGIN DECLARE SECTION;
*/
char com[200];

 /*
EXEC SQL END DECLARE SECTION;
*/


public:
DB::DB();
DB::DB(
/*
EXEC SQL BEGIN DECLARE SECTION;
*/

char table[15]
 /*
EXEC SQL
 END DECLARE SECTION;
*/

);
void DB::DBInsert(
/*
EXEC SQL BEGIN DECLARE SECTION;
*/






 double Dat, 
 double PAbs, 
 int Time, 
 int T, 
 char Mess, 
 char State 
 /*
EXEC SQL
 END DECLARE SECTION;
*/

);
DB::~DB();
};
DB::DB(){}
//-------------------------
DB::DB(
/*
EXEC SQL BEGIN DECLARE SECTION;
*/

char table[15]
 /*
EXEC SQL
 END DECLARE SECTION;
*/

){
    if( !db_init( &sqlca ) ) {
    	perror("Unable to initialize database interface\n" );
		exit(-1);
    }
/*    if( !db_find_engine( &sqlca, NULL ) ) {
	perror( "Database Engine/Station not running" );
	exit(-1);
    }
    EXEC SQL CONNECT TO "bno1" DATABASE "/usr/db/bno.db" USER "DBA" IDENTIFIED BY "SQL";
*/
    /*
EXEC SQL CONNECT "DBA" IDENTIFIED BY "SQL";
*/
	{
	dbpp_connect_40( (sqlcaptr), &__SQLV_db_1, __SQLV_db_2, __SQLV_db_3, SQLNULL, SQLNULL, SQLNULL );
#ifdef __SQLCODE
SQLCODE = __SQLCODE;
#endif
#ifdef __SQLSTATE
strncpy(SQLSTATE,__SQLSTATE,6);
#endif
	}

sprintf(com,
"insert into %s(Dat,PAbs,T,Mess,State,data)values(:?,:?,:?,:?,:?,:?)"
,table);
/*
EXEC SQL PREPARE S1 FROM
 :com;
*/

	{
	dbpp_prepare_describe( (sqlcaptr), &__SQLV_db_1, SQLNULL, _sql_ptrtypechk_(__SQL_ProgName,char), &__SQL_S1, com, (SQLDA _fd_ *)0, (SQLDA _fd_ *)0, 1, 0 );
#ifdef __SQLCODE
SQLCODE = __SQLCODE;
#endif
#ifdef __SQLSTATE
strncpy(SQLSTATE,__SQLSTATE,6);
#endif
	}

}
//-------------------------
void DB::DBInsert(
/*
EXEC SQL BEGIN DECLARE SECTION;
*/






 double Dat, 
 double PAbs, 
 int Time, 
 int T, 
 char Mess, 
 char State 
 /*
EXEC SQL
 END DECLARE SECTION;
*/

){
/*
EXEC SQL EXECUTE S1 USING :Dat,:PAbs,:T,:Mess,:State,:Time;
*/
	{
((SQLDA  *)&__SQLV_db_4)->sqlvar[0].sqldata = (void _sqldafar *)(&(Dat));
((SQLDA  *)&__SQLV_db_4)->sqlvar[1].sqldata = (void _sqldafar *)(&(PAbs));
((SQLDA  *)&__SQLV_db_4)->sqlvar[2].sqldata = (void _sqldafar *)(&(T));
((SQLDA  *)&__SQLV_db_4)->sqlvar[3].sqldata = (void _sqldafar *)(&(Mess));
((SQLDA  *)&__SQLV_db_4)->sqlvar[4].sqldata = (void _sqldafar *)(&(State));
((SQLDA  *)&__SQLV_db_4)->sqlvar[5].sqldata = (void _sqldafar *)(&(Time));
	dbpp_execute_into( (sqlcaptr), &__SQLV_db_1, SQLNULL, _sql_ptrtypechk_(__SQL_ProgName,char), &__SQL_S1, ((SQLDA  *)&__SQLV_db_4), (SQLDA _fd_ *)0 );
#ifdef __SQLCODE
SQLCODE = __SQLCODE;
#endif
#ifdef __SQLSTATE
strncpy(SQLSTATE,__SQLSTATE,6);
#endif
	}

}
//-------------------------
DB::~DB(){
    /*
EXEC SQL DISCONNECT;
*/
	{
	dbpp_disconnect( (sqlcaptr), &__SQLV_db_1, SQLNULL );
#ifdef __SQLCODE
SQLCODE = __SQLCODE;
#endif
#ifdef __SQLSTATE
strncpy(SQLSTATE,__SQLSTATE,6);
#endif
	}

    db_fini( &sqlca );
}
