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

//-----defined in alex---------
#include <Cname.h>
#include <data.h>
#include <Shmem.h>
#include <mediana.h>
#include <ctrlmes.h>
#include <display.h>
#include <log.h>
#include <timer.h>
#include <cmdproxy.h>
#include <simpmq.h>
#include <commq.h>

//#define DEBUG

class TheApp
		   :public CName, 
			public CLog,
			public Proxy,
			public CDisp,
			public CTimer, 
			public CMQueue
{
private:

	time_t t;
	tm *mtime;
	int hour,min;
	
	StateUps Data;
	
    struct sigevent event;
	time_t id_timer;
    struct itimerspec timer;
	int Count, TestFlag,FlagDisp;
public:

	
        TheApp::TheApp(char *[]) ;
int virtual TheApp::Run(void); 
   void TheApp::Cmd(char );
   void TheApp::OnInfo(void);
   void TheApp::State(void);
   void TheApp::BackThread(void);
   void TheApp::Disp(void);
	int TheApp::OnIdle(void);
	    TheApp::~TheApp();
};

//-------Massege Map begin--------
#define  HALT 		99
#define  DISP		1
#define  GET_INFO	5
#define  IDLE 		0
//-------Massege Map end----------
//-------������ ����� �訡�� ��砫�--------
#define  PWOFF		1 /*ᨣ��� �ய������ �室���� ����殮���*/
#define  PWON		2 /*ᨣ��� ����⠭������� �室���� ����殮���*/
#define  BTLOW		3 /*ᨣ��� ࠧ�鸞 ���२*/
#define  BTFAULT	4 /*����� �� ��ࠢ��*/
//-------������ ����� �訡�� �����--------
//-------������ ����� ������� � UPS--------
#define  LINE 	'D'//����ﭨ� �室��� �����
#define  STATE  'Q'//����ﭨ� ���
#define  TEMP  	'C'//���������
#define  UMAX 	'M'//����殮��� �室��� ����� ���ᨬ��쭮� ����� ����७�ﬨ
#define  UMIN 	'N'//����殮��� �室��� ����� �������쭮� ����� ����७�ﬨ
#define  UIN	'L'//����殮��� �室��� �����
#define  UOUT	'O'//����殮��� ��室��� �����
#define  UBAT 	'B'//����殮���  ���२
#define  CHBAT	'X'//����室������ �������� �����
#define  PBAT 	'f'//���� ���२
#define  TBAT  	'j'//�।���������� �६� ࠡ���  ��  ���३
#define  FREQ 	'F'//����� ��室��� �����
#define  PUPS	'P'//����㧪� ��魮��
#define  SELF 	'W'//������� ᠬ��஢�ન ���
//----------
static char PortBuf[300];
static int fdcom;	
//-------------------------
void alrmint(int sig)
{
if(sig==SIGALRM)
{}
}

