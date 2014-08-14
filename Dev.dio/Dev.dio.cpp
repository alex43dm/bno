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
//#include<filter.h>
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
#include</sys/mman.h>
//+++++++++++++++++++++++++++++++++++
#define SEC 1000000000

void timerint(int sig)
{
	if(sig==SIGUSR1)
		{
		}
}
int main(int arg, char*  argv[])
{
	int *BAx,BA;
	u_char 	mask, bt;
	u_int	*bit;
		int i,j;
	pid_t pid;
char memfile[5];
int fd,flag,N,n,k;

	time_t id_timer;
    struct itimerspec timer;
    struct sigevent event;
    struct sigaction sigact;
	sigset_t sigset;
	
//-------Name attach-------------------------
 pid=qnx_name_attach(0,"Dev.dio");
//--------Scheduler potitic-----------
qnx_scheduler( 0, 0, SCHED_FIFO, 16, 0 );
//qnx_scheduler( 0, 0, SCHED_FIFO, 8, i );
//----------Attach to the timer-------
	for( i = _SIGMIN; i <= _SIGMAX; ++i)
		if( i != SIGTERM  &&  i != SIGPWR)
			signal( i, SIG_IGN );
sigemptyset( &sigact.sa_mask );
sigaddset( &sigset, SIGUSR1 );
sigact.sa_handler = timerint;
sigaction(SIGUSR1,&sigact,NULL);
   event.sigev_signo  =SIGUSR1;
   id_timer = timer_create( CLOCK_REALTIME, &event );
    if( id_timer == -1 ) 
	{
	 cout<<"Unable to attach timer."<<endl; 
	}
	timer.it_value.tv_sec 		=1L;
	timer.it_value.tv_nsec	 	=0L;
	timer.it_interval.tv_sec 	=1L;
	timer.it_interval.tv_nsec 	=0L;
//---------------------------------------------------------------
sscanf(argv[1],"%X",&BA);

	N = (arg-2)/4;
	n = 48*(arg-2)/4;

		outp( BA + 0x5401, 0x00 ); //extend mode adress
		outp( BA + 0xA00D, 0x00 ); //disable interrupt

//--------------------------------------------------------------
strcpy( memfile, "devdio" );
 if ((fd=shm_open(memfile ,O_RDWR|O_CREAT|O_EXCL,0)) == -1) 
		{
		if( errno == EEXIST )fd=shm_open(memfile ,O_RDWR,0); 
        }
ltrunc(fd, 1+n * sizeof( u_int ), SEEK_SET);
(bit=(u_int *)mmap(0, 1+n * sizeof( u_int ),PROT_READ|PROT_WRITE,
                          MAP_SHARED, fd, 0)); 
//clear mas
memset(bit,0,1+n * sizeof( u_int ));
//---------------------------------------------------------------

BAx = (int *)malloc( N*sizeof(int) );	

for( j = 0; j < N; j++)
{
	BAx[j] = BA + 0xA000 + 0x400*atoi(argv[ 2+j*4 ]);
/* control register : no IRQ, time antidrebezg 100 ms, BANK = 1 */
		outp( BAx[j],  0x01 );

		/* mask register */ 
		for(i = 0; i < 3; i++)
		{
		 sscanf(argv[ 3+i+j*4 ],"%X",&mask);
			
	for( k = 0; k < 8; k++ )
		{
		  if( (mask != 0)&&(mask != 0xFF))
			{
				bt = 0;
				flag = 0x01;
				flag = flag << i; 
				flag = flag & mask;
			}else{
				flag=0;
				if(mask==0xFF)flag=1;
			}
		bit[ 24*N+j*24+i*8+k ] = flag;
		}

		 outp( BAx[j]+1+i, mask );
		}

// clear out
	
	outp( BAx[j], 0 );
	for( i = 1; i < 4; i++ ) 
		outp( BAx[j] + i, 0xFF );
}
//------------Main loop-------------------
//timer_settime( id_timer, 0, &timer, NULL );
do{
//if(bit[n+1] == 0)
//{
for(k = 0; k < N; k++)
{
//---------out port-------------------
  for(i = 0; i < 3; i++)
	{
	 bt = 0;

	 for( j = 0; j < 8; j++ )
	  {
	   if( (bit[ n/2 + k*24+i*8+j ] == 1) && (bit[ k*24+i*8+j ] == 1) )
	    {
		 flag = 0x01;
		 flag = flag << j; 
		 bt = bt | flag;
		}
       }

	  outp( BAx[k]+1+i, ~bt );
	}

//-----------in port-------------------
  for(i = 0; i < 3; i++)
	{
	 bt = inp( BAx[k]+1+i );
	 bt=~bt;	
	 for( j = 0; j < 8; j++ )
	  {
	   if( bit[ n/2 + k*24+i*8+j ] == 0 )
	    {
		 flag = 0x01;
		 flag = flag << j; 
		 flag = bt & flag;
			if( flag>0 )
				{
				 bit[ k*24+i*8+j ] = 1;
				}else{
				 bit[ k*24+i*8+j ] = 0;
				}
		}
       }
	}
}
//}
pause();
}while( 1 );
}

