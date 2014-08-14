int TheApp::OnTest7(void)
{
int i,j,pos,per;	
u_int bmas[13],mas[13],xxx;
float dat[13];
float pper,rez;
	
prus("выберите период",0);
pos=0;
do{
pper=pow(10,pos);
prus("период      ",40);
sprintf(col,"%3.0f",pper);
prus(col,47);
}while(Kbd(pos,4)!=Enter);

for(j=0;j<13;j++)dat[j]=0.0;
prus(WAIT,52);

per=(int)(pper);
for(i=0;i<per;i++)
	{
	 
			for(j=0;j<13;j++)
			{
			 outp( 0xA000+BA, (u_char)(j/6)); 
			 bmas[j]=(u_int)inpw(0xA000+BA+2*(j%6));
			}
			
		 	 timer_setup( 1,1,0,0 );
			 
			for(j=0;j<13;j++)
			{
			 outp( 0xA000+BA, (u_char)(j/6)); 
			 mas[j]=(u_int)inpw(0xA000+BA+2*(j%6));
			}
	 
		 for(j=0;j<13;j++)
			if(mas[j]>=bmas[j])
				{
					xxx=mas[j]-bmas[j];
					rez=(float)xxx;
					dat[j]=dat[j]+rez;
				}else{
					xxx=bmas[j]-mas[j];
					rez=(float)xxx;
					dat[j]=dat[j]+65535.0-xxx;
				}
			
		sprintf(col,"N=%d",i+1);
		prus(col,73);
	}
	
pos=0;	
//clrscr();
do{	
	if(pos!=3)
		{
		rez=dat[pos*4]/pper;
		sprintf(col,"XT%d-%9.2f ",pos*16+30,rez);
		prusl(col,0);
		sprintf(col,"XT%d-%9.2f ",pos*16+34,dat[pos*4+1]/pper);
		prus(col,20);
		sprintf(col,"XT%d-%9.2f ",pos*16+38,dat[pos*4+2]/pper);
		prusl(col,40);
		sprintf(col,"XT%d-%9.2f ",pos*16+42,dat[pos*4+3]/pper);
		prus(col,60);
		}else{
		clrscr();
		sprintf(col,"XT%d-%9.2f ",26,dat[12]/pper);
		prusl(col,0);
		prusl("",40);
		}
}while((Kbd(pos,3)!=Esc)&&(key!='e'));
unlock();
return 1;
}
