#ifndef __STORY_DATA_AUTO_H
#define __STORY_DATA_AUTO_H
#include "define.h"

struct _storyinfo_config {
//--Automatically generated
				short chapter;	//
				short rank;	//
				int id;	//
				short saveoffset;	//
				short type;	//
				short preid;	//
				int monsterid[4];	//
				int exp;	//
				int silver;	//
				int awardgroup;	//
				short ranknum;	//
				short ranknum_saveoffset;	//
				int ranksec;	//
				short ranksec_saveoffset;	//
				short body;	//
				short returnbody;	//
				short skip;	//
				short color;	//
};
typedef struct _storyinfo_config StoryInfoConfig;

struct _storyinfo {
//--Automatically generated
	StoryInfoConfig *config;
	int	maxnum;
};
typedef struct _storyinfo StoryInfo;

int storyinfo_init_auto();
int storyinfo_reload_auto();

#endif
