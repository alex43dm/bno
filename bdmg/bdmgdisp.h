
//-----------------------
int TheApp::KeyBd(void)
{
if( !Auto )
 	{
	if(kbhit())
		{
					getkey();
					
				 switch((char)key)
					{
						case '7':
						case 'm':
							
							if(Power==ON)
							{
								OnOff(ON);
							}else{
								OnOff(OFF);
							}
						break;
						case '9':
						case 'o':
							
						if(Blenk==OFF)
							{
								OnCBlenk(ON);
							}else{
								OnCBlenk(OFF);
							}
						break;
						case '5':
						case 'r':
						if(Time==ON)
							{
								Time = OFF;
							}else{
								Time = ON;
							}
						case '8':
						case 'n':
						if(SwFlag)
							{
								SwFlag = OFF;
							}else{
								SwFlag = ON;
								}
						break;
						case '2':
						case 'v':
							Dialog();
						break;
						case Esc:
						case 'e':
							AutoOnOff(ON);
						break;
					}
			}
}
return 0;
}
int TheApp::Display(void)
{
CData Pdat;
if(Time){
memcpy(&Pdat,&savedat, sizeof(Pdat) );
}else{
memcpy(&Pdat,&dat, sizeof(Pdat) );
}

//------Key board---------
	KeyBd();
		  if(ConFlag == ON)
			{
			
			if(DistFlag==ON) //флаг включения превышения при Dist=OFF 
				{
					sprintf(col,"%s ", Cod);
					prusl(col,0);			
					prus("перевышен предел измерения");			
					prusl("100c ожидания прошло:",40);			
					sprintf(col," %3dc",i);
					prus(col);			
				return 1;
				}
				
			
				 sprintf(col,"%s ", Cod);
				 prusl(col);			
				if(Power==1)
				{
					prusl("датчик выключен",40);			
					return 1;
				}
			 if(Pdat.Dat>0)
			 {
			 if(!SwFlag){
				 prus("мпд",10);			
				 sprintf(col,"=%-1.2e ", Pdat.Dat);
				 print(col);			
				 prus("гр",24);			
				 print("/");			
				 prus("ч");			
				 }else{
				 prus("мэд",10);			
				 sprintf(col,"=%-1.2e ", Pdat.Dat/0.0095);
				 print(col);			
				 prus("р",24);			
				 print("/");			
				 prus("ч");			
				 }
			 }else{
				 prus(" ниже фона    ");			
			 }
			 
			 if(savedat.Mess == Ok)
				 {
					 prus("п",30);	
				 }else{
					 prus("в",30);	
				 }
				 
				 prus(" т",32);			
				 sprintf(col,"=%-3d", Pdat.T);
				 print(col);			
				 prus("c");			
			 
			 
			 prusl("в",40);			
			 if(Power)
			 {
				 print("0");	
			 }else{
				 print("1");	
			 }
			 
			 prus("б");			
			 if(Blenk)
			 {
			 print("1");	
			 }else{
			 print("0");	
			 }
			 
			 
			 print(" S");	
			 sprintf(col,"=%-8.2f ",(double)SenCs);
			 print(col);	
			 print("F");	
			 sprintf(col,"=%-4.2f ",(double)Self);
			 print(col);	
			 
			 prus("м");		
			 sprintf(col,"=%-7.2f",(double)Math);
			 print(col);	
			 
//			 sprintf(col," %-5d",M);оттладка 25.10.01
//			 print(col);	
			 
			 prus("п",73);			
			 sprintf(col,"=%-3d",(int)(Pdat.PAbs*100));
			 print(col);			
			 
			 if(Dist == ON)
				 {
					 prus("д",29);	
				 }else{
					 prus(" ",29);	
				 }
			if(Time)
			{
					 t=time(NULL);
					 mtime=localtime(&t);
					sprintf(col,"%#02d.%#02d.%d %#02d:%#02d:%#02d",
					mtime->tm_mday,mtime->tm_mon+1,mtime->tm_year+1900,
					mtime->tm_hour,mtime->tm_min,mtime->tm_sec);
					prus(col,40);
			}
			}
return 0;
}
//-----------------------
int TheApp::Dialog(void)
{
ConFlag = OFF;
timer_delete(id_timer);
char str[10];
int i,flag;
double x;

flag = OFF;


 prusl("чувствительность ");			
			 sprintf(col,"=%6.2f ", SenCs);
			 print(col);
 prusl("выберите: Enter-изменить Space-отказ",40);
 if(getkey()==Enter)
 	{
	for(i=0;i<10;i++)str[i]=0;
 	prusl("чувствительность ");			
			 sprintf(col,"=%-7.2f ", SenCs);
			 print(col);
	 prusl("новое значение = ",40);			
	 i = 0;
		do{
		 str[i]=(char)getkey();
		 print(&str[i]);
		 i++;
		}while(key!=Enter);	 
		
	 if( (x=strtod(str,0)) <= 0 )
	 	{
		 prusl("не правильный ввод ",40);			
		 sleep(1);
		}else{
	 	SenCs = x;
		flag = ON;
		}
		
	}
				 	
 prusl("фон",0);			
			 sprintf(col,"=%4.2f ", Self);
			 print(col);	
 prusl("выберите: Enter-изменить Space-отказ",40);
 if(getkey()==Enter)
 	{
	for(i=0;i<10;i++)str[i]=0;
 	prusl("фон",0);			
			 sprintf(col,"=%2.2f ", Self);
			 print(col);	
	 prusl("новое значение = ",40);			
	 i = 0;
		do{
		 str[i]=(char)getkey();
		 print(&str[i]);
		 i++;
		}while(key!=Enter);	 
		
	 if( (x=strtod(str,0)) <= 0 )
	 	{
		 prusl("не правильный ввод ",40);			
		 sleep(1);
		}else{
	 	Self = x;
		flag = ON;
		}
	}
if(flag)
SaveData(Name,Power);

ConFlag = ON;
id_timer = timer_create( CLOCK_REALTIME, &event );
    if( id_timer == -1 ) 
	{
	 prus("требуется перезапуск системы",0);			
	}else{
	 clrscr();
	 prus("запуск системы",0);			
	timer_settime( id_timer, 0, &timer, NULL );
	}
return 1;
}
//-----------------------
