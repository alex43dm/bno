
int TheApp::OnTest11(void)
{
int i;
u_int ba,regs;
char str[3];//, *buf;

//3
//prusl("5515-��ଠ ",0);
//4
prusl("UNIO-��ଠ ",0);

		outp( BA + 0x5401, 0x00 ); //extend mode adress
		outp( BA + 0xA00D, 0x00 ); //disable interrupt
		for( i = 0, ba = BA + 0xA00E; i < 4; i++, ba += 0x400 )
			{
				str[0]=(char)inp( ba );
				itoa((int)inp( ba + 1 ),&str[1],10);
			 switch(i)
				{
				case 0:	
						break;
				case 1:
						if(strcmp(str,"g0")!=0)
							printrus("�⪠� ",23);
						break;
				case 2:
						break;
				case 3:
						if(strcmp(str,"g0")!=0)
							prus("�⪠� ",23);
						break;
				}
			}

//5
prus("5558-��ଠ",18);
		for( i = 0, ba = 0x100; i < 8; i++, ba += 0x08 )
			{
			regs = inp( ba + 3 );
				outp(ba + 3, 0xF0 );
						if(inp(ba+3)!=0xF0)
							prus("�⪠� ",45);
			outp(ba + 3, regs );
			}


prusl("��室-�� ������",40);
getkey();
unlock();
return 1;
}

