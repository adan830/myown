
//--------------------------------------------------------------------
// �ļ���:      SkillModule\impl\FormulaPropIndex.h
// ��  ��:      
// ˵  ��:      
// ��������:    2015��7��28��
// ������:       
//    :       
//--------------------------------------------------------------------

#ifndef __FORMULA_PROP_INDEX_H__
#define __FORMULA_PROP_INDEX_H__

#include "utils/StringPod.h"
#include "utils/arraypod.h"

class FormulaPropIndex 
{
public:
    // �ù���������һ�μ���
    static void RegisterPropIndex();
    static short ToPropIndex(const char* prop);
    static const char* ToPropName(int index);
private:

    static TStringPod<char, int>    s_prop_to_index;
    static TArrayPod<const char*, 1> s_index_to_prop;
}; // end of class FormulaPropIndex

#endif // __FORMULA_PROP_INDEX_H__

