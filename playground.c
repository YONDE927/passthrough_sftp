#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    int fd,nwritten,nbytes;
    char* buffer = "abcdefg";
    fd = open("I:\\DOC\\passthrough_sftp\\test.txt", O_CREAT|O_WRONLY,0777);
    if (fd < 0) {
      fprintf(stderr, "Can't open file for writing: %s\n",
              strerror(errno));
      return -1;
    }
    nwritten = write(fd, buffer, sizeof(buffer)-1);
    //printf("%d\n",sizeof(buffer)-1);
    //printf("%d\n",nwritten);
    if (nwritten != (sizeof(buffer)-1)) {
        fprintf(stderr, "Error writing: %s\n",
                strerror(errno));
        return -1;
    }

}