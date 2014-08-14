
//------------------------
int TheApp::AutoOnOff(int key)
{
if(key==1)
{
 sem_post(&p->lock);
 Auto = ON;
 ConFlag = OFF;
 Time=OFF;
 Error(OnChenel);
// if(*Power==1)*Power=0;
// if(*Blenk==ON)*Blenk=OFF;
 return 1;
}else{
 sem_wait(&p->lock);
 Auto = OFF;
 ConFlag = ON;
 Time=OFF;
 Error(OffChenel);
 return 1;
}

}
//------------------------
int TheApp::OnMeter(void)
{
//int i;
//u_int x;
//	timer_set_interal( 100, 0 );
//	timer_up();
//	marktime(dat->Time);
//for( i=0; i<3; i++ )
//	x = idata->St0[0];//read counters
//	dat->PAbs = rand();
//	dat->True = TRUE;
//	out->sendmes(dat);

return 0;
}
//------------------------
int TheApp::OnOff(int sw)
{
if(sw==1)
{
if(Power==1)
	{
	Error(OnSensor);
	
	ShOffPower();
	outp( SBAx, BNK ); 
	sti=(u_int)inpw( SBAx + ADR );
//    id_timer = timer_create( CLOCK_REALTIME, &event );
//	timer_settime( id_timer, 0, &timer, NULL );
	}
}else{
if(Power==0)
	{
//	timer_delete(id_timer);
	
	Error(OffSensor);
	SaveData(Name, 0);
	ShOnPower();
		if( !Blenk&&!MetrologyFlag )  
			  SendDat(mq_tec, dat);
			  SendDat(mq_arj, dat);
			  SendDat(mq_net0, dat);
//			  SendDat(mq_net1, dat);
			  dat.T=0;
			  M=0;
	}
}
return 0;
}
