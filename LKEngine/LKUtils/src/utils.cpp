/*
杂项功能函数
 */
#include "utils.h"
#ifndef S_ISDIR
#define S_ISDIR(m) (((m) & (_S_IFMT)) == (_S_IFDIR))
#endif

#ifdef MY_MALLOC_FUNC
long g_inc_mem_value = 1;
void *my_malloc( size_t size )
{
	void *buf;
	long value = InterlockedIncrement( &g_inc_mem_value );
	buf = malloc( size+8 );
	*((long *)buf) = value;
	*((long *)buf+1) = value+1;
	return (void *)((BYTE*)buf+8);
}

void *my_realloc( void *memblock, size_t size )
{
	if(memblock)
	{
		void *buf;
		long value1,value2;
		value1 = *((long *)memblock-2);
		value2 = *((long *)memblock-1);
		assert(value1+1==value2);
		buf = realloc( (void *)((BYTE*)memblock-8), size+8 );
		return (void *)((BYTE*)buf+8);
	}
	return my_malloc( size );
}

void my_free( void *memblock )
{
	long value1,value2;
	value1 = *((long *)memblock-2);
	value2 = *((long *)memblock-1);
	assert(value1+1==value2);
	 *((long *)memblock-2) = 0;
	 *((long *)memblock-1) = 0;
	free( (void *)((BYTE*)memblock-8) );
}

#endif

#ifdef WIN32
//char *StringW2A( const WCHAR *pWideCharStr, char *pMultiByteStr )
//{
//	int nLenMuti = (int)wcslen( pWideCharStr )*sizeof(WCHAR);
//	WideCharToMultiByte(CP_ACP,0,pWideCharStr,-1,pMultiByteStr,nLenMuti*2,NULL,NULL);
//	return pMultiByteStr;
//}
//
//WCHAR *StringA2W( const char *pMultiByteStr, WCHAR *pWideCharStr )
//{
//	int nLen = (int)strlen( pMultiByteStr );
//	MultiByteToWideChar(CP_ACP,0,pMultiByteStr,-1,pWideCharStr,nLen);
//	pWideCharStr[nLen] = 0;
//	return pWideCharStr;
//}

//Windows版flock函数（win95版本支持不足）
int flock (int fd,int op)
{
	HANDLE hdl = (HANDLE) _get_osfhandle (fd);
	DWORD flags = (op & LOCK_NB) ? LOCKFILE_FAIL_IMMEDIATELY : 0;
	OVERLAPPED offset = {(DWORD)NULL,(DWORD)NULL,0,0,(HANDLE)NULL};
	int ret = -1;
	if (hdl >= 0)
		switch (op & ~LOCK_NB)
		{
			case LOCK_EX:
				flags |= LOCKFILE_EXCLUSIVE_LOCK;
			case LOCK_SH:
#ifdef WIN95
				UnlockFile (hdl,(DWORD)NULL,(DWORD)NULL,1,0);
				if (LockFile (hdl,(DWORD)NULL,(DWORD)NULL,1,0))
					ret = 0;
#else
				UnlockFileEx (hdl,(DWORD)NULL,1,0,&offset);
				if (LockFileEx (hdl,flags,(DWORD)NULL,1,0,&offset))
					ret = 0;
#endif
				break;
			case LOCK_UN:
#ifdef WIN95
				UnlockFile (hdl,(DWORD)NULL,(DWORD)NULL,1,0);
#else
				UnlockFileEx (hdl,(DWORD)NULL,1,0,&offset);
#endif
				ret = 0;
			default:
				break;
		}
	return ret;
}
#else
static const char ntoa_table[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
};
char *strrev(char *string)
{
    size_t len = strlen(string);
    char *a = &string[0];
    char *b = &string[len-1];
    len /= 2;
    while ( len-- )
	{
        char c = *a;
        *a++ = *b;
        *b-- = c;
    }
    return string;
}

char *lltoa(long long value, char *string, int radix )
{
    char *bufp = string;

    if( value < 0 )
	{
        *bufp++ = '-';
        value = -value;
    }
    if( value )
	{
        while( value > 0 )
		{
            *bufp++ = ntoa_table[value % 10];
            value /= 10;
        }
    }
	else
	{
        *bufp++ = '0';
    }
    *bufp = '\0';

    /* The numbers went into the string backwards. :) */
    if ( *string == '-' )
	{
        strrev(string+1);
    }
	else
	{
        strrev(string);
    }
    return string;
}

