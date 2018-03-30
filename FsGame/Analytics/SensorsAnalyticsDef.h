//--------------------------------------------------------------------
// �ļ���:	SensorsAnalyticsDef.h
// ��  ��:	������ݷ����ռ�������궨��
// ˵  ��:	
// ��������:	2016��08��25��
// ������:	���
// ������:            
//--------------------------------------------------------------------
#ifndef __SENSORS_ANALYTICS_DEFINED_H__
#define __SENSORS_ANALYTICS_DEFINED_H__

#include "log4cplus/loggingmacros.h"

typedef log4cplus::Logger IAnalytisLogger;
#define ANALYTICS_LOG( logger, logEvent ) LOG4CPLUS_INFO( (logger), (logEvent) )


#define SENSORS_INNER_DISTINCT_ID	"distinct_id"
#define SENSORS_INNER_ORIGINAL_ID	"original_id"
#define SENSORS_INNER_TIME		"time"
#define SENSORS_INNER_TYPE		"type"
#define SENSORS_INNER_EVENT		"event"
#define SENSORS_INNER_PROJECT	"project"
#define SENSORS_INNER_PROPERTYIES	"properties"

#define SENSORS_EVENT_TYPE_TRACK			"track"
#define SENSORS_EVENT_TYPE_TRACK_SIGNUP		"track_signup"
#define SENSORS_EVENT_TYPE_PROFILE_SET		"profile_set"
#define SENSORS_EVENT_TYPE_PROFILE_SET_ONCE	"profile_set_once"
#define SENSORS_EVENT_TYPE_PROFILE_INC		"profile_increment"
#define SENSORS_EVENT_TYPE_PROFILE_DELETE	"profile_delete"
#define SENSORS_EVENT_TYPE_PROFILE_APPEND	"profile_append"
#define SENSORS_EVENT_TYPE_PROFILE_UNSET	"profile_unset"
#define SENSORS_EVENT_NAME_SIGNUP			"$SignUp"

#define SENSORS_PROPERTY_MANUFACTURER	"$manufacturer"
#define SENSORS_PROPERTY_MODEL			"$model"
#define SENSORS_PROPERTY_OS				"$os"
#define SENSORS_PROPERTY_OS_VER			"$os_version"
#define SENSORS_PROPERTY_APP_VER		"$app_version"
#define SENSORS_PROPERTY_WIFI			"$wifi"
#define SENSORS_PROPERTY_IP				"$ip"
#define SENSORS_PROPERTY_PROVINCE		"$province"
#define SENSORS_PROPERTY_CITY			"$city"
#define SENSORS_PROPERTY_SCREEN_WIDH	"$screen_width"
#define SENSORS_PROPERTY_SCREEN_HEIGHT	"$screen_heigh"


#define SENSORS_PROPERTY_NAME			"$name"
#define SENSORS_PROPERTY_SIGNUP_TIME	"$signup_time"

#define SENSORS_PROPERTY_EXTRA			"extra"

#endif