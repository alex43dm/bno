/***************************************************************************
                       Программа управления ИБП
                       ========================
 Формирует структуру состояния ИБП <tUps>. В структуре имя поля совпадает с
 командой обращения к ИБП (кроме "D") и с идентификатором при выводе в
 протокол. При выводе в протокол используется форма <Имя поля> = <значение>.
     Тип  Имя  No.       Параметр           Значение   Запись в
   данных поля п/п                          параметра протоколе
   ============================================================
   char   D    1   Состояние    входной
                   линии:
                   - есть напряжение        1         D = ON
                   - нет напряжения         0         D = OFF
   char   Q    2   Работающие   утилиты
                   ИБП
                   - норма выходного        0         Q = N
                    напряжения
                   - понижения              1         Q = LO
                   напряжения
                   - повышения              2         Q = HI
                   напряжения
                   - работа от батареи      3         Q = B
                   - разряд батареи и       4         Q = BT
                     выключение
                   - выключение по команде  5         Q = OFF
   double C    3   Температура, оС                    C = XX.X
   double M    4   Напряжение   входной               M = XXX.X
                   линии   максимальное
                   между измерениями, В
   double N    5   Напряжение   входной               N = XXX.X
                   линии    минимальное
                   между измерениями, В
   double L    6   Напряжение   входной               L = XXX.X
                   линии, В
   double O    7   Напряжение  выходной               O = XXX.X
                   линии, В
   double B    8   Напряжение  батареи,               B = XX.X
                   В
   char   X    9   Необходимость
                   заменить     батарею
                   (недостаточный
                   заряд)
                   - замена не нужна        0         X = NO
                   - нужна замена           1         X = YES
   double f    10  Заряд батареи,  %                  f = XXX
   short  j    11  Предполагаемое время               j = XXX
                   работы  от  батарей,
                   мин
   double F    12  Частота     выходной               F = XX.XX
                   линии,  Гц
   double P    13  Загрузка мощности, %               P = XXX
   char   W    14  Результат
                   самопроверки ИБП
                   - ненорма                0         W = NOK
                   - норма                  1         W = OK
 - Выключает питание контроллера.
 
 История:
 27.08.00 - восстановлен после сбоя диска.
 29.08.00 - откомпилирован под С++, стыкован обмен с АРМ и ССД.
 21.06.01 - обращение к сер.порту по имени
 ***************************************************************************/

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <String.h>
#include <signal.h>
#include <process.h>
#include <sys/name.h>
#include <sys/kernel.h>
#include <sys/proxy.h>
#include <time.h>
#include <unistd.h>

#include "../lib/com.h"
//#include "../lib/tools.h"

// параметры командной строки:
                           // UPS.exe
#define PN_SELF 		    1  // собственное имя процесса
#define COM_PORT        2  // имя порта, к которому подключен ИБП
#define PROC_TAKT       3  // период работы процесса
#define N_PARAMS  	    4  // общее число параметров


/*  Логика работы ИБП:
  - при первом обращении к порту задать начальные уставки и запустить
    тактовый таймер. Скорость обмена по порту 2400. Задержка перед
    выключением задается для ИБП специальной программой (например, 20 сек.).
  - Ожидать прихода сообщений:
    - по сообщению от таймера проверять содержимое входного буфера и
      заполнить структуру состояния ИБП;
    - принимать к исполнению команды от других процессов:
      - отдать cтруктуру состояния ИБП,
      - выключение ИБП полное,
      - имитация выключения входного питания.
      - другие команды, которые могут быть добавлены позже.
  Дополнительная информация - ИБП может управляться потенциалами контактов
  порта при наличии соответствующей распайки:
  - при сбое (пропадании) питания ИБП выдает сигнал CTS; (из MSR)
  - при разряде батареи ИБП выдает сигнал DCD;           (из MSR)
  - для отключения ИБП в порт следует подать сигнал DTR. (в MCR)
*/

void DeMes(char *Str) {
  printf("UPS: %s. Program terminate.\n",Str);
  exit(-1);
}

