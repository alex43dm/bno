//------------------------
int TheApp::OnTest0(void)
{
int BAx;
int i,j,p,ba,err[4][5],count = 1,
				flag;
u_int _st[13],regs;
int bad[13];
char str[6];
double t;

BAx=BA+0xA400;
for(i=0;i<13;i++)bad[i]=0;
for(j=0;j<4;j++)
for(i=0;i<5;i++)
err[j][i]=0;
prusl("сквозной тест",0);
prusl("выход-Esc останов-Space",40);

do{
flag=0;
for(i=0;i<13;i++)
{
 outp( 0xA000+BA, (u_char)(i/6)); 
 _st[i] = (u_int)inpw(0xA000+BA+2*(i%6));
}
//-----------------------------------------
timer_setup(1,1,0,0);

//0
		for( i = 0, ba = BA + 0xA00E; i < 4; i++, ba += 0x400 )
			{
				str[0]=(char)inp( ba );
				itoa((int)inp( ba + 1 ),&str[1],10);
			 switch(i)
				{
				case 0:	
						break;
				case 1:
						if(strcmp(str,"g0")!=0)
							{
							err[0][0]++;	
							flag = 1;
							}
						break;
				case 2:
						break;
				case 3:
						if(strcmp(str,"g0")!=0)
							{
							err[0][0]++;	
							flag = 1;
							}
						break;
				}
			}

		for( i = 0, ba = 0x100; i < 8; i++, ba += 0x08 )
			{
			regs = inp( ba + 3 );
				outp(ba + 3, 0xF0 );
						if(inp(ba+3)!=0xF0)
							{
							err[0][1]++;	
							flag = 1;
							}
			outp(ba + 3, regs );
			}


//1
for(i=40;i<44;i++)
		if(!pdio->GetBit(i))
		{
		err[1][i-40]++;	
		flag = 1;
		}
//2
	if(pdio->GetBit(45))
		{
		err[2][1]++;	
		flag = 1;
		}
		
	if(!pdio->GetBit(44))
		{
		err[2][0]++;	
		flag = 1;
		}
		
	if(pdio->GetBit(8))
		{
		err[2][3]++;	
		flag = 1;
		}
	if(pdio->GetBit(9))
		{
		err[2][4]++;	
		flag = 1;
		}

//3
outp( BAx + 4 , 0x97 );

do{
}while( (inp(BAx)&0x40) != 0x40 );


if( (inp(BAx)&0x20) != 0 )
	{
		err[3][0]++;	
		flag = 1;
	}else{

    t=-50.+(((4.*50000/(double)(inpw(BAx+6)))-14.4)*(350.+ 50.))/(72.-14.4);
	if(t<10)
		{
		err[3][1]++;
		flag = 1;
		}
	if(t>45)
		{
		err[3][2]++;
		flag = 1;
		}

	}
//4
for(i=0;i<13;i++)
{
 outp( 0xA000+BA, (u_char)(i/6)); 
 if(_st[i] != (u_int)inpw(0xA000+BA+2*(i%6)) )
 		{
		bad[i]++;	
		flag = 1;
		}
}
//-----------------------------------------------------------
keypress();
if(key==Space)
			{
				prusl("перемещение-PgUp,PgDn выбор-Enter",40);
			p=0;
				do
				{
				sprintf(col,"выбирите тест-%d",p+1);
				prusl(col,0);
				}			
				while(Kbd(p,4) != Enter);
				
				prusl("выход-любая клавиша",40);
				sprintf(col,"N=%d",count);
				prus(col,73);
				pcon = 0;
					 switch(p)
						{
						case 0 :
								sprintf(col,"отказ платы UNIO %d 5558 %d",err[0][0],err[0][1]);
								prusl(col);
								break;
						case 1 :
								sprintf(col,"+6в -%1d +12в -%1d +24вII-%1d +24вI-%1d",
								err[1][3],err[1][2],err[1][1],err[1][0]);
								prusl(col,0);
								break;
						case 2 :
								sprintf(col,"дверь-%d XT22-%d XT4-%d XT5-%d ",
								err[2][0],err[2][1],err[2][3],err[2][4]);
								prus(col,0);
								break;
						case 3 :
								sprintf(col,"T - %3.2f",t);
								prusl(col,0);
								
								prus("норма ",20);
								for(i=0;i<3;i++)if(err[3][i]!=0)
										prus("ошибка ",20);
										pcon = 41;
								if(err[3][0]!=0)
										{
										prus("аппаратная ",28);
										prus(itoa(err[3][0],str,10));
										}
								if(err[3][1]!=0)
										{
										prus("T<10 ",28);
										prus(itoa(err[3][1],str,10));
										}
								if(err[3][2]!=0)
										{
										prus("T>45 ",28);
										prus(itoa(err[3][2],str,10));
										}
										
								break;
						case 4 :
								prusl("",0);
								for(i=0;i<13;i++)
								{
									sprintf(col,"S%d-%d ",i,bad[i]);
									prus(col);
								}
								break;
						}
			getkey();
			prusl("сквозной тест ",0);
			prusl("выход-Esc останов-Space",40);
			
			}

if(flag)
{
prus(MERR,14);
				for(j=0;j<4;j++)
				for(i=0;i<5;i++)
				if(err[j][i]!=0)
						{
							sprintf(col,"%1d",(j+1));
							print(col,21+j);
						}
							
					for(i=0;i<13;i++)
					if(bad[i]!=0)
							{
							print("5",21+5);
							}

}else{
prus(" норма ",14);
}

sprintf(col,"N=%d",count);
prus(col,73);

count++;
}while((key!=Esc)&&(key!=0x65));
unlock();
Comand=IDLE;
return 1;
}
//------------------------
