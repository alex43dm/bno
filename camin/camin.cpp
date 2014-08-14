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
#include<dirent.h>
#include<math.h>
#include<errno.h>
#include<mqueue.h>
#include<signal.h>
#include</sys/proxy.h>
#include</sys/psinfo.h>
#include</sys/kernel.h>
#include</sys/types.h>
#include</sys/stat.h>
#include</sys/wait.h>
#include</sys/uio.h>
#include</sys/sched.h>
#include</sys/name.h>
#include<setjmp.h>
//---------------------
#include <Cname.h>
#include <data.h>
#include <display.h>
#include <log.h>
#include <Cname.h>
#include <list.h>
#include "/home/root/project/bdmg/cmdcode.h"

#define XGROOP 3
#define BELLS "\007\007\007"
#define MQPASS 	"/dev/mqueue"

static int LastCom;
static int Comand;
static int TimeOut;
static TestMode;

class TheApp :
public CName, 
public CLog,
public CDisp 
{
private:

 	int NSEN;


	time_t t,id, system_begin;
	tm *mtime;
    struct itimerspec timer;
    struct sigevent event;
	
	char 	user[4][20],
			paswd[4][10],
			groop[XGROOP][10];
	int curg; 
	int fd_log;	
	
	WCValDList<CList> list;

public:


             TheApp::TheApp();
 int virtual TheApp::Run(void); 
	int  TheApp::Login(void);
	int TheApp::TestVid(int );
	int TheApp::JobMenu(int );	
	int TheApp::CtrlMenu(int Sw = -1);	
	void TheApp::SaveLog( int );
	int TheApp::Start( void );
	int TheApp::Line1( void );
	int TheApp::Line2( void );
	int TheApp::Config( int );
	void TheApp::OnIdle(void);
   	int TheApp::SendCommand( const char * , int );
	    TheApp::~TheApp();
};
#include "line.h"

//-------Massege Map begin--------

#define  HALT 		99
#define  METER		1
#define  GLINE		5
#define  BDMG0		6
#define  BDMG1		7
//#define  MESS		8
#define  TESTVID    9
#define  TEX    	10
#define  KIP    	11
#define  MAIN    	12
#define  CTRL    	13
#define  IDLE 		0
//-------Massege Map end----------
//-------������ ����� ᮮ�饭�� � �訡��----------
#define  LOGING1		1 //�室 � ��⥬� ०�� ࠡ��
#define  LOGING2		2 //�室 � ��⥬� ०�� ���� ��
#define  LOGING3		3 //�室 � ��⥬� ०�� ����஫� ��
#define  LOGOUT			4 //��室 �� ��⥬�
#define  ELOGIN 		5 //�� �ࠢ���� ���� ��஫�
#define  STOPSYS		6 //��⠭�� ���
//-------����� ⠡���� ����� ᮮ�饭�� � �訡��----------

