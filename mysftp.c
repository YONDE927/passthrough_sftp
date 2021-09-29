#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libssh2.h>
#include <libssh2_config.h>
#include <libssh2_sftp.h>
#include <WinSock2.h>

const char* keyfile = "C:/Users/svyut/.ssh/redmine.pem";
const char* username = "redmine_user";
const char* server_ip = "153.126.189.240";

int main(int argc, char* argv[]){
	unsigned long hostaddr;
	int sock,i,auth_pw=0;
	struct sockaddr_in sin;
	const char *fingerprint;
	char *userauthlist;
	LIBSSH2_SESSION *session;
	int rc;
	LIBSSH2_SFTP *sftp_session;
	LIBSSH2_SFTP_HANDLE *sftp_handle;
	WSADATA wsadata;
	int err;

	err = WSAStartup(MAKEWORD(2,0),&wsadata);
	if(err != 0){
		fprintf(stderr,"WSAStartup failed %d.\n",err);
	}
	hostaddr = inet_addr(server_ip);

	rc = libssh2_init(0);

	if(rc!=0){
		fprintf(stderr,"libssh2 init failed %d.\n",rc);
	}

	//tcp connections
	sock = socket(AF_INET,SOCK_STREAM,0);
	//サーバーのアドレス構造体
	sin.sin_family = AF_INET;
	sin.sin_port = htons(22);
	sin.sin_addr.s_addr = hostaddr;
	if(connect(sock,(struct sockaddr*)&sin,sizeof(struct sockaddr_in))!=0){
		fprintf(stderr,"connection failed %d.\n");
		return -1;
	}

	//ssh session
	session = libssh2_session_init();

	if(!session){
		return -1;
	}

	libssh2_session_set_blocking(session,1);

	rc = libssh2_session_handshake(session,sock);

	return 0;
}
