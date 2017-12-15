#ifndef __STORY_DATA_AUTO_H
#define __STORY_DATA_AUTO_H
#include "define.h"

struct _storyinfo {
//--Automatically generated
				int id;	//
				short chapter;	//
				short rank;	//
				short type;	//
				short preid;	//
				short nextid;	//
				short unlock_preid;	//
				int monsterid[4];	//
				int exp;	//
				int silver;	//
				int awardgroup;	//
				short body;	//
				short lose_body;	//
				short skip;	//
				short color;	//
				short shapeback;	//
				short shape;	//
				short star_saveoffset;	//
				short hero_kind[2];	//
				short hero_odds;	//
				int hero_token;	//
				short hero_saveoffset;	//
				short restype;	//
				int rescount;	//
				short resnum;	//
				short resnum_saveoffset;	//
				int ressec;	//
				short ressec_saveoffset;	//
				short res_reset_num;	//
				short res_reset_saveoffset;	//
				short res_reset_token[6];	//
				int item_awardkind;	//
				int item_awardodds;	//
				short itemnum;	//
				short itemnum_saveoffset;	//
				short drawing_kind;	//
				short drawing_token;	//
				short drawing_saveoffset;	//
};
typedef struct _storyinfo StoryInfo;

int storyinfo_init_auto();
int storyinfo_reload_auto();

#endif
