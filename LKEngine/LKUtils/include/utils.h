/*
杂项功能函数
 */
#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include <malloc.h>
#include <direct.h>
#include <io.h>
#else
#include <pwd.h>
#include <sys/types.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>

#ifdef ANDROID
    #include <android/log.h>
    #define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
	#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,"native-activity",__VA_ARGS__)
#else
    #ifdef _IOS
void ios_logout( const char *format, ... );
        #define LOGI(...) ((void)ios_logout( __VA_ARGS__))
		#define LOGE(...) ((void)ios_logout( __VA_ARGS__))
    #else
        #define LOGI(...)
		#define LOGE(...)
    #endif
#endif

#ifdef WIN32
typedef __int64    i64;
#else
typedef long long    i64;
#endif

#ifdef MY_MALLOC_FUNC
void *my_malloc( size_t size );
void *my_realloc( void *memblock, size_t size );
void my_free( void *memblock );

inline void* operator new(size_t size)
{
	return my_malloc(size);
}

inline void operator delete(void *pointer)
{
	my_free(pointer);
}

inline void* operator new[](size_t size)
{
	return my_malloc(size);
}

inline void operator delete[](void *pointer)
{
	my_free(pointer);
}
#else
#define my_malloc malloc
#define my_realloc realloc
#define my_free free
#endif


//#ifdef ANDROID
#define LKSET_DWORD(t,s)	{ memcpy( t, s, sizeof(int) ); s+=sizeof(int); }
#define LKSET_UDWORD(t,s)	{ memcpy( t, s, sizeof(unsigned int) ); s+=sizeof(unsigned int); }
#define LKSET_WORD(t,s)	{ memcpy( t, s, sizeof(short) ); s+=sizeof(short); }
#define LKSET_UWORD(t,s)	{ memcpy( t, s, sizeof(unsigned short) ); s+=sizeof(unsigned short); }
#define LKSET_BYTE(t,s)	{ *(t) = *((unsigned char *)s); s+=sizeof(unsigned char); }
#define LKSET_SBYTE(t,s)	{ *(t) = *((char *)s); s+=sizeof(char); }
#define LKSET_LONG(t,s)	{ memcpy( t, s, sizeof(i64) ); s+=sizeof(i64); }
#define LKSET_FLOAT(t,s)	{ memcpy( t, s, sizeof(float) ); s+=sizeof(float); }
#define LKSET_DOUBLE(t,s)	{ memcpy( t, s, sizeof(double) ); s+=sizeof(double); }

#define LKSET_DWORD_L(t,s,l)	{ memcpy( t, s, sizeof(int) ); s+=sizeof(int); l+=sizeof(int); }
#define LKSET_WORD_L(t,s,l)	{ memcpy( t, s, sizeof(short) ); s+=sizeof(short); l+=sizeof(short); }
#define LKSET_BYTE_L(t,s,l)	{ *(t) = *((unsigned char *)s); s+=sizeof(unsigned char); l+=sizeof(unsigned char); }
#define LKSET_SYTE_L(t,s,l)	{ *(t) = *((char *)s); s+=sizeof(char); l+=sizeof(char); }
#define LKSET_LONG_L(t,s,l)	{ memcpy( t, s, sizeof(i64) ); s+=sizeof(i64); l+=sizeof(i64); }
#define LKSET_FLOAT_L(t,s,l)	{ memcpy( t, s, sizeof(float) ); s+=sizeof(float); l+=sizeof(float); }
#define LKSET_DOUBLE_L(t,s,l)	{ memcpy( t, s, sizeof(double) ); s+=sizeof(double); l+=sizeof(double); }

#define LKSET_DWORD_RECV(t,s,l)	{ if(l>0){memcpy( t, s, sizeof(int) ); s+=sizeof(int); l-=sizeof(int); }}
#define LKSET_WORD_RECV(t,s,l)	{ if(l>0){memcpy( t, s, sizeof(short) ); s+=sizeof(short); l-=sizeof(short); }}
#define LKSET_BYTE_RECV(t,s,l)	{ if(l>0){*(t) = *((unsigned char *)s); s+=sizeof(unsigned char); l-=sizeof(unsigned char); }}
#define LKSET_SBYTE_RECV(t,s,l)	{ if(l>0){*(t) = *((char *)s); s+=sizeof(char); l-=sizeof(char); }}
#define LKSET_LONG_RECV(t,s,l)	{ if(l>0){memcpy( t, s, sizeof(i64) ); s+=sizeof(i64); l-=sizeof(i64); }}
#define LKSET_FLOAT_RECV(t,s,l)	{ if(l>0){memcpy( t, s, sizeof(float) ); s+=sizeof(float); l-=sizeof(float); }}
#define LKSET_DOUBLE_RECV(t,s,l)	{ if(l>0){memcpy( t, s, sizeof(double) ); s+=sizeof(double); l-=sizeof(double); }}

