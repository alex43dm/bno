//------------------------
int TheApp::OnTest6(void)
{
int count = 0;

prus("��室-Esc ��⠭��-Space",40);
do{
	sprintf(col,"6�-%d 12�-%d 24�I-%d 24�II-%d"
	,pdio->GetBit(43),pdio->GetBit(42),pdio->GetBit(41)
	,pdio->GetBit(40));
	prus(col,0);
prus(col,0);
timer_setup(1);
keypress();
if(key==Space)
			{
			getkey();
			}
	count++;
	print("N=",73);
	print(itoa(count,col,10));

}while((key!=Esc)&&(key!=0x65));
Comand=IDLE;
unlock();
return 1;
}
