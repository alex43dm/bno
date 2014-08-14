#ifndef __bdmg
#define __bdmg

#define ON 		1
#define OFF 	0

//All errors
#define Ok 		1
#define Bad		-1

class CBdmgData
{
public:
double
		SenCs,
		Self,		//Level of an own background
		BlenkN,	//Maximum speed of the score from a blenker
		NMax,		//Maximum speed of the score
		NMin,		//Minimum speed of the score
		ScaleL,
		ScaleH,
		DPog,		//���. ����譮���	
		KDif;		//�����樥�� ����ண� ���
int		TimeAP,		//�६� ������⢨� ���.
	 	TimeReg;
char 	Cod[10];
		
int 	Cur;   //Currient count
int 	Auto,	//Automatic control of the sensor
    	Dist;	//Distance control by the sensor
//		Comand;	//Last command 
//***************************************************************
	CBdmgData()
	{
		Cur	 = 0;
	 	Auto = ON;	
    	Dist = OFF;	
	};
int	LoadData(char *Nfile)
	{
	char pass[20];
	int FlagOnOff;
	strcpy(pass,"/usr/bdmg/");
	strcat(pass,Nfile);
	strcat(pass,".dat");
	fstream file_dat(pass,ios::in,100);
		file_dat>>SenCs;
		file_dat>>Self;		//Level of an own background
		file_dat>>BlenkN;	//Maximum speed of the score from a blenker
		file_dat>>NMax;		//Maximum speed of the score
		file_dat>>NMin;		//Minimum speed of the score
		file_dat>>ScaleL;
		file_dat>>ScaleH;
		file_dat>>TimeAP;		//�६� ������⢨� ���.
		file_dat>>KDif;		//�����樥�� ����ண� ���
	 	file_dat>>TimeReg;
		file_dat>>DPog;		//���. ����譮���	
		file_dat>>Cod;	
		file_dat>>FlagOnOff;
		Cur	 = 0;
	 	Auto = ON;	
    	Dist = OFF;	
		return FlagOnOff;
	};
	SaveData(char *Nfile, int PW)
	{
	char pass[20];
	strcpy(pass,"/usr/bdmg/");
	strcat(pass,Nfile);
	strcat(pass,".dat");
	fstream file_dat(pass,ios::out,100);

		file_dat<<SenCs<<endl;
		file_dat<<Self<<endl;		//Level of an own background
		file_dat<<BlenkN<<endl;	//Maximum speed of the score from a blenker
		file_dat<<NMax<<endl;		//Maximum speed of the score
		file_dat<<NMin<<endl;		//Minimum speed of the score
		file_dat<<ScaleL<<endl;
		file_dat<<ScaleH<<endl;
		file_dat<<TimeAP<<endl;		//�६� ������⢨� ���.
		file_dat<<KDif<<endl;		//�����樥�� ����ண� ���
	 	file_dat<<TimeReg<<endl;
		file_dat<<DPog<<endl;		//���. ����譮���	
		file_dat<<Cod<<endl;		//���. ����譮���	
		file_dat<<PW<<endl;

	};
//***************************************************************

	~CBdmgData(){};

};
#endif
