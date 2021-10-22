#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <fcntl.h>

#define HOSTIP "172.22.27.137"
#define HOSTPORT 22
#define USERNAME "yuta"
#define PASSWORD "taiki927"
#define REMOTEPOINT "/home/yuta/tmp"
#define MNTPOINT "./mnt"
#define MAX_XFER_BUF_SIZE 16384

int sftp_init_(ssh_session *, sftp_session *);
int sftp_list_dir(ssh_session *, sftp_session *);
int sftp_read_sync(ssh_session *, sftp_session *);
int sftp_closeworld(ssh_session *, sftp_session *);

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

    attributes = sftp_stat(*sftp,path);
    if(!attributes){
        fprintf(stderr, "file info not obtained: %s\n",
                ssh_get_error(*ssh));
        return SSH_ERROR;
    }
    return SSH_OK;
}

//readdir
int sftp_list_dir(ssh_session *ssh, sftp_session *sftp)
{
    sftp_dir dir;
    sftp_attributes attributes;
    int rc;

    dir = sftp_opendir(*sftp, "/home/yuta/");
    if (!dir)
    {
        fprintf(stderr, "Directory not opened: %s\n",
                ssh_get_error(*ssh));
        return SSH_ERROR;
    }

    printf("Name                       Size Perms    Owner\tGroup\n");

    while ((attributes = sftp_readdir(*sftp, dir)) != NULL)
    {
        printf("%-20s %10llu %.8o %s(%d)\t%s(%d)\n",
               attributes->name,
               (long long unsigned int)attributes->size,
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
                ssh_get_error(*ssh));
        sftp_closedir(dir);
        return SSH_ERROR;
    }

    rc = sftp_closedir(dir);
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Can't close directory: %s\n",
                ssh_get_error(*ssh));
        return rc;
    }
    return rc;
}

//sftp_read_sync リモートファイルを指定したパスにダウンロードする。
int sftp_read_sync(ssh_session *session, sftp_session *sftp, char *path)
{
    int access_type;
    sftp_file file;
    char buffer[MAX_XFER_BUF_SIZE];
    char *remote_path, *local_path;
    int nbytes, nwritten, rc;
    int fd;

    //edit remote_path
    remote_path = (char *)malloc(strlen(REMOTEPOINT) + strlen(path));
    strcpy(remote_path, REMOTEPOINT);
    strcat(remote_path, path);
    //edit local_path
    local_path = (char *)malloc(strlen(MNTPOINT) + strlen(path));
    strcpy(local_path, MNTPOINT);
    strcat(local_path, path);

    //open file remote
    access_type = O_RDONLY;
    file = sftp_open(*sftp, remote_path, access_type, 0);
    if (file == NULL)
    {
        fprintf(stderr, "Can't open file for reading: %s\n",
                ssh_get_error(*session));
        return SSH_ERROR;
    }

    fd = open(local_path, O_CREAT | O_WRONLY, 0777);
    if (fd < 0)
    {
        fprintf(stderr, "Can't open file for writing: %s\n",
                strerror(errno));
        return SSH_ERROR;
    }
    printf("file descriptor : %d\n", fd);

    for (;;)
    {
        nbytes = sftp_read(file, buffer, sizeof(buffer));
        if (nbytes == 0)
        {
            break; // EOF
        }
        else if (nbytes < 0)
        {
            fprintf(stderr, "Error while reading file: %s\n",
                    ssh_get_error(*session));
            sftp_close(file);
            return SSH_ERROR;
        }
        printf("nbytes : %d\n", nbytes);
        printf("sizeof(buffer) : %d\n", sizeof(buffer));
        //printf("%s\n",buffer);
        nwritten = write(fd, buffer, nbytes);
        printf("%d\n", nwritten);
        if (nwritten != nbytes)
        {
            fprintf(stderr, "Error writing: %s\n",
                    strerror(errno));
            sftp_close(file);
            return SSH_ERROR;
        }
    }
    rc = sftp_close(file);
    free(local_path);
    free(remote_path);
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Can't close the read file: %s\n",
                ssh_get_error(*session));
        return rc;
    }

    return SSH_OK;
}

//write
int sftp_save(ssh_session* ssh, sftp_session* sftp,char* path)
{
    int access_type = O_WRONLY | O_CREAT | O_TRUNC;
    sftp_file file;
    int length = strlen(path);
    int rc, nwritten;
    char* local_path,remote_path,buffer;

    //edit remote_path
    remote_path = (char *)malloc(strlen(REMOTEPOINT) + strlen(path));
    strcpy(remote_path, REMOTEPOINT);
    strcat(remote_path, path);
    //edit local_path
    local_path = (char *)malloc(strlen(MNTPOINT) + strlen(path));
    strcpy(local_path, MNTPOINT);
    strcat(local_path, path);

    //open local file and read buffer


    //write buffer to remote file
    file = sftp_open(*sftp, remote_path ,access_type, 0777);
    if (file == NULL)
    {
        fprintf(stderr, "Can't open file for writing: %s\n",
                ssh_get_error(*ssh));
        return SSH_ERROR;
    }

    nwritten = sftp_write(file, buffer, length);
    if (nwritten != length)
    {
        fprintf(stderr, "Can't write data to file: %s\n",
                ssh_get_error(*ssh));
        sftp_close(file);
        return SSH_ERROR;
    }

    rc = sftp_close(file);
    free(local_path);
    free(remote_path);
    free(buffer);

    if (rc != SSH_OK)
    {
        fprintf(stderr, "Can't close the written file: %s\n",
                ssh_get_error(*ssh));
        return rc;
    }

    return SSH_OK;
}

int sftp_closeworld(ssh_session *ssh, sftp_session *sftp)
{
    //close sftp
    sftp_free(*sftp);
    printf("sftp stopped!\n");
    //free(sftp);
    //close ssh
    ssh_disconnect(*ssh);
    printf("ssh disconnected!\n");
    ssh_free(*ssh);
    //free(ssh);
    return 0;
}

//test
int main()
{
    ssh_session *ssh;
    sftp_session *sftp;

    ssh = (ssh_session *)malloc(sizeof(ssh_session));
    sftp = (sftp_session *)malloc(sizeof(sftp_session));
    sftp_initworld(ssh, sftp);

    sftp_closeworld(ssh, sftp);
    free(sftp);
    free(ssh);
}