TheApp::TheApp( char *args[] )
								:CName(args[0],5), 
								CLog(args[0],LOG_LOCAL0),
								Proxy(0,UPS),
								CDisp(),
								CTimer(), 
								CMQueue( args[0] )
{
int i;
InitProxy();
	
hour=atoi(args[1]);
min=atoi(args[2]);

//----------------------------------------------------------------
	for( i = _SIGMIN; i <= _SIGMAX; ++i)
		if( i != SIGTERM  &&  i != SIGPWR)
			signal( i, SIG_IGN );
/*			
	signal( SIGUSR1, &alrmint );

   event.sigev_signo  =SIGUSR1;
   id_timer = timer_create( CLOCK_REALTIME, &event );
    if( id_timer == -1 ) 
	{
	 cout<<"Unable to attach timer."<<endl; 
	}
	timer.it_value.tv_sec 		=2L;
	timer.it_value.tv_nsec	 	=0L;
	timer.it_interval.tv_sec 	=2L;
	timer.it_interval.tv_nsec 	=0L;
//----------------------------------------------------------------
*/
fdcom=open("/dev/ser2",O_RDWR|O_NONBLOCK);
close(0);
close(1);
close(2);
//----------------------------------------------------------------

Count=0;
FlagDisp=0;
TestFlag=1;
State();
//memset(&Data, 0,sizeof(StateUps));

};
//-----Main Loop----------
int TheApp::Run(void)
{

ifgetmes();
switch( Comand  ){
	case HALT  		: 	exit(0);
	case GET_INFO 	: 	OnInfo(); break;
	case DISP 		:  	if(FlagDisp==0){
							FlagDisp=1;
						}else{
							FlagDisp=0;
							}
						break;
	case IDLE 		:
	default			:	OnIdle(); break;
	} 
return 1;
}
//--------send mess------------------------
void TheApp::Cmd(char cmd)
{
write(fdcom,&cmd,1);
timer_setup(0,0,SEC/2,SEC/2);
read(fdcom,PortBuf,sizeof(PortBuf));

}
void TheApp::State(void)
{
int count,i,Ut;

char C[]="Y9QCMNLOBXfjF";
for(count=0;count<strlen(C);count++)
{
Cmd(C[count]);
switch (count) { // ������ ���ﭨ� ���
           case  0: // Y - ��砫쭠� ���樠������ �������
              if (strstr(PortBuf,"SM")==NULL) { // �⢥� ���?
                i=15;                      // ����� �������
                Data.Self=0;                 // ��� ����ࠢ��
              }
            break;
            case  1: Data.Line=(strcmp(PortBuf,"FF")!=0)? 0:1; // ����殮��� �� �室�
                     break;
            case  2: // Q - ������騥 �⨫��� ���:
              Ut=atoh(PortBuf);
              if (Ut & 0x08) Data.State=0; // �ᯮ�짮���� �室��� ����殮���
            //if (Ut & 0x??) Data.State=1; // ��������� ����殮���
              if (Ut & 0x04) Data.State=2; // ����襭�� ����殮���
              if (Ut & 0x10) Data.State=3; // ࠡ�� �� ���२
              if (Ut & 0x80) {
                Data.State=4; // ࠧ�� ���२
                Data.Self=0; // ����� ����ࠢ��
                warning(BTFAULT);
                if (Data.Line==0) warning(PWOFF); // �室���� ����殮��� ���
	           }else{
                	Data.Self=1; // ����� � ���浪�
				}
				   break;
            case  3: Data.Temp=atof(PortBuf); // ���������, ��
                     break;
            case  4: Data.UMax=atof(PortBuf); // ����殮��� �室��� ���ᨬ��쭮� ����� ����७�ﬨ, �
                     break;
            case  5: Data.UMin=atof(PortBuf); // ����殮��� �室��� �������쭮� ����� ����७�ﬨ, �
                     break;
            case  6: Data.UIn=atof(PortBuf); // ����殮��� �室��� �����, �
                     break;
            case  7: Data.UOut=atof(PortBuf); // ����殮��� ��室��� �����, �
                     break;
            case  8: Data.UBat=atof(PortBuf); // ����殮��� ���२, �
                     break;
            case  9: Data.Self=(strstr(PortBuf,"NO")==NULL)? // ������ �����?
                                              0:1; // (���?/��?)
                     break;
            case 10: Data.PBat=atof(PortBuf); // ���� ���२, %
                     break;
            case 11: Data.T=(short)atoi(PortBuf);
                     // �।���������� �६� ࠡ��� �� ���३, ���
                     break;
            case 12: Data.F=atof(PortBuf); // ����� ��室��� �����, ��
                     break;
            case 14: // १���� ��� ��������㥬 �� ᫥�.⠪�
            break;
          } //switch
}
t=time(NULL);
mtime=localtime(&t);
if(mtime->tm_hour==hour)
if(mtime->tm_min>=min)
{
	if(TestFlag)
		{
			Cmd('W');
			TestFlag=0;
		}
}
if(mtime->tm_hour==(hour+1))TestFlag=1;

}
void TheApp::OnInfo(void)
{
SendProxy((StateUps *)&Data , sizeof(StateUps) );
}
//-----------------------
int TheApp::OnIdle(void)
{
strcpy(PortBuf,"");
read(fdcom,PortBuf,sizeof(PortBuf));
		switch(PortBuf[0])
		{
			case '#': 	// ᨣ��� �������
			case '*': 	// ᨣ��� ����砭�� �������
			case 240: // ᨣ��� ������������ �몫�祭��
			case '%': // �����祭�� ������⭮
			case '+': // -"-
						break;
			case '!': // ᨣ��� �ய������ �室���� ����殮���
						warning(PWOFF);
						break;
			case '?': // ᨣ��� ࠧ�鸞 ���२
						warning(BTLOW);
						break;
			case '$': // ᨣ��� ����⠭������� �室���� ����殮���
						warning(PWON);
						break;
		}
Disp();
Count++;
if(Count==300){
	Count=0;
	State();
	}
timer_setup(1,1);	
return 1;
}
//----------
void TheApp::Disp(void)
{
if(FlagDisp)
{
sprintf(col,"���ﭨ� ��� %d �=%d T=%d��� t=%-2.1f",
Data.State,Data.ChBat,Data.T,Data.Temp);
prusl(col);
sprintf(col,"Ui=%-3.1f� Uo=%-3.1f� Ub=%-3.1f� F=%-2.1f�� ",
Data.UIn,Data.UOut,Data.UBat,Data.F);
prusl(col,40);
}
}
//-----------------------
TheApp::~TheApp()
{
close(fdcom);
}

//-----------------------
main(int arg, char*  argv[])
{
if(arg<2)
	{
	perror("Error. Enter correct setup data");
	exit(1);
	}
TheApp App(argv);
while(App.Run());

}

