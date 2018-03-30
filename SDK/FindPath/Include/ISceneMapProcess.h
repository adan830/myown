/************************************************************************/
/*                             ������ӿ�                               */
/************************************************************************/
#ifndef __ISceneMapProcess_h__
#define __ISceneMapProcess_h__

// ��Կͻ��˻����˵ĵ�ǰ������һЩ����
class ISceneMapProcess
{
public:
    /// \brief  ����ָ�����Ƿ�Ϸ�
	virtual bool TryPoint(float x, float y, float z) = 0;
	/// \brief  ���������Ƿ����ֱ�ߡ�
	virtual bool TryLine(float xb, float yb, float zb,
                         float xe, float ye, float ze) = 0;

	virtual bool IsExistWater(float xb, float yb, float zb) = 0;

	/// \brief  ��õ�ͼ��Ϣ
	virtual bool GetMapSize(float & left, float & top, float & width, float & height) = 0;

	virtual int GetLastTryError() = 0;
};

#endif