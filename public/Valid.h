//-------------------------------------------------------------------- 
// �ļ���:		Valid.h 
// ��  ��:		���ڵ��Ե���֤��Ч�Եĺ���
// ˵  ��:		
// ��������:	2011��4��26��
// ������:		��Ң˳
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_VALID_H
#define _PUBLIC_VALID_H

inline bool int_is_valid(const int& i)
{
	if (_isnan(i)) { return false; }

	return true;
}

inline bool float_is_valid(const float& f)
{
	if (_isnan(f)) { return false; }

	return true;
}

inline bool float2_is_valid(const float& f1, const float& f2)
{
	if (_isnan(f1)) { return false; }
	if (_isnan(f2)) { return false; }

	return true;
}

inline bool float3_is_valid(const float& f1, const float& f2, const float& f3)
{
	if (_isnan(f1)) { return false; }
	if (_isnan(f2)) { return false; }
	if (_isnan(f3)) { return false; }

	return true;
}

inline bool float4_is_valid(const float& f1, const float& f2, const float& f3, 
	const float& f4)
{
	if (_isnan(f1)) { return false; }
	if (_isnan(f2)) { return false; }
	if (_isnan(f3)) { return false; }
	if (_isnan(f4)) { return false; }

	return true;
}

inline bool v2_is_valid(const D3DXVECTOR2& v)
{
	if (_isnan(v.x)) { return false; }
	if (_isnan(v.y)) { return false; }

	return true;
}

inline bool v3_is_valid(const D3DXVECTOR3& v)
{
	if (_isnan(v.x)) { return false; }
	if (_isnan(v.y)) { return false; }
	if (_isnan(v.z)) { return false; }

	return true;
}

inline bool v4_is_valid(const D3DXVECTOR4& v)
{
	if (_isnan(v.x)) { return false; }
	if (_isnan(v.y)) { return false; }
	if (_isnan(v.z)) { return false; }
	if (_isnan(v.w)) { return false; }

	return true;
}

inline bool mtx_is_valid(const D3DXMATRIX& mtx)
{
	if (_isnan(mtx._11)) { return false; }
	if (_isnan(mtx._12)) { return false; }
	if (_isnan(mtx._13)) { return false; }
	if (_isnan(mtx._14)) { return false; }

	if (_isnan(mtx._21)) { return false; }
	if (_isnan(mtx._22)) { return false; }
	if (_isnan(mtx._23)) { return false; }
	if (_isnan(mtx._24)) { return false; }

	if (_isnan(mtx._31)) { return false; }
	if (_isnan(mtx._32)) { return false; }
	if (_isnan(mtx._33)) { return false; }
	if (_isnan(mtx._34)) { return false; }

	if (_isnan(mtx._41)) { return false; }
	if (_isnan(mtx._42)) { return false; }
	if (_isnan(mtx._43)) { return false; }
	if (_isnan(mtx._44)) { return false; }

	return true;
}

#endif // _PUBLIC_VALID_H
