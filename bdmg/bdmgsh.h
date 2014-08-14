#ifndef __shmem
#define __shmem

#include</sys/mman.h>

#define N 24

class CShmem
{
private:

	int fd;
	char *memfile;
	pid_t pidd;
	u_int *map;
	int AdBlenk, AdPower;
	

public:
	u_int Blenk, Power;

//-----------------------------------------

CShmem( int bl , int pw )
{
AdBlenk = bl;
AdPower = pw;
Blenk = 0;
Power = 0;

memfile = "devdio";
 fd = shm_open(memfile ,O_RDWR,0);
 if ( fd == -1 ) 
		{
		if( errno == EEXIST ){	fd=shm_open(memfile ,O_RDWR,0);}// key = 1;}
       }else{
		ltrunc(fd, 48 * sizeof( u_int ), SEEK_SET);
	   }
(map=(u_int *)mmap(0, 48 * sizeof( u_int ),PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0));
pidd=qnx_name_locate(0,"Dev.dio",4,NULL);
SetBit(AdBlenk,0);
SetBit(AdPower,0);
};

void SetBit(int nom, u_int state )
{
	map[nom]=state;
	kill(pidd,SIGUSR1);
}
u_int GetBit(int nom )
{
	kill(pidd,SIGUSR1);
	return map[nom];
}
void ShOnBlenk(void )
{
	map[AdBlenk]=1;
	kill(pidd,SIGUSR1);
	Blenk = 1;
}
void ShOffBlenk(void )
{
	map[AdBlenk]=0;
	kill(pidd,SIGUSR1);
	Blenk = 0;
}
void ShOnPower(void )
{
	map[AdPower]=1;
	kill(pidd,SIGUSR1);
	Power = 1;
}
void ShOffPower(void )
{
	map[AdPower]=0;
	kill(pidd,SIGUSR1);
	Power = 0;
}
//------------------------------------------
~CShmem()
{
  if (shm_unlink(memfile) == -1) 
	{
	  perror("hello: cannot unlink region");
   	}
};
};

#endif