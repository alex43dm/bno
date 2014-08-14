#include<conio.h>
#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<process.h>
#include<stddef.h>
#include<iostream.h>
#include<fstream.h>
#include<ctype.h>
#include<fcntl.h>
#include<math.h>
#include<errno.h>
#include</sys/name.h>
#include</sys/proxy.h>
#include</sys/psinfo.h>
#include</sys/kernel.h>
#include</sys/types.h>
#include</sys/stat.h>
#include</sys/wait.h>
#include</sys/uio.h>
#include</sys/sched.h>
#include<mqueue.h>
#include <ds.h>

//-----defined in alex---------
#include <Cname.h>
#include <data.h>
#include "bdmg.h"
#include <mediana.h>
#include <ctrlmes.h>
#include <display.h>
#include <log.h>
#include <myerr.h>
#include <cmdproxy.h>
#include "bdmgsh.h"
#include "errcode.h"
#include "cmdcode.h"
#include <simpmq.h>
#include <commq.h>
//#include "db.c"

#define PER 1
//#define BAx 0xA190
#define DEBUG

//класс бдмг
//static int Comand;
extern void timerint(int sig);
static u_int st,sti;
static u_int ADR;
static u_char BNK;
static SBAx; //0xA190

class TheApp: 	
				public CName, 
				public CBdmgData,
				public CShmem,
				public CDisp,
				public Proxy,
				public CLog,
				public CMQueue,
				public MEDIANA
//				,public DB
				 
{
private:
	u_int i,Start;
	int Time;
	int ConFlag,
		MetrologyFlag,
		DistFlag,
		DistCount,
		SwFlag;

	u_int M;
	double Math;
	u_char NulCount; 
	CData dat,savedat;

	u_int mq_prio;
	struct mq_attr mq_attrs;

	time_t t,id_timer;
	tm *mtime;
    struct itimerspec timer;
    struct sigevent event;
    struct sigaction sigact;
	sigset_t sigset;

	u_int kdv;
	char kdc,kdx;
	
//ds var
  ds_t ds_descriptor;
public:

	mqd_t 	mq_arj,
			mq_info,
			mq_tec,
			mq_err,
			mq_net0,	
			mq_net1,
			mq_ctrl;	


        TheApp::TheApp(char *[]);
int virtual TheApp::Run(int); 
	int TheApp::OnMeter(void);	
	int TheApp::OnTest(void);
	int TheApp::OnCBlenk(int );
	int TheApp::OnSetInfo(void);	
	int TheApp::OnMetrologyTest(void);
   	int TheApp::OnGetInfo(void);
   	int TheApp::Error(u_char);
	int TheApp::CheckChanel(const u_int );
   	int TheApp::SendDat(mqd_t, CData &);
	int TheApp::OnOff(int);
    int TheApp::AutoOnOff(int);
	int TheApp::Dialog(void);
	int TheApp::SetState(void);
	int TheApp::KDifferent(u_int xx);
	int TheApp::KeyBd(void);
	int TheApp::Display(void);
	int TheApp::Along(void);
    int TheApp::OnIdle(void);
	    TheApp::~TheApp();
};

TheApp::TheApp( char *args[] ): 
						CName(args[0],12,SCHED_RR), 
						CLog(args[0],LOG_LOCAL1),
						CMQueue( args[0] ),
						 CDisp(),
						 CBdmgData(),
						CShmem ( atoi(args[3]), atoi(args[4]) ),
						 Proxy(BDMG_INFO_IN,BDMG_INFO_OUT),
						 MEDIANA()
