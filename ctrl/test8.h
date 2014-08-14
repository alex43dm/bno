int TheApp::OnTest8(void)
{
int i;
char j;
char Black = 0x7F;
//A
p->b[0]=Black;
prus("");
for(i=1;i<80;i++)
		{
		p->b[i-1]=Space;
		if(keypress())
				{
				if(key==Esc)break;
				if(key==Space)getkey();
				}
		p->b[i]=Black;
		prus("");
		timer_setup(0,0,SEC/4,SEC/4);
		}
		clrscr();
//B
for(i=1;i<80;i++)p->b[i]=Black;
p->b[0]=Space;
prus("");
for(i=1;i<80;i++)
		{
		p->b[i-1]=Black;
		if(keypress())
				{
				if(key==Esc)break;
				if(key==Space)getkey();
				}
		p->b[i]=Space;
		prus("");
		timer_up();
		}
		clrscr();
for(j=0x61;j<0x7B;j++)p->b[j-0x61]=j;
prus("0123456789:./+-=!?",27);
for(j=0xA0;j<0xC1;j++)p->b[j-0xA0+46]=j;
prus("");
getkey();
clrscr();
sem_post(&p->lock);
Comand=IDLE;
return 1;
}
