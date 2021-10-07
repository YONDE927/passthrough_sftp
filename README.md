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
- libsshは現在vcpkgとcmakeを組み合わせて使えている状況なので、素のコンパイラやリンカがどうなっているか不明。
- そこにwinfspのfuseを追加することに頭を悩ませる。
- とりあえずsftpでpassthroughに付け加えたい関数群を実装しておく(getattr,getdir,read,write)
参考：https://github.com/billziss-gh/winfsp/tree/master/tst/passthrough-fuse3