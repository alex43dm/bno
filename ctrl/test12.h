#define Ser3  0x100
#define Ser4  0x108
//------------------------
int TheApp::OnTest12(void)
{
int fd0,fd1,i,
	err0[3],
	err1[3],
	flag0,
	flag1,
	count,
	flagt0,
	flagt1;
char bufi[10],bufo[10];
u_int reg0,reg1,t0,t1,t00,t11;

for(i=0;i<3;i++)
{
err0[i] = 0;
err1[i] = 0;
}
count =0;

prusl("тест RS-232 ",0);
prusl("не забудьте вставить перемычки",40);
timer_setup( 2,2 );
clrscr();

strcpy(bufo,"fuck_you");


do{
fd0 = open("/dev/ser3",O_RDWR|O_NONBLOCK);
fd1 = open("/dev/ser4",O_RDWR|O_NONBLOCK);

flag0 = 0;
flag1 = 0;


reg0 = inp(Ser3+4);//save reg Ser3+4
reg1 = inp(Ser4+4);//save reg Ser+4

//-------------------RTS-CTS-----------------------------

flagt0 = 0;
flagt1 = 0;
outp( Ser4+4, inp(Ser4+4) | 0x2 );
outp( Ser3+4, inp(Ser3+4) | 0x2 );
timer_setup( 0,0,SEC/2,SEC/2 );
//3
if( (inp(Ser3+6) & 0x10) != 0x10 )
	{
	flagt0 = 1;
	flag0 = 1;
	}
//4
if( (inp(Ser4+6) & 0x10) != 0x10 )
	{
	flagt1 = 1;
	flag1 = 1;
	}

outp( Ser4+4, inp(Ser4+4) & 0xFD );
outp( Ser3+4, inp(Ser3+4) & 0xFD );
timer_setup(0,0,SEC/2,SEC/2);
//3
if( (inp(Ser3+6) & 0x10) != 0) 
	{
	flagt0 = 1;
	flag0 = 1;
	}
//4
if( (inp(Ser4+6) & 0x10) != 0) 
	{
	flagt1 = 1;
	flag1 = 1;
	}
prusl("COM3 RST-CTS",0);
prusl("COM4 RST-CTS ",40);
if(!flagt0)
	{
	prus("норма       ",13);
	}else{
	err0[0]++;
	prus("ошибка N=",13);
	prus(itoa(err0[1],col,10));
	}
if(!flagt1)
	{
	prus("норма       ",53);
	}else{
	err1[0]++;
	prus("ошибка N=",53);
	prus(itoa(err0[1],col,10));
	}

//---------------------DTR-DSR,DCD,RI-------------------------
flagt0 = 0;
flagt1 = 0;
outp( Ser4+4, inp(Ser4+4) | 0x1 );
outp( Ser3+4, inp(Ser3+4) | 0x1 );
timer_setup(0,0,SEC/2,SEC/2);
//3
if( (t0=inp(Ser3+6) & 0xE0) != 0xE0 )
	{
	flag0 = 1;
	flagt0 = 1;
	}
//4
if( (t1=inp(Ser4+6) & 0xE0) != 0xE0 )
	{
	flagt1 = 1;
	flag1 = 1;
	}

outp( Ser4+4, inp(Ser4+4) & 0xFE );
outp( Ser3+4, inp(Ser3+4) & 0xFE );
timer_setup(0,0,SEC/2,SEC/2);
//3
if( (t00=inp(Ser3+6) & 0xE0) != 0) 
	{
	flagt0 = 1;
	flag0 = 1;
	}
//4
if( (t11=inp(Ser4+6) & 0xE0) != 0) 
	{
	flagt1 = 1;
	flag1 = 1;
	}
prusl("COM3 DTR-DSR ",0);
prusl("COM3 DTR-DSR ",40);
if(!flagt0)
	{
	prus("норма ",13);
	}else{
		prus("ошибка ",13);
		if( (t0 != 0xE0) && (t0 != 0) )
			{
			if( (0x80&t0)==0 )
						prus("DCD ");
			if( (0x40&t0)==0 )
						prus("RI ");
			if( (0x20&t0)==0 )
						prus("DSR ");
			}
			
		prus("ошибка ",13);
		if( (t00 != 0) && (t00 != 0xE0) )
			{
			if( (0x80&t00)!=0 )
						prus("DCD ");
			if( (0x40&t00)!=0 )
						prus("RI ");
			if( (0x20&t00)!=0 )
						prus("DSR ");
			}
	err0[1]++;
	prus("N=");
	print(itoa(err0[1],col,10));
	}
if(!flagt1)
	{
	prus("норма ",53);
	}else{
		prus("ошибка ",53);
		if( (t11 != 0) && (t11 != 0xE0) )
			{
			if( (0x80&t11)!=0 )
						prus("DCD ");
			if( (0x40&t11)!=0 )
						prus("RI ");
			if( (0x20&t11)!=0 )
						prus("DSR ");
			}
		prus("ошибка ",53);
		if( (t1 != 0xE0) && (t1 != 0) )
			{
			if( (0x80&t1)==0 )
						prus("DCD ");
			if( (0x40&t1)==0 )
						prus("RI ");
			if( (0x20&t1)==0 )
						prus("DSR ");
			}
	err1[1]++;
	prus("N=");
	prus(itoa(err0[1],col,10));
	}


outp( Ser3+4, reg0 );//back reg Ser3+4
outp( Ser4+4, reg1 );//back reg Ser4+4
//-----------------------------------------


i=write(fd0,bufo,strlen(bufo));
i=write(fd1,bufo,strlen(bufo));
timer_setup(0,0,SEC/2,SEC/2);

prusl("COM3 RXD-TXD ",0);

if(!flag0)
{
bufi[0]=0;
i=read(fd0,bufi,strlen(bufo));
if( strncmp(bufi,bufo,strlen(bufo)) != 0 )
		{
		err0[2]++;
		prus("ошибка N=",13);
		prus(itoa(err0[2],col,10));
		}else{
		prus("норма",13);
		}
}else{
prus("тест не возможен ",13);
}
			
prus("COM4 RXD-TXD ",40);

if(!flag1)
{
bufi[0]=0;
i=read(fd1,bufi,strlen(bufo));
if( strncmp(bufi,bufo,strlen(bufo)) != 0 )
		{
		err1[2]++;
		prus("ошибка N=",13);
		prus(itoa(err1[2],col,10));
		}else{
		prus("норма",13);
		}
}else{
prus("тест не возможен ",13);
}
	count++;
	sprintf(col,"N=%d",count);
	prus(col,73);

close(fd0);
close(fd1);

}while(keypress());

prusl("COM4 ",40);
i=0;
do{
	switch(i)
	{
	 case 0 :
				sprintf(col,"COM3 RST-CTS N=%d",err0[0]);
				prusl(col,0);
				sprintf(col,"COM4 RST-CTS N=%d",err1[0]);
				prusl(col,40);
	 			break;
	 case 1 :
				sprintf(col,"COM3 DTR-DSR N=%d",err0[1]);
				prusl(col,0);
				sprintf(col,"COM4 DTR-DSR N=%d",err1[1]);
				prusl(col,40);
	 			break;
	 case 2 :
				sprintf(col,"COM3 RXD-TXD N=%d",err0[2]);
				prusl(col,0);
				sprintf(col,"COM4 RXD-TXD N=%d",err1[2]);
				prusl(col,40);
	 			break;
	}
}while((Kbd(i,2)!=Esc)&&(key!='e'));

unlock();
timer_setup(1,1);
Comand=IDLE;
return 1;
}