#define LKSET_DWORD_SEND(t,s,l)	{ memcpy( t, s, sizeof(int) ); t+=sizeof(int); l+=sizeof(int); }
#define LKSET_WORD_SEND(t,s,l)	{ memcpy( t, s, sizeof(short) ); t+=sizeof(short); l+=sizeof(short); }
#define LKSET_BYTE_SEND(t,s,l)	{ *(t) = *((unsigned char *)s); t+=sizeof(unsigned char); l+=sizeof(unsigned char); }
#define LKSET_SBYTE_SEND(t,s,l)	{ *(t) = *((char *)s); t+=sizeof(char); l+=sizeof(char); }
#define LKSET_LONG_SEND(t,s,l)	{ memcpy( t, s, sizeof(i64) ); t+=sizeof(i64); l+=sizeof(i64); }
#define LKSET_FLOAT_SEND(t,s,l)	{ memcpy( t, s, sizeof(float) ); t+=sizeof(float); l+=sizeof(float); }
#define LKSET_DOUBLE_SEND(t,s,l)	{ memcpy( t, s, sizeof(double) ); t+=sizeof(double); l+=sizeof(double); }
/*
#define LKSET_DWORD(t,s)	{ *(t) = *((int *)s); s+=sizeof(int); }
#define LKSET_WORD(t,s)	{ *(t) = *((short *)s); s+=sizeof(short); }
#define LKSET_BYTE(t,s)	{ *(t) = *((char *)s); s+=sizeof(char); }
#define LKSET_LONG(t,s)	{ *(t) = *((i64 *)s); s+=sizeof(i64); }
#define LKSET_FLOAT(t,s)	{ *(t) = *((float *)s); s+=sizeof(float); }
#define LKSET_DOUBLE(t,s)	{ *(t) = *((double *)s); s+=sizeof(double); }

#define LKSET_DWORD_L(t,s,l)	{ *(t) = *((int *)s); s+=sizeof(int); l+=sizeof(int); }
#define LKSET_WORD_L(t,s,l)	{ *(t) = *((short *)s); s+=sizeof(short); l+=sizeof(short); }
#define LKSET_BYTE_L(t,s,l)	{ *(t) = *((char *)s); s+=sizeof(char); l+=sizeof(char); }
#define LKSET_LONG_L(t,s,l)	{ *(t) = *((i64 *)s); s+=sizeof(i64); l+=sizeof(i64); }
#define LKSET_FLOAT_L(t,s,l)	{ *(t) = *((float *)s); s+=sizeof(float); l+=sizeof(float); }
#define LKSET_DOUBLE_L(t,s,l)	{ *(t) = *((double *)s); s+=sizeof(double); l+=sizeof(double); }

#define LKSET_DWORD_RECV(t,s,l)	{ *(t) = *((int *)s); s+=sizeof(int); l-=sizeof(int); }
#define LKSET_WORD_RECV(t,s,l)	{ *(t) = *((short *)s); s+=sizeof(short); l-=sizeof(short); }
#define LKSET_BYTE_RECV(t,s,l)	{ *(t) = *((char *)s); s+=sizeof(char); l-=sizeof(char); }
#define LKSET_LONG_RECV(t,s,l)	{ *(t) = *((i64 *)s); s+=sizeof(i64); l-=sizeof(i64); }
#define LKSET_FLOAT_RECV(t,s,l)	{ *(t) = *((float *)s); s+=sizeof(float); l-=sizeof(float); }
#define LKSET_DOUBLE_RECV(t,s,l)	{ *(t) = *((double *)s); s+=sizeof(double); l-=sizeof(double); }

#define LKSET_DWORD_SEND(t,s,l)	{ *(t) = *((int *)s); t+=sizeof(int); l+=sizeof(int); }
#define LKSET_WORD_SEND(t,s,l)	{ *(t) = *((short *)s); t+=sizeof(short); l+=sizeof(short); }
#define LKSET_BYTE_SEND(t,s,l)	{ *(t) = *((char *)s); t+=sizeof(char); l+=sizeof(char); }
#define LKSET_LONG_SEND(t,s,l)	{ *(t) = *((i64 *)s); t+=sizeof(i64); l+=sizeof(i64); }
#define LKSET_FLOAT_SEND(t,s,l)	{ *(t) = *((float *)s); t+=sizeof(float); l+=sizeof(float); }
#define LKSET_DOUBLE_SEND(t,s,l)	{ *(t) = *((double *)s); t+=sizeof(double); l+=sizeof(double); }
*/
#define LKSET_MEM(t,s,m)	{ memcpy( t, s, m ); s+=m; }
#define LKSET_MEM_L(t,s,m,l)	{ memcpy( t, s, m ); s+=m; l+=m; }
#define LKSET_MEM_RECV(t,s,m,l)	{ memcpy( t, s, m ); s+=m; l-=m; }
#define LKSET_MEM_SEND(t,s,m,l)	{ memcpy( t, s, m ); t+=m; l+=m; }