int GetModuleFileName( char* sModuleName, char* sFileName, int nSize )
{
	int ret = -1;
	char sLine[1024] = { 0 };
	void* pSymbol = (void*)"";
	FILE *fp;
	char *pPath;
	fp = fopen ("/proc/self/maps", "r");
	if ( fp != NULL )
	{
		while (!feof (fp))
		{
			unsigned long start, end;
			if ( !fgets (sLine, sizeof (sLine), fp))
				continue;
			if ( !strstr (sLine, " r-xp ") || !strchr (sLine, '/'))
				continue;
			sscanf (sLine, "%lx-%lx ", &start, &end);
			if (pSymbol >= (void *) start && pSymbol < (void *) end)
			{
				char *tmp;
				size_t len;
				pPath = strchr (sLine, '/');
				tmp = strrchr (pPath, '\n');
				if (tmp) *tmp = 0;
				ret = 0;
				strcpy( sFileName, pPath );
			}
		}
		fclose (fp);
	}
	return ret;
}

void SetRect( RECT* prc, int left, int top, int right, int bottom )
{
	prc->left = left;
	prc->top = top;
	prc->right = right;
	prc->bottom = bottom;
}

BOOL IntersectRect( RECT* pdrc, const RECT* psrc1, const RECT* psrc2 )
{
	pdrc->left = (psrc1->left > psrc2->left) ? psrc1->left : psrc2->left;
	pdrc->top  = (psrc1->top > psrc2->top) ? psrc1->top : psrc2->top;
	pdrc->right = (psrc1->right < psrc2->right) ? psrc1->right : psrc2->right;
	pdrc->bottom = (psrc1->bottom < psrc2->bottom) 
					? psrc1->bottom : psrc2->bottom;

	if(pdrc->left >= pdrc->right || pdrc->top >= pdrc->bottom)
		return FALSE;

	return TRUE;
}

void OffsetRect( RECT* prc, int x, int y )
{
	prc->left += x;
	prc->top += y;
	prc->right += x;
	prc->bottom += y;
}

void InflateRect( RECT* prc, int cx, int cy )
{
	prc->left -= cx;
	prc->top -= cy;
	prc->right += cx;
	prc->bottom += cy;
}

BOOL PtInRect( const RECT* prc, POINT pt )
{
	if ( pt.x >= prc->left && pt.x < prc->right && pt.y >= prc->top && pt.y < prc->bottom )
		return TRUE;

	return FALSE;
}
#endif

#define MAX_LOGPIECE_COUNT  (10000000)

static LogInfo s_loginfo;
static char s_bloginit = 0;
static int s_netlog_count = 0;
static int s_gamelog_count = 0;
int open_netlog()
{
	int rtn;
	rtn = u_logopen( 0, "net.log" );
	if( rtn == 0 )
		write_netlog( "********************NET LOG START********************" );
	return rtn;
}

int open_gamelog()
{
	int rtn;
	rtn = u_logopen( 1, "game.log" );
	if( rtn == 0 )
		write_gamelog( "********************GAME LOG START********************" );
	return rtn;
}

int write_netlog( const char *format, ... )
{
	//int rtn;
	//va_list arg_ptr;
	//va_start( arg_ptr, format );
	//rtn = u_log( 0, format, arg_ptr );
	//va_end( arg_ptr );
	//return rtn;
	return 0;
}

int write_gamelog( const char *format, ... )
{
	int rtn;
	va_list arg_ptr;
	va_start( arg_ptr, format );
	rtn = u_log( 1, format, arg_ptr );
	va_end( arg_ptr );
	return rtn;
}

char g_NotPrintMsg = 0;
int printf_msg( const char *format, ... )
{
	if ( g_NotPrintMsg )
	{
		return 0;
	}

	int rtn;
	va_list arg_ptr;
	va_start( arg_ptr, format );
	rtn = vfprintf( stdout, format, arg_ptr );
	fprintf( stdout, "\t\t\t\r" );
	va_end( arg_ptr );
	return rtn;
}

