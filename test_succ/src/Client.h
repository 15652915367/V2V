#ifndef CONNECT_CLIENT_CLIENT_H
#define CONNECT_CLIENT_CLIENT_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include <utility>
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <bitset>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>

#include "muduo/net/TcpClient.h"
#include "muduo/base/Logging.h"
#include "muduo/base/Thread.h"
#include "muduo/base/Singleton.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/Buffer.h"

#include "muduo/net/EventLoopThreadPool.h"


#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/aes.h>


#endif
