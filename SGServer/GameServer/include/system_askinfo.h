#ifndef __SYSTEM_ASKINFO_H
#define __SYSTEM_ASKINFO_H
// 客户端发过来的简单请求

#define ASKINFO_NORMAL				0

int system_askinfo( int actor_index, int msgid, char *pstr, int *pvalue );

#endif
