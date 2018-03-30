//--------------------------------------------------------------------
// �ļ���:      Formula.h
// ��  ��:      ��ʽ���ʽ��
// ˵  ��:      ���߻����õĹ�ʽ������һ��Formula��Formula����һ����׺���ʽ
//              ��ʾһ����ʽ������ʽ�е����������������ķ�ʽ���档�ڻ�ȡ��ʽֵ
//              ��ʱ��ͨ��������������ֵ����0��1�����ٶ��ҵ�����������ȡ����ֵ��
//              ����ÿ�μ����ظ������ַ�������׺���ʽ���Կ��ٵط�����ֵ�ڵ�����
//              �����������Ч�ʡ�
// ��������:    2015��7��28��
// ������:       
//    :       
//--------------------------------------------------------------------


#ifndef __FORMULA_H__
#define __FORMULA_H__

#include <string>
#include <vector>
#include "FsGame/SkillModule/Impl/FormulaStack.h"
#include "server/IGameObj.h"
#include "public/Macros.h"

class Formula;
typedef  Formula* (*RecursionFormulaFunc)(int formula_index);

class Formula
{
    enum ENodeType
    {
        NODE_TYPE_VARIANT,
        NODE_TYPE_DIGIT,
        NODE_TYPE_OP,
		NODE_TYPE_SPECIAL
    };
    enum EOperatorType
    {
        OPERATOR_TYPE_UNKNOWN,
        OPERATOR_TYPE_ADD,				// +
        OPERATOR_TYPE_SUB,				// -
        OPERATOR_TYPE_MUL,				// *
        OPERATOR_TYPE_DIV,				// /
		OPERATOR_TYPE_INT,				// % floatȡ��
		OPERATOR_TYPE_MAX,				// > ȡ���ֵ
		OPERATOR_TYPE_MIN,				// < ȡ��Сֵ
		OPERATOR_TYPE_RANDOM,			// | ȡ���ֵ
    };
    enum EObjectType
    {
        OBJECT_TYPE_SELF,			// ʩ���߻�����һ����ʽ
        OBJECT_TYPE_TARGET,			// Ŀ��
		OBJECT_TYPE_SELF_SKILL		// ʩ���߼���
    };

public:
    Formula();
    ~Formula();

    bool Parse(const char* str, const char* szMinValue, const char* szMaxValue);
    void Clear();
    bool GetValue(IGameObj* pSelfObj, IGameObj* pTargetObj, IGameObj* pSelfSkill, float& value, RecursionFormulaFunc fun);

	// �������ֵ
	void ClearCacheVal();
private:
    int GetPriority(const std::string& opt);
    bool IsOperator(const std::string& opt);
    bool IsOperator(char opt);
    bool IsVariant(const std::string& token);
    bool IsVariant(char token);
    bool IsDigit(const std::string& token);
    bool IsDigit(char token);

    // ����׺���ʽ����Ԥ���������ÿ��token
    bool PreParse(const char *str, std::vector<std::string>& out_token);

     // ת��Ϊ��׺��ʽ
    bool ToPostfix(const char* str, std::vector<std::string>& suffix);

    EOperatorType ToOperatorIndex(const std::string& token);
    bool Calculate(FormulaStack<float>& statck, EOperatorType op_type);
    bool GetVariantValue(IGameObj* pSelfObj, 
						IGameObj* pTargetObj, 
						IGameObj* pSelfSkill,
                        int obj_type, int prop_index, 
                        float& value, RecursionFormulaFunc fun);

    // ��value���õ�ȡֵ��Χ��
    // Լ��-1��һ������ֵ�����ڱ�ʾ�Ƿ������޻�������
    float AdjustFightData(float value);

private:
// 	typedef struct _ExpNode
// 	{
// 		int type;
// 		union
// 		{
// 			char op;
// 			float fValue;
// 			char chValue[MAX_NODE_EXP];
// 		};
// 		char from;
// 		int sign;
// 		struct _ExpNode* left;
// 		struct _ExpNode* right;
// 	} ExpNode;

    struct TokenNode
    {
        ENodeType node_type;
        union
        {
            // �������ڵ�
            EOperatorType op_type;

            // ���Խڵ�
            struct
            {
                short obj_type;         // ���Զ�������
                short prop_index;       // ��������
            };

            // ��ֵ�ڵ�
            float digit_value;
        };

        TokenNode()
        {
            memset(this, 0, sizeof(*this));
        }
    };
    enum {MAX_VECTOR_LEN = 100};
    TokenNode m_postfix[MAX_VECTOR_LEN];
    int m_len;

    // ��ʽ��ȡֵ��Χ
    float m_min_value;
    float m_max_value;

    std::string m_formula;	// ���湫ʽ�����ı������ڷ���
	float		m_fCacheVal;		// ����ֵ
	bool		m_bCache;			// �Ƿ񻺴�
};

#endif // __FORMULA_H__