int u_loginit( const char *logpath )
{
	int tmpi;
	int tmplen;
	if( !s_bloginit )
		memset( &s_loginfo, 0, sizeof(LogInfo) );
	s_loginfo.m_logfile = (char **)malloc( sizeof(char *) * MAX_LOGNUM  );
	if( s_loginfo.m_logfile == NULL )
		return -1;
	s_loginfo.m_logfp = (FILE **)malloc( sizeof(FILE *) * MAX_LOGNUM  );
	if( s_loginfo.m_logfp == NULL )
		return -1;
	if( logpath )
	{
		strcpy( s_loginfo.m_logpath, logpath );
		tmplen = (int)strlen( s_loginfo.m_logpath );
		if( tmplen > 0 && ( s_loginfo.m_logpath[tmplen-1] != '\\' || s_loginfo.m_logpath[tmplen-1] != '/' ) )
			strcat( s_loginfo.m_logpath, "/" );
		u_mkdir( s_loginfo.m_logpath );
	}
	for( tmpi = 0; tmpi < MAX_LOGNUM; tmpi++ )
	{
		s_loginfo.m_logfile[tmpi] = (char *)malloc( sizeof(char) * MAX_PATH  );
		if( s_loginfo.m_logfile[tmpi] == NULL )
			return -1;
		s_loginfo.m_logfp[tmpi] = NULL;
	}

	s_bloginit = 1;
	return 0;
}

int u_logopen( int log_index, const char *logfile )
{
	char filename[MAX_PATH];
	time_t t;
	struct tm *newtime;

	if( !s_bloginit )
		return -1;
	if( log_index < 0 || log_index >= MAX_LOGNUM )
		return -1;
	if( s_loginfo.m_logfp[log_index] )
		return -1;

	time(&t);
	newtime = localtime( &t );

	if( s_loginfo.m_logpath == NULL )
		sprintf( filename, "%.4d_%.2d_%.2d-%.2d_%.2d_%.2d.%s", newtime->tm_year+1900, newtime->tm_mon+1, newtime->tm_mday, newtime->tm_hour,newtime->tm_min, newtime->tm_sec, logfile );
	else
		sprintf( filename, "%s%.4d_%.2d_%.2d-%.2d_%.2d_%.2d.%s", s_loginfo.m_logpath, newtime->tm_year+1900, newtime->tm_mon+1, newtime->tm_mday, newtime->tm_hour,newtime->tm_min, newtime->tm_sec, logfile );
	s_loginfo.m_datesign = newtime->tm_year*10000 + newtime->tm_mon*100 + newtime->tm_mday;
	s_loginfo.m_logfp[log_index] = fopen( filename, "a+" );
	if( s_loginfo.m_logfp[log_index] == NULL )
		return -1;
	strcpy( s_loginfo.m_logfile[log_index], logfile );
	return 0;
}

int u_logclose( int log_index )
{
	if( !s_bloginit )
		return -1;
	if( s_loginfo.m_logfp[log_index] )
	{
		fclose( s_loginfo.m_logfp[log_index] );
		s_loginfo.m_logfp[log_index] = NULL;
	}
	return 0;
}

int u_logcloseall()
{
	int tmpi;

	for(tmpi = 0; tmpi < MAX_LOGNUM; tmpi++ )
	{
		u_logclose( tmpi );
		if( s_loginfo.m_logfile[tmpi] )
			free( s_loginfo.m_logfile[tmpi] );
	}
	free( s_loginfo.m_logfile );
	free( s_loginfo.m_logfp );

	s_bloginit = 0;
	return 0;
}