TheApp::TheApp() :
CName("mlog",5), 
CLog("mlog",LOG_LOCAL0),
CDisp()
{
//--------------------Open Password File---------------------
	fstream file_dat("/etc/config/passwd",ios::in,100);
		file_dat>>groop[0]; 
		file_dat>>groop[1]; 
		file_dat>>groop[2]; 

	sem_init(&p->lock,1,0);
	
fd_log=open("/usr/bdmg/camin.log",O_RDWR);   
system("stty +numlock term=qnx </dev/con1");
Comand = IDLE;
};
//------------------------
//-----Main Loop----------
int TheApp::Run(void)
{

switch( Comand  ){
	case HALT  	: 
				exit(0);
	case GLINE	:
				JobMenu(-1);	
				break;
	case TESTVID: 
				TestVid(-1);	
					break;
	case CTRL	:
				if(!TestMode)CtrlMenu();	
				break;
	case IDLE 	:
	default   	: OnIdle();
				break;
	} 
return 1;
}
//-------------------------
TheApp::~TheApp()
{
}
int TheApp::SendCommand( const char *cod, int cmd)
{
int i;
CList obj;

strcpy(obj.name,cod);
i = list.index(obj);
if( i !=-1 )
{
	obj = list.find(i);
	obj<<cmd;
	return 0;
}
return -1;
}
//--------tests---------------
int TheApp::TestVid( int Sw )
{
int os;

if(Sw==-1)
{
		os = curg;
		do{
		sprintf(col,"��� %d",os+1);
		prusl(col);

		switch(os)
			{
			case 0 :prusl("᪢����� ���",40);					break;
			case 1:	prusl("�ନ஢���� ᨣ����� +6�",HULF);	break;
			case 2:	prusl("�ନ஢���� ᨣ����� +12�",HULF);	break;
			case 3:	prusl("�ନ஢���� ��᮪�������� ᨣ�����",HULF);	break;
			case 4:	prusl("�ਥ� ������� ᨣ�����",HULF);	break;
			case 5:	prusl("�ਥ� ���������� ᨣ�����",HULF);	break;
			case 6:	prusl("����஫� ������ ������ ����殮���",HULF);	break;
			case 7:	prusl("�ਥ� �������� ᨣ�����",HULF);	break;
			case 8:	prusl("�஢�ઠ ��������",HULF);			break;
			case 9:	prusl("�஢�ઠ ����������",HULF);			break;
			case 10:prusl("����� �� ������� �������",HULF);	break;
			case 11:prusl("�஢�ઠ ���� �३�",HULF);			break;
			case 12:prusl("�஢�ઠ RS-232",HULF);				break;
			case 13:prusl("�஢�ઠ RS-485  1-2",HULF);			break;
			case 14:prusl("�஢�ઠ RS-485  3-4",HULF);			break;
			case 15:prusl("��騩 ��� ���",HULF);				break;
			case 16:prusl("��室",HULF);prusl(" ",0);	 		break;
			}
		}while(Kbd(os,16)!=Enter);
		curg =os;
		if(os == 16)
			{
			Comand = LastCom;
			warning(LOGOUT);
			return 1;
			}
			SaveLog(os);
}else{
	os = Sw;
}	
switch(os)
{
case 1:	
case 2:	
case 3:	
case 10:	
	warning(STOPSYS);
	clrscr();
	prus("��������! �ᯮ�짮����� �⮣� ०���",2);
	prus("�ਢ���� � ����襭�� ࠡ��� ���",45);
	sleep(3);
}
os++;
SendCommand( "ctrl_com", os);
clrscr();
sleep(5);
sem_wait(&p->lock);

return 1;
}
//------Job-------------
int TheApp::JobMenu( int Sw  )	
{
int pos;
char str[2],name[15];

if(TestMode)
{
	prus("�������� ��⥬� ?",0);
	prus("�� - ������ ����, ��� - ��",40);
	if( Enter==getkey() )
		{	
			system("startcam");
			exit(1);
		}else{
			return 1;
		}
	
}

for(pos=0;pos<15;pos++)name[pos]=0;
clrscr();
if(Sw==-1)
{
		pos=0;
		do{
			if( pos<(NSEN+1) )
				{
				sprintf(col,"�����⥫�� ����� %d",pos+1);
				prusl(col);
				}
			if( pos==(NSEN+1) )
				prusl("���ﭨ� ������ ���ப�����",0);
			if( pos==(NSEN+2) )
				prusl("���ﭨ� ���",0);
			if( pos==(NSEN+3) )
				prusl("�஢�ઠ �裡",0);
			if( pos==(NSEN+4) )
				prusl("���ﭨ� ���",0);
			if( pos==(NSEN+5) )
				prusl("��室",0);
		}while(Kbd(pos,NSEN+5)!=Enter);


		if(pos==NSEN+5)
			{
			Comand = IDLE;
			warning(LOGOUT);
			return 1;
			}
		SaveLog(pos);
}else{
	pos = Sw;
}

if(pos<NSEN+1)
{	
strncpy(name,"bdmg",4);
itoa(pos,str,10);
strcat(name,str);
strcat(name,"_com");

SendCommand( name, 11 );
prus("  �������");
do{
getkey();
if( (key=='5')||(key=='r') )
	{
	SendCommand( name, LASTDAT );
	getkey();
	SendCommand( name, LASTDAT );
 	}
if( (key=='8')||(key=='n') )
	SendCommand( name, SWDAT );
}while((key != Esc)&&(key != 'e'));
SendCommand( name, 12 );
}
//-------------
if(pos == NSEN+1)
{
SendCommand( "termr_com", 8 );
prus("  �������");
getkey();
SendCommand( "termr_com", 7 );
}
if(pos == NSEN+2)
{
SendCommand( "ctrl_com", 18 );
prus("  �������");
getkey();
SendCommand( "ctrl_com", 18 );
}
if( pos==(NSEN+3) )
{
prus(" �������");
Line1();
getkey();
prusl("�������");
prusl("",40);
Line2();
getkey();
}
if( pos==(NSEN+4) )
{
prus("�������");
SendCommand( "ups_com", 1 );
getkey();
SendCommand( "ups_com", 1 );
}
sleep(2);
return 1;
}
//-------------------------------------------
#define  METER		1
#define  TEST		2
#define  LOAD_DATA	3
#define  METROLOGY_TEST	4
#define  INFO		5
#define  OFFD		7
#define  OND		8
#define  DISTON		9
#define  DISTOFF	10
#define  DISPON		11
#define  DISPOFF	12
#define  AUTO		13
#define  AUTOOFF	14
#define  ONBLK		15
#define  OFFBLK		16
//------Control-------------
int TheApp::CtrlMenu( int Sw )	
{
int pos;
char str[2],name[15];

for(pos=0;pos<15;pos++)name[pos]=0;

clrscr();
if(Sw==-1)
{
		pos=0;
		do{

		if(pos < (NSEN+1)){
			sprintf(col,"����஫� �����⥫쭮�� ������ %d",pos+1);
			prusl(col);
			}
		if(pos == (NSEN+1))
			prusl("����஫� ������ ���ப�����");
		if( pos==(NSEN+2) )
				prusl("���ﭨ� ���",0);
		if(pos == (NSEN+3))
			prusl("���䨣���� ���");
		if(pos == (NSEN+4))
			prusl("��室");
		}while(Kbd(pos,NSEN+4)!=Enter);
		
		if(pos==NSEN+4)
			{
			Comand = IDLE;
			warning(LOGOUT);
			return 1;
			}
		SaveLog(pos);
}else{
	pos=Sw;
}

if(pos<(NSEN+1))	
{
strncpy(name,"bdmg",4);
itoa(pos,str,10);
strcat(name,str);
strcat(name,"_com");
sem_post(&p->lock);
SendCommand( name, AUTOOFF );
prus("  �������");
sleep(5);
sem_wait(&p->lock);
}
//-------------
if(pos == (NSEN+1))
{
sem_post(&p->lock);
SendCommand( "termr_com", 3 );
prus("  �������");
sleep(5);
sem_wait(&p->lock);
}
if(pos == (NSEN+2))
{
prus(" �������");
SendCommand( "ups_com", 1 );
getkey();
SendCommand( "ups_com", 1 );
clrscr();
}
if(pos == (NSEN+3))
{
pos=0;
do{
if(pos<4)sprintf(col,"�롥�� ����� ��� %d",pos+1);
if(pos==4)sprintf(col,"��室");
prusl(col,40);
}while(Kbd(pos,4)!=Enter);
if(pos<4)Config(pos);
}
return 1;
}
//-------------------------------------------
int TheApp::Config( int Nom)
{
DIR *dr;
struct dirent *d;
FILE *fd;
int err;

dr=opendir("/etc/config");
while( (d = readdir(dr) ) != NULL )
	{
	 if( strstr(d->d_name,"sysinit") != NULL ){
	 strcpy(col,"/etc/config/");
	 strcat(col,d->d_name);
		fd=fopen(col,"r+");   
		break;
		}
	}
closedir(dr);
while(!feof(fd)){
err=fscanf(fd,"%s",col);
if( strstr(col,"BNO") != NULL ){
		fd->_ptr=fd->_ptr-err;
	switch(Nom)
		{
		case 0:strcpy(col,"BNO=XS23");break;
		case 1:strcpy(col,"BNO=XS24");break;
		case 2:strcpy(col,"BNO=XS25");break;
		case 3:strcpy(col,"BNO=XS26");break;
		}
		fputs(col,fd);
//	err=fprintf(fd,"%s",col);
//	fflush(fd);
	break;
	}
}
fclose(fd);
return 1;
}
//-------------------------------------------
int TheApp::Login(void)
{
char _user[10],_paswd[10];
int pos,i;

//time_t t;
//struct tm *tp;



alarm(30);
prusl("�롥�� ०��: ");
pos=0;

do{
switch(pos)
	{
	case 0 :
				prus("ࠡ��       ",16);
				break;
	case 1:
				prus("�஢�ઠ ��� ",16);
				break;
	case 2:
				prus("����஫� ��  ",16);
				break;
	case 3:
				prus("             ",16);
				break;
	}
if(TimeOut)
		{
		Comand =IDLE;
		TimeOut = 0;
		return -1;
		}
}while(Kbd(pos,3)!=Enter);

curg=pos;
prusl("��४��祭�� ���������� - ALPHA ", 40);

if(pos==3)
{
			prusn("������ ���",0);

			i=0;
			do{
			if(TimeOut)
					{
					Comand =IDLE;
					TimeOut = 0;
					return -1;
					}
				_user[i]=(char)getkey();
				i++;
			}while( (key != Enter)&&(i < 10 ) );		
			
			for(i=0;i<3;i++)
			if(strcmp(_user,user[i])==0)
			{
				prus("�ਢ�� ");
				print(_user);
				prus("������ ��஫�");
					i=0;
					do{
					if(TimeOut)
							{
							Comand =IDLE;
							TimeOut = 0;
							return -1;
							}
						_paswd[i]=(char)getkey();
						i++;
					}while( (key != Enter)&&(i < 10 ) );		

					  if(strcmp(_paswd,paswd[i])==0)
						{
						prus("����� � ��⥬� ࠧ�襭");
						alarm(0);
						sleep(3);
						getkey();
						clrscr();
						return 1;
						}
			}

}else{
		prusl("������ ��஫�",0);
		pcon = 16;
		i=0;
		do{
		if(TimeOut)
				{
				Comand =IDLE;
				TimeOut = 0;
				return -1;
				}
			_paswd[i]=(char)getkey();
			prus("*");
			i++;
		}while( (key != Enter)&&(i < 10)  );		
		_paswd[i]=0;
		
		warning((char)(pos+1));
		if(strncmp(_paswd,groop[pos],strlen(groop[pos]))==0)
			{
			alarm(0);
//			clrscr();
			prusl("����� � ��⥬� ࠧ�襭",40);
			sleep(1);
			return 1;
			}

}
//-------------------
prusl("������ ���� ��஫�",40);
warning(ELOGIN);
sleep(2);
Comand = IDLE;
return -1;

}
//-----------------------
void TheApp::OnIdle(void)
{
clrscr();
getkey();
prus("-��⥬� ��� �� �� ���-",7);
prus("���� ��ࠡ�⪨  ",49);
prus(getenv("BNO"));
sleep(3);
SendCommand( "tech_com", 1 );
sleep(3);
sem_wait(&p->lock);
clrscr();
if(Login()!=-1)
{
		switch(curg)
		{
			case 0:
					Comand=GLINE;
					break;
			case 1:
					Comand=TESTVID;
					break;
			case 2:
					Comand=CTRL;
					break;
			case 3:
					Comand=MAIN;
					break;
		}
			curg=0;
}

}
//-----------------------
void TheApp::SaveLog( int ppos )
{
lseek(fd_log,0,SEEK_SET);
write(fd_log,&Comand,sizeof(ppos)); 
write(fd_log,&ppos,sizeof(ppos)); 
}
//-----------------------
int TheApp::Start( void )
{
struct dirent *d;
DIR *dr;
int pos;
CList obj;
char str[3];

sleep(3);
dr=opendir("/dev/mqueue");
while( (d = readdir(dr) ) != NULL )
	{
	 if( strstr(d->d_name,"_com") != NULL )
		{	
			CList one(( char *)d->d_name);
			list.append(one);
		}
	}
closedir(dr);

NSEN=23;
strcpy(obj.name,"bdmg23_com");
while( (list.index(obj) == -1)&&(NSEN > 0) )
{
NSEN--;
strcpy(obj.name,"bdmg");
itoa(NSEN,str,10);
strcat(obj.name,str);
strcat(obj.name,"_com");
}

read(fd_log,&Comand,sizeof(int)); 
read(fd_log,&pos,sizeof(int)); 
if(pos!=-1)
{
		sleep(1);
		switch( Comand  ){

			case GLINE	:
						
						JobMenu(pos);	
						break;
			case TESTVID: 
						TestVid(pos);	
							break;
			case CTRL	:
						CtrlMenu(pos);	
						break;
		}
}
SaveLog( -1 );
Comand = IDLE;
warning(LOGOUT);
return 0;
}

//-----------------------

void BreakOnTimer(int xx)
{
if(xx == SIGALRM)
{
TimeOut = 1;
}
}
//------------------------

main(int arg, char*  argv[])
{
if(arg>2)
{
if(strcmp(argv[1],"t")==NULL)
	TestMode = 1;
}else{
TestMode = 0;
}
//--------Create groop session--------
TheApp App;
TimeOut = 0;
struct sigaction act;
sigset_t sigset;
sigemptyset( &sigset );
sigaddset( &sigset, SIGUSR1 );
act.sa_handler = BreakOnTimer;
sigaction(SIGALRM,&act,NULL);

App.Start();

while(App.Run());


}

