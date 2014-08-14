//------------------------
int TheApp::OnSetInfo(void)
{
Info inf;

ResvProxy((Info *) &inf , sizeof(inf) );

	if( (inf.SenCs>0)&&(inf.SenCs<99999) )		SenCs	= inf.SenCs;
	if( (inf.Self>0)&&(inf.Self<5) )			Self	= inf.Self;		
	if( (inf.BlenkN>0)&&(inf.BlenkN<200) )		BlenkN	= inf.BlenkN;	
	if( (inf.TimeAP>0)&&(inf.TimeAP<30) )		TimeAP	= inf.TimeAP;		
	if( (inf.KDif>1)&&(inf.KDif<2) )			KDif	= inf.KDif;		
	if( (inf.TimeReg>0)&&(inf.TimeReg<1000) )	TimeReg	= inf.TimeReg;	
	if( (inf.DPog>0.01)&&(inf.DPog<0.99) )		DPog	= inf.DPog;		
	
SaveData(Name,Power);
return 0;
}
int TheApp::OnGetInfo(void)
{
Info inf;

		inf.SenCs	= SenCs;
		inf.Self	= Self;		
		inf.BlenkN	= BlenkN;	
		inf.TimeAP	= TimeAP;		
		inf.KDif	= KDif;		
		inf.TimeReg	= TimeReg;	
		inf.DPog	= DPog;		
		inf.Blenk	= Blenk;		
		inf.Power	= Power;		
SendProxy((Info *) &inf , sizeof(inf) );
return 0;
}
