#ifndef __HTTP_H_
#define __HTTP_H_

#include "utils.h"

#define  HTTPMAX_BUFFLEN 2048

// http连接
SOCKET HttpConnect( char *host, unsigned short port );

// HTTP关闭socket
void HttpClose( SOCKET socket );

// 解析流
int HttpParse( char *stream, char *pSign, char *outString );

// 从尾部根据标记解析
int HttpGetPartTail( char *stream, char *pSign, char *outString );

// 解析正文
char *HttpGetHeaderEnd( char *stream );

// 发送get指令
int HttpGet( SOCKET socket, char *http_get, char *host );

// 字符串转义一下
char *HttpString( const char *str, char *out );

#endif

