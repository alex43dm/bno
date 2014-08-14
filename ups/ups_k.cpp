/***************************************************************************
                       �ணࠬ�� �ࠢ����� ���
                       ========================
 ��ନ��� �������� ���ﭨ� ��� <tUps>. � ������� ��� ���� ᮢ������ �
 �������� ���饭�� � ��� (�஬� "D") � � �����䨪��஬ �� �뢮�� �
 ��⮪��. �� �뢮�� � ��⮪�� �ᯮ������ �ଠ <��� ����> = <���祭��>.
     ���  ���  No.       ��ࠬ���           ���祭��   ������ �
   ������ ���� �/�                          ��ࠬ��� ��⮪���
   ============================================================
   char   D    1   ����ﭨ�    �室���
                   �����:
                   - ���� ����殮���        1         D = ON
                   - ��� ����殮���         0         D = OFF
   char   Q    2   ������騥   �⨫���
                   ���
                   - ��ଠ ��室����        0         Q = N
                    ����殮���
                   - ���������              1         Q = LO
                   ����殮���
                   - ����襭��              2         Q = HI
                   ����殮���
                   - ࠡ�� �� ���२      3         Q = B
                   - ࠧ�� ���२ �       4         Q = BT
                     �몫�祭��
                   - �몫�祭�� �� �������  5         Q = OFF
   double C    3   ���������, ��                    C = XX.X
   double M    4   ����殮���   �室���               M = XXX.X
                   �����   ���ᨬ��쭮�
                   ����� ����७�ﬨ, �
   double N    5   ����殮���   �室���               N = XXX.X
                   �����    �������쭮�
                   ����� ����७�ﬨ, �
   double L    6   ����殮���   �室���               L = XXX.X
                   �����, �
   double O    7   ����殮���  ��室���               O = XXX.X
                   �����, �
   double B    8   ����殮���  ���२,               B = XX.X
                   �
   char   X    9   ����室������
                   ��������     �����
                   (���������
                   ����)
                   - ������ �� �㦭�        0         X = NO
                   - �㦭� ������           1         X = YES
   double f    10  ���� ���२,  %                  f = XXX
   short  j    11  �।���������� �६�               j = XXX
                   ࠡ���  ��  ���३,
                   ���
   double F    12  �����     ��室���               F = XX.XX
                   �����,  ��
   double P    13  ����㧪� ��魮��, %               P = XXX
   char   W    14  �������
                   ᠬ��஢�ન ���
                   - ����ଠ                0         W = NOK
                   - ��ଠ                  1         W = OK
 - �몫�砥� ��⠭�� ����஫���.
 
 �����:
 27.08.00 - ����⠭����� ��᫥ ᡮ� ��᪠.
 29.08.00 - �⪮�����஢�� ��� �++, ��몮��� ����� � ��� � ���.
 21.06.01 - ���饭�� � ��.����� �� �����
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

// ��ࠬ���� ��������� ��ப�:
                           // UPS.exe
#define PN_SELF 		    1  // ᮡ�⢥���� ��� �����
#define COM_PORT        2  // ��� ����, � ���஬� ������祭 ���
#define PROC_TAKT       3  // ��ਮ� ࠡ��� �����
#define N_PARAMS  	    4  // ��饥 �᫮ ��ࠬ��஢


/*  ������ ࠡ��� ���:
  - �� ��ࢮ� ���饭�� � ����� ������ ��砫�� ��⠢�� � ��������
    ⠪⮢� ⠩���. ������� ������ �� ����� 2400. ����প� ��।
    �몫�祭��� �������� ��� ��� ᯥ樠�쭮� �ணࠬ��� (���ਬ��, 20 ᥪ.).
  - ������� ��室� ᮮ�饭��:
    - �� ᮮ�饭�� �� ⠩��� �஢����� ᮤ�ন��� �室���� ���� �
      ��������� �������� ���ﭨ� ���;
    - �ਭ����� � �ᯮ������ ������� �� ��㣨� ����ᮢ:
      - �⤠�� c������� ���ﭨ� ���,
      - �몫�祭�� ��� ������,
      - ������ �몫�祭�� �室���� ��⠭��.
      - ��㣨� �������, ����� ����� ���� ��������� �����.
  �������⥫쭠� ���ଠ�� - ��� ����� �ࠢ������ ��⥭樠���� ���⠪⮢
  ���� �� ����稨 ᮮ⢥�����饩 �ᯠ���:
  - �� ᡮ� (�ய������) ��⠭�� ��� �뤠�� ᨣ��� CTS; (�� MSR)
  - �� ࠧ�拉 ���२ ��� �뤠�� ᨣ��� DCD;           (�� MSR)
  - ��� �⪫�祭�� ��� � ���� ᫥��� ������ ᨣ��� DTR. (� MCR)
*/

