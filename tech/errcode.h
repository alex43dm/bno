#ifndef _TECERR
#define _TECERR

#define ON 		1
#define OFF 	0
#define LHSCALE 0.01	
#define MHSCALE 1.0	
#define HHSCALE 1000.0 	
#define MQPATH "/dev/mqueue/" 	
#define DPATH "/usr/bdmg/" 	
//-------Messege Code------
#define BLb		 	0x01
#define PWb		 	0x02
#define CTb		 	0x04
#define MTb		 	0x08
//-------------------------
#define NG	 		6
//-------Massege Map begin--------
#define  HALT 		99
#define  DISP		1
#define  SET_INFO	3
#define  GET_INFO	5
#define  IDLE 		0
//-------Massege Map end----------
//-------Error Code for Net--------
#define OffSenX		1 //
#define OnSenX		2 //
//-------Command Code---------------

#endif