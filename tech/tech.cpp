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
#include<dirent.h>
//---------------------

#include <Cname.h>
#include <data.h>
#include <log.h>
#include <myerr.h>
#include <cmdproxy.h>
#include <display.h>
#include "hash.h"
#include "errcode.h"
#include "/home/root/project/bdmg/errcode.h"
#include "/home/root/project/bdmg/cmdcode.h"
#include <commq.h>
#include <simpmq.h>

extern void alarmint(int sig);
static int AlrmFlag;

#define LHSCL 0.0001
#define MHSCL 0.01
#define HHSCL 10

#define SLEN 15

class TheApp :	
	public CName, 
	public CLog,
	public CDisp,
	public Proxy, 
	public CMQueue
{
private:
	pid_t pid;

	struct dirent *d;
	DIR *dr;
	WCValDList<CList> list;

	u_int mq_prio;
	struct mq_attr mq_attrs;
	
	ITech inf;

int 	Cur,   //Currient count
	 	Auto,	//Automatic control of the sensor
    	Dist;	//Distance control by the sensor
float 	OffL,	//Level off low chanel
      	OnL,	//Level on low chanel
      	OffM,	//Level off medium chanel
      	OnM,	//Level on medium chanel
      	OffH,	//Level off hight chanel
		Alarm,PAlarm;

    struct sigevent event;
    struct sigaction sigact;
	sigset_t sigset;
	
int	table[NG];
int	state[NG][3];
CData last_dat[NG][3];
u_char		SW;
int NS;


public:
	CSQueue in,
			err;

             TheApp::TheApp();
	   pid_t TheApp::Link( const char *);		
 int virtual TheApp::Run(void); 
	int TheApp::OnLoadData(void);	
	int TheApp::OnMetrologyTest(void);
   	int TheApp::OnInfo(void);
	int TheApp::OnIdle(void);
	int TheApp::GetMes(void);
	void TheApp::Start(void);
	void TheApp::SendCom(const char *, int );
	int TheApp::Control(CList *, CData *);
	CList TheApp::SeachSen(char *, char *);
	CList TheApp::SeachSen( int , int );
	int TheApp::Disp(void);
	    TheApp::~TheApp();
};