int u_log( int log_index, const char *format, va_list arg_ptr )
{
	char filename[260];
	time_t t;
	struct tm *newtime;
	int datesign_now;
//	va_list arg_ptr;

	if( !s_bloginit )
		return -1;
	if( log_index < 0 || log_index >= MAX_LOGNUM )
		return -1;
	if( s_loginfo.m_logfp[log_index] == NULL )
		return -1;

	// 打印时间
	time(&t);
	newtime = localtime( &t );
	
	// 判断文件名是否是今天的
	datesign_now = newtime->tm_year*10000 + newtime->tm_mon*100 + newtime->tm_mday;
	if( s_loginfo.m_datesign != datesign_now || 
		s_loginfo.m_logcount[log_index] >= MAX_LOGPIECE_COUNT )
	{
		if( s_loginfo.m_logpath == NULL )
			sprintf( filename, "%.4d_%.2d_%.2d-%.2d_%.2d_%.2d.%s", newtime->tm_year+1900, newtime->tm_mon+1, newtime->tm_mday, newtime->tm_hour,newtime->tm_min, newtime->tm_sec, s_loginfo.m_logfile[log_index] );
		else
			sprintf( filename, "%s%.4d_%.2d_%.2d-%.2d_%.2d_%.2d.%s", s_loginfo.m_logpath, newtime->tm_year+1900, newtime->tm_mon+1, newtime->tm_mday, newtime->tm_hour,newtime->tm_min, newtime->tm_sec, s_loginfo.m_logfile[log_index] );
//		sprintf( filename, "%.4d%.2d%.2d.%s", newtime->tm_year+1900, newtime->tm_mon+1, newtime->tm_mday, s_loginfo.m_logfile[log_index] );
		s_loginfo.m_datesign = datesign_now;
		fclose( s_loginfo.m_logfp[log_index] );
		s_loginfo.m_logfp[log_index] = fopen( filename, "a+" );
		s_loginfo.m_logcount[log_index] = 0;
	}
	if( !s_loginfo.m_logfp[log_index] )
	{
		printf_msg( "log file handle invalid log_index = %d ", log_index );
		return -1;
	}
	++s_loginfo.m_logcount[log_index];
//	fprintf( s_loginfo.m_logfp[log_index], "[%.4d-%.2d-%.2d %.2d:%.2d:%.2d] ", newtime->tm_year+1900, newtime->tm_mon+1, newtime->tm_mday, newtime->tm_hour, newtime->tm_min, newtime->tm_sec );
	fprintf( s_loginfo.m_logfp[log_index], "[%.2d:%.2d:%.2d] ", newtime->tm_hour, newtime->tm_min, newtime->tm_sec );

//	va_start( arg_ptr, format );
	vfprintf( s_loginfo.m_logfp[log_index], format, arg_ptr );
//	va_end( arg_ptr );

	fprintf( s_loginfo.m_logfp[log_index], "\n" );
	fflush( s_loginfo.m_logfp[log_index] );
	return 0;
}

/* 复制文件 */
int u_copy_file(char *from,char *to)
{
	static char buf[MAXGETSIZE];
	int from_fd, rcount, rval, to_fd, wcount, wresid;
	char *bufp;
	int fflag=1;

	if(strcmp(from,to)==0)
		return -1;

	if ((from_fd = _open(from, O_RDONLY, 0)) == -1)
		return -1;

	if (fflag) //创建方式
	{
		(void)_unlink(to);
		to_fd = _open(to, O_WRONLY | O_TRUNC | O_CREAT );
	}
	else //覆盖方式
		to_fd = _open(to, O_WRONLY | O_TRUNC, 0);
	if (to_fd == -1)
	{
		(void)_close(from_fd);
		return -1;
	}
	rval = 0;
	while ((rcount = _read(from_fd, buf, MAXGETSIZE)) > 0)
	{
		for (bufp=buf,wresid=rcount;;bufp+=wcount,wresid-=wcount)
		{
			wcount = _write(to_fd, bufp, wresid);
			if (wcount >= wresid || wcount <= 0)
				break;
		}
		if (wcount != wresid)
		{
			rval = -1;
			break;
		}
	}
	if (rcount < 0)
		rval = -1;

	(void)_close(from_fd);
	if (_close(to_fd))
		rval = -1;
	return (rval);
}


/* 内存分配出错的处理(可适应需要修改) */
//void u_memerr (const char *str)
//{
//	printf_msg ("%s: Not enough memory.\n", str);
//	exit (1);
//}
//
//void * u_malloc (size_t size)
//{
//	void *res;
//
//	res = malloc (size);
//	if (!res)
//		u_memerr ("malloc");
//	return res;
//}
//
//void * u_realloc (void *obj, size_t size)
//{
//	void *res;
//
//	if (obj)
//		res = realloc (obj, size);
//	else
//		res = malloc (size);
//	if (!res)
//		u_memerr ("realloc");
//	return res;
//}

