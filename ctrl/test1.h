int TheApp::OnTest1(void)
{
int i;
	
prus("формирование сигналов +6в",0);
prus("вкл-Enter выкл-Space выход-Esc",40);
do{
if(key==Enter){

	for(i=0;i<8;i+=2)	pdio->SetBit(i,1);
	for(i=16;i<24;i+=2)	pdio->SetBit(i,1);
	for(i=32;i<40;i+=2)	pdio->SetBit(i,1);
	pdio->SetBit(30,1);
}
if(key==Space){

	for(i=0;i<8;i+=2)	pdio->SetBit(i,0);
	for(i=16;i<24;i+=2)	pdio->SetBit(i,0);
	for(i=32;i<40;i+=2)	pdio->SetBit(i,0);
	pdio->SetBit(30,0);
}
getkey();
}while((key!=Esc)&&(key!='e'));

	for(i=0;i<8;i+=2)	pdio->SetBit(i,0);
	for(i=16;i<24;i+=2)	pdio->SetBit(i,0);
	for(i=32;i<40;i+=2)	pdio->SetBit(i,0);
	pdio->SetBit(30,0);

clrscr();
unlock();
Comand=IDLE;
return 1;
}

