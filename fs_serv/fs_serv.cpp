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
#include<errno.h>
#include</sys/proxy.h>
#include</sys/psinfo.h>
#include</sys/kernel.h>
#include</sys/types.h>
#include</sys/stat.h>
#include</sys/wait.h>
#include</sys/uio.h>
#include</sys/sched.h>
#include</sys/name.h>
#include</sys/statfs.h>
//---------------------
#include <Cname.h>
#include <data.h>
#include <Shmem.h>
#include <ctrlmes.h>
#include <display.h>
#include <log.h>
#include <cmdproxy.h>
#include <simpmq.h>
#include <commq.h>

#define NDAT 30

//+++++++++++++++++++++++++++++++++++++++++++++
class TheApp : 
public CName, 
public CLog,
public CMQueue
{
int fd;
	time_t time_now;
	struct tm *now;
	char FlagSave,FlagNow;
	int HourSave,MinSave;
		FILE *fdb, *fdnow;
		char *buf;

public:
	CSQueue in;
        TheApp::TheApp(int ,char *[]) ;
 int virtual TheApp::Run(void); 
	int TheApp::OnLoadData(void);	
   	void TheApp::OnInfo(void);
   	void TheApp::space(void);
	void TheApp::OnIdle(void);
	    TheApp::~TheApp();
};

//-------Massege Map begin--------

#define  HALT 		99
#define  LOAD_DATA	1
#define  CONNECT	2
#define  DISCONNECT	3
#define  IDLE 		0
//-------Massege Map end----------
//-------Таблица кодов ошибок начало--------

//-------Таблица кодов ошибок конец--------

TheApp::TheApp(int arc, char *args[] ): 
CName(args[0],7), 
CLog(args[0],LOG_LOCAL0),
CMQueue( args[0] )
{
HourSave = atoi(args[1]);
MinSave = atoi(args[2]);
//----------------- mesege-------------------
in.Init("fs_in",50,sizeof(CData));
//------Open Archiv File--------
system("gzip -c -3 /usr/db/akro.db > /usr/db/akro.db.gz");
//char pass[20];
//strcpy( pass, getenv("DB") );
//strcat(pass,"/usr/db/akro.db");
fdb=fopen("/usr/db/akro.db","w+");
if(arc==4)
{
	if(atoi(args[3])<1000)
		buf = new char [atoi(args[3])*sizeof(CData)];
}else{
	buf = new char [20*sizeof(CData)];
}
setbuf(fdb,buf);

FlagNow = 0;
FlagSave = 0;
//-----------------------------------
close(0);
close(1);
close(2);
};
//-----Main Loop----------
int TheApp::Run(void)
{
ifgetmes();
switch( Comand ){
	case HALT  		: exit(0);
	case IDLE 	:
	default   	: OnIdle();
				break;
	} 
return 1;
}
//-----------------------
void TheApp::space(void)
{
int fl,flag;
struct statfs fs;
float per;
statfs("/", &fs, fl, flag);
per=(float)(fs.f_bfree*100./fs.f_blocks);
if(per<10.)
	system("savedat");
}
//-----------------------
void TheApp::OnIdle(void)
{
CData dat;
in.getmes((CData *) &dat);
fwrite(&dat,sizeof(dat),1,fdb);
if(FlagNow)
	fwrite(&dat,sizeof(dat),1,fdnow);
time(&time_now);
now=localtime(&time_now);
if( (now->tm_hour>HourSave)&&(FlagSave==1))FlagSave=0;
if( (now->tm_hour==HourSave)&&(now->tm_min>=MinSave)&&(FlagSave==0) )
		{
		fflush(fdb);
		system("gzip -c -3 /usr/db/akro.db > /usr/db/akro.db.gz");
		FlagSave = 1;
		fseek(fdb,0,SEEK_SET);
		space();
		}
}
//-----------------------
TheApp::~TheApp()
{
close(fd);
delete(buf);
}
//-----------------------

main(int arg, char*  argv[])
{
if(arg<3)
	{
	perror("Error. Enter correct setup data");
	exit(1);
	}

TheApp App(arg,argv);

while(App.Run());

}

