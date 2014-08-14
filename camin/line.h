int TheApp::Line1( void )
{
pid_t pid;
int   fd[2];
char  buffer[82],*p,
buf[82];
int   status;
sigset_t sigset;
	

    if( pipe( fd ) == -1 ) {
//       perror( "pipe" );
       return -1;
    }

    if( ( pid = fork() ) == -1 ) {
  //     perror( "fork" );
       return -1;
    }
    if( pid == 0 ) {
		sigaddset( &sigset, 2 );
		close( fd[0] );
		dup2(fd[1],stdout->_handle);
		system("ping ssd1");
		close( fd[1] );
		exit(0);
    } 
		close( fd[1] );
		signal( 2, SIG_IGN );
		sleep(4);
	  	kill(0,2);
		fcntl(fd[0],O_NONBLOCK);
		strcpy(buf,"");
		while( read( fd[0], buffer, sizeof( buffer ) ) )
		{
		if( (p=strstr(buffer,"ping statistics"))!=NULL)
			        printf("статистика\n");
		p=strstr(buffer,"packets transmitted");
		if( p!=NULL){
			        prusl("статистика канала бно-ссд1 пакеты:");
					sprintf(buf,"%d передано %d принято %d",
					atoi(p-2),atoi(p+21),atoi(p+41));
			        prusl(buf,40);
			        prus("% потеряно");
					}
		}

wait( &status );
close( fd[0] );
return 0;
}
int TheApp::Line2( void )
{
pid_t pid;
int   fd[2];
char  buffer[82],*p,
buf[82];
sigset_t sigset;
int   status;
	

    if( pipe( fd ) == -1 ) {
//       perror( "pipe" );
       return -1;
    }

    if( ( pid = fork() ) == -1 ) {
  //     perror( "fork" );
       return -1;
    }
    if( pid == 0 ) {
		sigaddset( &sigset, 2 );
		close( fd[0] );
		dup2(fd[1],stdout->_handle);
		system("ping ssd2");
		close( fd[1] );
		exit(0);
    }
		close( fd[1] );
		signal( 2, SIG_IGN );
		sleep(4);
	  	kill(0,2);
		fcntl(fd[0],O_NONBLOCK);
		strcpy(buf,"");
		while( read( fd[0], buffer, sizeof( buffer ) ) )
		{
		if( (p=strstr(buffer,"ping statistics"))!=NULL)
			        printf("статистика\n");
		p=strstr(buffer,"packets transmitted");
		if( p!=NULL){
			        prusl("статистика канала бно-ссд2 пакеты:");
					sprintf(buf,"%d передано %d принято %d",
					atoi(p-2),atoi(p+21),atoi(p+41));
			        prusl(buf,40);
			        prus("% потеряно");
					}
		}

wait( &status );
close( fd[0] );
return 0;
}
