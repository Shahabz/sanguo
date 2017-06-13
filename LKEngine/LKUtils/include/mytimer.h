
#define MAX_TIMER	16	// 可以使用的计时器的个数

void init_timer();
void append_timer( void *proc, void *para, int speed );
void exit_timer();
int delete_timer( void *proc );


