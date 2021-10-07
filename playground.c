#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REMOTEPOINT "/home/yuta/tmp"

int main(){
    char *remote_path;
    char *path = "/test.txt";
    int len;
    len = strlen(REMOTEPOINT);
    printf("%d\n",len);

    remote_path = (char*)malloc(strlen(REMOTEPOINT) + strlen(path));
    strcpy(remote_path,REMOTEPOINT);
    strcat(remote_path,path);
    printf("%s\n",remote_path);

    return 0;
}