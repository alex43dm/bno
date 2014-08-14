//------------------------
int TheApp::OnCBlenk(int key)
{
if(key)
{
	Error(OnBlenk);
	ShOnBlenk();
}else{
	Error(OffBlenk);
	ShOffBlenk();
}
return (int)Blenk;
}
//------------------------
int TheApp::OnMetrologyTest(void)
{
if(MetrologyFlag)
	{
		MetrologyFlag=OFF;
	}else{
		MetrologyFlag=ON;
	}
return 0;
}
//------------------------
int TheApp::OnTest(void)
{
int count,i;
u_int x;
double MAXN;
//проверка на выполнение теста
if(Power==1) return Error(NoTest);
if(Blenk==1) return Error(NoTest);
if(MetrologyFlag) return Error(NoTest);
if( SenCs>=1000 ){ MAXN=100;}else{ MAXN=10; }
if( (savedat.Dat*SenCs+Self)>MAXN )return Error(NoTest);

count=0;
OnCBlenk(ON);
do{
M=0;		
for(i=0;i<100;i++)
{
		if( sti >= st )
			{
				x = sti - st;
			}else{
				x = 65535 - st + sti;
			}

		M = M + x;		
pause();
}
Math =(double)M;
Math =Math/100.0;

if(BlenkN<=(Math+1.644*sqrt(Math))){
	Error(TestOk);
	OnCBlenk(OFF);
	M=0;		
	return 0;
	}
count++;
}while(count<2);
OnCBlenk(OFF);
Error(TestBad);
M=0;		

return -1;
}
