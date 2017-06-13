
#ifndef __GMCMD_H
#define __GMCMD_H

#define GMC_BASE				0
#define GMC_TEST				(GMC_BASE+1)	// 测试
#define GMC_SC					(GMC_BASE+2)	// 脚本使用
#define GMC_ACTIVITY			(GMC_BASE+3)	// 

// GM指令使用信息
#define GMR_OK					1		// 成功
#define GMR_NOTGMTOOLS			0		// 未使用/不允许Gmtools
#define GMR_CHECK				-1		// 已使用/超时
#define GMR_ERROR				-10
#define GMR_ACCESS_DENIED		-11
#define GMR_NOT_ONLINE			-12
#define GMR_ARG_ERROR			-13

struct _cmdinfo {
	char m_allowgmtools;			// 是否允许GMTools访问
};
typedef struct _cmdinfo SCmdInfo;

int gm_init();
char gm_isallow_gmtools( short cmdid );

int gm_proc( short cmd, int arg1, int arg2, int arg3, int arg4, char *str );
int gm_set_result( int client_index, int value );
int gm_get_result( int client_index );
#endif