/* 复制一个字符串并返回其指针，注意使用完后释放 */
char * u_strdup (const char *str)
{
	int len = (int)strlen (str);
	char *s1 = (char *)my_malloc( len + 1 );
//	if (!s1)
//		u_memerr ("strdup");
	memcpy (s1, str, len + 1);
	return s1;
}

/* 复制一个内存一个区域并返回其指针，注意使用完后释放 */
char * u_memdup (const char *begin, const char *end)
{
	char *res = (char *)my_malloc( end - begin + 1 );
	memcpy (res, begin, end - begin);
	res[end - begin] = '\0';
	return res;
}

/* 以sign分割字符串，并且返回一字符串数组的指针，标记后的空格忽略，注意使用完释放 */
char ** u_strcut (const char *str,char sign)
{
	char **res;
	const char *strp;
	int tmpi = 0;
	
	if (!str || !*str)
		return NULL;
	res = NULL;
	strp = str;
	while (*str)
	{
		if (*str == sign)
		{
			res = (char **)my_realloc( res, (tmpi + 2) * sizeof (char *) );
			res[tmpi] = u_memdup (strp, str);
			res[++tmpi] = NULL;
			str++;
			while (*str==' ' || *str=='\t')
				str++;
			strp = str;
		}
		else
			str++;
	}
	res = (char **)my_realloc( res, (tmpi + 2) * sizeof (char *) );
	res[tmpi] = u_memdup (strp, str);
	res[tmpi + 1] = NULL;
	return res;
}
char ** u_strncut (const char *str,char sign,int count)
{
	char **res;
	const char *strp;
	int tmpi = 0;
	
	if (!str || !*str)
		return NULL;
	res = NULL;
	strp = str;
	while (*str)
	{
		if (*str == sign && tmpi<count)
		{
			res = (char **)my_realloc( res, (tmpi + 2) * sizeof (char *) );
			res[tmpi] = u_memdup (strp, str);
			res[++tmpi] = NULL;
			str++;
			while (*str==' ' || *str=='\t')
				str++;
			strp = str;
		}
		else
			str++;
	}
	res = (char **)my_realloc( res, (tmpi + 2) * sizeof (char *) );
	res[tmpi] = u_memdup (strp, str);
	res[tmpi + 1] = NULL;
	return res;
}
char ** u_strcut_ex (const char *str, char sign, int *count )
{
	char **res;
	const char *strp;
	int tmpi = 0;
	
	if (!str || !*str)
		return NULL;
	*count = 0;
	res = NULL;
	strp = str;
	while (*str)
	{
		if (*str == sign)
		{
			res = (char **)my_realloc( res, (tmpi + 2) * sizeof (char *) );
			res[tmpi] = u_memdup (strp, str);
			res[++tmpi] = NULL;
			str++;
			while (*str==' ' || *str=='\t')
				str++;
			strp = str;
			*count += 1;
		}
		else
			str++;
	}
	res = (char **)my_realloc( res, (tmpi + 2) * sizeof (char *) );
	res[tmpi] = u_memdup (strp, str);
	res[tmpi + 1] = NULL;
	*count += 1;
	return res;
}
void u_strchop (char *str)
{
	int len;
	for(len=(int)strlen(str);len>0;len--)
	{
		if(*(str+len-1)=='\r' || *(str+len-1)=='\n')
			*(str+len-1)=0;
	}
}
#ifndef WIN32
char * u_username (char *where)
{
	struct passwd *pwd;
	
	if (!(pwd = getpwuid (getuid ())) || !pwd->pw_name)
		return NULL;
	if (where)
	{
		strcpy (where, pwd->pw_name);
		return where;
	}
	else
		return pwd->pw_name;
}

void u_gotoback (void)
{
	pid_t pid;
	/* Whether we arrange our own version of opt.lfilename here.  */
	pid = fork ();
	if (pid < 0)
	{
		/* 出错 */
		exit (1);
	}
	else if (pid != 0)
	{
		/* 父进程退出 */
		exit (0);
	}
	/* 子进程继续执行 */
}
#else
char *u_read_registry (HKEY hkey, char *subkey, char *valuename, char *buf, int *len)
{
	HKEY result;
	DWORD size = *len;
	DWORD type = REG_SZ;
	if (RegOpenKeyEx (hkey, subkey, (DWORD)NULL, KEY_READ, &result) != ERROR_SUCCESS)
		return NULL;
	if (RegQueryValueEx (result, valuename, NULL, &type, (LPBYTE)buf, &size) != ERROR_SUCCESS)
		buf = NULL;
	*len = size;
	RegCloseKey (result);
	return buf;
}

