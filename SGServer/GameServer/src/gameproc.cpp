#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

#include "db.h"
#include "dbredis.h"
#include "account.h"
#include "gameproc.h"
#include "actor.h"
#include "script.h"
#include "utils.h"
#include "gamelog.h"
#include "define.h"
#include "client.h"
#include "server_process_auto.h"
#include "login.h"
#include "timegmcmd.h"
#include "wqueue.h"
#include "server_netrecv_auto.h"
#include "actor_notify.h"
#include "system.h"
#include "item.h"
#include "equip.h"
#include "global.h"
#include "map.h"
#include "mapunit.h"
#include "zoneunit.h"
#include "gmcmd.h"
#include "award.h"
#include "global_netprocess.h"
#include "script_auto.h"
#include "activity.h"
#include "script_auto.h"
#include "global_cmdqueue.h"
#include "city.h"
#include "quest.h"
#include "world_quest.h"
#include "world_boss.h"
#include "hero.h"
#include "chat.h"
#include "army.h"
#include "army_group.h"
#include "map_zone.h"
#include "map_town.h"
#include "map_enemy.h"
#include "map_res.h"
#include "map_event.h"
#include "map_call.h"
#include "map_pickup.h"
#include "nation.h"
#include "nation_hero.h"
#include "king_war.h"
#include "rank.h"
#include "pay.h"
#include "wishing.h"
#include "girl.h"
#include "robot.h"

#ifndef WIN32 // ��Щͷ�ļ�������ulimit���õ�
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif

extern MYSQL *myGame;
SConfig g_Config;
extern Global global;

int g_speed = 0;
int g_sec = 0;	// ����

int g_mysql_wait = 0;
int g_save_wait = 0;

int g_last_cmd = 0;

int g_maxactornum = 512;
int g_frame_count;
extern int g_nReadQueueHead;
extern int g_nReadQueueTail;
extern Actor *g_actors;
extern char g_user_queue_logic;

#ifndef WIN32
extern int linuxmsg_recv();
#endif

extern int serv_setstat( int stat );

