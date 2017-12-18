#ifndef __QUEST_TALK_DATA_AUTO_H
#define __QUEST_TALK_DATA_AUTO_H
#include "define.h"

struct _questtalk {
//--Automatically generated
				int id;	//
				int nextid;	//
				short herokind;	//
				int talk_textid;	//
				int btn_textid;	//
				int trigger_kind[2];	//
				int trigger_num[2];	//
				int limit_questid;	//
				int complete_questid;	//
};
typedef struct _questtalk QuestTalk;

int questtalk_init_auto();
int questtalk_reload_auto();

#endif
