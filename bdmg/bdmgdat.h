//-биты состояния канала---
#define BLb		 	0x01 //состояние бленкера
#define PWb		 	0x02 //состояние высокого
#define CTb		 	0x04 //состояние объекта
#define MTb		 	0x08 //метрологическое измерение
//--------send mess------------------------
int TheApp::SendDat(mqd_t _mq, CData &dat )
{
dat.Time=time(NULL)-3600*6;
/*
mtime=localtime(&t);
memcpy(&dat.Time,mtime, sizeof(tm) );
*/
SetState();

mq_getattr( _mq, &mq_attrs );
if(mq_attrs.mq_maxmsg > mq_attrs.mq_curmsgs )
		{
		mq_prio=0;
		mq_send( _mq, &dat, sizeof(CData), mq_prio );
		}
if(_mq==mq_arj)
	memcpy( &savedat,&dat, sizeof(dat) );
return 0;

}
//------------------------
int TheApp::SetState(void)
{
char state = 0;
if(Power==ON ) state|=PWb;
if(Blenk==ON ) state|=BLb;
if(Dist==ON   )	state|=CTb;
if(MetrologyFlag==ON) state|=MTb;
dat.State = state;
return 0;
}
