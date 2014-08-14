//------------------------
int TheApp::OnTest9(void)
{

prusl("ALPHA - перелючение раскладки клавиатуры",0);
timer_setup(1);
clrscr();
prus("нажмите любую клавишу ",0);
prus("выход-Enter",40);

do{

//str[1]=(char)getkey();
//print(str);
getkey();
sprintf(col,"%c",key);
print(col,25);

}while(key!=Enter);
unlock();
Comand=IDLE;

return 1;
}
