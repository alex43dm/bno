
int TheApp::CheckChanel(const u_int xx)
{

		if( xx == 0 )
			{
			 NulCount++; 
			 return 1;
			}

		if( xx > NMax )
			{
			 dat.Mess= NNMax; 
			 return 1;
			}

		if( xx < NMin )
			{
			 dat.Mess= NNMin; 
			 return 1;
			}

return 0;
}
//-----------------------
int TheApp::OnIdle(void)
{
u_int x;
double per,mx;

if(Power==1)
{
	Display();
	pause();
	return 0;
}

//Stop process
pause();
if(strcmp(Name,"bdmg0")==0)//синхронизация генератора
sendsig("gen",SIGUSR1);
dat.T=dat.T+1 ;

	//mesume gamma 0-3
		if( sti >= st )
			{
				x = sti - st;
			}else{
				x = 65535 - st + sti;
			}

		CheckChanel(x);
		KDifferent(x);
		MFilter(x,Math);
		M = M + x;		
		mx =(float)M;
		per=(float)dat.T;
		Math=(double)(mx/dat.T);
//--------------------------


 if( Math > Self )
	{
	dat.Dat = (double)( (Math - Self)/(SenCs*1000) );
	dat.PAbs =(double)2.0*sqrt( (1.0+(2.0*Self)/(Math-Self)) / ((Math-Self)*dat.T) );
	}else{
	dat.Dat = -1.0;
	dat.PAbs=1.0;	
	}
	

//------Display----------
	Display();
			
//-----if pogr ok----

if( dat.T%5==0 )
{
	if(NulCount>=5)	 Error(NNNul);
		NulCount=0;

	 if((dat.PAbs<=DPog)||(dat.T>=TimeReg))
		{
		  		
			  if(dat.PAbs<=DPog)dat.Mess = Ok; 
				
					if( !Blenk&&!MetrologyFlag )  
					  SendDat(mq_tec, dat);
					  SendDat(mq_arj, dat);
					  SendDat(mq_net0, dat);
					  SendDat(mq_net1, dat);
	/*set ds var*/
//  sprintf(col,"%1.2e %1.2e %d %d",
//	  dat.Dat,dat.PAbs,dat.T,dat.State);
  //   ds_set(ds_descriptor,Name,col,30);
/*
t=time(NULL);
DBInsert(
	dat.Dat,		// измеренные данные
	dat.PAbs,		// погрешность
	t,		// время измерения
	dat.T,			// длительность измерения (с)
	dat.Mess,		// признак достоверности измерения
	dat.State 		// состояние канала
);
*/
			  dat.T=0;
			  M=0;
				//выключение датчика
				Along();
		}				
dat.Mess = 0; 
			 
}


return 0;
}
//---------------------------------------------------
int TheApp::KDifferent(u_int xx)
{

if( (xx-kdv) >= KDif ){
	kdc++;}else{kdc=0;}
kdv=xx;

if( dat.T%4==0 ){
if( kdc == 4 ){
	 Error(KDiff);
	 if(ConFlag == ON)	 prus("к",70);	
	 }else{
	 if(ConFlag == ON)	 prus(" ",70);	
	 }
kdc=0;
}
return 0;
}
//---------------------------------------------------
int TheApp::Along( void )
{
if( (Dist == OFF)&&(dat.Dat > 1.4*ScaleH) )
{
	DistFlag=ON;//флаг включения превышения при Dist=OFF 
	OnOff(OFF);
	for(i=0;i<100;i++)
	{	
		Display();
		Run(1);
		pause();
	}
	OnOff(ON);
	for(i=0;i<10;i++)
	{
		Display();
		Run(1);
		pause();
	}
	DistFlag=OFF;//флаг включения превышения при Dist=OFF 
}
return 0;
}
