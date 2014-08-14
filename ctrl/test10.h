#define Ser7  0x120
#define Ser8  0x128
//------------------------
int TheApp::OnTest10(void)
{
int fd0,fd1,i,
	err0[3],
	err1[3],
	flag0,
	flag1,
	count;
	
char bufi[10],bufo[10];
u_int reg,regx;

for(i=0;i<3;i++)
{
err0[i] = 0;
err1[i] = 0;
}
count =0;

prusl("тест по модемным каналам",0);
prusl("не забудьте вставить перемычки",40);
timer_setup(2);
prusl("модем 1 ",0);
prusl("модем 2 ",40);
do{
flag0 = 0;
flag1 = 0;

reg = inp(Ser7+4);//save reg Ser7+4
regx = inp(Ser8+4);//save reg Ser+4

if( (inp(Ser7+6) & 0x20)  != 0x20 ){ err0[0]++; flag0 = 1; }
if(  (inp(Ser8+6) & 0x20) != 0x20 ){ err1[0]++; flag1 = 1; }

outp( Ser7+4, inp(Ser7+4) | 0x2 );
outp( Ser8+4, inp(Ser8+4) | 0x2 );
timer_setup(0,0,SEC/4,SEC/4);
if( (inp(Ser7+6) & 0x10) != 0x10 ){ err0[1]++; flag0 = 1; }
if( (inp(Ser8+6) & 0x10) != 0x10 ){ err1[1]++; flag1 = 1; }
	
outp( Ser7+4, inp(Ser7+4) & 0xFD );
outp( Ser8+4, inp(Ser8+4) & 0xFD );
timer_up();
if( (inp(Ser7+6) & 0x10) != 0 ){ err0[1]++; flag0 = 1; } 
if( (inp(Ser8+6) & 0x10) != 0 ){ err1[1]++; flag1 = 1; }
	
outp( Ser7+4, reg );//back reg Ser7+4
outp( Ser8+4, regx );//back reg Ser8+4

strcpy(bufo,"fuck_you");
fd0 = open("/dev/ser7",O_RDWR|O_NONBLOCK);
fd1 = open("/dev/ser8",O_RDWR|O_NONBLOCK);

for(i=0;i<2;i++)
{
write(fd0,bufo,strlen(bufo));
read(fd0,bufi,strlen(bufo));
write(fd1,bufo,strlen(bufo));
read(fd1,bufi,strlen(bufo));
timer_up();
}

i=write(fd0,bufo,strlen(bufo));
timer_up();
bufi[0]=0;
i=read(fd0,bufi,strlen(bufo));
if( strncmp(bufi,bufo,strlen(bufo)) != 0 ){	err0[2]++; flag0 = 1; }
			
i=write(fd1,bufo,strlen(bufo));
timer_up();
bufi[0]=0;
i=read(fd1,bufi,strlen(bufo));
if( strncmp(bufi,bufo,strlen(bufo)) != 0 ){	err1[2]++; flag1 = 1; }

	count++;

close(fd0);
close(fd1);

if(flag0==0)
{ 
	prus("норма ",9);
}else
	prus("ошибка",9);
	
if(flag1==0)
{ 
	prus("норма ",49);
}else
	prus("ошибка",49);
	
	print("N=",73);
	print(itoa(count,col,10));

}while(keypress());
//------------------
i=0;
do{
	switch(i)
	{
	 case 0 :
	 			sprintf(col,"RST-CTS N=%d",err0[0]);
				prus(col,9);
	 			sprintf(col,"RST-CTS N=%d",err1[0]);
				prus(col,49);
	 			break;
	 case 1 :
	 			sprintf(col,"DTR-DSR N=%d",err0[1]);
				prus(col,9);
	 			sprintf(col,"DTR-DSR N=%d",err1[1]);
				prus(col,49);
	 			break;
	 case 2 :
	 			sprintf(col,"RXD-TXD N=%d",err0[2]);
				prus(col,9);
	 			sprintf(col,"RXD-TXD N=%d",err1[2]);
				prus(col,49);
	 			break;
	}
}while((Kbd(i,2)!=Esc)&&(key!='e'));

unlock();
return 1;
}