int SetTimer(   // запуск таймера для процессов
  char *Period, // с периодом
  int Mes       // и заданным сообщением
) {
  pid_t             proxy;
  struct sigevent   event;
  struct itimerspec timer;
  struct timespec   ts;
  timer_t           *timeID;

  if ((timeID=(timer_t *)malloc(sizeof(timer_t)))==NULL
  ||  (proxy=qnx_proxy_attach(0,&Mes,sizeof(Mes),-1))<0
  ) return -1;
  event.sigev_signo=-proxy; // таймер привязан к указанному процессу
	if ((timeID[0]=timer_create(CLOCK_REALTIME,&event))==-1)
    return -1;
  timer.it_value.tv_sec     =
  timer.it_interval.tv_sec  = (long)atoi(Period);
  timer.it_value.tv_nsec    = 
  timer.it_interval.tv_nsec = 0L;
  timer_settime(timeID[0],0,&timer,NULL);
    
  return 0;
}

void main(int argc, char *argv[]) {
  tMsgKadr  Mes[1];
  tUps      Data[1]; // блок параметров ИБП
  int       TestF=0; // признак теста
  int       rez;     // результат обработки
  char      Verbose=0; // для текстовой отладки
  Data->W=1;       // настоящая проверка будет потом
  if(argc<N_PARAMS)
    DeMes("bad number of parameters");
  if(qnx_name_attach(0,argv[PN_SELF])<0)
	  DeMes("can't attach his name");
  if (SetTimer(argv[PROC_TAKT],SM_TAKT)<0  // основной рабочий такт
  ||  SetTimer("86400",SM_TEST)<0)         // суточный такт
    DeMes("can't activate timer");
  CommPort  ComPort(argv[COM_PORT],2400,COMM_TEXT,&rez);
  if (rez!=0)
    DeMes("can't activate COM port");
  printf("%s Start. Connect with port %s\n",argv[PN_SELF],argv[COM_PORT]);
  for(;;) {
    pid_t Sender=Receive(0,Mes,sizeof(tMsgKadr)); // проба
    if (Verbose || Mes->Msg.Msg==SM_VERBOSE) printf("%s: ",argv[PN_SELF]);
    switch (Mes->Msg.Msg) {
      case SM_TAKT: { // каждый такт (10 сек. или 120)
        int  i;
        char QuerryChr[]="Y9QCMNLOBXfjFWU";
                     //   0123456789ABCDEF - номер в структуре
        char AnswerChr[]="YDQCMNLOBXfjFW";
        char PortBuf[300];
        char *bp;
        Reply(Sender,0,0);
        if (Verbose) printf("SM_TAKT:\n");
        PortBuf[0]=0;
        for (i=0; i<15; i++) {
          ComPort.Write(QuerryChr+i,1,1000); // отправили команду
          ComPort.ReadLn(PortBuf,300,1000,1000); // получили ответ
          // отбросим мусор сзади
          for (bp=PortBuf; *bp!=0 && *bp!=10 && *bp!=13; bp++);
          *bp=0;
          // отбросим мусор в начале
          for (
            bp=PortBuf;
            // проигнорируем символы:
                *bp=='#' // сигнал диалога
            ||  *bp=='*' // сигнал окончания диалога
            ||  *bp==240 // сигнал немедленного выключения
            ||  *bp=='%' // назначение непонятно
            ||  *bp=='+' // -"-
            ||  *bp=='!' // сигнал пропадания входного напряжения
            ||  *bp=='?' // сигнал разряда батареи
            ||  *bp=='$' // сигнал восстановления входного напряжения
            ; bp++
          );
          if (Verbose) printf("  UPS Answer: %2d) %c = %s\n",i,AnswerChr[i],bp);
          switch (i) { // анализ состояния ИБП
            case  0: // Y - Начальная инициализация диалога
              if (strcmp(bp,"SM")!=0) { // ответа нет?
                i=15;                      // конец диалога
                Data->W=0;                 // ИБП неисправен
              }
            break;
            case  1: Data->D=(strcmp(bp,"FF")!=0)? 0:1; // Напряжение на входе
                     break;
            case  2: { // Q - Работающие утилиты ИБП:
              int Ut=atoh(bp);
              if (Ut & 0x08) Data->Q=0; // использовано входное напряжение
            //if (Ut & 0x??) Data->Q=1; // понижение напряжения
              if (Ut & 0x04) Data->Q=2; // повышение напряжения
              if (Ut & 0x10) Data->Q=3; // работа от батареи
              if (Ut & 0x80) {
                             Data->Q=4; // разряд батареи
                Data->W=0; // батарея неисправна
                if (Data->D==0) { // входного напряжения нет
                  goto UnitOff;   // выключяемся
                }
              }
              else
                Data->W=1; // батарея в порядке
              //    5 - выключение по команде см. SM_UNIT_OFF
            } break;
            case  3: Data->C=atof(bp); // Температура, °С
                     break;
            case  4: Data->M=atof(bp); // Напряжение входное максимальное между измерениями, В
                     break;
            case  5: Data->N=atof(bp); // Напряжение входное минимальное между измерениями, В
                     break;
            case  6: Data->L=atof(bp); // Напряжение входной линии, В
                     break;
            case  7: Data->O=atof(bp); // Напряжение выходной линии, В
                     break;
            case  8: Data->B=atof(bp); // Напряжение батареи, В
                     break;
            case  9: Data->X=(strcmp(bp,"NO")==0)? // Менять батарею?
                                              0:1; // (Нет?/Да?)
                     break;
            case 10: Data->f=atof(bp); // Заряд батареи, %
                     break;
            case 11: Data->j=(short)atoi(bp);
                     // Предполагаемое время работы от батарей, мин
                     break;
            case 12: Data->F=atof(bp); // Частота выходной линии, Гц
                     if (TestF==0    // время теста не пришло
                     || (Data->D==0   // входного напряжения нет
                        && Data->W==0)  // аккумулятор неисправен
                     )
                       i=15;         // тест не делаем
                     break;
            case 14: // результат теста анализируем на след.такте
              TestF=0; // следующий тест - через сутки
            break;
          } //switch
        } //for
      } break;
      case SM_TEST: // раз в сутки
        Reply(Sender,0,0);
        if (Verbose) printf("SM_TEST: => SM_TAKT\n");
        TestF=1; // при обращении к ИБП будет проведен тест
      break;
			case SM_UNIT_OFF:
        Data[1].Q=5; // ИБП выключен по команде
        Mes->Msg.Msg=RM_UNIT_OFF;
        Reply(Sender,Mes,sizeof(tMsgShort));
        UnitOff:     // ИБП выключен из-за разряда батареи
      {
        char *StopStr="YKKR";
        char Buf[10];
        int  i;
        for (i=0; i<4; i++) {
          ComPort.Write(StopStr+i,1,1000);
          ComPort.ReadLn(Buf,10,5000,5000); // иначе не работает!!!
        }
        DeMes("UPS SWITCH OFF Now!!!"); // через установленное в ИБП время
      } break;
      case SM_GETLAST:
        Mes->Msg.Msg=RM_GETLAST;
        Mes->LenKadr=sizeof(tUps);
        memcpy(Mes->Kadr,Data,sizeof(tUps));
        Reply(Sender,Mes,sizeof(tMsgShort)+sizeof(Mes->LenKadr)+sizeof(tUps));
      break;
      case SM_PING: // проверка наличия процесса
        if (Verbose) printf("PING\n");
        Mes->Msg.Msg=RM_PING;
        Reply(Sender,Mes,sizeof(Mes->Msg));
      break;
      case SM_VERBOSE: // выводить все текстовые сообщения на консоль
        Verbose=!Verbose;
        printf("Verbose=%d\n",Verbose);
        Reply(Sender,0,0);
      break;
      case SM_SHUTDOWN: // закончить работу процесса
        Reply(Sender,0,0);
        DeMes("shutdown");
      default:
        if (Verbose) printf("unknown message - %d\n",Mes->Msg.Msg);
        Mes->Msg.Msg=RM_UNDEF;
        Reply(Sender,Mes,sizeof(Mes->Msg));
      break;
    }
  }
}
