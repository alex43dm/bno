int TheApp::OnTest4(void)
{
int count = 0;


prus("выход-Esc останов-Space",40);
do{
keypress();
if(key==Space)
			{
			 getkey();
			}
	sprintf(col,"дверь-%d XT22-%d XT4-%d XT5-%d"
	,pdio->GetBit(44),pdio->GetBit(45),
	pdio->GetBit(8),pdio->GetBit(9));
	prus(col,0);
prus(col,0);
timer_setup(1);
	count++;
	print("N=",73);
	print(itoa(count,col,10));

}while((key!=Esc)&&(key!='e'));
unlock();
Comand=IDLE;

return 1;
}