#define LKSET_STR(t,s,m)	{ if(m>0) {memcpy( t, s, m ); *(t+m-1)=0; s+=m;} }
#define LKSET_STR_L(t,s,m,l)	{ if(m>0) {memcpy( t, s, m ); *(t+m-1)=0; s+=m; l+=m;} }
#define LKSET_STR_RECV(t,s,m,l)	{ if(m>0) {memcpy( t, s, m ); *(t+m-1)=0; s+=m; l-=m;} }
#define LKSET_STR_SEND(t,s,m,l)	{ if(m>0) {memcpy( t, s, m ); *(t+m-1)=0; t+=m; l+=m;} }

#define u_isdigit(x)  isdigit ((unsigned char)(x))

#define WM_STRING						0xefff
#define WM_JOYSTICK_UP                  0xe001
#define WM_JOYSTICK_DOWN                0xe002
#define WM_JOYSTICK_LEFT                0xe003
#define WM_JOYSTICK_RIGHT               0xe004
#define WM_JOYSTICK_LUP                 0xe005
#define WM_JOYSTICK_LDOWN               0xe006
#define WM_JOYSTICK_RUP                 0xe007
#define WM_JOYSTICK_RDOWN               0xe008

#ifdef WIN32
#define LOCK_SH 1
#define LOCK_EX 2
#define LOCK_NB 4
#define LOCK_UN 8

//char *StringW2A( const WCHAR *pWideCharStr, char *pMultiByteStr );
//WCHAR *StringA2W( const char *pMultiByteStr, WCHAR *pWideCharStr );
int flock (int fd,int op);

#ifndef snprintf
#define snprintf _snprintf
#endif // !snprintf

#if _MSC_VER >= 1900
#define timezone _timezone
#define daylight _daylight
#define tzname _tzname
#endif

#pragma warning(disable:4101)
#pragma warning(disable:4267)
#else

#define CALLBACK
#define MAX_PATH		260
#define TRUE			1
#define FALSE			0
#ifndef NULL
#define NULL	0
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define TEXT(quote)			quote
#define SetCurrentDirectory(path) chdir(path)

#define _open open
#define _close close
#define _read read
#define _write write
#define _unlink unlink

typedef int        SOCKET;
#define INVALID_SOCKET		-1

#define DT_TOP                      0x00000000
#define DT_LEFT                     0x00000000
#define DT_CENTER                   0x00000001
#define DT_RIGHT                    0x00000002
#define DT_VCENTER                  0x00000004
#define DT_BOTTOM                   0x00000008
#define DT_WORDBREAK                0x00000010
#define DT_SINGLELINE               0x00000020
#define DT_EXPANDTABS               0x00000040
#define DT_TABSTOP                  0x00000080
#define DT_NOCLIP                   0x00000100
#define DT_EXTERNALLEADING          0x00000200
#define DT_CALCRECT                 0x00000400
#define DT_NOPREFIX                 0x00000800
#define DT_INTERNAL                 0x00001000

#define WM_MOUSEMOVE                    0x0200
#define WM_LBUTTONDOWN                  0x0201
#define WM_LBUTTONUP                    0x0202
#define WM_LBUTTONDBLCLK                0x0203
#define WM_RBUTTONDOWN                  0x0204
#define WM_RBUTTONUP                    0x0205
#define WM_RBUTTONDBLCLK                0x0206
#define WM_MBUTTONDOWN                  0x0207
#define WM_MBUTTONUP                    0x0208
#define WM_MBUTTONDBLCLK                0x0209
//#define WM_MOUSEWHEEL					0x020A //  [6/4/2009 yxx]

#define VK_BACK           0x08
#define VK_TAB            0x09

#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12

#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28

#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_ESCAPE         0x1B
#define VK_RETURN         0x0D

#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B

#define WM_KEYDOWN                      0x0100
#define WM_KEYUP                        0x0101
#define WM_CHAR                         0x0102
#define WM_SYSKEYDOWN                   0x0104
#define WM_SYSKEYUP                     0x0105

#define FW_DONTCARE         0
#define FW_THIN             100
#define FW_EXTRALIGHT       200
#define FW_LIGHT            300
#define FW_NORMAL           400
#define FW_MEDIUM           500
#define FW_SEMIBOLD         600
#define FW_BOLD             700
#define FW_EXTRABOLD        800
#define FW_HEAVY            900

typedef int	HWND;
typedef int	HICON;
typedef int HCURSOR;

