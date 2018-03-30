//--------------------------------------------------------------------
// �ļ���:		CoreConfig.h
// ��  ��:		
// ˵  ��:		
// ��������:	2008��5��14��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_CORECONFIG_H
#define _PUBLIC_CORECONFIG_H

#include "FastStr.h"

// ������������

struct CCoreConfig
{
	// ������
	void* m_hInstance;
	// �����ھ��
	void* m_hWinHandle;
	// ���̶߳�ջ��ַ
	size_t m_nStackAddr;
	// ���̶߳�ջ��С
	size_t m_nStackSize;
	// ��ѭ������ʱ��
	int m_nSleep;
	// �Ƿ�ʹ�õ�����־ϵͳ
	bool m_bDebugLog;
	// �Ƿ�ʱ���¼��ؽű�
	bool m_bScriptReload;
	// �Ƿ�ʹ���ڴ��
	bool m_bUseMemoryPool;
	// ʹ��ר�õ��ڴ������
	bool m_bUseExternMalloc;
	// ʹ���ڴ���֤
	bool m_bMemoryValidate;
	// ����·��
	TFastStr<char, 128> m_strWorkPath;
	// �ű�·��
	TFastStr<char, 128> m_strScriptPath;
	// ��Դ�ļ�·��
	TFastStr<char, 128> m_strResourcePath;
	// �ļ�ϵͳ
	TFastStr<char, 32> m_strFileSystem;
	// �ļ�ϵͳ������Ϣ
	TFastStr<char, 32> m_strFileConfig;
	// ��ʵ������
	TFastStr<char, 32> m_strMainEntity;
	// �����ű�
	TFastStr<char, 32> m_strMainScript;
	// �����߼���
	TFastStr<char, 32> m_strMainLogic;
	// �������ļ���
	TFastStr<char, 32> m_strMainConfig;
	// Ҫ���ص�ģ��DLL�б�
	TFastStr<char, 128> m_strLoadDlls;

	CCoreConfig()
	{
		m_hInstance = NULL;
		m_hWinHandle = NULL;
		m_nStackAddr = 0;
		m_nStackSize = 0;
		m_nSleep = 0;
		m_bDebugLog = false;
		m_bScriptReload = false;
		m_bUseMemoryPool = false;
		m_bUseExternMalloc = false;
		m_bMemoryValidate = false;
	}
};

#endif // _PUBLIC_CORECONFIG_H

