#include<conio.h>
#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<process.h>
#include<stddef.h>
#include<ctype.h>
#include<fcntl.h>
#include<math.h>
#include<ioctl.h>
#include<signal.h>
#include<errno.h>
#include<sys/name.h>
#include<sys/proxy.h>
#include<sys/psinfo.h>
#include<sys/kernel.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/uio.h>
#include<sys/sched.h>
//--------------------
#include<mqserv.h>
#include <data.h>
#include <tcpsock.h>
#include <Cname.h>
#include <log.h>
#include <net.info.h>

#define SIZEDAT 200

main(int arg, char*  argv[])
{


if(arg<3)
{
	perror("missing parametrs");
	exit(1);
}

	

char *name = new  char[15];
strcpy(name,argv[0]);
strcat(name,".com");
CMQueue mq_com(name,2,sizeof(int));
strcpy(name,argv[0]);
strncat(name,argv[1],1);
CName myname(name,16);
CLogg log(myname.Name,LOG_LOCAL2);
strcpy(name,"net.dat");
strcat(name,argv[1]);
myname.gr_join(name);
delete(name);

int key;
//----List of Send-Recved Data-------
	Info bdmg;
	ITech tech;	
	StateBno bno;
	CTerm term;
	StateUps ups;
	time_t tp;
	
CMQueue mq_dat(argv[0],SIZEDAT);
char sockaddr[6];
strcpy(sockaddr,"ssd");
strcat(sockaddr,argv[1]);
key=atoi(argv[2]);
Socket sock(sockaddr,key,"info");
pause();
sock.connect();
log.info("connect");

do{
mq_com.getmes(&key);
switch(key)
{
	case BDMG_INFO_OUT : 
			if( mq_dat.getmes((Info *)&bdmg, sizeof(Info)) )
			sock.send_buf((Info *)&bdmg,sizeof(Info));
			break;
	case BDMG_INFO_IN : 
			if( sock.Recv((Info *)&bdmg,sizeof(Info)) )
			mq_dat.sendmes((Info *)&bdmg, sizeof(Info));
			break;
	case TECHIN : 
			if( mq_dat.getmes((ITech *)&tech, sizeof(ITech)) )
			sock.send_buf((ITech *)&tech,sizeof(ITech));
			break;
	case TECHOUT : 
			if( sock.Recv((ITech *)&tech,sizeof(ITech)) )
			mq_dat.sendmes((ITech *)&tech, sizeof(ITech));
			break;
	case BNOOUT : 
			if( mq_dat.getmes(&bno, sizeof(StateBno)) )
			sock.send_buf(&bno,sizeof(StateBno));
			break;
	case BNOIN : 
			if( sock.Recv(&bno,sizeof(StateBno)) )
			mq_dat.sendmes(&bno, sizeof(StateBno));
			break;
	case TEMIN : 
			if( mq_dat.getmes(&term, sizeof(CTerm)) )
			sock.send_buf(&term,sizeof(CTerm));
			break;
	case TEMOUT : 
			if( sock.Recv(&term,sizeof(CTerm)) )
			mq_dat.sendmes(&term, sizeof(CTerm));
			break;
	case TIN : 
			if( mq_dat.getmes(&tp, sizeof(time_t)) )
			sock.send_buf(&tp,sizeof(time_t));
			break;
	case TOUT : 
			if( sock.Recv(&tp,sizeof(time_t)) )
			mq_dat.sendmes(&tp, sizeof(time_t));
			break;
	case UPS : 
			if( mq_dat.getmes(&ups, sizeof(StateUps)) )
			sock.send_buf(&ups,sizeof(StateUps));
			break;
}
if(iscon==0){
		sock.connect();
		log.info("connect");
		}
}while(1);


}
