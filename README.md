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

## 内部処理手順
![Untitled Diagram-Page-2 drawio](https://user-images.githubusercontent.com/42487271/136437244-b80c029f-6712-4d00-8d68-973c9ed4e420.png)

![Untitled Diagram-Page-3 drawio](https://user-images.githubusercontent.com/42487271/136437171-6c23fdfb-b210-49c9-8398-7fc1b9d1a426.png)


