//--------------------------------------------------------------------
// �ļ���:		IEntScript.h
// ��  ��:		
// ˵  ��:		
// ��������:	2007��2��9��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_IENTSCRIPT_H
#define _PUBLIC_IENTSCRIPT_H

#include "Macros.h"
#include "IVarList.h"

// IEntScript
// ʵ��ű�

class ILogic;

class IEntScript
{
public:
	virtual ~IEntScript() = 0;

	// ��ýű�����
	virtual const char* GetName() const = 0;
	// ����߼���
	virtual ILogic* GetLogic() const = 0;
	// ��ӻص�
	virtual bool AddCallback(const char* event, const char* func) = 0;
	// ɾ���ص�
	virtual bool RemoveCallback(const char* event) = 0;
	// ��ջص�
	virtual void ClearCallback() = 0;
	// ��ûص�����
	virtual size_t GetCallbackCount() const = 0;
	// ��ûص��¼��б�
	virtual size_t GetCallbackList(IVarList& result) const = 0;
	// ��ûص���Ӧ�Ľű�����
	virtual const char* GetCallbackFunc(const char* event) const = 0;
};

inline IEntScript::~IEntScript() {}

#endif // _PUBLIC_IENTSCRIPT_H

