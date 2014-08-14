#include<conio.h>
#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>
#include <ctype.h>
#include <stdarg.h>
EXEC SQL INCLUDE SQLCA;
#include "sqldef.h"


class DB{ 

EXEC SQL BEGIN DECLARE SECTION;
char com[200];
EXEC SQL END DECLARE SECTION;

public:
DB::DB();
DB::DB(
EXEC SQL BEGIN DECLARE SECTION;
char table[15]
EXEC SQL END DECLARE SECTION;
);
void DB::DBInsert(
EXEC SQL BEGIN DECLARE SECTION;
	double	Dat,		// измеренные данные
	double	PAbs,		// погрешность
	int  	Time,		// время измерения
	int  	T,			// длительность измерения (с)
	char	Mess,		// признак достоверности измерения
	char	State 		// состояние канала
EXEC SQL END DECLARE SECTION;
);
DB::~DB();
};
DB::DB(){}
//-------------------------
DB::DB(
EXEC SQL BEGIN DECLARE SECTION;
char table[15]
EXEC SQL END DECLARE SECTION;
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
    EXEC SQL CONNECT "DBA" IDENTIFIED BY "SQL";
sprintf(com,
"insert into %s(Dat,PAbs,T,Mess,State,data)values(:?,:?,:?,:?,:?,:?)"
,table);
EXEC SQL PREPARE S1 FROM :com;
}
//-------------------------
void DB::DBInsert(
EXEC SQL BEGIN DECLARE SECTION;
	double	Dat,		// измеренные данные
	double	PAbs,		// погрешность
	int  	Time,		// время измерения
	int  	T,			// длительность измерения (с)
	char	Mess,		// признак достоверности измерения
	char	State 		// состояние канала
EXEC SQL END DECLARE SECTION;
){
EXEC SQL EXECUTE S1 USING :Dat,:PAbs,:T,:Mess,:State,:Time;
}
//-------------------------
DB::~DB(){
    EXEC SQL DISCONNECT;
    db_fini( &sqlca );
}
