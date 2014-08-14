#ifndef _List
#define _List

#include <wclist.h>
#include <data.h>


class CList
{
public:
	int gr,//номер группы
		nb;//номер в группе
	char  name[15];	//XQ процесса
	char  bdmg[15];	//имя процесса
	mqd_t mq;
//--------------------------------------------	
	CList::CList()	
	{
	 mq = -1;
	 gr = -1;	 
	 nb = -1;
	}
	
//--------------------------------------------	
	CList::CList( const char * mqname, char *nam, char *xbdmg )	
	{
	 char mqn[3], *xx;
	 strcpy(name,nam);
	 strcpy(bdmg,xbdmg);
	 int com;
	 
	 while( (mq = mq_open( mqname, O_WRONLY ))==-1)sleep(1);
		mqn[0]=mqname[4];
		if( strlen(mqname) > 9 )
			{
			mqn[1]=mqname[5];
			mqn[2]=0;
			}else{
			mqn[1]=0;
			}
			gr=(int)(strtol(mqn,&xx,10)/4);
			nb=(int)(strtol(mqn,&xx,10)%4);
			
		com=10;//выключение дистанцинного управления
		if(nb==2)	 
			mq_send(mq,&com,sizeof(int),0);
		
	}
	
//--------------------------------------------	
	CList::CList(const CList & in)	
	{
	 strcpy(name,in.name);
	 strcpy(bdmg,in.bdmg);
	 mq = in.mq;
	 gr = in.gr;
	 nb = in.nb;
	}
//--------------------------------------------	
	int CList::operator ==(const CList & in) const
	{
	
	 if( 	(strcmp(name,in.name) == 0)||
	 		(strcmp(bdmg,in.bdmg) == 0)||
			((gr==in.gr)&&(nb==in.nb)) )
		 {
			 return TRUE;
		 }else{
			return FALSE;	  
		 }
	 
	}	
//--------------------------------------------	
	CList CList::operator << (const int command) const
	{
	int err;
	struct mq_attr mq_attrs;
		mq_getattr( mq, &mq_attrs );
		if(mq_attrs.mq_maxmsg > mq_attrs.mq_curmsgs )
				{
					err=mq_send(mq,(int *)&command,sizeof(int),0);
				}
	return *this;	
	}	
	
//--------------------------------------------	
	CList::~CList()	
	{
//	 mq_close(mq);
	}
	
};

#endif