//						 ,DB(args[0])
						 
						
{
short i;

//----------Attach to the timer-------
/*
sigemptyset( &sigact.sa_mask );
sigaddset( &sigset, SIGUSR1 );
sigact.sa_handler = timerint;
sigaction(SIGUSR1,&sigact,NULL);
*/
	for( i = _SIGMIN; i <= _SIGMAX; ++i)
		if( i != SIGTERM  &&  i != SIGPWR)
			signal( i, SIG_IGN );
			
	signal( SIGUSR1, &timerint );

   event.sigev_signo  =SIGUSR1;
   id_timer = timer_create( CLOCK_REALTIME, &event );
    if( id_timer == -1 ) 
	{
	 cout<<"Unable to attach timer."<<endl; 
	}
	timer.it_value.tv_sec 		=1L;
	timer.it_value.tv_nsec	 	=0L;
	timer.it_interval.tv_sec 	=1L;
	timer.it_interval.tv_nsec 	=0L;
//----------------- mesege-------------------


 while ( (mq_arj=mq_open( "fs_in", O_WRONLY )) == -1 )		sleep(1);
 while ( (mq_net0=mq_open( "net.dat1", O_WRONLY )) == -1 )	sleep(1);
 while ( (mq_net1=mq_open( "net.dat2", O_WRONLY )) == -1 )	sleep(1);
 while ( (mq_tec=mq_open( "tech_in", O_WRONLY )) == -1 )	sleep(1);
 while ( (mq_err=mq_open( "tech_err", O_WRONLY )) == -1 )	sleep(1);
 InitProxy();
	
//if( !LoadData(Name) )OnOff(OFF);
LoadData(Name);


strcpy( dat.Name, Cod );
strcpy(savedat.Name, Cod);
/*
//-------------create ds var------------
char flag=0;
  ds_descriptor = ds_register();
  if(ds_descriptor==-1){
    perror("ds_register");
  }
 if(ds_create(ds_descriptor, Name, flag, 0)==-1)
		perror("ds_create");
*/  
//filter define
K=(1.33-1.10)/(ScaleH-ScaleL);
Km=3.0;

close(1);
close(2);
//--------First test the sensor------
kdv=0;
Time = 0;
M=0;Math=0;
dat.T = 0;
kdc = 0;
ConFlag = OFF;
Dist=ON;
MetrologyFlag=OFF;
DistFlag=OFF; //флаг включения превышения при Dist=OFF 
SwFlag=OFF;//флаг грей-рентген
NulCount=0; 
t=time(NULL);
mtime=localtime(&t);
memcpy(&savedat.Time,mtime,sizeof(tm));
Auto = ON;
Start = 1;
outp( SBAx, BNK ); 
sti=(u_int)inpw( SBAx + ADR );
timer_settime( id_timer, 0, &timer, NULL );

};
//-----Main Loop----------
int TheApp::Run(int SWITCH)
{
ifgetmes();
if(SWITCH==1)return 0;
	
switch( Comand  ){
	case HALT  			: exit(0);
	case METER 			: OnMeter();			break;
	case TEST			: OnTest();				break;
	case SET_INFO		: OnSetInfo();			break;
	case METROLOGY_TEST	: OnMetrologyTest();	break;
	case GET_INFO		: OnGetInfo();			break;
	case OFFBLK			: OnCBlenk(OFF);		break;
	case ONBLK			: OnCBlenk(ON);			break;
	case OFFD			: OnOff(OFF);			break;
	case OND			: OnOff(ON);			break;
	case AUTO			: AutoOnOff(ON);		break;
	case AUTOOFF		: AutoOnOff(OFF);		break;
	case DISTON			: Dist = ON;			break;
	case DISTOFF		: Dist = OFF;			break;
	case DISPON			: ConFlag = ON;			break;
	case DISPOFF		: ConFlag = OFF;		break;
	case LASTDAT		: if(Time){Time=OFF;}else{Time=ON;}	break;
	case SWDAT			: if(SwFlag){SwFlag = OFF;}else{SwFlag=ON;}	break;
	case IDLE 			:
	default				: OnIdle();				break;
	} 
return 1;
}
//-----------------------
TheApp::~TheApp()
{
}
//-----------------------
