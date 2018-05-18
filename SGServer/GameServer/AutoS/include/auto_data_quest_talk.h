#ifndef __QUEST_TALK_DATA_AUTO_H
#define __QUEST_TALK_DATA_AUTO_H
#include "define.h"

struct _questtalk {
//--Automatically generated
				int id;	//
				int nextid;	//
				short shape;	//
				int talk_textid;	//
				short format;	//
				int btn_textid;	//
				short op;	//
				int trigger_kind[2];	//
				int trigger_num[2];	//
				int limit_questid;	//
				int complete_questid;	//
				int next_questid;	//
				short guide;	//
};
typedef struct _questtalk QuestTalk;

int questtalk_init_auto();
int questtalk_reload_auto();

#endif
