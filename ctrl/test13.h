//------------------------
int TheApp::OnTest13(void)
{
int fd0,fd1,i,
	err0,
	err1,
	count;
char bufi[10],bufo[10];

err0 = 0;
err1 = 0;
count =0;

prusl("тест RS-485 ",0);
prusl("не забудьте вставить перемычки",40);
timer_setup( 2,2 );

strcpy(bufo,"fuck_you");


prusl("COM5 RXD-TXD ",0);
prusl("COM6 RXD-TXD ",40);

do{
//-----------------------------------------

fd0 = open("/dev/ser5",O_RDWR|O_NONBLOCK);
fd1 = open("/dev/ser6",O_RDWR|O_NONBLOCK);

i=write(fd1,bufo,strlen(bufo));
timer_setup(0,0,SEC/2,SEC/2);


bufi[0]=0;
i=read(fd0,bufi,strlen(bufo));
if( strncmp(bufi,bufo,strlen(bufo)) != 0 )
		{
		err0++;
		sprintf(col,"ошибка N=%d",err0);
		prus(col,13);
		}else{
		prus("норма         ",13);
		}
			

i=write(fd0,bufo,strlen(bufo));
timer_setup(0,0,SEC/2,SEC/2);

bufi[0]=0;
i=read(fd1,bufi,strlen(bufo));
if( strncmp(bufi,bufo,strlen(bufo)) != 0 )
		{
		err1++;
		sprintf(col,"ошибка N=%d",err1);
		prus(col,53);
		}else{
		prus("норма         ",53);
		}
		
count++;
sprintf(col,"N=%d",count);
prus(col,73);

close(fd0);
close(fd1);

}while(keypress());

sprintf(col,"COM5 RXD-TXD N=%d",err0);
prusl(col,0);
sprintf(col,"COM6 RXD-TXD N=%d",err1);
prusl(col,40);
sprintf(col,"N=%d",count);
prus(col,73);
getkey();
		

unlock();
return 1;
}
