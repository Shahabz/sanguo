#ifndef __TEACHER_AWARD_DATA_AUTO_H
#define __TEACHER_AWARD_DATA_AUTO_H
#include "define.h"

struct _teacheraward {
//--Automatically generated
				int id;	//
				short student_level;	//
				short maxnum;	//
				int awardkind[5];	//
				int awardnum[5];	//
};
typedef struct _teacheraward TeacherAward;

int teacheraward_init_auto();
int teacheraward_reload_auto();

#endif
