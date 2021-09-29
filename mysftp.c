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
	

	return 0;
}