char *u_username (char *where)
{
	char buf[32], *ptr;
	int len = sizeof (buf);
	if (GetUserName (buf, (LPDWORD) &len) == TRUE)
	{
		;
	}
	else if (!!(ptr = getenv ("USERNAME")))
	{
		strcpy (buf, ptr);
	}
	else if (!u_read_registry (HKEY_LOCAL_MACHINE, "Network\\Logon","username", buf, &len))
	{
		return NULL;
	}
	if (where)
	{
		strncpy (where, buf, len);
		return where;
	}
	return u_strdup (buf);
}

void u_gotoback (void)
{
	//int windows_nt_p=0;
	//OSVERSIONINFO os;
	//if (GetVersionEx (&os) == TRUE	&& os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	//	windows_nt_p = 1;

	//if (!windows_nt_p)
	//	FreeConsole ();
}

#endif /* not WINDOWS */


int u_file_is_exist (const char *filename)
{
  struct stat buf;
  return stat (filename, &buf) >= 0;
}

/* 如果 path 是目录返回0, 否则返回1 */
int u_file_non_dir (const char *path)
{
	struct stat buf;
	if (stat (path, &buf) != 0)
		return 0;
	return S_ISDIR (buf.st_mode) ? 0 : 1;
}

/* 创建所有级别目录 */
int u_mkdir (const char *directory)
{
	int quit = 0;
	int tmpi;
	char *dir;
	
	dir = u_strdup (directory);
	
	for (tmpi = (*dir == '/' || *dir == '\\' ); 1; tmpi++)
	{
		for (; dir[tmpi] && (dir[tmpi] != '/' && dir[tmpi] != '\\'); tmpi++)
			;
		if (!dir[tmpi])
			quit = 1;
		dir[tmpi] = '\0';
		/* 判断是否有同名的文件或者目录存在 */
		if (!u_file_is_exist (dir))
		{
#ifndef WIN32
			if (mkdir (dir, 0777) < 0)
#else
			if( tmpi >= 1 && dir[tmpi-1] == ':' )
			{
				if (quit)
					break;
				else
					dir[tmpi] = '/';
				continue;
			}
			if (_mkdir (dir) < 0)
#endif
			{
				my_free(dir);
				return -1;
			}
		}
		if (quit)
			break;
		else
			dir[tmpi] = '/';
	}
	my_free(dir);
	return 0;
}

/* 创建文件的所有级别目录 */
int u_mkpath (const char *directory)
{
	int tmpi;
	char *dir;
	
	dir = u_strdup (directory);
	
	for (tmpi = (*dir == '/' || *dir == '\\' ); 1; tmpi++)
	{
		for (; dir[tmpi] && (dir[tmpi] != '/' && dir[tmpi] != '\\'); tmpi++)
			;
		if (!dir[tmpi])
			break;
		dir[tmpi] = '\0';
		/* 判断是否有同名的文件或者目录存在 */
		if (!u_file_is_exist (dir))
		{
#ifndef WIN32
			if (mkdir (dir, 0777) < 0)
#else
			if( tmpi >= 1 && dir[tmpi-1] == ':' )
			{
				dir[tmpi] = '/';
				continue;
			}
			if (_mkdir (dir) < 0)
#endif
			{
				my_free(dir);
				return -1;
			}
		}
		dir[tmpi] = '/';
	}
	my_free(dir);
	return 0;
}

/* 如果是pattern是string的前缀，返回1，否则返回0 */
int u_frontcmp (const char *string, const char *pattern)
{
	for (; *pattern && *string && (*pattern == *string); ++pattern, ++string);
	return !*pattern;
}

/* 判断后缀:
   match_backwards ("abc", "bc") -> 1
   match_backwards ("abc", "ab") -> 0
   match_backwards ("abc", "abc") -> 1 */
int u_backcmp (const char *string, const char *pattern)
{
	int tmpi, tmpj;
	for (tmpi = (int)strlen (string), tmpj = (int)strlen (pattern); tmpi >= 0 && tmpj >= 0; tmpi--, tmpj--)
	if (string[tmpi] != pattern[tmpj])
		break;
	if (tmpj == -1)
		return 1;
	else
		return 0;
}

