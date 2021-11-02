#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <fuse3/fuse.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define HOSTIP "172.23.20.169"
#define HOSTPORT 22
#define USERNAME "yuta"
#define PASSWORD "taiki927"
#define MAX_XFER_BUF_SIZE 16384

char* mountpoint = "/home/yuta/tmp";

int show_remote_processes(ssh_session);
int sftp_init_(ssh_session*,sftp_session*);
int sftp_getattr(ssh_session*,sftp_session*,char*);
int sftp_list_dir(ssh_session, sftp_session,char*);
int sftp_read_sync(ssh_session, sftp_session );

int main()
{
  ssh_session myssh;
  sftp_session mysftp;
  
  sftp_init_(&myssh,&mysftp);

  //do something
  //show_remote_processes(my_ssh_session);
  sftp_getattr(&myssh,&mysftp,"/dir1/file3");

  sftp_list_dir(myssh,mysftp,"/dir1");

  //sftp_read_sync(my_ssh_session,mysftp);
  
  //delete sftp session
  sftp_free(mysftp);

  //close ssh
  ssh_disconnect(myssh);
  printf("disconnected!\n");
  ssh_free(myssh);

  return 0;
}

int sftp_init_(ssh_session *ssh, sftp_session *sftp)
{
    int rc;
    unsigned int port = HOSTPORT;
    const char *user = USERNAME;
    const char *password = PASSWORD;

    //ssh session
    //ssh = (ssh_session*)malloc(sizeof(ssh_session));
    *ssh = ssh_new();

    if (*ssh == NULL)
        exit(-1);

    ssh_options_set(*ssh, SSH_OPTIONS_HOST, HOSTIP);
    ssh_options_set(*ssh, SSH_OPTIONS_PORT, &port);

    //Connect to server
    rc = ssh_connect(*ssh);
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Error connecting to localhost: %s\n",
                ssh_get_error(*ssh));
        exit(-1);
    }
    printf("connected!\n");

    //User authentication
    rc = ssh_userauth_password(*ssh, user, password);
    if (rc != SSH_AUTH_SUCCESS)
    {
        fprintf(stderr, "Error authenticating with password: %s\n",
                ssh_get_error(*ssh));
        ssh_disconnect(*ssh);
        ssh_free(*ssh);
        exit(-1);
    }
    printf("User auth success!\n");

    //sftp session
    //sftp = (sftp_session*)malloc(sizeof(sftp_session));
    *sftp = sftp_new(*ssh);
    if (*sftp == NULL)
    {
        fprintf(stderr, "Error allocating SFTP session: %s\n",
                ssh_get_error(*ssh));
        return SSH_ERROR;
    }

    rc = sftp_init(*sftp);
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Error initializing SFTP session: code %d.\n",
                sftp_get_error(*sftp));
        sftp_free(*sftp);
        return rc;
    }
    printf("sftp session start!\n");

    return SSH_OK;
}

//fstat
int sftp_getattr(ssh_session *ssh,sftp_session *sftp,char *path){
    sftp_attributes attributes;
    struct fuse_stat* stat;
    char* _path;
    //pathに/home/yutaを付ける。
    _path = malloc(sizeof(mountpoint) + sizeof(path));
    strcpy(_path,mountpoint);
    strcat(_path,path);
    printf("getattr %s\n",_path);
    //sttp_statを呼ぶ。attributesにstatを格納
    attributes = sftp_stat(*sftp,_path);
    if(!attributes){
        fprintf(stderr, "file info not obtained: %s\n",
                ssh_get_error(*ssh));
        return SSH_ERROR;
    }
    stat = malloc(sizeof(struct fuse_stat));
    stat->st_mode = attributes->permissions;
    stat->st_nlink = 1;
    stat->st_size = attributes->size;
    stat->st_uid = attributes->uid;
    stat->st_gid = attributes->gid;
    free(stat);
    printf("name : %s\n",attributes->name);
    printf("longname : %s\n",attributes->longname);
    printf("permission : %o\n",attributes->permissions);
    printf("size : %ld\n",attributes->size);
    printf("uid : %d\n",attributes->uid);
    printf("gid : %d\n",attributes->gid);
    free(_path);
    return SSH_OK;
}

