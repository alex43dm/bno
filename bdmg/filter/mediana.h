#ifndef _med
#define _med

#define NN 5
/*----------------------------------------

----------------------------------------*/
//++++++++++++++++++++++++++++++++++++++++++
class MEDIANA
{
 public:
	double K,Km;
  u_int   *m_buf;

  MEDIANA();
  void MFilter(u_int &, u_int );       //цифровой медианный фильтр
  void MClear(u_int &);
  ~MEDIANA();
};
//***************Методы*********************
MEDIANA::MEDIANA()
{
  m_buf=(u_int*)calloc(NN,sizeof(u_int));
}

void MEDIANA::MClear(u_int & init)
{ int i;
  for(i=0;i<NN;i++){
  m_buf[i]=init;
  }
} 
void MEDIANA::MFilter(u_int &in, u_int Mx)
{
int i,j=0,flag;
u_int m,//ret,
	x[NN];

	for(i=(NN-1);i>=0;i--){
	m_buf[i]=m_buf[i-1];
	x[i]=m_buf[i];};

	 m_buf[0]=in;
	 x[0]=in; 		              // буфер медианы

	do{
		flag=0;
	for(i=1;i< NN;i++){
		if(x[i]>x[i-1]){m=x[i];
				x[i]=x[i-1];
				x[i-1]=m;
				flag=1;
				};
		};
		j=j++;
	}while( (flag) && (j<(NN-1)) );
if(Mx!=0)
{	
	if( (in/Mx)>=(Km-K*in) )	
	 in=x[2];
}
}
MEDIANA::~MEDIANA()
{
	free(m_buf);
}
#endif