/* 返回扩展名:
   suffix ("foo.bar")       -> "bar"
   suffix ("foo.bar.baz")   -> "baz"
   suffix ("/foo/bar")      -> NULL
   suffix ("/foo.bar/baz")  -> NULL  */
char *u_suffix (const char *str)
{
	int tmpi;
	
	for (tmpi = (int)strlen (str); tmpi && str[tmpi] != '/' && str[tmpi] != '.'; tmpi--);
	if (str[tmpi++] == '.')
		return u_strdup (str + tmpi);
	else
		return NULL;
}

/* 从文件中读取一行 */
char * u_read_line (FILE *fp)
{
	char *line;
	int tmpi, bufsize, ch;

	tmpi = 0;
	bufsize = 40;
	line = (char *)my_malloc( bufsize );

	while ((ch = getc (fp)) != EOF && ch != '\n')
	{
		if (tmpi > bufsize - 1)
		line = (char *)my_realloc( line, (bufsize <<= 1) );
		line[tmpi++] = ch;
	}
	if (ch == EOF && !tmpi)
	{
		my_free( line );
		return NULL;
	}

	if (tmpi == bufsize)
		line = (char *)my_realloc( line, tmpi + 1 );
	line[tmpi] = '\0';
	return line;
}

/* 从当前文件指针读取整个文件到字符串数组 */
char **u_load_file (FILE *fp,int *count)
{
	int line_count = 10;
	int tmpi = 0;
	char **buf;
	if(fp==NULL)
		return NULL;
	buf = (char **)my_malloc( sizeof(char *)*(line_count+1) );
	while (!feof (fp) && !ferror (fp))
	{
		if(tmpi>=line_count)
		{
			line_count<<=1;
			buf = (char **)my_realloc( buf, sizeof(char *)*(line_count+1) );
		}
		if((*(buf+tmpi) = u_read_line (fp))==NULL)
			break;
		tmpi++;
	}
	*(buf+tmpi)=NULL;
	*count=tmpi;
	return buf;
}

// 二进制读取文件，注意释放
char *u_load_binary( char *filename )
{
	FILE *fp;
	int filesize;
	int bufsize;
	char *pbuf;
	fp = fopen( filename, "rb" );
	if ( fp == NULL )
		return NULL;
	fseek( fp, 0, SEEK_END );
	filesize = ftell( fp );
	bufsize = filesize;
	if ( bufsize <= 0 )
	{
		fclose( fp );
		return NULL;
	}
	pbuf = (char *)malloc( bufsize );
	if ( pbuf == NULL )
	{
		fclose( fp );
		return NULL;
	}
	fseek( fp, 0, SEEK_SET );
	if ( fread( pbuf, 1, bufsize, fp ) != bufsize )
	{
		fclose( fp );
		return NULL;
	}
	fclose( fp );
	return pbuf;
}

/* 释放数组 */
void u_free_vec (char **vec)
{
	if (vec)
	{
		char **p = vec;
		while (*p)
			my_free( *p++ );
		my_free( vec );
	}
}
/* 长整型数字转换成字符串 */
void long2str(char *buffer, long number)
{
	char *p;
	int i, l;

	if (number < 0)
	{
		*buffer++ = '-';
		number = -number;
	}
	p = buffer;

	do
	{
		*p++ = (char)(number % 10 + '0');
		number /= 10;
	}while (number);

	l = (int)(p - buffer - 1);
	for (i = l/2; i >= 0; i--)
	{
		char c = buffer[i];
		buffer[i] = buffer[l - i];
		buffer[l - i] = c;
	}
	buffer[l + 1] = '\0';
}

