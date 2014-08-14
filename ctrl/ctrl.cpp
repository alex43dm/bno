#include<conio.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<process.h>
#include<stddef.h>
#include<iostream.h>
#include<fstream.h>
#include<ctype.h>
#include<fcntl.h>
#include<dirent.h>
#include<math.h>
#include<errno.h>
#include<mqueue.h>
#include</sys/proxy.h>
#include</sys/psinfo.h>
#include</sys/kernel.h>
#include</sys/types.h>
#include</sys/stat.h>
#include</sys/wait.h>
#include</sys/uio.h>
#include</sys/sched.h>
#include</sys/name.h>
//---------------------
#include <Cname.h>
#include <Shmem.h>
#include <ctrlmes.h>
#include <display.h>
#include <log.h>
#include <myerr.h>
#include<timer.h>
#include<dispmes.h>
#include <cmdproxy.h>
#include <simpmq.h>
#include <commq.h>

static u_short BA;

class TheApp: 
	public CName, 
	public CLog, 
	public CTimer, 
	public CDisp,
	public CMQueue,
	public Proxy
{
private:
	time_t system_begin;
	CShmem *pdio;
	StateBno State;
	int DispFlag;
public:

        TheApp::TheApp(void);
int virtual TheApp::Run(void); 
	int TheApp::OnTest0(void);	
	int TheApp::OnTest1(void);	
	int TheApp::OnTest2(void);	
	int TheApp::OnTest3(void);	
	int TheApp::OnTest4(void);	
	int TheApp::OnTest5(void);	
	int TheApp::OnTest6(void);	
	int TheApp::OnTest7(void);	
	int TheApp::OnTest8(void);	
	int TheApp::OnTest9(void);	
	int TheApp::OnTest10(void);	
	int TheApp::OnTest11(void);	
	int TheApp::OnTest12(void);	
	int TheApp::OnTest13(void);	
	int TheApp::OnTest14(void);	
	int TheApp::OnTest15(void);	
	char TheApp::TestBeginUnio(void);
	char TheApp::TestBeginModem(void);
	char TheApp::TestBeginStart(void);
	char TheApp::TestUPS();
   	int TheApp::OnLink(void);
   	int TheApp::Start(void);
	void TheApp::printrn(u_char *, const char  * , int );
	void TheApp::printn(u_char *, const char  * , int );
	mqd_t TheApp::Seachmq(const char  * );
	void TheApp::Error(u_char );
	int TheApp::Error(u_char, const char * );
	int TheApp::OnInfo(void);
	int TheApp::OnIdle(void);
	    TheApp::~TheApp();
};

//-------Massege Map begin--------

#define  HALT 		99
#define  TEST0		1
#define  TEST1		2
#define  TEST2		3
#define  TEST3		4
#define  TEST4		5
#define  TEST5		6
#define  TEST6		7
#define  TEST7		8
#define  TEST8		9
#define  TEST9		10
#define  TEST10		11
#define  TEST11		12
#define  TEST12		13
#define  TEST13		14
#define  TEST14		15
#define  TEST15		16
#define  TIME		17
#define  STATE		18
#define  GET_STATE	19
#define  IDLE 		0
//-------Massege Map end----------

//-------Таблица кодов ошибок начало--------

#define  BADLOAD	1 /*ошибка загрузки*/
#define  DOOR		2 /*открыта дверь*/
#define  E6V		3 /*отказ БП 6В*/
#define  E12V		4 /*откак БП 12В*/
#define  E24IV		5 /*отказ БП 24IВ*/
#define  E24IIV		6 /*отказ БП 24IIВ*/
#define  ERES		7 /*контакт изменил значение*/
#define  EUPS1 		8 /*переключение на работу от батарей*/
#define  EUPS2 		9 /*разряд аккумулятора*/

#define  BNOOK 		10 /*разряд аккумулятора*/
#define  BNOBAD		11 /*разряд аккумулятора*/

#define  IDOOR		12 /*открыта дверь*/
#define  IE6V		13 /*отказ БП 6В*/
#define  IE12V		14 /*откак БП 12В*/
#define  IE24IV		15 /*отказ БП 24IВ*/
#define  IE24IIV	16 /*отказ БП 24IIВ*/
#define  IERES		17 /*контакт изменил значение*/
#define  IEUPS1 	18 /*переключение на работу от батарей*/
#define  IEUPS2 	19 /*разряд аккумулятора*/

