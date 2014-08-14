#include "bdmgmain.h"
#include "bdmgdisp.h"
#include "bdmginfo.h"
#include "bdmgerr.h"
#include "bdmgdat.h"
#include "bdmgjob.h"
#include "bdmgcom.h"
#include "bdmgtest.h"


void timerint(int sig)
{
	if(sig==SIGUSR1)
		{
			st = sti;
			outp( SBAx, BNK ); 
			sti=(u_int)inpw( SBAx + ADR );
		}
}
main(int arg, char*  argv[])
{
if(arg==5)
	{	
	SBAx=0xA190;
	}else{
	if(arg==6)
		{
		 	sscanf(argv[5],"%X",&SBAx);
			SBAx=SBAx+0xA000;
		}else{
			perror("Error. Enter correct setup data");
			exit(1);
		}
	}

ADR=(u_char)( 2*( atoi(argv[2])%6 ) );
BNK=(u_char)( (atoi(argv[2]))/6 );

sleep(2);

strcat(argv[0],argv[1]);
TheApp App(argv);

//sleep(10); //погрев датчика 
//App.OnTest(); //тест начальной загрузки 

while(App.Run(0));

}

