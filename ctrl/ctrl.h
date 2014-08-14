char TheApp::TestBeginUnio()
{
//проверка UNIO
char i,c,d,j;
int ba;

	
//clrscr();
russcrol("проверка ");
prus("платы ");
prus("UNIO ");
		outp( BA + 0x5401, 0x00 ); //extend mode adress
		outp( BA + 0xA00D, 0x00 ); //disable interrupt
		
		for( i = 0, ba =0xA00E+BA; i < 4; i++, ba += 0x400 )
		{
		    c = (char)inp( ba );
			d = (char)inp( ba + 1 );
			switch(i)
			{
			case 0 :
//					if( (c!='c')&&(d!=0) )
						{
//						prus("ошибка - в прошивке 0",0);
//						error("UNIO error");
//						sleep(1);
//						return -1;		
						}
						for( j = 1; j < 4; j++ )
						outp( BA+0xA000+j, 0x00 ); // Blank counts		
						outp( BA+0xA000, 0x00 ); // Blank counts		
					break;
			case 1 :
					if( (c!='g')&&(d!=0) )
						{
						russcrol("ошибка - в прошивке 1");
						error("UNIO error 1");
						timer_setup(1);
						return -1;		
						}
					break;
			case 2 :
					if( (c!='g')&&(d!=0) )
						{
						russcrol("ошибка - в прошивке 2");
						error("UNIO error 2");
						timer_setup(1);
						return -1;		
						}
					break;
			case 3 :
					if( (c!='g')&&(d!=0) )
						{
						russcrol("ошибка - в прошивке 3");
						error("UNIO error 3");
						timer_setup(1);
						return -1;		
						}
					break;
			}
		
		}
		
prus("норма");			
timer_setup(2);
return 0;		
};

//проверка модемов		
#define Ser7  0x120
#define Ser8  0x128
char TheApp::TestBeginModem()
{
u_int reg,regx;
char RetVal = 0;

//clrscr();
russcrol("проверка ");
prus("платы ");
prus("5558 ");
reg = inp(Ser7+4);//save reg Ser7+4
regx = inp(Ser8+4);//save reg Ser+4
//DSR
if( (inp(Ser7+6) & 0x20)  != 0x20 ) 
	{
	prus("модем 1 ошибка ",61);
	error("modem 1 error");
	RetVal = -1;
	timer_setup(1);
	}
if(  (inp(Ser8+6) & 0x20) != 0x20 ) 
	{
	prus("модем 2 ошибка ",61);
	error("modem 2 error");
	RetVal = -2;
	timer_setup(1);
	}
//CTS-RTS
outp( Ser7+4, inp(Ser7+4) | 0x2 );
outp( Ser8+4, inp(Ser8+4) | 0x2 );
timer_setup(0,0,SEC/4,SEC/4);
if( (inp(Ser7+6) & 0x10) != 0x10 )
	{
	prus("модем 1 ошибка ",61);
	error("modem 1 error");
	RetVal = -1;
	timer_setup(1);
	}
if( (inp(Ser8+6) & 0x10) != 0x10 )
	{
	prus("модем 2 ошибка ",61);
	error("modem 2 error");
	RetVal = -2;
	timer_setup(1);
	}
	
outp( Ser7+4, inp(Ser7+4) & 0xFD );
outp( Ser8+4, inp(Ser8+4) & 0xFD );
timer_setup(0,0,SEC/4,SEC/4);
//CTS-RTS
if( (inp(Ser7+6) & 0x10) != 0) 
	{
	prus("модем 1 ошибка ",61);
	error("modem 1 error");
	RetVal = -1;
	timer_setup(1);
	}

if( (inp(Ser8+6) & 0x10) != 0 )
	{
	prus("модем 2 ошибка ",61);
	error("modem 2 error");
	RetVal = -2;
	timer_setup(1);
	}
	
outp( Ser7+4, reg );//back reg Ser7+4
outp( Ser8+4, regx );//back reg Ser8+4
if(RetVal==0)prus("норма");			
timer_setup(2);
return RetVal;		

}
//анали загрузки
char TheApp::TestBeginStart()
{
clrscr();
char WD_test;
char RetVal = 0;

outp(0x70,0x0E);
timer_setup( 0, SEC/2);
WD_test = (char)inp(0x71);
			russcrol("анализ ");
			prus("начальной ");
			prus("загрузки  ");
if(WD_test!=0)
	{
			russcrol("ошибка начальной загрузки  ");
				timer_setup(1);

				if(WD_test&0x01)
					{
					russcrol("нмд-ошибка                  ");
					error("hdd error");
					RetVal = -1;
					timer_setup(1);
					}
				if(WD_test&0x10)
					{
					russcrol("озу-ошибка                  ");
					error("ram error");
					RetVal = -2;
					timer_setup(1);
					}
				if(WD_test&0x20)
					{
					russcrol("конфигурация системы-ошибка ");
					error("cof error");
					RetVal = -3;
					timer_setup(1);
					}
				if(WD_test&0x40)
					{
					russcrol("контрольная сумма-ошибка    ");
					error("sum error");
					RetVal = -4;
					timer_setup(1);
					}
				if(WD_test&0x80)
					{
					russcrol("аккумулятор кмоп-ошибка     ");
					error("battary error");
					RetVal = -5;
					timer_setup(1);
					}
	}
if(RetVal==0)prus("норма");			
timer_setup(2);
return RetVal;

}
//тест UPS
char TheApp::TestUPS()
{
char RetVal = 0;
char C[]="Y9QX";
char PortBuf[30];
int i,Ut,fd;

//clrscr();

fd=open("/dev/ser2",O_RDWR|O_NONBLOCK);
			russcrol("проверка ");
			prus("ипб ");
for(i=0;i<strlen(C);i++)
{
write(fd,&C[i],1);
timer_setup( 0,0,SEC/2,SEC/2);
read(fd,PortBuf,sizeof(PortBuf));
switch (i) { // анализ состояния ИБП
           case  0: // Y - Начальная инициализация диалога
              		if (strstr(PortBuf,"SM")==NULL) 
						{
            	    	i=15;                      // конец диалога
						prus("нет ответа ");
						timer_setup(1);
						RetVal = -1;
	    	          	}
		            break;
            case  1: 
					if(strstr(PortBuf,"FF")==NULL)
						{
					 	prus("старт от батареи");
						timer_setup(1);
						RetVal = -2;
						}
                     break;
            case  2: 
    	          Ut=atoh(PortBuf);
	              if (Ut & 0x80){
				   prus("батарея разряжена");
						timer_setup(1);
						RetVal = -3;
				   }
				   break;
            case  3:if(strstr(PortBuf,"NO")==NULL)
					{
				 		prus("замените батарею");
						timer_setup(1);
						RetVal = -4;
					}
                     break;
          } //switch
}
if(RetVal==0)prus("норма");			
close(fd);
timer_setup(2);
return RetVal;

}