CC=g++
SRC = \
../AutoS/src/auto_city.cpp \
../AutoS/src/auto_data_official_tech.cpp \
../AutoS/src/global_netrecv_auto.cpp \
../AutoS/src/global_structsend_auto.cpp \
../AutoS/src/auto_actor_equip.cpp \
../AutoS/src/auto_data_platinfo.cpp \
../AutoS/src/auto_data_map_zoneinfo.cpp \
../AutoS/src/server_netrecv_auto.cpp \
../AutoS/src/auto_city_building.cpp \
../AutoS/src/auto_data_upgrade.cpp \
../AutoS/src/auto_map_town.cpp \
../AutoS/src/auto_data_official_forging.cpp \
../AutoS/src/auto_data_nation_equip.cpp \
../AutoS/src/auto_data_trainqueue.cpp \
../AutoS/src/auto_actor_item.cpp \
../AutoS/src/auto_army.cpp \
../AutoS/src/auto_data_quest.cpp \
../AutoS/src/auto_map_res.cpp \
../AutoS/src/auto_data_equip.cpp \
../AutoS/src/auto_data_equip_washrule.cpp \
../AutoS/src/auto_nation.cpp \
../AutoS/src/auto_actor.cpp \
../AutoS/src/global_process_auto.cpp \
../AutoS/src/server_process_auto.cpp \
../AutoS/src/auto_data_monster.cpp \
../AutoS/src/server_structrecv_auto.cpp \
../AutoS/src/auto_data_official_gov.cpp \
../AutoS/src/auto_data_paycountry.cpp \
../AutoS/src/auto_data_equip_wash.cpp \
../AutoS/src/auto_data_map_eventinfo.cpp \
../AutoS/src/auto_data_hero_corpsmul.cpp \
../AutoS/src/auto_map_enemy.cpp \
../AutoS/src/auto_map_event.cpp \
../AutoS/src/auto_data_building_upgrade.cpp \
../AutoS/src/auto_data_item.cpp \
../AutoS/src/auto_data_city_tech.cpp \
../AutoS/src/auto_city_building_res.cpp \
../AutoS/src/auto_data_material_make.cpp \
../AutoS/src/auto_actor_hero.cpp \
../AutoS/src/auto_army_group.cpp \
../AutoS/src/auto_data_city_guard.cpp \
../AutoS/src/server_structsend_auto.cpp \
../AutoS/src/auto_data_bodytoken.cpp \
../AutoS/src/auto_city_attr.cpp \
../AutoS/src/auto_data_hero_skill.cpp \
../AutoS/src/auto_data_trainlong.cpp \
../AutoS/src/auto_data_map_enemyinfo.cpp \
../AutoS/src/auto_map_zone.cpp \
../AutoS/src/auto_data_story.cpp \
../AutoS/src/auto_city_building_barracks.cpp \
../AutoS/src/auto_data_weather.cpp \
../AutoS/src/auto_data_map_towninfo.cpp \
../AutoS/src/global_netsend_auto.cpp \
../AutoS/src/auto_data_map_resinfo.cpp \
../AutoS/src/global_structrecv_auto.cpp \
../AutoS/src/auto_data_hero.cpp \
../AutoS/src/auto_data_hero_colorup.cpp \
../AutoS/src/server_netsend_auto.cpp \
../AutoS/src/auto_data_vip.cpp \
../AutoS/src/auto_data_paygoods.cpp \
../AutoS/src/auto_city_guard.cpp \
../AutoS/src/auto_data_fight.cpp \
login_user.cpp \
army_march.cpp \
fight.cpp \
global_cmdqueue.cpp \
vip.cpp \
actor_gmcmd.cpp \
client.cpp \
story.cpp \
system_askinfo.cpp \
script_auto.cpp \
award.cpp \
chat.cpp \
nation.cpp \
db.cpp \
script.cpp \
army_fight.cpp \
building.cpp \
actor.cpp \
dbredis.cpp \
actor_func.cpp \
item.cpp \
actor_send.cpp \
map_event.cpp \
timegmcmd.cpp \
map_enemy.cpp \
login_net.cpp \
activity.cpp \
map.cpp \
city.cpp \
mapunit.cpp \
global_netprocess.cpp \
army_group.cpp \
city_tech.cpp \
system.cpp \
item_send.cpp \
wqueue.cpp \
actor_times.cpp \
army.cpp \
account.cpp \
map_res.cpp \
script_reg_auto.cpp \
equip.cpp \
city_attr.cpp \
army_mail.cpp \
map_town.cpp \
global.cpp \
quest.cpp \
area.cpp \
mem_dump.cpp \
gameproc.cpp \
gamelog.cpp \
main.cpp \
item_proc.cpp \
gmcmd.cpp \
mail.cpp \
actor_notify.cpp \
timecalc.cpp \
hero.cpp \


