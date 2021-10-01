#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libssh2.h>
#include "libssh2_config.h"
#include <libssh2_sftp.h>
#include <WinSock2.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

//const char* keyfile = "C:/Users/svyut/.ssh/redmine.pem";
const char* username = "yuta";
const char* server_ip = "172.25.217.120";
const char* password = "taiki927";
const char* sftppath = "/home/yuta/test.txt";

int main(){
	// unsigned long hostaddr;
	// int sock,i,auth_pw=0;
	// struct sockaddr_in sin;
	// const char *fingerprint;
	// char *userauthlist;
	// LIBSSH2_SESSION *session;
	int rc;
	// LIBSSH2_SFTP *sftp_session;
	// LIBSSH2_SFTP_HANDLE *sftp_handle;
	// WSADATA wsadata;
	// int err;

	// printf("wsastartup\n");

	// err = WSAStartup(MAKEWORD(2,0),&wsadata);
	// if(err != 0){
	// 	fprintf(stderr,"WSAStartup failed %d.\n",err);
	// 	return 1;
	// }
	// hostaddr = inet_addr(server_ip);

	//rc = libssh2_init(0);
	rc = libssh2_init(LIBSSH2_INIT_NO_CRYPTO);
	printf("done %d\n",rc);
	return 0;

	// if(rc!=0){
	// 	fprintf(stderr,"libssh2 init failed %d.\n",rc);
	// 	return 1;
	// }

	// //tcp connections
	// sock = socket(AF_INET,SOCK_STREAM,0);
	// //サーバーのアドレス構造体
	// sin.sin_family = AF_INET;
	// sin.sin_port = htons(22);
	// sin.sin_addr.s_addr = hostaddr;
	// if(connect(sock,(struct sockaddr*)&sin,sizeof(struct sockaddr_in))!=0){
	// 	fprintf(stderr,"connection failed .\n");
	// 	return -1;
	// }

	// //ssh session
	// session = libssh2_session_init();

	// if(!session){
	// 	return -1;
	// }

	// libssh2_session_set_blocking(session,1);

	// rc = libssh2_session_handshake(session,sock);

	// fingerprint = libssh2_hostkey_hash(session,LIBSSH2_HOSTKEY_HASH_SHA1);

	// userauthlist = libssh2_userauth_list(session,username,strlen(username));

	// if(libssh2_userauth_password(session,username,password)){
	// 	fprintf(stderr,"Auth by password failed.\n");
	// 	goto shutdown;
	// }

	// //sftp session
	// fprintf(stderr,"libssh2_sftp_init()!\n");
	// sftp_session = libssh2_sftp_init(session);
	
	// //sftp open
	// sftp_handle = libssh2_sftp_open(sftp_session,sftppath,LIBSSH2_FXF_READ,0);
	// if(!sftp_handle){
	// 	fprintf(stderr,"Unable to open file\n");
	// 	goto shutdown;
	// }
	// fprintf(stderr,"sftp_open is done\n");

	// do{
	// 	char mem[1024];
	// 	rc = libssh2_sftp_read(sftp_handle,mem,sizeof(mem));
	// 	if(rc>0){
	// 		fwrite(mem,rc,rc,stdout);
	// 	}else{
	// 		break;
	// 	}
	// }while(1);

	// //sftp close
	// libssh2_sftp_close(sftp_handle);
	// //sftp session shutdown
	// libssh2_sftp_shutdown(sftp_session);


	// shutdown:
	// libssh2_session_disconnect(session,"Normal Shutdown");
	// libssh2_session_free(session);
	// closesocket(sock);
	// libssh2_exit();
	// return 0;



}
