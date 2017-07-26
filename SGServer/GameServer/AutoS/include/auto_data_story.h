#ifndef __STORY_DATA_AUTO_H
#define __STORY_DATA_AUTO_H
#include "define.h"

struct _storyinfo {
//--Automatically generated
				int id;	//
				short chapter;	//
				short rank;	//
				short saveoffset;	//
				short type;	//
				short preid;	//
				int monsterid[4];	//
				int awardgroup;	//
				short ranknum;	//
				short body;	//
				short returnbody;	//
				short skip;	//
};
typedef struct _storyinfo StoryInfo;

int storyinfo_init_auto();
int storyinfo_reload_auto();

#endif
