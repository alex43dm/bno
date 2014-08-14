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
#include<errno.h>
#include<dirent.h>
#include<sys/name.h>
#include<sys/proxy.h>
#include<sys/psinfo.h>
#include<sys/kernel.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/uio.h>
#include<sys/sched.h>


#include<signal.h>

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <data.h>
#include <netdb.h>
//--------------------
#include<Cname.h>
#include<mqueue.h>
#include <tcpsock.h>
#include <data.h>
#include <list.h>
#include <log.h>


#define SIZEDAT sizeof(CCom)
#define SIZE sizeof(struct sockaddr_in)
#define MQPASS 	"/dev/mqueue"

int bdmg[]={2,3,4,5,7,8,9,10,15,16};
int tech[]={3,4};

int pcomand(char *name, int command)
{
int i;

if( strstr(name,"bdmg")!=NULL )
for(i=0;i<10;i++)
	if(command==bdmg[i])return 1;
	
if( strstr(name,"tech")!=NULL )
for(i=0;i<2;i++)
	if(command==tech[i])return 1;

if( strstr(name,"termr")!=NULL )
	if(command==5)return 1;
	
if( strstr(name,"ups")!=NULL )
	if(command==5)return 1;
	
if( strstr(name,"ltime")!=NULL )
for(i=0;i<2;i++)
	if(command==tech[i])return 1;
	
return -1;
}
main(int arg, char*  argv[])
{

if(arg<3)
{
	perror("missing parametrs");
	exit(1);
}
struct dirent *d;
DIR *dr;
int i;
CCom com;
WCValDList<CList> list;
char name[15];

strcpy(name,argv[0]);
strcat(name,argv[1]);
CName myname(name,16);

CLogg log(myname.Name,LOG_LOCAL2);

strcpy(name,"net.dat");
strcat(name,argv[1]);
myname.gr_join(name);

sleep(5);

dr=opendir(MQPASS);
while( (d = readdir(dr) ) != NULL )
	{
	 if(strstr(d->d_name,"_com") != NULL)
		{	
			CList one(( char *)d->d_name);
			list.append(one);
		}
	}

closedir(dr);

CList obj;

char sockaddr[6];
strcpy(sockaddr,"ssd");
strcat(sockaddr,argv[1]);
Socket sock(sockaddr,atoi(argv[2]),"com");
pause();
sock.connect();
log.info("connect");
int err;
//-------------
do{
//		err=recv(sockfd,(CCom *)&com,sizeof(com),0);
		err=sock.Recv((CCom *)&com,sizeof(com));
		if( err > 0 )
			{
				strcpy(obj.name,com.name);
				strcat(obj.name,"_com");
				if( (i = list.index(obj)) != 0 )
					{
					obj = list.find(i);
					if(pcomand(com.name,com.com))
						obj<<com.com;
					}
			}else{
			log.error("net down !!!");
			/*
				strcpy(obj.name,"fs_com");
				if( (i = list.index(obj)) != -1 )
					{
					obj = list.find(i);
					obj<<3;
					}
			*/
	   		myname.gr_kill(SIGPIPE);
			sock.connect();
			log.info("connect");
//			obj<<2;
//			sleep(2);
			}
	
		}while(1);


}
