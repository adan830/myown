//----------------------------------------------------------
// �ļ���:      DeclareProperty.h
// ��  ��:      ���Խӿ�����
// ˵  ��:      ͨ���궨�壬�Զ�������Ե�Get��Set�ӿڣ���ǿ���Եķ�װ����
// ������:      hehui
// ��������:    2012��12��24��
//    :       
//----------------------------------------------------------
#ifndef FSGAME_INTERFACE_DECLARE_PROPERTY_H_
#define FSGAME_INTERFACE_DECLARE_PROPERTY_H_

#include "Fsgame/Define/header.h"
#include "utils/extend_func.h"


/** INT�����Խӿ� */
#define DECLARE_PROPERTY_INT_VARIANT(name, get_func, set_func) \
    public: \
    bool get_func(IGameObj* pGameObj, int& value) { \
		if (NULL == pGameObj)						\
			return false;							\
        if (!pGameObj->FindAttr(#name))                 \
            return false;                              \
        value = pGameObj->QueryInt(#name);              \
        return true;                                   \
    } \
    bool set_func(IGameObj* pGameObj, const int value) {  \
		if (NULL == pGameObj)						\
			return false;							\
        if (!pGameObj->FindAttr(#name))                 \
            return false;                                  \
        PERSISTID npc = pGameObj->GetObjectId();\
        ::extend_warning(LOG_TRACE, "%d-%d [PropertySet]:%s = %d", \
                         npc.nIdent, npc.nSerial, #name, value);\
        pGameObj->SetInt(#name, value);                 \
        return true; \
    }

// Ĭ�ϲ���
#define DECLARE_PROPERTY_INT(name) DECLARE_PROPERTY_INT_VARIANT(name, Get##name, Set##name)
///////////////////////////////////////////////////////////////////////////

/** float�����Խӿ� */
#define DECLARE_PROPERTY_FLOAT_VARIANT(name, get_func, set_func) \
    public: \
    bool get_func(IGameObj* pGameObj, float& value) { \
		if (NULL == pGameObj)						\
			return false;							\
        if (!pGameObj->FindAttr(#name))                       \
            return false;                                         \
        value = pGameObj->QueryFloat(#name);                 \
        return true; \
    } \
    bool set_func(IGameObj* pGameObj, const float value) { \
		if (NULL == pGameObj)						\
			return false;							\
        if (!pGameObj->FindAttr(#name)) \
            return false; \
        PERSISTID npc = pGameObj->GetObjectId();\
        ::extend_warning(LOG_TRACE, "%d-%d [PropertySet]:%s = %f", \
                         npc.nIdent, npc.nSerial, #name, value);\
        pGameObj->SetFloat(#name, value); \
        return true; \
    }

// Ĭ�ϲ���
#define DECLARE_PROPERTY_FLOAT(name) DECLARE_PROPERTY_FLOAT_VARIANT(name, Get##name, Set##name)
///////////////////////////////////////////////////////////////////////////

/** char�����Խӿ� */
#define DECLARE_PROPERTY_STRING_VARIANT(name, get_func, set_func) \
    public: \
    bool get_func(IGameObj* pGameObj, std::string& value) { \
		if (NULL == pGameObj)							\
			return false;								\
        if (!pGameObj->FindAttr(#name))                       \
            return false;                                         \
        value = pGameObj->QueryString(#name);                 \
        return true; \
    } \
    bool set_func(IGameObj* pGameObj, const char* value) { \
		if (NULL == pGameObj)						\
			return false;							\
        if (!pGameObj->FindAttr(#name)) \
            return false; \
        PERSISTID npc = pGameObj->GetObjectId();\
        ::extend_warning(LOG_TRACE, "%d-%d [PropertySet]:%s = %s", \
                         npc.nIdent, npc.nSerial, #name, value);\
        pGameObj->SetString(#name, value); \
        return true; \
    }

// Ĭ�ϲ���
#define DECLARE_PROPERTY_STRING(name) DECLARE_PROPERTY_STRING_VARIANT(name, Get##name, Set##name)
///////////////////////////////////////////////////////////////////////////

/** object�������Խӿ� */
#define DECLARE_PROPERTY_OBJECT_VARIANT(name, get_func, set_func) \
    public: \
    bool get_func(IGameObj* pGameObj, PERSISTID& value) { \
		if (NULL == pGameObj)							\
			return false;								\
        if (!pGameObj->FindAttr(#name))                       \
            return false;                                         \
        value = pGameObj->QueryObject(#name);                 \
        return true; \
    } \
    bool set_func(IGameObj* pGameObj, const PERSISTID& value) { \
		if (NULL == pGameObj)						\
			return false;							\
        if (!pGameObj->FindAttr(#name)) \
            return false; \
        PERSISTID npc = pGameObj->GetObjectId();\
        ::extend_warning(LOG_TRACE, "%d-%d [PropertySet]:%s = %d-%d", \
                         npc.nIdent, npc.nSerial, #name, value.nIdent, value.nSerial);\
        pGameObj->SetObject(#name, value); \
        return true; \
    }

// Ĭ�ϲ���
#define DECLARE_PROPERTY_OBJECT(name) DECLARE_PROPERTY_OBJECT_VARIANT(name, Get##name, Set##name)
///////////////////////////////////////////////////////////////////////////

/** INT64�����Խӿ� */
#define DECLARE_PROPERTY_INT64_VARIANT(name, get_func, set_func) \
	public: \
	bool get_func(IGameObj* pGameObj, int64_t& value) { \
	if (NULL == pGameObj)						\
		return false;							\
	if (!pGameObj->FindAttr(#name))                 \
	return false;                              \
	value = pGameObj->QueryInt64(#name);              \
	return true;                                   \
	} \
	bool set_func(IGameObj* pGameObj, const int64_t value) {  \
	if (NULL == pGameObj)						\
	return false;								\
	if (!pGameObj->FindAttr(#name))                 \
	return false;                                  \
	PERSISTID npc = pGameObj->GetObjectId();\
	::extend_warning(LOG_TRACE, "%d-%d [PropertySet]:%s = %d", \
	npc.nIdent, npc.nSerial, #name, value);\
	pGameObj->SetInt64(#name, value);                 \
	return true; \
	}

// Ĭ�ϲ���
#define DECLARE_PROPERTY_INT64(name) DECLARE_PROPERTY_INT64_VARIANT(name, Get##name, Set##name)

#endif // FSGAME_INTERFACE_DECLARE_PROPERTY_H_