//-----------------------------------
TheApp::TheApp( ):
	CName("tech",5), 
	CLog("tech",LOG_LOCAL0),
	CDisp(),
	Proxy(TECHOUT,TECHIN),
	CMQueue( "tech" )
{
int i,j;

sigemptyset( &sigact.sa_mask );
sigaddset( &sigset, SIGUSR1 );
sigact.sa_handler = alarmint;
sigaction(SIGUSR1,&sigact,NULL);
//----------------- mesege-------------------
in.Init("tech_in",20,sizeof(CData));
err.Init("tech_err",20,sizeof(CCom));
InitProxy();
//---------------------------------------------
		Cur	 = 0;
	 	Auto = 1;	
    	Dist = 0;	
		Comand 	= 0; 
      	OffL	= 1.4*LHSCL;	//Level off low chanel
      	OnL		= 1.1*LHSCL;	//Level on low chanel
      	OffM	= 1.4*MHSCL;	//Level off medium chanel
      	OnM		= 1.1*MHSCL;	//Level on medium chanel
      	OffH	= 1.1*HHSCL;	//Level off hight chanel
		
		Alarm	= 2.0952e-06;
		PAlarm	= 5.2380e-07;


close(0);
close(1);
close(2);

AlrmFlag = 0;

//sleep(5);
sleep(7);
char name[35];
char cod[10],xcod[10];
dr=opendir(MQPATH);
while( (d = readdir(dr) ) != NULL )
	{
	 if( (strstr(d->d_name,"_com")!= NULL) 
	 	&&(strstr(d->d_name,"bdmg") != NULL) )
		{	
		
			strcpy(cod,d->d_name);
			strrev(cod);
			strcpy( cod,strchr(cod,'_') );
			strrev(cod);
			cod[strlen(cod)-1]=0;
			strcpy(name,DPATH);
			strcat(name,cod);
			strcat(name,".dat");
			fstream file_dat(name,ios::in,100);
			while(!file_dat.eof())
			{
			 file_dat.getline(xcod,10);
			 if( strstr(xcod,"XS")!= NULL )
			 	{
					CList one(d->d_name,xcod,cod);
					list.append(one);
				}
			}
		}
	}

closedir(dr);

char str[3];
CList obj;
NS=23;
obj.gr=-1;
obj.nb=-1;
strcpy(obj.name,"");
strcpy(obj.bdmg,"bdmg23");
while( (list.index(obj) == -1)&&(NS > 0) )
{
NS--;
obj.gr=-1;
obj.nb=-1;
strcpy(obj.name,"");
strcpy(obj.bdmg,"bdmg");
itoa(NS,str,10);
strcat(obj.bdmg,str);
}
NS=(NS+1)/4;
for(i=0;i<NG;i++){
	table[i]=0;
	for(j=0;j<3;j++){
		state[i][j]=1;
		memset(&last_dat[i][j], 0,sizeof(CData));
		}
		
	}


};
void TheApp::Start(void)
{
}
//-----Main Loop----------
int TheApp::Run(void)
{
ifgetmes();
switch( Comand  )
	{
	case HALT  		: exit(0);
	case SET_INFO	: OnLoadData();			break;
	case GET_INFO 	: OnInfo(); 			break;
	case DISP 		: Disp();				break;
	case IDLE 		:
	default   		: OnIdle();				break;
	} 
return 1;
}
//------------------------------
int TheApp::OnLoadData(void)
{

ResvProxy((ITech *) &inf , sizeof(inf) );
	 	Auto 	= inf.Auto;	
    	Dist 	= inf.Dist;	//Distance control by the sensor
      	OffL 	= inf.OffL;	//Level off low chanel
      	OnL 	= inf.OnL;	//Level on low chanel
      	OffM 	= inf.OffM;	//Level off medium chanel
      	OnM 	= inf.OnM;	//Level on medium chanel
      	OffH 	= inf.OffH;	//Level off hight chanel

return 1;
}
//------------------------
int TheApp::OnInfo(void)
{
	 	inf.Auto 	= Auto;	
    	inf.Dist 	= Dist;	//Distance control by the sensor
      	inf.OffL 	= OffL;	//Level off low chanel
      	inf.OnL 	= OnL;	//Level on low chanel
      	inf.OffM 	= OffM;	//Level off medium chanel
      	inf.OnM 	= OnM;	//Level on medium chanel
      	inf.OffH 	= OffH;	//Level off hight chanel

SendProxy((ITech *)&inf , sizeof(inf) );
return 0;
}
//----------------------------------------------------------
CList TheApp::SeachSen(char *cod, char *name )
{
int i;
CList obj;
strcpy(obj.name,cod);
strcpy(obj.bdmg,name);
obj.gr=-1;
obj.nb=-1;
i = list.index(obj);
return	obj = list.find(i);
}
//----------------------------------------------------------
CList TheApp::SeachSen( int GR, int NB )
{
int i;
CList obj;
strcpy(obj.name,"");
strcpy(obj.bdmg,"");
obj.gr=GR;
obj.nb=NB;
i = list.index(obj);
return	obj = list.find(i);
}
//----------------------------------------------------------
void TheApp::SendCom(const char *cod, int Mes )
{
int i;
CList obj;
strcpy(obj.name,cod);
	if( (i = list.index(obj)) != 0 );
		{
		obj = list.find(i);
				obj<<Mes;
		}
}
//----------------------------------------------------
int TheApp::GetMes(void)
//обработка сообщений 
{
CList obj,xobj;
CCom mes;
int Flag; //изменение состояния канала
while(err.ifmes())
{
err.getmes((CCom *)&mes);
obj=SeachSen("",mes.name);
	if( obj.mq != -1 )
	{
	switch(mes.com)
		{
			case OffSensor	: 
			case OnBlenk	: 
							if( (state[obj.gr][obj.nb]==0) )
							{
								Flag=0;
							}else{
								Flag=1;
								state[obj.gr][obj.nb]=0;
							}
							break;	
			case OnSensor	: 
			case OffBlenk	: 
							if( (state[obj.gr][obj.nb]==1) )
							{
								Flag=0;
							}else{
								Flag=1;
								state[obj.gr][obj.nb]=1;
							}
							break;	
		}
		
		if(Flag==1)
		{
			switch(obj.nb)
				{
					case 1: 
							if(state[obj.gr][obj.nb]==0)
							//выключение дистанционного управления
							{
								xobj=SeachSen( obj.gr,0);
								xobj<<DISTOFF;
							}
							if(state[obj.gr][obj.nb]==1)
							//включение дистанционного управления
							{
								xobj=SeachSen( obj.gr,0);
								xobj<<DISTON;
							}
							break;	
					case 2: 
							if(state[obj.gr][obj.nb]==0)
							//выключение дистанционного управления
							{
							xobj=SeachSen( obj.gr,1);
							xobj<<DISTOFF;
							}
							if(state[obj.gr][obj.nb]==1)
							//включение дистанционного управления
							{
							xobj=SeachSen( obj.gr,1);
							xobj<<DISTON;
							}
				}
		}
	}
}//конец обработки сообщений
return 1;
}
//-------------------------------------------------
int TheApp::Control(CList *ob, CData *dt)
//управление высоким напряжением 
{
CList xobj;

//проверка данных на управление
if( dt->State&PWb )return 0;
if( dt->State&MTb )return 0;
if( dt->State&BLb )return 0;
if( dt->State&MTb )return 0;

	switch(table[ob->gr])
		{
		case 0 :
		
				if(ob->nb==1)
					if(dt->Dat>=OffL)
						{
							if(state[ob->gr][0]==1) 
								{
								table[ob->gr]++;
								xobj=SeachSen( ob->gr,0);
								xobj<<7;
//								state[ob->gr][0]=0;
								}
						}
				break; 
		case 1 :
				if(ob->nb==1)
					if(dt->Dat<=OnL)
						{
							if(state[ob->gr][0]==0) 
								{
								table[ob->gr]--;
								xobj=SeachSen( ob->gr,0);
								xobj<<8;
//								state[ob->gr][0]=1;
								}
						}
				if(ob->nb==2)
					if(dt->Dat>=OffM)
						{
							if(state[ob->gr][1]==1) 
								{
								table[ob->gr]++;
								xobj=SeachSen( ob->gr,1);
								xobj<<7;
//								state[ob->gr][1]=0;
								}
						}
				break; 
		case 2 :
				if(ob->nb==2)
					if(dt->Dat<=OnM)
						{
							if(state[ob->gr][1]==0) 
								{
								table[ob->gr]--;
								xobj=SeachSen( ob->gr,1);
								xobj<<8;
//								state[ob->gr][1]=1;
								}
						}
				break; 
		}
return 0;
}
//-------------------------------------------------
int TheApp::OnIdle(void)
{
CData dat;
CList obj;
int nom;

GetMes();
//начало управления
while(in.ifmes())
{
in.getmes((CData *)&dat);
obj=SeachSen(dat.Name,"");
if(obj.nb==0)
	memcpy(&last_dat[obj.gr][obj.nb],&dat,sizeof(CData));
if( (obj.mq != -1)&&(obj.nb!=3)&&(obj.nb!=0) )
nom=4*obj.gr+obj.nb;
Control(&obj,&dat);
}//конец управления
sleep(1);
return 0;
}
//-----------------------
int TheApp::Disp(void)
{
int i;
clrscr();
for(i=0;i<NS;i++)
{
if(last_dat[i][0].Dat!=0){
sprintf(col,"тк%c мпд=%-1.2e гр/ч",
	last_dat[i][0].Name[6],last_dat[i][0].Dat);
prus(col,0);			
if(last_dat[i][0].Dat >= PAlarm)
	{
	if(last_dat[i][0].Dat >= Alarm){
		prusl("превышена аварийная уставка",40);			
		}else{
		prusl("превышена контрольная уставка",40);			
		}
//	break;
	}else{
	prusl("норма",40);			
	}
sleep(2);
}else{
sprintf(col,"данных нет");
prus(col,0);			
sleep(1);
}

	
}
 sem_post(&p->lock);
 return 0;
}
//-----------------------
TheApp::~TheApp()
{
}
//-----------------------
void alarmint(int sig)
{
	if(sig==SIGALRM)
		{
		AlrmFlag = 1;
		}
}

main()
{
TheApp App;
App.Start();
while(App.Run());

}