//-------Таблица кодов ошибок конец--------

#include "ctrl.h"

//-------------------------

TheApp::TheApp(void): 
						CName("ctrl",7), 
						CMQueue( "ctrl" ),
						CLog(), 
						CTimer(), 
						CDisp(),
						Proxy(BNOIN,BNOOUT) 
{
int fd_log;
tm *mtime;

init_log("ctrl",LOG_LOCAL0);
info("start system"); 
//начальный тест загрузки
//-----------провека наличия плат крейта-----------------

State.TestAll = TestBeginStart();
State.TestUPS = TestUPS();
//проверка модемов		
State.TestModem = TestBeginModem();
//проверка UNIO
if( (State.TestUnio = TestBeginUnio())!=0 )
	{
		prus("бно ",0);
		if( getenv("BNO")!=NULL )
			{
			prus( getenv("BNO") );
			prus( "C01" );
			}else{
			prus("XXXX");
			}
		prus(" не норма ");
		getkey();
		prus("запуск в тестовом режиме",40);
		system("starterr");
		info("press OK for requst bat system");
		
	}else{

		russcrol("бно ");
		if( getenv("BNO")!=NULL )
			{
			prus( getenv("BNO") );
			prus( "C01" );
			}else{
			prus("XXXX");
			}
		
		prus(" норма ");
		timer_setup(1);
		prus("старт ");
		system("startnext");
		info("bno ok");
		//-----------последний старт системы-----------
//		clrscr();
//--------------------Open Log File---------------------
fd_log=open("/usr/bdmg/.time",O_CREAT|O_EXCL|O_RDWR,0600);	
if(fd_log!=-1)
		{
		system_begin=time(NULL);
		mtime=localtime(&system_begin);
		write(fd_log,mtime,sizeof(tm));
		}
	if(fd_log == -1)
		if( errno == EEXIST )
			fd_log=open("/usr/bdmg/.time",O_RDWR,0600);	
		mtime = new tm;
//--------------------Open Log File---------------------
		//Last start
		russcrol("последний старт бно:");
		lseek(fd_log,0,SEEK_SET);
		read(fd_log,mtime,sizeof(tm));
		sprintf(col,"%#02d.%#02d.%d %#02d:%#02d:%#02d",
		mtime->tm_mday,mtime->tm_mon+1,mtime->tm_year+1900,
		mtime->tm_hour,mtime->tm_min,mtime->tm_sec);
		prus(col);
		timer_setup(2);
		//-------Set new time in log--------------
		lseek(fd_log,0,SEEK_SET);
		system_begin=time(NULL);
		mtime=localtime(&system_begin);
		write(fd_log,mtime,sizeof(tm));

		russcrol("текущее время:");
		sprintf(col,"%#02d.%#02d.%d %#02d:%#02d:%#02d",
		mtime->tm_mday,mtime->tm_mon+1,mtime->tm_year+1900,
		mtime->tm_hour,mtime->tm_min,mtime->tm_sec);
		prus(col);
	}
close(fd_log);
//----------------- mesege-------------------
InitProxy();
//----------------------------------------------------
init_net("ctrl");
DispFlag = 0;
State.door 		= 1;
State.reserve 	= 1;
State.Power6v 	= 1;
State.Power12v 	= 1;
State.Power24Iv = 1;
State.Power24IIv= 1;
State.battery 	= 1;
State.accumul 	= 1;
};
//-----Main Loop----------
int TheApp::Run(void)
{
ifgetmes();
		
switch( Comand  ){
	case HALT  	: 
				exit(0);
	case TEST0	: OnTest0();	break;
	case TEST1	: OnTest1();	break;
	case TEST2	: OnTest2();	break;
	case TEST3	: OnTest3();	break;
	case TEST4	: OnTest4();	break;
	case TEST5	: OnTest5();	break;
	case TEST6	: OnTest6();	break;
	case TEST7	: OnTest7();	break;
	case TEST8	: OnTest8();	break;
	case TEST9	: OnTest9();	break;
	case TEST10	: OnTest10();	break;
	case TEST11	: OnTest11();	break;
	case TEST12	: OnTest12();	break;
	case TEST13	: OnTest13();	break;
	case TEST14	: OnTest14();	break;
	case TEST15	: OnTest15();	break;
	case STATE	: if(DispFlag == 1)
						{
						 DispFlag = 0;
						}else{
						 DispFlag = 1;
						}
						clrscr();
						Comand = IDLE;
						break;
	case GET_STATE	: OnInfo();	break;
	case IDLE 	:
	default   	: OnIdle();
				break;
	} 
return 1;
}
int TheApp::OnInfo(void)
{
int fd_log,cmd;
fd_log=open("/usr/bdmg/camin.log",O_RDONLY);   
read(fd_log,&cmd,sizeof(int)); 
State.sesion=(char)cmd;
SendProxy((StateBno *)&State,sizeof(State));
close(fd_log);
return 0;
}
//-------------------------
TheApp::~TheApp()
{
}
//-----------------------
int TheApp::OnIdle(void)
{
u_int x;
for(x=0;x<3;x++)
{
if( ifmes() != 0 ) return 1;
timer_setup(1); 
}
		
if( State.door !=(x=pdio->GetBit(44)) )
	{
	if(State.door != 1){warning(DOOR);}else{warning(IDOOR);}
//	if(State.door != 1){Error(DOOR);}else{Error(IDOOR);}
	State.door =(char)x;
	}
if( State.reserve 	!=(x=pdio->GetBit(45)) )
	{
	if(State.reserve != 0){warning(ERES);}else{warning(IERES);}
	State.reserve =(char)x;
	}
if( State.Power6v 	!=(x=pdio->GetBit(43)) )
	{
	if(State.Power6v != 1){warning(E6V);}else{warning(IE6V);}
	State.Power6v =(char)x;
	}
if( State.Power12v !=(x=pdio->GetBit(42)) )
	{
	if(State.Power12v != 1){warning(E12V);}else{warning(IE12V);}
	State.Power12v =(char)x;
	}
if( State.Power24Iv !=(x=pdio->GetBit(41)) )
	{
	if(State.Power24Iv != 1){warning(E24IV);}else{warning(IE24IV);}
	State.Power24Iv =(char)x;
	}
if( State.Power24IIv !=(x=pdio->GetBit(40)) )
	{
	if(State.Power24IIv != 1){warning(E24IIV);}else{warning(IE24IIV);}
	State.Power24IIv =(char)x;
	}
if( State.battery != (x=pdio->GetBit(8))  )
	{
	if(State.battery != 1){warning(EUPS1);}else{warning(IEUPS1);}
	State.battery =(char)x;
	}
if( State.accumul 	!= (x=pdio->GetBit(9))  )
	{
	if(State.accumul != 1){warning(EUPS2);}else{warning(IEUPS2);}
	State.accumul =(char)x;
	}
if(DispFlag == 1)
{
	sprintf(col,"дверь-%d 6в-%d 12в-%d 24вI-%d 24вII-%d"
	,State.door,State.Power6v,State.Power12v
	,State.Power24Iv,State.Power24IIv);
	prus(col,0);
	sprintf(col,"резерв-%d ибп/сеть-%d  ак-%d "
	,pdio->GetBit(45),State.battery,State.accumul);
	prus(col,40);
}
return 0;
}
void TheApp::Error(u_char in)
{
warning(in);
net(in);
}
//------------------------------------------------
int TheApp::Start(void)
{
pdio = new CShmem;
pdio->init();
return 0;
}
#include "test0.h"
#include "test1.h"
#include "test2.h"
#include "test3.h"
#include "test4.h"
#include "test5.h"
#include "test6.h"
#include "test7.h"
#include "test8.h"
#include "test9.h"
#include "test10.h"
#include "test11.h"
#include "test12.h"
#include "test13.h"
#include "test14.h"
#include "test15.h"
main(int arg, char*  argv[])
{
if(arg==2)
{	
sscanf(argv[1],"%X",&BA);
}else{
perror("Error. Enter correct setup data");
exit(1);
}
uid_t uid,euid;
gid_t gid,egid;
//--------Create groop session--------
euid = setuid(0);
uid = seteuid(0);
gid = setgid(0);
egid = setegid(0);
//setsid();

TheApp App;
App.Start();

while(App.Run());


}

