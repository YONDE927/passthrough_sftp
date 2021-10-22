# passthroug_sftp

## 依存関係
- libssh

## プロジェクトツール
- cmake


## 9/30
sftpクライアントをwinsock2を用いて実装中。

passthroughに組み込む設計を図に書き込む。

## 10/07
ssh関係をlibsshを用いることに変更。

問題点：
- dokanは2008で更新が止まっていたので使うのやめてwinfspをfuse実装に用いることにしたがリンクの方法がわからない。
- libsshは現在vcpkgとcmakeを組み合わせて使えている状況
- そこにwinfspのfuseを追加することに頭を悩ませる。

試したこと：
- Linux上でlibfuseをgccのオプションを用いてリンク
- Windows上でvcpkg,cmakeを用いたlibsshのリンク

試してないこと!

- Linux上でlibfuseとlibsshをリンク

対応：
- とりあえずsftpでpassthroughに付け加えたい関数群を実装しておく(getattr,getdir,read,write)
- cmakeがどのようにlibsshを参照しているのか解明する。

参考：https://github.com/billziss-gh/winfsp/tree/master/tst/passthrough-fuse3

## 10/22
成果:
- Cygwinを用いることでwinfsp由来のlibfuse3とlibsshを簡単にリンクさせることが可能になりました。
- パススルーのファイルシステムにssh接続・sftpセッションの取得までは動かすようになりました。
- open命令時にリモートファイルをローカルにダウンロードして、ローカルファイルのファイルハンドラを返すように変更しました。

問題
- openを呼び出すサンプルを書こうにも、ほとんどの命令でgetattr(lstat)が呼び出されており、ローカルに存在しないものを要求する時点でFile not foundエラーでopen前にはじかれてしまう。
- getattrやreaddir時にリモートファイル情報を取得するため、sftp_getattrを利用するもこの関数の返すファイル情報構造体sftp_statとfuse_statのメンバ変数が大分違うので変換で止まっている。

### sftp_statとfuse_stat
fuse_stat 
```c
#define FSP_FUSE_STAT_FIELD_DEFN        \
    fuse_dev_t st_dev;                  \
    fuse_ino_t st_ino;                  \
    fuse_mode_t st_mode;                \
    fuse_nlink_t st_nlink;              \
    fuse_uid_t st_uid;                  \
    fuse_gid_t st_gid;                  \
    fuse_dev_t st_rdev;                 \
    fuse_off_t st_size;                 \
    struct fuse_timespec st_atim;       \
    struct fuse_timespec st_mtim;       \
    struct fuse_timespec st_ctim;       \
    fuse_blksize_t st_blksize;          \
    fuse_blkcnt_t st_blocks;            \
    struct fuse_timespec st_birthtim;
```

sftp_stat

```c
struct sftp_attributes_struct {
    char *name;
    char *longname; /* ls -l output on openssh, not reliable else */
    uint32_t flags;
    uint8_t type;
    uint64_t size;
    uint32_t uid;
    uint32_t gid;
    char *owner; /* set if openssh and version 4 */
    char *group; /* set if openssh and version 4 */
    uint32_t permissions;
    uint64_t atime64;
    uint32_t atime;
    uint32_t atime_nseconds;
    uint64_t createtime;
    uint32_t createtime_nseconds;
    uint64_t mtime64;
    uint32_t mtime;
    uint32_t mtime_nseconds;
    ssh_string acl;
    uint32_t extended_count;
    ssh_string extended_type;
    ssh_string extended_data;
};
```


## 内部処理手順
![Untitled Diagram-Page-2 drawio](https://user-images.githubusercontent.com/42487271/136437244-b80c029f-6712-4d00-8d68-973c9ed4e420.png)

![Untitled Diagram-Page-3 drawio](https://user-images.githubusercontent.com/42487271/136437171-6c23fdfb-b210-49c9-8398-7fc1b9d1a426.png)


