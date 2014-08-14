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
#include <mediana.h>
#include <ctrlmes.h>
#include <display.h>
#include <log.h>
#include <cmdproxy.h>
#include <simpmq.h>
#include "Sh.h"

#define BAx 0xA550
#define ON 	1
#define OFF 0

class TheApp : 
public CName, 
public CShmem,
public CLog,
public Proxy, 
public CDisp 
{
private:
	CTerm dat;
	double Line, Corr;
	int Comand, ConFlag, Auto;
public:
	CSQueue ctrl;
	
        TheApp::TheApp(char *[]) ;
	int TheApp::Run(void); 
	int TheApp::SendDat( void );
   	int TheApp::OnInfo(void);
   void TheApp::DistOnOff(int key);
	int TheApp::OnDisp(void);
	int TheApp::OnIdle(void);
	    TheApp::~TheApp();
};

//-------Massege Map begin--------
#define  HALT 		99
#define  GET_INFO	5
#define  DISPON		8 //2
#define  DISPOFF	7 //1
#define  AUTO		4
#define  AUTOOFF	3
#define  IDLE 		0
//-------Massege Map end----------
//-------Таблица кодов ошибок начало--------

#define  TALARM		1 /*превышение температуры БНО*/
#define  TOBR		2 /*обрыв сопротивления*/

//-------Таблица кодов ошибок конец--------

TheApp::TheApp( char *args[] ): 
CName(args[0],5), 
CShmem (26),
CLog(args[0],LOG_LOCAL0),
Proxy(TEMOUT,TEMIN),
CDisp()
{

ctrl.Init("termr_com",5,sizeof(int));
InitProxy();

Line = strtod(args[1],0);
Corr = strtod(args[2],0);

Auto = ON;
ConFlag = OFF;

};
//-----Main Loop----------
int TheApp::Run(void)
{

if( !ctrl.ifgetmes((int *) &Comand) ) Comand=IDLE;
switch( Comand  ){
	case HALT  	: 
				exit(0);
	case GET_INFO 	: 
				SendDat();
				break;
	case DISPON	:
				clrscr();
				ConFlag = ON;
				break;
	case DISPOFF:
				clrscr();
				ConFlag = OFF;
				break;
	case AUTO	:
				DistOnOff(0);
				break;
	case AUTOOFF:
				DistOnOff(1);				
				break;
	case IDLE 	:
	default		:
				OnIdle();
				break;
	} 
return 1;
}
//------------------------
void TheApp::DistOnOff(int key)
{
if(key==1)
{
if(ConFlag==OFF)
{
 sem_wait(&p->lock);
 ConFlag = ON;
 Auto = OFF;
 Comand=IDLE;
}
}else{
if(ConFlag==ON)
{
 sem_post(&p->lock);
 ConFlag = OFF;
 Auto = ON;
 Comand=IDLE;
}
}

}
//--------send mess------------------------
int TheApp::SendDat( void )
{
dat.Time=time(NULL);
dat.Ventil=(short)Power;
SendProxy((CTerm *) &dat , sizeof(dat) );
return 0;

}
//-----------------------
int TheApp::OnIdle(void)
{
u_int word;
int i;
float rez;
int Adr;
Adr=0xA400+0x150;

if(!ConFlag)
{

for(i=0;i<3;i++)
{
if( ctrl.ifmes() ) return 1;
sleep(1);
}

}	
//last_error=0;
//-----------------------
outp( Adr + 4 , 0x97 );
while( (inp(Adr)&0x40) != 0x40 );

if( (inp(Adr)&0x20) == 0 )
	{

	word=inpw(Adr+6);
	if(word!=0)
	{
	rez=(float)word;
	dat.Dat = (double)(80.31 + ( (4.0*50000.0/rez) - 14.4 )*(229.67-80.31)/(72.0-14.4) );
//	dat.Dat = (double)(80.75 + ( (4.0*50000.0/rez) - 14.4 )*(234.75-80.75)/(72.0-14.4) );
//	dat.Dat = (double)(80.75 + ( (4.0*50000.0/rez) - 14.4 )*(363.8-80.75)/(72.0-14.4) );
	dat.Dat = ( dat.Dat - 100 )/0.385;
	}else{
	dat.Dat=0;
	}
	if( dat.Dat >= 50.0 )
			{
//			dat.Dat = 0;
			warning(TALARM);
			}
			
	}else{
	dat.Dat = 0;
	warning(TOBR);
		if((inpw(Adr+6)&0x80)==0x80)
			dat.Dat = 0;
		
	}	

if(Auto)
{
//if(!last_error)
	if(Power)
	{
		if( dat.Dat<(Line-Corr) )
			ShOffPower();
	}else{
		if( dat.Dat>(Line+Corr) )
			ShOnPower();
	}
	
//}else{
					
}
			
OnDisp();
return 1;

}
//-----------------------
int TheApp::OnDisp(void)
{
int k;
//--------------Display----------------------
  if(ConFlag == ON)
	{
			
			 sprintf(col,"вентилятор-%d", Power);
			 prusl(col,0);			
			sleep(1);
			 sprintf(col,"T=%2.1f ", dat.Dat);
			 prusl(col,40);			
			 
	//---------Key board--------------
	if(Auto)
	if(kbhit())
		{
					k=getch();
						if(k==0xFF)k=getch();
					 switch((char)k)
						{
						case '7':
						case 'm':
							
								if(Power==ON)
									{
										ShOffPower();
									}else{
										ShOnPower();
									}
								break;
						case Esc:
						case 'e':
								DistOnOff(0);
								return 1;
						}
		}
	}
	return 0;
}
//-----------------------
TheApp::~TheApp()
{
}
//-----------------------

main(int arg, char*  argv[])
{
if(arg<3)
	{
	perror("Error. Enter correct setup data");
	exit(1);
	}
TheApp App(argv);
while(App.Run());

}

