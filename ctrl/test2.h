int TheApp::OnTest2(void)
{
int i;	


prus("формирование сигналов +12в ",0);
prus("вкл-Enter выкл-Space выход-Esc",40);
do{

if(key==Enter){
for(i=1;i<9;i+=2)pdio->SetBit(i,1);
for(i=17;i<25;i+=2)pdio->SetBit(i,1);
for(i=33;i<41;i+=2)pdio->SetBit(i,1);
pdio->SetBit(i,1);
}
if(key==Space){
for(i=1;i<9;i+=2)pdio->SetBit(i,0);
for(i=17;i<25;i+=2)pdio->SetBit(i,0);
for(i=33;i<41;i+=2)pdio->SetBit(i,0);
pdio->SetBit(i,0);
}
getkey();
}while((key!=Esc)&&(key!=0x65));

for(i=1;i<9;i+=2)pdio->SetBit(i,0);
for(i=17;i<25;i+=2)pdio->SetBit(i,0);
for(i=33;i<41;i+=2)pdio->SetBit(i,0);
pdio->SetBit(i,0);
unlock();
Comand=IDLE;
return 1;
}

