//------------------------
int TheApp::OnTest14(void)
{
int fd0,fd1,i,
	err0,
	err1,
	count;
char bufi[10],bufo[10];

err0 = 0;
err1 = 0;
count =0;

prusl("��� RS-232 ",0);
prusl("�� ������ ��⠢��� ��६�窨",40);
timer_setup( 2,2 );

strcpy(bufo,"fuck_you");


prusl("COM9 RXD-TXD ",0);
prusl("COM10 RXD-TXD ",40);

do{
//-----------------------------------------

fd0 = open("/dev/ser9",O_RDWR|O_NONBLOCK);
fd1 = open("/dev/ser10",O_RDWR|O_NONBLOCK);

i=write(fd1,bufo,strlen(bufo));
timer_setup(0,0,SEC/2,SEC/2);

bufi[0]=0;
i=read(fd0,bufi,strlen(bufo));
if( strncmp(bufi,bufo,strlen(bufo)) != 0 )
		{
		err0++;
		sprintf(col,"�訡�� N=%d",err0);
		prus(col,13);
		}else{
		prus("��ଠ         ",13);
		}
			
i=write(fd0,bufo,strlen(bufo));
timer_setup(0,0,SEC/2,SEC/2);

bufi[0]=0;
i=read(fd1,bufi,strlen(bufo));
if( strncmp(bufi,bufo,strlen(bufo)) != 0 )
		{
		err1++;
		sprintf(col,"�訡�� N=%d",err1);
		prus(col,54);
		}else{
		prus("��ଠ         ",54);
		}
		
count++;
sprintf(col,"N=%d",count);
prus(col,73);

close(fd0);
close(fd1);

}while(keypress());

sprintf(col,"COM9 RXD-TXD N=%d",err0);
prusl(col,0);
sprintf(col,"COM10 RXD-TXD N=%d",err1);
prusl(col,40);
sprintf(col,"N=%d",count);
prus(col,73);
getkey();
		

unlock();
return 1;
}