int process_init( int max_connection )
{
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
#ifndef WIN32
	struct rlimit out_data={0};
	if ( getrlimit( RLIMIT_CORE, &out_data ) == 0 )
	{
		if ( out_data.rlim_cur != -1 )
		{
			out_data.rlim_cur = -1;
			out_data.rlim_max = -1;
			if ( setrlimit( RLIMIT_CORE, &out_data ) != 0 )
			{
				printf_msg( "setrlimit( RLIMIT_CORE, &out_data ) Error!" );
				return -1;
			}
		}
	}
	else
	{
		printf_msg( "getrlimit( RLIMIT_CORE, &out_data ) Error!" );
		return -1;
	}

	if ( getrlimit( RLIMIT_NOFILE, &out_data ) == 0 )
	{
		if ( out_data.rlim_cur < max_connection )
		{
			out_data.rlim_cur = 8196;
			out_data.rlim_max = 8196;
			if ( setrlimit( RLIMIT_NOFILE, &out_data ) != 0 )
			{
				printf_msg( "setrlimit( RLIMIT_NOFILE, &out_data ) Error!" );
				return -1;
			}
		}
	}
	else
	{
		printf_msg( "getrlimit( RLIMIT_NOFILE, &out_data ) Error!" );
		return -1;
	}

	// �ٴμ��
	if ( getrlimit( RLIMIT_CORE, &out_data ) == 0 )
	{
		if ( out_data.rlim_cur != -1 )
		{
			printf_msg( "getrlimit( RLIMIT_CORE, &out_data ) too small!" );
			return -1;
		}
	}
	else
	{
		printf_msg( "getrlimit( RLIMIT_CORE, &out_data ) Error!" );
		return -1;
	}

	if ( getrlimit( RLIMIT_NOFILE, &out_data ) == 0 )
	{
		if ( out_data.rlim_cur < max_connection )
		{
			printf_msg( "getrlimit( RLIMIT_NOFILE, &out_data ) too small!" );
			return -1;
		}
	}
	else
	{
		printf_msg( "getrlimit( RLIMIT_NOFILE, &out_data ) Error!" );
		return -1;
	}
#endif
	g_frame_count = 0;
	g_maxactornum = max_connection;

	if ( g_Config.server_code > 2000 )
	{
		printf_msg( "g_Config.server_code > 2000!" );
		return -1;
	}
	serv_setstat( 2 );
	// ���г�ʼ��
	init_wqueue();

	// ִ�ж��г�ʼ��
	exec_queue_init();
	brush_enemy_queue_init();

	// ���ݿ��ʼ��
	if ( db_init() >= 0 )
		printf_msg( "Mysql Module ready!" );
	else
	{
		printf_msg( "Mysql Module Error!" );
		return -1;
	}
	serv_setstat( 4 );

	/*if ( dbredis_init() >= 0 )
		printf_msg( "Redis Module ready!" );
	else
	{
		printf_msg( "Redis Module Error!" );
		return -1;
	}*/
	serv_setstat( 5 );

	// �ű���ʼ��
	if ( lua_init( 0 ) >= 0 )
		printf_msg( "Lua Module ready!" );
	else
	{
		printf_msg( "Lua Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 6 );

	// account��ʼ��
	if ( account_init() >= 0 )
		printf_msg( "Account Module ready!" );
	else
	{
		printf_msg( "Account Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 6 );

	// ȫ�����ݳ�ʼ��
	if ( global_init() >= 0 )
		printf_msg( "Global Module ready!" );
	else
	{
		printf_msg( "Global Module Error!" );
		return -1;
	}
	serv_setstat( 5 );

	// ƽ̨��Ϣ��ʼ��
	if ( platinfo_init_auto() >= 0 )
		printf_msg( "Plat Module ready!" );
	else
	{
		printf_msg( "Plat Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 7 );

	//// ��Ʒ��ʼ��
	//if ( paygoods_init() >= 0 )
	//	printf_msg( "PayGoods Module ready!" );
	//else
	//{
	//	printf_msg( "PayGoods Module Error!" );
	//	return -1;
	//}
	//LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	//paycountry_init();
	//serv_setstat( 10 );

	//// ֧���̵��ʼ��
	//if ( paystore_init() >= 0 )
	//	printf_msg( "PayStore Module ready!" );
	//else
	//{
	//	printf_msg( "PayStore Module Error!" );
	//	return -1;
	//}
	//LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	//serv_setstat( 11 );

	// ��ɫ���ݳ�ʼ��
	if ( actors_init() >= 0 )
		printf_msg( "Actor Module ready!" );
	else
	{
		printf_msg( "Actor Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 13 );

	// ��½�߳�����
	if ( login_init() >= 0 )
		printf_msg( "Login Module ready!" );
	else
	{
		printf_msg( "Login Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 14 );

	// GMTOOLS
	if( gm_init() < 0 )
	{
		printf_msg( "GM Module Error!" );
		return -1;
	}
	LOGI("%s-%d",__FUNCTION__,__LINE__);
	//global_process_init();
	serv_setstat( 15 );
	lastname_init_auto();
	firstname_init_auto();
	robotcreate_queue_init();
	// ���������ݳ�ʼ��
	if ( robotai_init_auto() < 0 )
	{
		printf_msg( "robotai_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );
	// ���������ݳ�ʼ��
	if ( robotbase_init_auto() < 0 )
	{
		printf_msg( "robotbase_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// �������ݳ�ʼ��
	if ( itemkind_init() < 0 )
	{
		printf_msg( "ItemKind Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// װ�����ݳ�ʼ��
	if ( equipinfo_init_auto() < 0 )
	{
		printf_msg( "equipinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// װ��ϴ�����ݳ�ʼ��
	if ( equipwashinfo_init_auto() < 0 )
	{
		printf_msg( "equipwashinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// װ��ϴ�����ݳ�ʼ��
	if ( equipwashrule_init_auto() < 0 )
	{
		printf_msg( "equipwashrule_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// Ӣ�����ݳ�ʼ��
	if ( heroinfo_init_auto() < 0 )
	{
		printf_msg( "heroinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// Ӣ��ͻ�����ݳ�ʼ��
	if ( herocolorupinfo_init_auto() < 0 )
	{
		printf_msg( "herocolorupinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );
	
	// Ӣ�ۼ������ݳ�ʼ��
	if ( heroskillinfo_init_auto() < 0 )
	{
		printf_msg( "heroskillinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// Ů�����ݳ�ʼ��
	if ( girlinfo_init_auto() < 0 )
	{
		printf_msg( "girlinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// Ů�����Ƕ����ݳ�ʼ��
	if ( girllove_init_auto() < 0 )
	{
		printf_msg( "girllove_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// ��Ůϵͳ���ݳ�ʼ��
	if ( girlson_init_auto() < 0 )
	{
		printf_msg( "girlson_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// Ů���̵����ݳ�ʼ��
	if ( girlshop_init_auto() < 0 )
	{
		printf_msg( "girlshop_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// Ů���̵����ݳ�ʼ��
	if ( girlshopupdate_init_auto() < 0 )
	{
		printf_msg( "girlshopupdate_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// ���нڵ����ݳ�ʼ��
	if ( fangshinode_init_auto() < 0 )
	{
		printf_msg( "fangshinode_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// ���лʹ���Ժ���ݳ�ʼ��
	if ( fangshipalace_init_auto() < 0 )
	{
		printf_msg( "fangshipalace_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// ������ʼ��
	if ( nationequipinfo_init_auto() < 0 )
	{
		printf_msg( "nationequipinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// ������ʼ��
	if ( nationequipopen_init_auto() < 0 )
	{
		printf_msg( "nationequipopen_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// ������ʼ��
	if ( nationequipremake_init_auto() < 0 )
	{
		printf_msg( "nationequipremake_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// ����������ʼ��
	if ( nationupgrade_init_auto() < 0 )
	{
		printf_msg( "nationupgrade_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// ��λ��ʼ��
	if ( nationplace_init_auto() < 0 )
	{
		printf_msg( "nationplace_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// ��ְ��ʼ��
	if ( nationofficial_init_auto() < 0 )
	{
		printf_msg( "nationofficial_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// ���������ʼ��-����
	if ( nationquest_init_auto() < 0 )
	{
		printf_msg( "nationquest_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// ���������ʼ��-����
	if ( nationmission_init_auto() < 0 )
	{
		printf_msg( "nationmission_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// ��������
	if ( nationheroinfo_init_auto() < 0 )
	{
		printf_msg( "nationheroinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// Ӣ����������ݳ�ʼ��
	if ( herocorpsmulinfo_init_auto() < 0 )
	{
		printf_msg( "herocorpsmulinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// Ӣ��Ѱ��
	if ( herovisit_init_auto() < 0 )
	{
		printf_msg( "herovisit_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// Ӣ��Ѱ��
	if ( hero_visit_init() < 0 )
	{
		printf_msg( "hero_visit_init Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// ������Ϣ��ʼ��
	if ( monsterinfo_init_auto() < 0 )
	{
		printf_msg( "monsterinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// ���������ݳ�ʼ��
	if ( awardgroup_init() < 0 )
	{
		printf_msg( "AwardGroup Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// �������ݳ�ʼ��
	if ( upgradeinfo_init_auto() < 0 )
	{
		printf_msg( "upgradeinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// �����������ݳ�ʼ��
	if ( buildingupgrade_init_auto() < 0 )
	{
		printf_msg( "buildingupgrade_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// �����������ݳ�ʼ��
	if ( buildingresunlock_init_auto() < 0 )
	{
		printf_msg( "buildingresunlock_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// �Ƽ�
	if ( techinfo_init_auto() < 0 )
	{
		printf_msg( "techinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ��Ӷ
	if ( officialforging_init_auto() < 0 )
	{
		printf_msg( "officialforging_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ��Ӷ
	if ( officialgov_init_auto() < 0 )
	{
		printf_msg( "officialgov_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ��Ӷ
	if ( officialtech_init_auto() < 0 )
	{
		printf_msg( "officialtech_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// VIP
	if ( vipinfo_init_auto() < 0 )
	{
		printf_msg( "vipinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// VIP���
	if ( vipbag_init_auto() < 0 )
	{
		printf_msg( "vipbag_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// VIP�̵�
	if ( vipshop_init_auto() < 0 )
	{
		printf_msg( "vipshop_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ���֧���۸��ʼ��
	if ( payprice_init() >= 0 )
		printf_msg( "PayPrice Module ready!" );
	else
	{
		printf_msg( "PayPrice Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 8 );

	// ���֧��������Ϣ
	if ( paycountry_init() >= 0 )
		printf_msg( "paycountry_init Module ready!" );
	else
	{
		printf_msg( "paycountry_init Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 9 );

	// ��Ʒ��ʼ��
	if ( paygoods_init_auto() >= 0 )
		printf_msg( "paygoods_init_auto Module ready!" );
	else
	{
		printf_msg( "paygoods_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 10 );

	// ֧���̵��ʼ��
	if ( paystore_init_auto() >= 0 )
		printf_msg( "paystore_init_auto Module ready!" );
	else
	{
		printf_msg( "paystore_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 11 );

	// ļ��ʱ��
	if ( trainlonginfo_init_auto() < 0 )
	{
		printf_msg( "trainlonginfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ļ������
	if ( trainqueueinfo_init_auto() < 0 )
	{
		printf_msg( "trainqueueinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ����
	if ( questinfo_init_auto() < 0 )
	{
		printf_msg( "questinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ����Ի�
	if ( questtalk_init_auto() < 0 )
	{
		printf_msg( "questtalk_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ��ǽ����
	if ( cityguardinfo_init_auto()  < 0 )
	{
		printf_msg( "cityguardinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ��������������ʯ
	if ( bodytokeninfo_init_auto() < 0 )
	{
		printf_msg( "bodytokeninfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ��������
	if ( materialmakeinfo_init_auto() < 0 )
	{
		printf_msg( "materialmakeinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ����
	if ( storyinfo_init_auto() < 0 )
	{
		printf_msg( "storyinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// �����ͼ-����
	if ( mapzoneinfo_init_auto() < 0 )
	{
		printf_msg( "mapzoneinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// �����ͼ-����
	if ( maptowninfo_init_auto() < 0 )
	{
		printf_msg( "maptowninfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// �����ͼ-����
	if ( mapenemyinfo_init_auto() < 0 )
	{
		printf_msg( "mapenemyinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// �����ͼ-��Դ��
	if ( mapresinfo_init_auto() < 0 )
	{
		printf_msg( "mapresinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// �����ͼ-����¼�
	if ( mapeventinfo_init_auto() < 0 )
	{
		printf_msg( "mapeventinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );

	// �����ͼ-ʰȡ��Ʒ
	if ( mappickupinfo_init_auto() < 0 )
	{
		printf_msg( "mappickupinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ����boss
	if ( worldbossinfo_init_auto() < 0 )
	{
		printf_msg( "worldbossinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );

	// ��������
	if ( worldquestinfo_init_auto() < 0 )
	{
		printf_msg( "worldquestinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ���������б�
	if ( worldquest_init() < 0 )
	{
		printf_msg( "worldquest_init Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// Ѫս�ʳ�
	if ( kingwarconfig_init_auto() < 0 )
	{
		printf_msg( "kingwarconfig_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// Ѫս�ʳǾݵ�
	if ( kingwartowninfo_init_auto() < 0 )
	{
		printf_msg( "kingwartowninfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// �̵�
	if ( shop_init_auto() < 0 )
	{
		printf_msg( "shop_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ����
	if ( weatherinfo_init_auto() < 0 )
	{
		printf_msg( "weatherinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// �۱���
	if ( wishingshop_init_auto() < 0 )
	{
		printf_msg( "wishingshop_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// �۱���-���
	if ( wishingpack_init_auto() < 0 )
	{
		printf_msg( "wishingpack_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ��߸����
	if ( tiancequest_init_auto() < 0 )
	{
		printf_msg( "tiancequest_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ʦͽ
	if ( teacheraward_init_auto() < 0 )
	{
		printf_msg( "teacheraward_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// ʦͽ
	if ( teachershop_init_auto() < 0 )
	{
		printf_msg( "teachershop_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// �
	activityinfo02_init_auto();
	activityinfo03_init_auto();
	activityinfo04_init_auto();
	activityinfo05_init_auto();
	activityinfo06_init_auto();
	activityinfo08_init_auto();
	activityinfo10_init_auto();

	activity_init();
	time_gmcmd_init();
	db_closedata();
	sc_Script_Init();
	//pushwork_init();
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 48 );

	// ����
	weather_load();

	// װ�����ID��ʼ��
	if ( equip_maxid_init() < 0 )
	{
		printf_msg( "equip_maxid_init Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// Ӣ�����ID��ʼ��
	if ( hero_maxid_init() < 0 )
	{
		printf_msg( "hero_maxid_init Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// �����ͼ��ʼ�����ϸ�˳��Ҫ�󣬲�����ı䣩
	if ( map_init() < 0 )
	{
		printf_msg( "WorldMap Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 101 );

	// �ӵ�ͼ�������ϸ�˳��Ҫ�󣬲�����ı䣩
	if ( map_zone_load() < 0 )
	{
		printf_msg( "MapZone Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 107 );

	// �������е�ͼ��ʾ��Ԫ�ṹ���ڳǳغͳ�������֮ǰ��ʼ�����ϸ�˳��Ҫ�󣬲�����ı䣩
	if ( mapunit_init() < 0 )
	{
		printf_msg( "MapUnit Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 102 );

	// �������е�ͼ��������ʾ��Ԫ�ṹ���ڳǳغͳ�������֮ǰ��ʼ�����ϸ�˳��Ҫ�󣬲�����ı䣩
	if ( zoneunit_init() < 0 )
	{
		printf_msg( "ZoneUnit Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 102 );

	// �������г��У��ϸ�˳��Ҫ�󣬲�����ı䣩
	if ( city_load() < 0 )
	{
		printf_msg( "CityLoad Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 107 );

	// ���س����ϸ�˳��Ҫ�󣬲�����ı䣩
	if ( map_town_load() < 0 )
	{
		printf_msg( "map_town_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );

	// ���ع��ң��ϸ�˳��Ҫ�󣬲�����ı䣩
	if ( nation_load() < 0 )
	{
		printf_msg( "nation_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );

	// �������ܣ��ϸ�˳��Ҫ�󣬲�����ı䣩
	if ( map_enemy_load() < 0 )
	{
		printf_msg( "map_enemy_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 111 );

	// ������Դ�㣨�ϸ�˳��Ҫ�󣬲�����ı䣩
	if ( map_res_load() < 0 )
	{
		printf_msg( "map_res_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 112 );

	// ���ع����������ϸ�˳��Ҫ�󣬲�����ı䣩
	if ( nation_hero_load() < 0 )
	{
		printf_msg( "nation_hero_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 111 );

	// ����ʰȡ��Ʒ���ϸ�˳��Ҫ�󣬲�����ı䣩
	if ( map_pickup_load() < 0 )
	{
		printf_msg( "map_pickup_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 111 );

	// ���ػʳ�Ѫս�ݵ㣨�ϸ�˳��Ҫ�󣬲�����ı䣩
	if ( kingwar_town_load() < 0 )
	{
		printf_msg( "kingwar_town_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );

	// �������в��ӣ��ϸ�˳��Ҫ�󣬲�����ı䣩
	if ( army_load() < 0 )
	{
		printf_msg( "army_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 116 );

	// �������м��ᣨ�ϸ�˳��Ҫ�󣬲�����ı䣩
	if ( armygroup_load() < 0 )
	{
		printf_msg( "armygroup_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 116 );

	// �������е�ͼ�¼����ϸ�˳��Ҫ�󣬲�����ı䣩
	if ( map_event_load() < 0 )
	{
		printf_msg( "map_event_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 116 );

	// ��������boss���ϸ�˳��Ҫ�󣬲�����ı䣩
	if ( world_boss_load() < 0 )
	{
		printf_msg( "world_boss_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );

	// ���ص�ͼ�ٻ�
	if ( map_call_load() < 0 )
	{
		printf_msg( "map_call_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 116 );

	// �������͵����
	if ( paybag_load() < 0 )
	{
		printf_msg( "paybag_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );

	// ȫ������
	if ( world_data_init() < 0 )
	{
		printf_msg( "TalkCacheLoad Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 122 );

	// ���컺��
	chat_cache_load();

	// ������־����
	nationlog_cache_load();

	// ˢ��ͼ
	sc_OnWorldMapBrush();
	serv_setstat( 117 );
	
	// ���а�
	if ( rank_init() < 0 )
	{
		printf_msg( "Rank Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 118 );

	// �����������а�
	if ( nation_rank_calc() < 0 )
	{
		printf_msg( "nation_rank_calc Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 118 );

	// ���ݿ���߳�����
	if ( dbwork_start() >= 0 )
		printf_msg( "dbwork Module ready!" );
	else
	{
		printf_msg( "dbwork Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 119 );

	// �������״γ�ʼ��
	process_serverinit();
	return 0;
}

// �������ر�
void process_close()
{
	int tmpi;
	// �ر�ÿ���ͻ���
	for ( tmpi = 0; tmpi < g_maxactornum; tmpi++ )
	{
		actor_remove( tmpi );
		printf_msg( "actor_save %d/%d\r", tmpi + 1, g_maxactornum );
	}
	printf_msg( "\n" );


	// ����һ��������
	mysql_query( myGame, "START TRANSACTION" );

	// ���гǳر���
	city_save( NULL );
	printf_msg( "\n" );

	// ���й��ұ���
	nation_save( NULL );
	printf_msg( "\n" );

	// ���в��ӱ���
	army_save( NULL );
	printf_msg( "\n" );

	// ���м��ᱣ��
	armygroup_save( NULL );
	printf_msg( "\n" );

	// ���е�������
	map_zone_save( NULL );
	printf_msg( "\n" );

	// ���г��򱣴�
	map_town_save( NULL );
	printf_msg( "\n" );

	// �������ܱ���
	map_enemy_save( NULL );
	printf_msg( "\n" );

	// ������Դ�㱣��
	map_res_save( NULL );
	printf_msg( "\n" );

	// ��������¼��㱣��
	map_event_save( NULL );
	printf_msg( "\n" );

	// �����ٻ�����
	map_call_save( NULL );
	printf_msg( "\n" );

	// ���й�����������
	nation_hero_save( NULL );
	printf_msg( "\n" );

	// ����ʰȡ�ﱣ��
	map_pickup_save( NULL );
	printf_msg( "\n" );

	// ��������boss
	world_boss_save( NULL );
	printf_msg( "\n" );
	
	// ����Ѫս�ʳǾݵ㱣��
	kingwar_town_save( NULL );
	printf_msg( "\n" );

	// �ύ
	mysql_query( myGame, "COMMIT;" );

	dbwork_stop();
	db_closegame();
	dbredis_close();
	lua_exit();
	/*pushwork_destory();*/
}

// ����-1,��ʾ����ֱ��������client
int process_onclose( int client_index )
{
	return actor_del( client_index );
}

int process_error( int client_index, int err )
{
	return 0;
}

int actors_save_everyone( char type );
int process_ondestory()
{
	// ���н�ɫ���ݱ���
	actors_save_everyone( 1 );
	return 0;
}
int Clear_Welcome();
int process_logic_wait_in();
int process_logic_wait_out();

int g_nExitHour = -1;
int g_nExitMin = -1;
int g_nExitNotify = 0;
time_t g_tExitTime = 0;
void process_exitlogic()
{
	static int s_count = 0;
	time_t t;
	char szMsg[MAX_PATH];
	int min = -1;

	if ( g_tExitTime <= 0 || g_nExitHour < 0 || g_nExitHour > 23 || g_nExitMin < 0 || g_nExitMin > 59 )
		return;
	time( &t );

	if ( g_tExitTime - t <= 0 )	// ����
		min = 0;
	else if ( g_tExitTime - t <= 60 )	// 1����
		min = 1;
	else if ( g_tExitTime - t <= 180 )	// 3����
		min = 3;
	else if ( g_tExitTime - t <= 300 )	// 5����
		min = 5;
	else if ( g_tExitTime - t <= 600 )	// 10����
		min = 10;
	else if ( g_tExitTime - t <= 14400 )	// 4Сʱ����(240����)
		min = -2;

	if ( min > 0 )
	{
		sprintf( szMsg, "�װ�������������Ǻã�������ά����Ҫ��������������%d������ͣ����Ϊ�������⣬�뾡�����ߣ�ף����Ϸ��죡", min );
		//system_talk( szMsg, TALK_CHANNEL_SYSTEM );
	}
	else if ( min == 0 )
	{
		g_nExitHour = -1;
		g_nExitMin = -1;
		g_tExitTime = 0;
		server_exit();
	}
	else if ( min == -2 )
	{
		if ( g_nExitNotify && (s_count % 60) == 0 )
		{
			sprintf( szMsg, "�װ�������������Ǻã���������ά������Ҫ������������%d��%d������ͣ����", g_nExitHour, g_nExitMin );
			//system_talk( szMsg, TALK_CHANNEL_SYSTEM );
		}
		s_count++;
	}
}

// 0~6 0~23 0~60 0~1
int process_set_exit( int weekday, int hour, int min, int isnotify )
{
	time_t t;
	struct tm *newtime;
	int weekdiff;

	if ( hour < 0 || min < 0 || hour > 23 || min > 59 )
	{
		g_nExitHour = -1;
		g_nExitMin = -1;
		g_tExitTime = 0;
		return -1;
	}

	time( &t );
	newtime = localtime( &t );

	if ( weekday < 0 || weekday > 6 )	// ����
	{
		g_tExitTime = t + (hour - newtime->tm_hour) * 60 * 60 + (min - newtime->tm_min) * 60;
		if ( g_tExitTime < t )
			g_tExitTime += 24 * 60 * 60;
	}
	else
	{
		weekdiff = weekday - newtime->tm_wday;
		if ( weekdiff <= 0 )
			weekdiff += 7;
		g_tExitTime = t + (hour - newtime->tm_hour) * 60 * 60 + (min - newtime->tm_min) * 60 + weekdiff * 24 * 60 * 60;
	}

	g_nExitHour = hour;
	g_nExitMin = min;
	g_nExitNotify = isnotify;

	return 0;
}

// ������Ҫ����ִ�е�������Էŵ�������
// ���СʱΪ-1����ʾ�״�ִ�У��������ο��Թ�
int process_oclock_process( int hour )
{
	if ( hour == 0 )
	{ // �¿�����
		paycard_give();
		// ˢ�¹�����������
		nation_mission_update();

		// ˢ�¹����������а�
		if ( system_getweek() == 5 )
		{
			nation_rank_update();
		}

		// Ů��
		girl_update();
	}
	else if ( hour == 1 )
	{
	}
	
	
	// ���ҳ�
	if ( hour == 4 || hour == 8 || hour == 12 || hour == 16 || hour == 20 )
	{
		nation_hero_subloyal();
	}

	if ( hour == global.nation_quest_timer[0] || hour == global.nation_quest_timer[1] || hour == global.nation_quest_timer[2] )
	{ // ˢ�¹�������
		nation_quest_update();
	}

	if ( hour == global.town_owner_award )
	{ // ��������
		map_town_owner_award();
	}
	sc_OnClockProcess( hour );
	return 0;
}

static int nextclock_timestump = 0;
static int nexclock_hour = -1;
// �����ж���������������Ҫ������¼�������������
int process_oclock_check()
{
	time_t t;
	time( &t );

	// ��ǰʱ���Ѿ������¸�ʱ�ӵ�ʱ����
	if ( t > nextclock_timestump )
	{
		process_oclock_process( nexclock_hour );
		// ��ʱ���������һ��Сʱ
		t += 3600;
		// ��ȡʱ��
		struct tm *newtime;
		newtime = localtime( &t );
		// �����µ�����
		struct tm BeginTm = { 0 };
		BeginTm.tm_year = newtime->tm_year;
		BeginTm.tm_mon = newtime->tm_mon;
		BeginTm.tm_mday = newtime->tm_mday;
		BeginTm.tm_hour = newtime->tm_hour;
		BeginTm.tm_min = 0;
		BeginTm.tm_sec = 0;
		// ��������ʱ���
		nextclock_timestump = (int)mktime( &BeginTm );
		nexclock_hour = newtime->tm_hour;
	}
	return 0;
}

// ÿ��ִ��30��,Ŀǰ������30֡�߼�
int process_logic()
{
	int tick = 0;
	g_speed++;

	// ÿִ֡�е��ӳٶ��в���
	for ( int tmpi = 0; tmpi < 16; tmpi++ )
	{
		if ( exec_queue_fetch() < 0 )
			break;
	}

	// ˢ���ܶ���
	brush_enemy_queue_fetch();

	// 1/10�������һ��mysql����
	if ( g_speed % 6 == 0 )
	{
		process_logic_wait_in();
	}

	// 1/6�������һ��д����
	if ( g_speed % 10 == 1 )
	{
		process_logic_wait_out();
	}

	// 10��һ�οͻ����߼�
	if ( g_speed % 300 == 0 )
	{
		client_logic();
	}

	// 5֡һ���߼�
	if ( g_speed % 5 == 0 )
	{
		actors_logic(); // ִ�����еĽ�ɫlogic
		map_logic(); // �����ͼ���߼�,��Ҫ��������
	}

	//	1����һ���߼�
	tick = g_speed % 30;
	if ( tick == 0 )
	{
		g_sec++;		// ����ɵ���������������������
		dbwork_fetchcomplete();
		globalcmd_fetch();
		robotcreate_queue_fetch(); // ����һ�������˲���
#ifndef WIN32
		linuxmsg_recv();
#endif
	}
	else if ( tick == 1 )
	{
		city_logic_sec( 0, 0 );
	}
	//else if ( tick == 2 )
	//{
	//	city_logic_sec();
	//}
	//else if ( tick == 3 )
	//{
	//	city_logic_sec();
	//}
	else if ( tick == 4 )
	{
		map_town_alllogic();
	}
	else if ( tick == 5 )
	{
		army_alllogic();
		armygroup_alllogic();
	}
	else if ( tick == 6 )
	{
		kingwar_activity_logic();
		nation_official_logic();
		map_call_logic();
		nation_hero_logic();
	}
	
	// 1����һ���߼�
	tick = g_speed % 1800;
	if ( tick == 0 )
	{
		wcount(); // ��¼����
		map_res_logic(); // ��Դ������ɾ�����
		sc_Script_Timer();
		process_exitlogic();
		process_oclock_check();
		weather_logic();
		time_gmcmd_logic();
		activity_logic();
	}
	else if ( tick == 10 )
	{
		
	}
	else if ( tick == 11 )
	{
		rank_reset();
		rank_server();
		paybag_logic();
	}

	if ( g_speed > 108000 )
	{
		tick = g_speed % 108000;
		if ( tick == 0 )
		{ // ÿСʱһ���߼�
			sc_Script_Hour();
		}
	}
	g_frame_count++;
	return 0;
}

int process_client( int client_index, char *tmpbuf, int readsize, int exec_code )
{
	short cmd;
	char *ptr;
	int tmpsize;
	int packsize;
	int allsize;
	char clientstat;

	//if( readsize < sizeof(short) )
	//	return -1;
	//ptr = tmpbuf; 
	//cmd = *((short *)ptr); ptr+=sizeof(short);
	//tmpsize = readsize - sizeof(short);

	ptr = tmpbuf;
	allsize = 0;

	g_last_cmd = -100;

	if ( client_getwait( client_index ) )
	{
		write_gamelog( "need wait:%d", client_index );
		return -1;
	}

	clientstat = client_getstat( client_index );
	while ( allsize < readsize )
	{
		packsize = *((short *)ptr); ptr += sizeof(short);
		if ( packsize <= 0 || packsize>1024 )
			break;
		cmd = *((short *)ptr); ptr += sizeof(short);
		tmpsize = packsize - sizeof(short);
		g_last_cmd = cmd;
		if ( client_index >= 0 && clientstat < CLIENT_STAT_LOGIN )
		{
			// �������δ��¼( �������Ե��õ�¼����ָ�� )
			if ( cmd != CMDC_LOGIN )
			{
				write_gamelog( "wrong command(no login):%d ip:%s", cmd, client_getip(client_index) );
				net_closesock_direct( client_index );
				return -1;
			}
		}
		else if ( clientstat == CLIENT_STAT_LOGIN )
		{
			// ����ҽ�����¼��δ������Ϸ(���Դ�����ɫ�������н�ɫ�����Խ�����Ϸ) ����ЩЭ��һ���ߵ�
			if ( cmd != CMDC_CREATE && cmd != CMDC_LIST && cmd != CMDC_DELETE && cmd != CMDC_ENTERGAME /*&& cmd != CMDC_HEARTBEAT*/ )
			{
				net_closesock_direct( client_index );
				return -1;
			}
		}
		else if ( clientstat == CLIENT_STAT_VER_ERR )
		{
			return -1;
		}
		if ( proc_command_S( client_index, cmd, ptr, tmpsize, exec_code ) < 0 )
		{
			if ( clientstat == CLIENT_STAT_LOGIN )
			{
				SClientInfo ClientInfo;
				char szUserID[21];
				client_getuserinfo( client_index, &ClientInfo );
				lltoa( ClientInfo.userid, szUserID, 10 );
				write_gamelog( "wrong command:%d userid:%s ip:%s", cmd, szUserID, client_getip(client_index) );
				net_closesock_direct( client_index );
			}
			else if ( clientstat == CLIENT_STAT_ENTERGAME )
			{
				write_gamelog( "wrong command:%d ip:%s", cmd, client_getip(client_index) );
				actor_remove( client_index );
			}
			else
			{
				write_gamelog( "wrong command:%d ip:%s", cmd, client_getip(client_index) );
				net_closesock_direct( client_index );
			}
			return -1;
		}
		ptr += tmpsize;
		allsize += packsize + sizeof(short);
	}
	return 0;
}

//-----------------------------------------------------------------------------
// process_logic_wait_in
// ����˵��: ��ɫ������Ϸ�Ķ���
//-----------------------------------------------------------------------------
int process_logic_wait_in()
{
	int client_index;
	short cmd;
	char tmpbuf[MAX_CMDPACKAGE_SIZE];
	int size;
	int exec_code;

	g_mysql_wait = 1;
	if ( fetch_wqueue( &client_index, &cmd, tmpbuf, &size, &exec_code, 0 ) == 0 )
	{
		//		if( cmd != WCMD_LEAVE && client_get_exec_code( client_index ) != exec_code )
#ifndef _USEGATE
		if ( client_get_exec_code( client_index ) != exec_code )
			return 0;
#endif
		// ��ʼ����һ��
		switch ( cmd )
		{
		case WCMD_ENTERGAME:
			return netrecv_entergame_S( client_index, tmpbuf, size );
			break;
		case WCMD_CREATE:
			return netrecv_create_S( client_index, tmpbuf, size );
			break;
		case WCMD_LIST:
			return netrecv_list_S( client_index, tmpbuf, size );
			break;
		}
	}
	// �����ǿյģ������κδ���
	return 0;
}

//-----------------------------------------------------------------------------
// process_logic_wait_out
// ����˵��: ��ɫ�˳���Ϸ���������ݵĶ���
//-----------------------------------------------------------------------------
int process_logic_wait_out()
{
	int client_index;
	short cmd;
	char tmpbuf[MAX_CMDPACKAGE_SIZE];
	int size;
	int exec_code;

	g_save_wait = 1;
	if ( fetch_wqueue( &client_index, &cmd, tmpbuf, &size, &exec_code, 1 ) == 0 )
	{
		// ��ʼ����һ��
		switch ( cmd )
		{
		case WCMD_LEAVE:
			return actor_remove( client_index );
			break;
		case WCMD_SAVE:
			return actor_real_saveall( client_index, NULL );
			break;
		}
	}

	// �����ǿյģ������κδ���
	return 0;
}

int process_dbreload()
{
	db_opendata();

	global_reload();
	awardgroup_reload();
	platinfo_reload_auto();
	itemkind_reload();

	bodytokeninfo_reload_auto();
	buildingresunlock_reload_auto();
	buildingupgrade_reload_auto();
	cityguardinfo_reload_auto();
	techinfo_reload_auto();
	equipinfo_reload_auto();
	equipwashinfo_reload_auto();
	equipwashrule_reload_auto();
	heroinfo_reload_auto();
	herocolorupinfo_reload_auto();
	herocorpsmulinfo_reload_auto();
	heroskillinfo_reload_auto();
	herovisit_reload_auto();
	mapenemyinfo_reload_auto();
	mapeventinfo_reload_auto();
	mapresinfo_reload_auto();
	maptowninfo_reload_auto();
	monsterinfo_reload_auto();
	paygoods_reload_auto();
	paystore_reload_auto();
	questinfo_reload_auto();
	questtalk_reload_auto();
	shop_reload_auto();
	storyinfo_reload_auto();
	upgradeinfo_reload_auto();
	vipbag_reload_auto();
	vipshop_reload_auto();
	worldbossinfo_reload_auto();
	worldquestinfo_reload_auto();
	girlinfo_reload_auto();
	girllove_reload_auto();
	girlson_reload_auto();
	girlshop_reload_auto();
	girlshopupdate_reload_auto();
	fangshinode_reload_auto();
	fangshipalace_reload_auto();
	robotai_reload_auto();
	robotbase_reload_auto();

	activityinfo02_reload_auto();
	activityinfo03_reload_auto();
	activityinfo04_reload_auto();
	activityinfo05_reload_auto();
	activityinfo06_reload_auto();
	activityinfo08_reload_auto();
	activityinfo10_reload_auto();

	db_closedata();
	return 0;
}

// ������������ʼ��
int process_serverinit()
{
	if ( world_data_getcache( WORLD_DATA_SERVERINIT ) > 0 )
		return -1;
	int nowstamp = (int)time( NULL );
	// ȫ������
	activity_set( ACTIVITY_6, nowstamp, nowstamp, nowstamp + 86400 * 7, nowstamp + 86400 * 14 );
	// �ؼ����
	activity_10_init();
	world_data_set( WORLD_DATA_SERVERINIT, (int)time(NULL), NULL, NULL );
	return 0;
}
