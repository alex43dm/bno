int TheApp::OnTest5(void)
{
u_int word;
int Adr;
float ddat,ext;
int err;

outp( BA + 0x5401, 0 );//extend mode adress
Adr=BA+0xA400;
do{
outp( Adr + 4 , 0x97 );
while( (err=inp(Adr)&0x40) != 0x40 );
if( (err=inp(Adr)&0x20) != 0 )
	{
		if((inpw(Adr+6)&0x80)==0x80)
				{
				prusl("переполнение измерителя частоты",0);
				}else{
				prusl("ошыбка в канале измерителя частоты",0);
				}
	}else{

	word=inpw(Adr+6);
	if(word!=0)
	{
	ext=(float)word;
	ddat=80.31+(4.0*50000.0/ext-14.4)*(229.67-80.31)/(72.0-14.4);
	}else{
	ddat=-100;
	}
	sprintf(col,"R = %3.2f Om",ddat);
	prusl(col,0);
	}
prus("выход по Esc",40);
sleep(1);
}while(keypress());
unlock();
Comand=IDLE;
return 1;
}
