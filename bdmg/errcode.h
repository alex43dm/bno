#ifndef _ERR
#define _ERR
//-------Error Code---------------
//#define Ok			 	1 //нормальное измерение
#define NNMax 			2 //частота выше максимальной
#define NNMin 			3 //частота ниже минимальной
#define NNNul 			7 //5c частота нулевая
//-------Error Code for Net--------
#define NoTest 			1 //не возможно провести тест по бленкеру
#define KDiff 			2 //быстрый рост значения
#define OffSensor		3 //выключено высокое
#define OnSensor		4 //включено высокое
#define OffChenel		5 //канал выключен с уровня БНО
#define OnChenel		6 //канал включен с уровня БНО
#define NNNul 			7 //5c частота нулевая
#define TestBad			10 //результат теста по бленкеру
#define TestOk			9 
#define OffBlenk		8 //выключен бленкер 
#define OnBlenk			11 //включен бленкер
//------------------------

#endif