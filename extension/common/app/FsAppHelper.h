//--------------------------------------------------------------------
// �ļ���:		FsAppHelper.h
// ��  ��:		Ӧ�ó�������
// ˵  ��:		
// ��������:		2013��5��18��
// ������:		���
//--------------------------------------------------------------------
#ifndef __FS_APP_HELPER_H__
#define __FS_APP_HELPER_H__

#ifdef __cplusplus
namespace FsFramework
{
	namespace app
	{
		extern "C"
		{
#endif 
			typedef void(*CrashingHook)();
			typedef void(*CrashedHook)();

			typedef bool(*ThreadHook)(bool bTerminate);

			void SetProcessExceptionHandlers();
			void UnSetProcessExceptionHandlers();

			void EventThreadStart();
			void EventThreadStop();
			ThreadHook SetThreadHook( ThreadHook cbHook );

			void DisableUnHandledExceptionFilter();
			void EnableUnHandlerExceptionFliter();

			void SetCrashFile( const char* pszFileName );
			const char* GetCrashFile();
			CrashingHook SetCrahingHook(CrashingHook cbHook);
			CrashedHook SetCrashedHook(CrashedHook cbHook);
#ifdef __cplusplus
		}
	}
}
#endif 

#endif // END __FS_APP_HELPER_H__