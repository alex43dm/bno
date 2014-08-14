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
#include<Cname.h>
#include<mqserv.h>
#include <data.h>
#include <tcpsock.h>
#include <log.h>

#define SIZEDAT sizeof(CData)

main(int arg, char*  argv[])
{

if(arg<3)
{
	perror("missing parametrs");
	exit(1);
}
FILE *db;
CData dat;
char *name = new  char[15];
strcpy(name,argv[0]);
strcat(name,argv[1]);
CName myname(name,16);
myname.gr_begin();
CLogg log(myname.Name,LOG_LOCAL2);
CMQueue mq(myname.Name,20,SIZEDAT);
char sockaddr[6];
delete(name);
char dbname[30];
strcpy(dbname,"/home/ftp/pub/now");
strcat(dbname,argv[1]);
strcat(dbname,".db");

strcpy(sockaddr,"ssd");
strcat(sockaddr,argv[1]);
Socket sock(sockaddr,atoi(argv[2]),"dat");
pause();
		sock.connect(); 
		log.info("connect");
		do{
			mq.getmes((CData *)&dat);
			sock.send_dat(&dat);
			if(iscon==0){
				db=fopen(dbname,"w");
				strcpy(dat.Name,"XS23R10B1");
				fwrite(&dat,sizeof(dat),1,db);
				fflush(db);
					while(sock.OneConnect()){
						while( mq.getdata((CData *)&dat) )
						fwrite(&dat,sizeof(dat),1,db);
						}
				fclose(db);
				log.info("connect");
				}
		}while(1);



}
