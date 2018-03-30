//--------------------------------------------------------------------
// �ļ���:		IGameObj.h
// ��  ��:		
// ˵  ��:		
// ��������:	2011��2��22��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_IGAMEOBJ_H
#define _SERVER_IGAMEOBJ_H

#include "../public/Macros.h"
#include "../public/PersistId.h"
#include "LenType.h"
#include "IRecord.h"

// ��Ϸ����ӿ�

class IGameObj
{
public:
	virtual ~IGameObj() = 0;

	/// \brief ��ö�������
	/// \return �������ͣ���KnlConst.h��
	virtual int GetClassType() = 0;
	/// \brief ��ö����
	virtual PERSISTID GetObjectId() = 0;
	/// \brief ȡ����ű���
	virtual const char* GetScript() = 0;
	/// \brief ȡ����������
	virtual const char* GetConfig() = 0;
	/// \brief �������
	virtual const wchar_t* GetName() = 0;
	/// \brief ������
	virtual int GetGroupId() = 0;
	/// \brief ȡ�����������е�λ��
	/// \return ��1��ʼ
	virtual int GetIndex() = 0;
	/// \brief ȡ������
	virtual IGameObj* GetParent() = 0;
	/// \brief ��������������
	virtual int GetCapacity() = 0;
	/// \brief �����Ӷ�������
	virtual int GetChildCount() = 0;
	/// \brief ȡ����ָ��λ�õ��Ӷ���
	/// \param index �����е�λ�ã���1��ʼ��
	virtual IGameObj* GetChildByIndex(int index) = 0;
	/// \brief �����ֲ����Ӷ���
	/// \param name �Ӷ��������
	virtual IGameObj* GetChild(const wchar_t* name) = 0;
	/// \brief ��ñ��������Ĵ���
	virtual int GetWeakRefs() = 0;
	
	/// \brief ��������X
	virtual len_t GetPosiX() = 0;
	/// \brief ��������Y
	virtual len_t GetPosiY() = 0;
	/// \brief ��������Z
	virtual len_t GetPosiZ() = 0;
	/// \brief ������
	virtual len_t GetOrient() = 0;
	
	/// \brief �����Ƿ���һ������
	/// \param name ������
	virtual bool FindAttr(const char* name) = 0;
	/// \brief �����������
	/// \param name ������
	virtual int GetAttrType(const char* name) = 0;
	/// \brief �����������
	virtual int GetAttrCount() = 0;
	/// \brief ������������б�
	/// \param result ����������б�
	virtual int GetAttrList(IVarList& result) = 0;
	/// \brief ��������ֵ
	/// \param name ������
	/// \param value ����ֵ
	virtual bool SetInt(const char* name, int value) = 0;
	virtual bool SetInt64(const char* name, int64_t value) = 0;
	virtual bool SetFloat(const char* name, float value) = 0;
	virtual bool SetDouble(const char* name, double value) = 0;
	virtual bool SetString(const char* name, const char* value) = 0;
	virtual bool SetWideStr(const char* name, const wchar_t* value) = 0;
	virtual bool SetObject(const char* name, const PERSISTID& value) = 0;
	/// \brief ��ѯ����
	/// \param name ������
	virtual int QueryInt(const char* name) = 0;
	virtual int64_t QueryInt64(const char* name) = 0;
	virtual float QueryFloat(const char* name) = 0;
	virtual double QueryDouble(const char* name) = 0;
	virtual const char* QueryString(const char* name) = 0;
	virtual const wchar_t* QueryWideStr(const char* name) = 0;
	virtual PERSISTID QueryObject(const char* name) = 0;

	/// \brief ������Ե�����ֵ������-1��ʾδ�ҵ���
	/// \param name ������
	virtual int GetAttrIndex(const char* name) = 0;
	/// \brief ͨ������ֵ��������ֵ
	/// \param index ��������
	/// \param value ����ֵ
	virtual bool SetIntByIndex(int index, int value) = 0;
	virtual bool SetInt64ByIndex(int index, int64_t value) = 0;
	virtual bool SetFloatByIndex(int index, float value) = 0;
	virtual bool SetDoubleByIndex(int index, double value) = 0;
	virtual bool SetStringByIndex(int index, const char* value) = 0;
	virtual bool SetWideStrByIndex(int index, const wchar_t* value) = 0;
	virtual bool SetObjectByIndex(int index, const PERSISTID& value) = 0;
	/// \brief ͨ������ֵ��ѯ����
	/// \param index ��������
	virtual int QueryIntByIndex(int index) = 0;
	virtual int64_t QueryInt64ByIndex(int index) = 0;
	virtual float QueryFloatByIndex(int index) = 0;
	virtual double QueryDoubleByIndex(int index) = 0;
	virtual const char* QueryStringByIndex(int index) = 0;
	virtual const wchar_t* QueryWideStrByIndex(int index) = 0;
	virtual PERSISTID QueryObjectByIndex(int index) = 0;

	/// \brief ��������Ƿ����
	/// \param name ������
	virtual bool GetAttrVisible(const char* name) = 0;
	/// \brief ��������Ƿ񹫹�����
	/// \param name ������
	virtual bool GetAttrPublicVisible(const char* name) = 0;
	/// \brief ��ÿ��������Ƿ�ʱˢ��
	/// \param obj ����
	/// \param name ������
	virtual bool GetAttrRealtime(const char* name) = 0;
	/// \brief ��������Ƿ񱣴�
	/// \param obj ����
	/// \param name ������
	virtual bool GetAttrSaving(const char* name) = 0;
	/// \brief ���ÿ��������Ƿ�����
	/// \param name ������
	/// \param value �Ƿ�����
	virtual bool SetAttrHide(const char* name, bool value) = 0;
	/// \brief ��ÿ��������Ƿ�����
	/// \param name ������
	virtual bool GetAttrHide(const char* name) = 0;


