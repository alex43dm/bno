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
//-------Таблица кодов ошибок начало--------
#define  PWOFF		1 /*сигнал пропадания входного напряжения*/
#define  PWON		2 /*сигнал восстановления входного напряжения*/
#define  BTLOW		3 /*сигнал разряда батареи*/
#define  BTFAULT	4 /*батарея не исправна*/
//-------Таблица кодов ошибок конец--------
//-------Таблица кодов диалога с UPS--------
#define  LINE 	'D'//Состояние входной линии
#define  STATE  'Q'//Состояние ИБП
#define  TEMP  	'C'//Температура
#define  UMAX 	'M'//Напряжение входной линии максимальное между измерениями
#define  UMIN 	'N'//Напряжение входной линии минимальное между измерениями
#define  UIN	'L'//Напряжение входной линии
#define  UOUT	'O'//Напряжение выходной линии
#define  UBAT 	'B'//Напряжение  батареи
#define  CHBAT	'X'//Необходимость заменить батарею
#define  PBAT 	'f'//Заряд батареи
#define  TBAT  	'j'//Предполагаемое время работы  от  батарей
#define  FREQ 	'F'//Частота выходной линии
#define  PUPS	'P'//Загрузка мощности
#define  SELF 	'W'//Результат самопроверки ИБП
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
switch (count) { // анализ состояния ИБП
           case  0: // Y - Начальная инициализация диалога
              if (strstr(PortBuf,"SM")==NULL) { // ответа нет?
                i=15;                      // конец диалога
                Data.Self=0;                 // ИБП неисправен
              }
            break;
            case  1: Data.Line=(strcmp(PortBuf,"FF")!=0)? 0:1; // Напряжение на входе
                     break;
            case  2: // Q - Работающие утилиты ИБП:
              Ut=atoh(PortBuf);
              if (Ut & 0x08) Data.State=0; // использовано входное напряжение
            //if (Ut & 0x??) Data.State=1; // понижение напряжения
              if (Ut & 0x04) Data.State=2; // повышение напряжения
              if (Ut & 0x10) Data.State=3; // работа от батареи
              if (Ut & 0x80) {
                Data.State=4; // разряд батареи
                Data.Self=0; // батарея неисправна
                warning(BTFAULT);
                if (Data.Line==0) warning(PWOFF); // входного напряжения нет
	           }else{
                	Data.Self=1; // батарея в порядке
				}
				   break;
            case  3: Data.Temp=atof(PortBuf); // Температура, °С
                     break;
            case  4: Data.UMax=atof(PortBuf); // Напряжение входное максимальное между измерениями, В
                     break;
            case  5: Data.UMin=atof(PortBuf); // Напряжение входное минимальное между измерениями, В
                     break;
            case  6: Data.UIn=atof(PortBuf); // Напряжение входной линии, В
                     break;
            case  7: Data.UOut=atof(PortBuf); // Напряжение выходной линии, В
                     break;
            case  8: Data.UBat=atof(PortBuf); // Напряжение батареи, В
                     break;
            case  9: Data.Self=(strstr(PortBuf,"NO")==NULL)? // Менять батарею?
                                              0:1; // (Нет?/Да?)
                     break;
            case 10: Data.PBat=atof(PortBuf); // Заряд батареи, %
                     break;
            case 11: Data.T=(short)atoi(PortBuf);
                     // Предполагаемое время работы от батарей, мин
                     break;
            case 12: Data.F=atof(PortBuf); // Частота выходной линии, Гц
                     break;
            case 14: // результат теста анализируем на след.такте
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
			case '#': 	// сигнал диалога
			case '*': 	// сигнал окончания диалога
			case 240: // сигнал немедленного выключения
			case '%': // назначение непонятно
			case '+': // -"-
						break;
			case '!': // сигнал пропадания входного напряжения
						warning(PWOFF);
						break;
			case '?': // сигнал разряда батареи
						warning(BTLOW);
						break;
			case '$': // сигнал восстановления входного напряжения
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
sprintf(col,"состояние ибп %d б=%d T=%dмин t=%-2.1f",
Data.State,Data.ChBat,Data.T,Data.Temp);
prusl(col);
sprintf(col,"Ui=%-3.1fв Uo=%-3.1fв Ub=%-3.1fв F=%-2.1fгц ",
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

