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

試してないこと
- Linux上でlibfuseとlibsshをリンク

対応：
- とりあえずsftpでpassthroughに付け加えたい関数群を実装しておく(getattr,getdir,read,write)
- cmakeがどのようにlibsshを参照しているのか解明する。

参考：https://github.com/billziss-gh/winfsp/tree/master/tst/passthrough-fuse3
