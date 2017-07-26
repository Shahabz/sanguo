#ifndef __QUEST_DATA_AUTO_H
#define __QUEST_DATA_AUTO_H
#include "define.h"

struct _questinfo {
//--Automatically generated
				int questid;	//
				short type;	//
				short nameid;	//
				int sort;	//
				int preid;	//
				int datatype;	//
				int datakind;	//
				short dataoffset;	//
				int needvalue;	//
				short brushlevel;	//
				short brushnum;	//
				short brushrange;	//
				int awardkind[5];	//
				int awardnum[5];	//
				short trigger_type[2];	//
				int trigger_kind[2];	//
				int trigger_value[2];	//
};
typedef struct _questinfo QuestInfo;

int questinfo_init_auto();
int questinfo_reload_auto();

#endif
