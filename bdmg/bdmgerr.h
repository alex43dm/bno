//------------------------
#include "errcode"
int TheApp::Error(u_char err)
{
warning(err);
//net(err);
//mq_send(mq_err,&errn,sizeof(errn),0);
//mq_send(mq_err1,&errn,sizeof(errn),0);

return 1;

}
