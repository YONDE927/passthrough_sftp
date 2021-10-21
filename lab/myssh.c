#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define HOSTIP "172.25.217.120"
#define HOSTPORT 22
#define USERNAME "yuta"
#define PASSWORD "taiki927"
#define MAX_XFER_BUF_SIZE 16384

int show_remote_processes(ssh_session);
int sftp_initworld(ssh_session,sftp_session*);
int sftp_list_dir(ssh_session, sftp_session);
int sftp_read_sync(ssh_session, sftp_session );

int main()
{
  ssh_session my_ssh_session;
  sftp_session mysftp;
  int rc;
  unsigned int port = HOSTPORT;
  const char *user = USERNAME;
  const char *password = PASSWORD;
 
  my_ssh_session = ssh_new();
  
  if (my_ssh_session == NULL)
    exit(-1);


  ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, HOSTIP);
  ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
 
  //Connect to server
  rc = ssh_connect(my_ssh_session);
  if (rc != SSH_OK)
  {
    fprintf(stderr, "Error connecting to localhost: %s\n",
            ssh_get_error(my_ssh_session));
    exit(-1);
  }
  printf("connected\n");

  //User authentication
  rc = ssh_userauth_password(my_ssh_session, user, password);
  if (rc != SSH_AUTH_SUCCESS)
  {
    fprintf(stderr, "Error authenticating with password: %s\n",
            ssh_get_error(my_ssh_session));
    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
    exit(-1);
  }
  printf("user auth success\n");

  //init sftp
  sftp_initworld(my_ssh_session,&mysftp);
  

  //do something
  show_remote_processes(my_ssh_session);

  sftp_list_dir(my_ssh_session,mysftp);

  sftp_read_sync(my_ssh_session,mysftp);
  
  //delete sftp session
  sftp_free(mysftp);

  //close ssh
  ssh_disconnect(my_ssh_session);
  printf("disconnected!\n");
  ssh_free(my_ssh_session);

  return 0;
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

int sftp_initworld(ssh_session session,sftp_session*sftp)
{
  int rc;

  *sftp = sftp_new(session);
  if (*sftp == NULL)
  {
    fprintf(stderr, "Error allocating SFTP session: %s\n",
            ssh_get_error(session));
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

  return SSH_OK;
}

int sftp_list_dir(ssh_session session, sftp_session sftp)
{
  sftp_dir dir;
  sftp_attributes attributes;
  int rc;
 
  dir = sftp_opendir(sftp, "/home/yuta/");
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