void DeMes(char *Str) {
  printf("UPS: %s. Program terminate.\n",Str);
  exit(-1);
}

int SetTimer(   // ����� ⠩��� ��� ����ᮢ
  char *Period, // � ��ਮ���
  int Mes       // � ������� ᮮ�饭���
) {
  pid_t             proxy;
  struct sigevent   event;
  struct itimerspec timer;
  struct timespec   ts;
  timer_t           *timeID;

  if ((timeID=(timer_t *)malloc(sizeof(timer_t)))==NULL
  ||  (proxy=qnx_proxy_attach(0,&Mes,sizeof(Mes),-1))<0
  ) return -1;
  event.sigev_signo=-proxy; // ⠩��� �ਢ易� � 㪠������� ������
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
  tUps      Data[1]; // ���� ��ࠬ��஢ ���
  int       TestF=0; // �ਧ��� ���
  int       rez;     // १���� ��ࠡ�⪨
  char      Verbose=0; // ��� ⥪�⮢�� �⫠���
  Data->W=1;       // ������� �஢�ઠ �㤥� ��⮬
  if(argc<N_PARAMS)
    DeMes("bad number of parameters");
  if(qnx_name_attach(0,argv[PN_SELF])<0)
	  DeMes("can't attach his name");
  if (SetTimer(argv[PROC_TAKT],SM_TAKT)<0  // �᭮���� ࠡ�稩 ⠪�
  ||  SetTimer("86400",SM_TEST)<0)         // ����� ⠪�
    DeMes("can't activate timer");
  CommPort  ComPort(argv[COM_PORT],2400,COMM_TEXT,&rez);
  if (rez!=0)
    DeMes("can't activate COM port");
  printf("%s Start. Connect with port %s\n",argv[PN_SELF],argv[COM_PORT]);
  for(;;) {
    pid_t Sender=Receive(0,Mes,sizeof(tMsgKadr)); // �஡�
    if (Verbose || Mes->Msg.Msg==SM_VERBOSE) printf("%s: ",argv[PN_SELF]);
    switch (Mes->Msg.Msg) {
      case SM_TAKT: { // ����� ⠪� (10 ᥪ. ��� 120)
        int  i;
        char QuerryChr[]="Y9QCMNLOBXfjFWU";
                     //   0123456789ABCDEF - ����� � �������
        char AnswerChr[]="YDQCMNLOBXfjFW";
        char PortBuf[300];
        char *bp;
        Reply(Sender,0,0);
        if (Verbose) printf("SM_TAKT:\n");
        PortBuf[0]=0;
        for (i=0; i<15; i++) {
          ComPort.Write(QuerryChr+i,1,1000); // ��ࠢ��� �������
          ComPort.ReadLn(PortBuf,300,1000,1000); // ����稫� �⢥�
          // ���ᨬ ���� ᧠��
          for (bp=PortBuf; *bp!=0 && *bp!=10 && *bp!=13; bp++);
          *bp=0;
          // ���ᨬ ���� � ��砫�
          for (
            bp=PortBuf;
            // �ந�����㥬 ᨬ����:
                *bp=='#' // ᨣ��� �������
            ||  *bp=='*' // ᨣ��� ����砭�� �������
            ||  *bp==240 // ᨣ��� ������������ �몫�祭��
            ||  *bp=='%' // �����祭�� ������⭮
            ||  *bp=='+' // -"-
            ||  *bp=='!' // ᨣ��� �ய������ �室���� ����殮���
            ||  *bp=='?' // ᨣ��� ࠧ�鸞 ���२
            ||  *bp=='$' // ᨣ��� ����⠭������� �室���� ����殮���
            ; bp++
          );
          if (Verbose) printf("  UPS Answer: %2d) %c = %s\n",i,AnswerChr[i],bp);
          switch (i) { // ������ ���ﭨ� ���
            case  0: // Y - ��砫쭠� ���樠������ �������
              if (strcmp(bp,"SM")!=0) { // �⢥� ���?
                i=15;                      // ����� �������
                Data->W=0;                 // ��� ����ࠢ��
              }
            break;
            case  1: Data->D=(strcmp(bp,"FF")!=0)? 0:1; // ����殮��� �� �室�
                     break;
            case  2: { // Q - ������騥 �⨫��� ���:
              int Ut=atoh(bp);
              if (Ut & 0x08) Data->Q=0; // �ᯮ�짮���� �室��� ����殮���
            //if (Ut & 0x??) Data->Q=1; // ��������� ����殮���
              if (Ut & 0x04) Data->Q=2; // ����襭�� ����殮���
              if (Ut & 0x10) Data->Q=3; // ࠡ�� �� ���२
              if (Ut & 0x80) {
                             Data->Q=4; // ࠧ�� ���२
                Data->W=0; // ����� ����ࠢ��
                if (Data->D==0) { // �室���� ����殮��� ���
                  goto UnitOff;   // �몫��塞��
                }
              }
              else
                Data->W=1; // ����� � ���浪�
              //    5 - �몫�祭�� �� ������� �. SM_UNIT_OFF
            } break;
            case  3: Data->C=atof(bp); // ���������, ��
                     break;
            case  4: Data->M=atof(bp); // ����殮��� �室��� ���ᨬ��쭮� ����� ����७�ﬨ, �
                     break;
            case  5: Data->N=atof(bp); // ����殮��� �室��� �������쭮� ����� ����७�ﬨ, �
                     break;
            case  6: Data->L=atof(bp); // ����殮��� �室��� �����, �
                     break;
            case  7: Data->O=atof(bp); // ����殮��� ��室��� �����, �
                     break;
            case  8: Data->B=atof(bp); // ����殮��� ���२, �
                     break;
            case  9: Data->X=(strcmp(bp,"NO")==0)? // ������ �����?
                                              0:1; // (���?/��?)
                     break;
            case 10: Data->f=atof(bp); // ���� ���२, %
                     break;
            case 11: Data->j=(short)atoi(bp);
                     // �।���������� �६� ࠡ��� �� ���३, ���
                     break;
            case 12: Data->F=atof(bp); // ����� ��室��� �����, ��
                     if (TestF==0    // �६� ��� �� ��諮
                     || (Data->D==0   // �室���� ����殮��� ���
                        && Data->W==0)  // �������� ����ࠢ��
                     )
                       i=15;         // ��� �� ������
                     break;
            case 14: // १���� ��� ��������㥬 �� ᫥�.⠪�
              TestF=0; // ᫥���騩 ��� - �१ ��⪨
            break;
          } //switch
        } //for
      } break;
      case SM_TEST: // ࠧ � ��⪨
        Reply(Sender,0,0);
        if (Verbose) printf("SM_TEST: => SM_TAKT\n");
        TestF=1; // �� ���饭�� � ��� �㤥� �஢���� ���
      break;
			case SM_UNIT_OFF:
        Data[1].Q=5; // ��� �몫�祭 �� �������
        Mes->Msg.Msg=RM_UNIT_OFF;
        Reply(Sender,Mes,sizeof(tMsgShort));
        UnitOff:     // ��� �몫�祭 ��-�� ࠧ�鸞 ���२
      {
        char *StopStr="YKKR";
        char Buf[10];
        int  i;
        for (i=0; i<4; i++) {
          ComPort.Write(StopStr+i,1,1000);
          ComPort.ReadLn(Buf,10,5000,5000); // ���� �� ࠡ�⠥�!!!
        }
        DeMes("UPS SWITCH OFF Now!!!"); // �१ ��⠭�������� � ��� �६�
      } break;
      case SM_GETLAST:
        Mes->Msg.Msg=RM_GETLAST;
        Mes->LenKadr=sizeof(tUps);
        memcpy(Mes->Kadr,Data,sizeof(tUps));
        Reply(Sender,Mes,sizeof(tMsgShort)+sizeof(Mes->LenKadr)+sizeof(tUps));
      break;
      case SM_PING: // �஢�ઠ ������ �����
        if (Verbose) printf("PING\n");
        Mes->Msg.Msg=RM_PING;
        Reply(Sender,Mes,sizeof(Mes->Msg));
      break;
      case SM_VERBOSE: // �뢮���� �� ⥪�⮢� ᮮ�饭�� �� ���᮫�
        Verbose=!Verbose;
        printf("Verbose=%d\n",Verbose);
        Reply(Sender,0,0);
      break;
      case SM_SHUTDOWN: // �������� ࠡ��� �����
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
