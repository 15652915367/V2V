需要boost开发库。

编译muduo需要-DMUDUO_STD_STRING。

$ cat /etc/redhat-release 

CentOS Linux release 7.3.1611 (Core)

$uname -a

Linux node95.localdomain 3.10.0-514.26.2.el7.x86_64 #1 SMP Tue Jul 4 15:04:05 UTC 2017 x86_64 x86_64 x86_64 GNU/Linux

$ g++ --version

g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-11)

Copyright © 2015 Free Software Foundation, Inc.

\# yum -y install boost-devel

生成并安装调试版本：

$ BUILD_TYPE=debug ./build.sh install

生成并安装发布版本：

$ ./build.sh install

AES/CBC/PKCS5Padding, aes secret key length 16 bytes.

RSA/ECB/PKCS1Padding, rsa key length 128 bytes.

rsa public key is X509 encoded text.

rsa private key is PKCS8 encoded text.

北汽车机网关服务端地址和端口：182.92.4.184 55555。