INCLUDE = -I ../include  -I ../AutoS/include -I  ../../LKUtils/include  -I ../../LKNetServer/include  -I ../../Lua/src  -I /usr/include/mysql  -I ../../hiredis_linux
LIBRARY = -L ../../../public/lib -L /usr/lib/mysql
OBJ = $(SRC:.cpp=.o)
LIB = -llua -llknet -llkutils -lmysqlclient -lhiredis -lpthread -ldl -lm
EXE = ../../../exec/sgserver
CFLAGS = -g
$(EXE) : $(OBJ)
	$(CC)  $(CFLAGS) $(INCLUDE) $(LIBRARY) -o $(EXE) $(CF) $(OBJ) $(LIB)


../AutoS/src/auto_city.o: ../AutoS/src/auto_city.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_official_tech.o: ../AutoS/src/auto_data_official_tech.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/global_netrecv_auto.o: ../AutoS/src/global_netrecv_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/global_structsend_auto.o: ../AutoS/src/global_structsend_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_actor_equip.o: ../AutoS/src/auto_actor_equip.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_platinfo.o: ../AutoS/src/auto_data_platinfo.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_map_zoneinfo.o: ../AutoS/src/auto_data_map_zoneinfo.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/server_netrecv_auto.o: ../AutoS/src/server_netrecv_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_city_building.o: ../AutoS/src/auto_city_building.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_upgrade.o: ../AutoS/src/auto_data_upgrade.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_map_town.o: ../AutoS/src/auto_map_town.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_official_forging.o: ../AutoS/src/auto_data_official_forging.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_nation_equip.o: ../AutoS/src/auto_data_nation_equip.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_trainqueue.o: ../AutoS/src/auto_data_trainqueue.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_actor_item.o: ../AutoS/src/auto_actor_item.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_army.o: ../AutoS/src/auto_army.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_quest.o: ../AutoS/src/auto_data_quest.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_map_res.o: ../AutoS/src/auto_map_res.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_equip.o: ../AutoS/src/auto_data_equip.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_equip_washrule.o: ../AutoS/src/auto_data_equip_washrule.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_nation.o: ../AutoS/src/auto_nation.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_actor.o: ../AutoS/src/auto_actor.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/global_process_auto.o: ../AutoS/src/global_process_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/server_process_auto.o: ../AutoS/src/server_process_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_monster.o: ../AutoS/src/auto_data_monster.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/server_structrecv_auto.o: ../AutoS/src/server_structrecv_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_official_gov.o: ../AutoS/src/auto_data_official_gov.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_paycountry.o: ../AutoS/src/auto_data_paycountry.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_equip_wash.o: ../AutoS/src/auto_data_equip_wash.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_map_eventinfo.o: ../AutoS/src/auto_data_map_eventinfo.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_hero_corpsmul.o: ../AutoS/src/auto_data_hero_corpsmul.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_map_enemy.o: ../AutoS/src/auto_map_enemy.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_map_event.o: ../AutoS/src/auto_map_event.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_building_upgrade.o: ../AutoS/src/auto_data_building_upgrade.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_item.o: ../AutoS/src/auto_data_item.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_city_tech.o: ../AutoS/src/auto_data_city_tech.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_city_building_res.o: ../AutoS/src/auto_city_building_res.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_material_make.o: ../AutoS/src/auto_data_material_make.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_actor_hero.o: ../AutoS/src/auto_actor_hero.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_army_group.o: ../AutoS/src/auto_army_group.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_city_guard.o: ../AutoS/src/auto_data_city_guard.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/server_structsend_auto.o: ../AutoS/src/server_structsend_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_bodytoken.o: ../AutoS/src/auto_data_bodytoken.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_city_attr.o: ../AutoS/src/auto_city_attr.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_hero_skill.o: ../AutoS/src/auto_data_hero_skill.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_trainlong.o: ../AutoS/src/auto_data_trainlong.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_map_enemyinfo.o: ../AutoS/src/auto_data_map_enemyinfo.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_map_zone.o: ../AutoS/src/auto_map_zone.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_story.o: ../AutoS/src/auto_data_story.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_city_building_barracks.o: ../AutoS/src/auto_city_building_barracks.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_weather.o: ../AutoS/src/auto_data_weather.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_map_towninfo.o: ../AutoS/src/auto_data_map_towninfo.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/global_netsend_auto.o: ../AutoS/src/global_netsend_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_map_resinfo.o: ../AutoS/src/auto_data_map_resinfo.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/global_structrecv_auto.o: ../AutoS/src/global_structrecv_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_hero.o: ../AutoS/src/auto_data_hero.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_hero_colorup.o: ../AutoS/src/auto_data_hero_colorup.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/server_netsend_auto.o: ../AutoS/src/server_netsend_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_vip.o: ../AutoS/src/auto_data_vip.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_paygoods.o: ../AutoS/src/auto_data_paygoods.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_city_guard.o: ../AutoS/src/auto_city_guard.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_data_fight.o: ../AutoS/src/auto_data_fight.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
login_user.o: login_user.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
army_march.o: army_march.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
fight.o: fight.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
global_cmdqueue.o: global_cmdqueue.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
vip.o: vip.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
actor_gmcmd.o: actor_gmcmd.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
client.o: client.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
story.o: story.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
system_askinfo.o: system_askinfo.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
script_auto.o: script_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
award.o: award.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
chat.o: chat.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
nation.o: nation.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
db.o: db.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
script.o: script.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
army_fight.o: army_fight.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
building.o: building.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
actor.o: actor.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
dbredis.o: dbredis.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
actor_func.o: actor_func.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
item.o: item.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
actor_send.o: actor_send.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
map_event.o: map_event.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
timegmcmd.o: timegmcmd.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
map_enemy.o: map_enemy.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
login_net.o: login_net.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
activity.o: activity.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
map.o: map.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
city.o: city.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
mapunit.o: mapunit.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
global_netprocess.o: global_netprocess.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
army_group.o: army_group.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
city_tech.o: city_tech.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
system.o: system.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
item_send.o: item_send.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
wqueue.o: wqueue.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
actor_times.o: actor_times.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
army.o: army.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
account.o: account.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
map_res.o: map_res.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
script_reg_auto.o: script_reg_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
equip.o: equip.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
city_attr.o: city_attr.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
army_mail.o: army_mail.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
map_town.o: map_town.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
global.o: global.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
quest.o: quest.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
area.o: area.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
mem_dump.o: mem_dump.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
gameproc.o: gameproc.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
gamelog.o: gamelog.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
main.o: main.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
item_proc.o: item_proc.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
gmcmd.o: gmcmd.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
mail.o: mail.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
actor_notify.o: actor_notify.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
timecalc.o: timecalc.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
hero.o: hero.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<


all: $(EXE)
clean :
	rm -f *.o
	rm -f ../AutoS/src/*.o
	rm -f $(EXE)