//读取配置文件
//注释标记到行结束为注释部分
//变量标记为变量开始部分，= 分隔标记与实际变量
//实际变量之间可能有空格，tab等分隔
//一个变量一行表示完
#define REMARK '#'
#define MAXLINESIZE 1024
int u_getcfg(char *configfile, LPCALLBACK_GETVALUE process )
{
	char linebuf[MAXLINESIZE],*psign,*pvel,*ptmp;
	FILE *fp=NULL;
	int off,tmplen;
	if( process == NULL )
		return -1;
	off=0;
	if((fp=fopen(configfile,"rb"))==NULL)
		return -1;
	while(!feof(fp))
	{
		//读取一行
		fgets(linebuf,MAXLINESIZE,fp);
		//找到第一个有意义的字符
		psign=linebuf;
		while(1)
		{
			//搜索完毕
			if( *psign==0 || *psign=='\r' || *psign=='\n' || *psign==REMARK )
			{
				psign=NULL;
				break;
			}
			if( *psign !=' ' && *psign != '\t' )
				break;
			psign++;
		}
		if(psign)//找到了标记
		{
			pvel=strchr(psign,'=');
			if( pvel && pvel!=psign ) //找到=
			{
				ptmp=pvel-1;
				*pvel=0;
				while(ptmp>=psign)
				{
					if( *ptmp!='\r' && *ptmp!='\n' && *ptmp !=' ' && *ptmp != '\t')
						break;
					*ptmp = 0;
					ptmp--;
				}
				pvel++;
				while(1)
				{
					//搜索完毕
					if( *pvel==0 || *pvel=='\r' || *pvel=='\n' || *pvel==REMARK )
					{
						pvel=NULL;
						break;
					}
					if( *pvel !=' ' && *pvel != '\t' )
						break;
					pvel++;
				}
				if(pvel)//找到了标记
				{
					ptmp=strchr(pvel,REMARK);
					if(ptmp)
						*ptmp=0;

					tmplen=(int)strlen(pvel);
					ptmp=pvel+tmplen-1;
					while(ptmp>pvel)
					{
						if( *ptmp!='\r' && *ptmp!='\n' && *ptmp !=' ' && *ptmp != '\t')
							break;
						*ptmp = 0;
						ptmp--;
					}
					process(psign,pvel);
					/*替换此处*/
				}
				else  //没有找到非空字符
					continue;
			}
			else  //没有找到=或者标记是=
				continue;
		}
		else //没有找到非空字符
			continue;
	}
	return 0;
}

int u_ordstr(char *str,int mode)
{
	char *buf,*pstr,*ptmp;
	int tmplen;
	//1 去掉首部无意义字符
	//2 去掉尾部无意义字符
	tmplen=(int)strlen(str);
	buf=(char *)alloca(tmplen*sizeof(char));
	strcpy(buf,str);
	pstr=buf;
	if(mode & 1)
	{
		while(1)
		{
			if( *pstr==0 )
				break;
			if( *pstr !=' ' && *pstr != '\t' && *pstr!='\r' && *pstr!='\n' )
				break;
			pstr++;
		}
	}
	if(*pstr==0)
		return 0;
	if(mode & 2)
	{
		tmplen=(int)strlen(pstr);
		ptmp=pstr+tmplen-1;
		while(ptmp>pstr)
		{
			if( *ptmp!='\r' && *ptmp!='\n' && *ptmp !=' ' && *ptmp != '\t')
				break;
			*ptmp = 0;
			ptmp--;
		}
	}
	strcpy(str,pstr);
	return 0;
}

void *load_dll( const char *dllfile )
{
	void *handle = NULL;
#ifdef WIN32
	handle = (void *)LoadLibrary( dllfile );
#else
	handle = dlopen( dllfile, RTLD_NOW );
#endif
	return handle;
}

void unload_dll( void *handle )
{
	if ( handle == NULL )
	{
		return;
	}
#ifdef WIN32
	FreeLibrary( (HMODULE)handle );
#else
	dlclose( handle );
#endif
}

void *load_function( void *handle, const char *name )
{
	void *funaddress = NULL;
#ifdef WIN32
	funaddress = (void *)GetProcAddress( (HMODULE)handle, name );
#else
	funaddress = dlsym( handle, name );
#endif
	return funaddress;
}


char *str_replace(char *src,char oldChar,char newChar)
{
	char *head = src;
	while(*src!='\0')
	{
		if(*src==oldChar) 
			*src=newChar;
		src++;
	}
	return head;
}

void u_str_tolower( char *ptr )
{
	char *p;
	for( p = ptr; (p!=NULL)&&(*p!=0) ; p++ )
		*p = tolower( *p );
}

void u_str_toupper( char *ptr )
{
	char *p;
	for( p = ptr; (p!=NULL)&&(*p!=0) ; p++ )
		*p = toupper( *p );
}