int show_remote_processes(ssh_session session)
{
  ssh_channel channel;
  int rc;
  char buffer[256];
  int nbytes;
 
  channel = ssh_channel_new(session);
  if (channel == NULL)
    return SSH_ERROR;
 
  rc = ssh_channel_open_session(channel);
  if (rc != SSH_OK)
  {
    ssh_channel_free(channel);
    return rc;
  }
 

  rc = ssh_channel_request_exec(channel, "ps aux");
  if (rc != SSH_OK)
  {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return rc;
  }
 
  nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
  while (nbytes > 0)
  {
    if (write(1,buffer, nbytes) != (unsigned int) nbytes)
    {
      ssh_channel_close(channel);
      ssh_channel_free(channel);
      return SSH_ERROR;
    }
    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
  }
 
  if (nbytes < 0)
  {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return SSH_ERROR;
  }

  ssh_channel_send_eof(channel);
  ssh_channel_close(channel);
  ssh_channel_free(channel);
 
  return SSH_OK;
}

int sftp_list_dir(ssh_session session, sftp_session sftp,char *path)
{
  sftp_dir dir;
  sftp_attributes attributes;
  int rc;
  char* _path;
  //pathに/home/yutaを付ける。
  _path = malloc(sizeof(mountpoint) + sizeof(path));
  strcpy(_path,mountpoint);
  strcat(_path,path);
  printf("opendir %s\n",_path);

  dir = sftp_opendir(sftp,_path);
  if (!dir)
  {
    fprintf(stderr, "Directory not opened: %s\n",
            ssh_get_error(session));
    return SSH_ERROR;
  }
 
  printf("Name                       Size Perms    Owner\tGroup\n");
 
  while ((attributes = sftp_readdir(sftp, dir)) != NULL)
  {
    printf("%-20s %10llu %.8o %s(%d)\t%s(%d)\n",
     attributes->name,
     (long long unsigned int) attributes->size,
     attributes->permissions,
     attributes->owner,
     attributes->uid,
     attributes->group,
     attributes->gid);
 
     sftp_attributes_free(attributes);
  }
  free(_path);
 
  if (!sftp_dir_eof(dir))
  {
    fprintf(stderr, "Can't list directory: %s\n",
            ssh_get_error(session));
    sftp_closedir(dir);
    return SSH_ERROR;
  }
 
  rc = sftp_closedir(dir);
  if (rc != SSH_OK)
  {
    fprintf(stderr, "Can't close directory: %s\n",
            ssh_get_error(session));
    return rc;
  }
  return rc;
}

 
int sftp_read_sync(ssh_session session, sftp_session sftp)
{
  int access_type;
  sftp_file file;
  char buffer[MAX_XFER_BUF_SIZE];
  int nbytes, nwritten, rc;
  int fd;
 
  access_type = O_RDONLY;
  file = sftp_open(sftp, "/home/yuta/test.txt",
                   access_type, 0);
  if (file == NULL) {
      fprintf(stderr, "Can't open file for reading: %s\n",
              ssh_get_error(session));
      return SSH_ERROR;
  }
 
  fd = open("I:\\DOC\\passthrough_sftp\\test.txt", O_CREAT|O_WRONLY,0777);
  if (fd < 0) {
      fprintf(stderr, "Can't open file for writing: %s\n",
              strerror(errno));
      return SSH_ERROR;
  }
  printf("file descriptor : %d\n",fd);

  for (;;) {
      nbytes = sftp_read(file, buffer, sizeof(buffer));
      if (nbytes == 0) {
          break; // EOF
      } else if (nbytes < 0) {
          fprintf(stderr, "Error while reading file: %s\n",
                  ssh_get_error(session));
          sftp_close(file);
          return SSH_ERROR;
      }
      printf("nbytes : %d\n",nbytes);
      printf("sizeof(buffer) : %ld\n",sizeof(buffer));
      //printf("%s\n",buffer);
      nwritten = write(fd, buffer, nbytes);
      printf("%d\n",nwritten);
      if (nwritten != nbytes) {
          fprintf(stderr, "Error writing: %s\n",
                  strerror(errno));
          sftp_close(file);
          return SSH_ERROR;
      }
  }
 
  rc = sftp_close(file);
  if (rc != SSH_OK) {
      fprintf(stderr, "Can't close the read file: %s\n",
              ssh_get_error(session));
      return rc;
  }
 
  return SSH_OK;
}