	// ������ʵ��λ��־������Ŀ���ǽ�Լ�洢���������ͱ���Ϊ������
	/// \brief ���ñ�־
	/// \param name ������
	/// \param pos ��־λ��0-31��
	virtual bool SetAttrFlag(const char* name, int pos) = 0;
	/// \brief �����־
	/// \param name ������
	/// \param pos ��־λ��0-31��
	virtual bool ClearAttrFlag(const char* name, int pos) = 0;
	/// \brief ���Ա�־
	/// \param obj ����
	/// \param name ������
	/// \param pos ��־λ��0-31��
	virtual bool TestAttrFlag(const char* name, int pos) = 0;

	

	/// \brief ��ñ�����
	virtual int GetRecordCount() = 0;
	/// \brief ��ñ������б�
	/// \param result ���������б�
	virtual int GetRecordList(IVarList& result) = 0;
	/// \brief ���ָ�������ı����ʽӿ�
	/// \param index �������
	virtual IRecord* GetRecordByIndex(int index) = 0;
	/// \brief ��ö�������ʽӿ�
	/// \param name �����
	virtual IRecord* GetRecord(const char* name) = 0;
	/// \brief ��ñ�������ֵ������-1��ʾδ�ҵ���
	/// \param name �����
	virtual int GetRecordIndex(const char* name) = 0;
	
	/// \brief ������ʱ����
	/// \param name ������
	virtual bool FindData(const char* name) = 0;
	/// \brief �����ʱ��������
	virtual int GetDataCount() = 0;
	/// \brief �����ʱ���������б�
	/// \param result �ṹ�������б�
	virtual int GetDataList(IVarList& result) = 0;
	/// \brief ������ʱ����
	/// \param name ������
	/// \param value ��ֵ
	virtual bool AddDataInt(const char* name, int value) = 0;
	virtual bool AddDataInt64(const char* name, int64_t value) = 0;
	virtual bool AddDataFloat(const char* name, float value) = 0;
	virtual bool AddDataDouble(const char* name, double value) = 0;
	virtual bool AddDataString(const char* name, const char* value) = 0;
	virtual bool AddDataWideStr(const char* name, const wchar_t* value) = 0;
	virtual bool AddDataObject(const char* name, const PERSISTID& value) = 0;
	virtual bool AddDataBinary(const char* name, const void* pdata, int len) = 0;
	/// \brief ɾ����ʱ����
	/// \param name ������
	virtual bool RemoveData(const char* name) = 0;
	/// \brief �����ʱ��������
	/// \param name ������
	virtual int GetDataType(const char* name) = 0;
	/// \brief ������ʱ����
	/// \param name ������
	/// \param value ��ֵ
	virtual bool SetDataInt(const char* name, int value) = 0;
	virtual bool SetDataInt64(const char* name, int64_t value) = 0;
	virtual bool SetDataFloat(const char* name, float value) = 0;
	virtual bool SetDataDouble(const char* name, double value) = 0;
	virtual bool SetDataString(const char* name, const char* value) = 0;
	virtual bool SetDataWideStr(const char* name, const wchar_t* value) = 0;
	virtual bool SetDataObject(const char* name, const PERSISTID& value) = 0;
	virtual bool SetDataBinary(const char* name, const void* pdata, int len) = 0;
	/// \brief ��ѯ��ʱ����
	/// \param name ������
	virtual int QueryDataInt(const char* name) = 0;
	virtual int64_t QueryDataInt64(const char* name) = 0;
	virtual float QueryDataFloat(const char* name) = 0;
	virtual double QueryDataDouble(const char* name) = 0;
	virtual const char* QueryDataString(const char* name) = 0;
	virtual const wchar_t* QueryDataWideStr(const char* name) = 0;
	virtual PERSISTID QueryDataObject(const char* name) = 0;
	virtual void* QueryDataBinary(const char* name, int* len = NULL) = 0;
	
	/// \brief ��ñ��Ƿ����
	/// \param obj ����
	/// \param name �����
	virtual bool GetRecordVisible(const char* name) = 0;
	/// \brief ��ñ��Ƿ񹫹�����
	/// \param obj ����
	/// \param name �����
	virtual bool GetRecordPublicVisible(const char* name) = 0;
	/// \brief ��ñ��Ƿ񱣴�
	/// \param obj ����
	/// \param name �����
	virtual bool GetRecordSaving(const char* name) = 0;

	// �ñ�ʵ��λ��־������Ŀ���ǽ�Լ�洢�������Ϊ���У�������Ϊ������
	/// \brief ���ñ�־
	/// \param obj ����
	/// \param name �����
	/// \param pos ��־λ��ţ��кų�32��0-31��
	virtual bool SetRecordFlag(const char* name, int pos) = 0;
	/// \brief �����־
	/// \param obj ����
	/// \param name �����
	/// \param pos ��־λ��ţ��кų�32��0-31��
	virtual bool ClearRecordFlag(const char* name, int pos) = 0;
	/// \brief ���Ա�־
	/// \param obj ����
	/// \param name �����
	/// \param pos ��־λ��ţ��кų�32��0-31��
	virtual bool TestRecordFlag(const char* name, int pos) = 0;



	/// \brief ��ҵ��Ӵ��Ƿ����
	/// \param id �������
	virtual bool FindViewport(int id) = 0;

	/// \brief ��õ�ǰ�ƶ�ģʽ
	virtual int GetMoveMode() = 0;
};

inline IGameObj::~IGameObj() {}

#endif // _SERVER_IGAMEOBJ_H
