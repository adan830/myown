//--------------------------------------------------------------------
// �ļ���:		GMModule.h
// ��  ��:		GM����
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __GMModule_H__
#define __GMModule_H__

#include "Fsgame/Define/header.h"
#include <string>
#include <unordered_map>

#define GMLEVEL_MAX 51

class GMModule : public ILogicModule
{
public:
	// ��ʼ��
	virtual bool Init( IKernel* pKernel );

	// �ͷ�
	virtual bool Shut( IKernel* pKernel );
	
	// ��ȡGMȨ�޶����ļ�
	bool LoadGMFile( IKernel* pKernel );

private:
	// ִ��GM����
	bool DoGMCommand( IKernel* pKernel, const PERSISTID& self, const wchar_t* content );

	// �ж�GM�Ƿ���Ȩ��ִ��ĳ������
	bool CanExecute( IKernel* pKernel, const PERSISTID& self, const char* command );

	// GM������Ӧ����
	static int OnCustomGM( IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args );

	// ִ��gm����
	bool RunGmCommand(IKernel* pKernel, const PERSISTID& self, const char* cmd, const IVarList& args, int nOffset, int nCount, std::string& outMsg);

	// gm�����ض�
	static void ReloadConfig(IKernel *pKernel);

	// GM��������ƣ�map<GM�����֣�����>��
	std::unordered_map< std::string, int > m_mapGMLimit;
	
	int m_nGMDefaultRight;
public:
	friend class GmccModule;
	static GMModule* m_pGMModule;

};

#endif	// __GMModule_H__