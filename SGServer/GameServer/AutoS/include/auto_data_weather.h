#ifndef __WEATHER_DATA_AUTO_H
#define __WEATHER_DATA_AUTO_H
#include "define.h"

struct _weatherinfo_config {
//--Automatically generated
				short day;	//
				short weather;	//
				short ability;	//
				int value;	//
				short nextday;	//
};
typedef struct _weatherinfo_config WeatherInfoConfig;

struct _weatherinfo {
//--Automatically generated
	WeatherInfoConfig *config;
	int	maxnum;
};
typedef struct _weatherinfo WeatherInfo;

int weatherinfo_init_auto();
int weatherinfo_reload_auto();

#endif