#ifndef BOOL
typedef signed char			BOOL;
#endif
typedef unsigned int	WPARAM;
typedef long			LPARAM;
typedef long			LRESULT;
typedef unsigned long	DWORD;
typedef unsigned int	UINT;
typedef unsigned char BYTE;

typedef struct _RECT
{
    int left;
    int top;
    int right;
    int bottom;
} RECT;
typedef RECT* LPRECT;

typedef struct _POINT
{
    int x;
    int y;
} POINT;
typedef POINT* LPPOINT;
void SetRect( RECT* prc, int left, int top, int right, int bottom );
BOOL IntersectRect( RECT* pdrc, const RECT* psrc1, const RECT* psrc2 );
void OffsetRect( RECT* prc, int x, int y );
void InflateRect( RECT* prc, int cx, int cy );
BOOL PtInRect( const RECT* prc, POINT pt );

#endif
#define EXITED_SOCKET			-2

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 255
#define MAX_SYSPWD_LENGTH	5

#define MAX_ACTORNAME_LENGTH 20

#define MAXGETSIZE	2048

#ifndef MAX_PATH
#define MAX_PATH 260
#endif
#define MAX_LOGNUM 5

struct _loginfo
{
	FILE **m_logfp;
	char **m_logfile;
	char m_logpath[MAX_PATH];
	int m_logcount[MAX_LOGNUM];
	int m_datesign;
};
typedef struct _loginfo LogInfo;

typedef int (*LPCALLBACK_GETVALUE)( const char *pSign, const char *pValue );

/****************************************************
 *内存分配
 ****************************************************/
/* 内存分配出错的处理(可适应需要修改) */
//void u_memerr (const char *str);
//void * u_malloc (size_t size);
//void * u_realloc (void *obj, size_t size);
/* 释放数组 */
void u_free_vec (char **vec);

/****************************************************
 *字符串处理
 ****************************************************/

/* 复制一个字符串并返回其指针，注意使用完后释放 */
char * u_strdup (const char *str);
/* 复制一个内存一个区域并返回其指针，注意使用完后释放 */
char * u_memdup (const char *begin, const char *end);
/* 以sign分割字符串，并且返回一字符串数组的指针，标记后的空格忽略，注意使用完释放 */
char ** u_strcut (const char *str,char sign);
char ** u_strncut (const char *str,char sign,int count);
char ** u_strcut_ex (const char *str, char sign, int *count );
void u_strchop (char *str);

/****************************************************
 *系统操作
 ****************************************************/

/* 获取当前用户的用户名 */
char * u_username (char *where);
/* 将程序转入后台执行 */
void u_gotoback(void);

/****************************************************
 *文件目录操作
 ****************************************************/
int u_copy_file(char *from,char *to);
/* 判断文件是否存在 */
int u_file_is_exist (const char *filename);
/* 判断是否是真正的文件，如果 path 是目录返回0， 否则返回1 */
int u_file_non_dir (const char *path);
/* 创建所有级别目录 */
int u_mkdir (const char *directory);
int u_mkpath (const char *directory);
/* 判断前缀，如果是pattern是string的前缀，返回1，否则返回0 */
int u_frontcmp (const char *string, const char *pattern);
/* 判断后缀 */
int u_backcmp (const char *string, const char *pattern);
/* 返回扩展名:
   suffix ("foo.bar")       -> "bar"
   suffix ("foo.bar.baz")   -> "baz"
   suffix ("/foo/bar")      -> NULL
   suffix ("/foo.bar/baz")  -> NULL  */
char *u_suffix (const char *str);
/* 从文件中读取一行 */
char * u_read_line (FILE *fp);
/* 从当前文件指针读取整个文件到字符串数组 */
char **u_load_file (FILE *fp,int *count);
/* 读取二进制内容，注意释放 */
char *u_load_binary( char *filename );
void long2str(char *buffer, long number);

int u_getcfg(char *configfile, LPCALLBACK_GETVALUE process );

int u_loginit( const char *logpath );
int u_logopen( int log_index, const char *logfile );
int u_logclose( int log_index );
int u_logcloseall();
int u_log( int log_index, const char *format, va_list arg_ptr );

int open_netlog();
int open_gamelog();
int write_netlog( const char *format, ... );
int write_gamelog( const char *format, ... );

int printf_msg( const char *format, ... );

void *load_dll( const char *dllfile );
void unload_dll( void *handle );
void *load_function( void *handle, const char *name );

char *str_replace(char *src,char oldChar,char newChar);

void u_str_tolower( char *ptr );
void u_str_toupper( char *ptr );

#ifdef WIN32
#define lltoa _i64toa
#define atoll _atoi64
#else
#define Sleep(ms) usleep(ms*1000)
char* lltoa(long long value, char*  str, int radix );
int GetModuleFileName( char* sModuleName, char* sFileName, int nSize );
#endif

#endif
