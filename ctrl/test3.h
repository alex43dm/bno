int TheApp::OnTest3(void)
{
prus("�ନ஢���� ��᮪�������� ᨣ����� ",0);
prus("���-Enter �몫-Space ��室-Esc",40);
do{
if(key==Enter){
pdio->SetBit(26,1);
pdio->SetBit(27,1);
}
if(key==Space){
pdio->SetBit(26,0);
pdio->SetBit(27,0);
}
getkey();
}while((key!=Esc)&&(key!=0x65));
pdio->SetBit(26,0);
pdio->SetBit(27,0);
unlock();
Comand=IDLE;
return 1;
}
