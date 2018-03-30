
#include "exptree.h"

#include <float.h>
#include <stack>
#include "utils/util_func.h"
#include "utils/cau.h"

//������ȼ�
typedef struct _Priority
{
    char op;
    int isp;
    int icp;
} Priority;
const int symbols = 8;
static Priority pri_table[symbols] =
{
    {'#', 0, 0},
    {'+', 3, 2},
    {'-', 3, 2},
    {'*', 5, 4},
    {'/', 5, 4},
    {'^', 6, 7},
    {'(', 1, 8},
    {')', 8, 1}
};
inline bool IsDigit(char ch)
{
    return ch >= '0' && ch <= '9';
}
inline bool OpNumber(char ch)
{
    return ((ch >= '0' && ch <= '9' || ch == '.')  ||
            (ch == '@' || ch == '#' || ch == '$' || ch == '%' || ch == '&') || (ch == 'R' || ch == 'P' || ch == 'I' || ch == 'M' || ch == 'N'));
}
inline bool IsLetterUnderline(char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}
inline bool FloatEqualZero(float f)
{
    return (f >= -FLT_EPSILON && f <= FLT_EPSILON);
}
bool IsDigitStr(const char* str , float& val)
{
    bool bRet = true;
    val = 0.00f;
    if (str == NULL)
    {
        return false;
    }
    size_t len = strnlen(str, MAX_EXP);
    if (len >= MAX_EXP)
    {
        return false;
    }
    float hasDot = false;
    float w = 1;
    for (int i = 0; i < (int)len; ++i)
    {
        if (IsDigit(str[i]) || str[i] == '.')
        {
            if (str[i] == '.')
            {
                if (hasDot)
                {
                    bRet = false;
                    break;
                }
                hasDot = true;
            }
            else
            {
                if (hasDot)
                {
                    w *= 0.1f;
                    val += (str[i] - '0') * w;
                }
                else
                {
                    val = val * 10 + str[i] - '0';
                }
            }
        }
        else
        {
            bRet = false;
            break;
        }
    }
    return bRet;
}


bool GetPropNameFromFormula(const char* szFormula, std::vector<std::pair<char, std::string> >& vecRes)
{
    if ('\0' == *szFormula)
    {
        return false;
    }

    const char* ptrCur = szFormula;
    const char* ptrStart = ptrCur;

    while ('\0' != *ptrCur)
    {
        bool bEnd = false;
        switch (*ptrCur)
        {
            case '(':
            case ')':
            case ' ':
            case '+':
            case '-':
            case '*':
            case '/':
            case '@':
            case '$':
			case ',':
			case ']':
			case '^':
			case '#':
                bEnd = true;
                break;
            default:
                break;
        } // end of switch (*ptr)

        if (bEnd)
        {
            if (ptrCur > ptrStart)
            {
                std::string str;
                str.insert(0, ptrStart, ptrCur - ptrStart);
                float val = 0.0f;
                if (!IsDigitStr(str.c_str(), val))
                {
                    char flag = (char)(*(ptrStart - 1));
                    vecRes.push_back(std::make_pair(flag, str));
                };
            }
            ptrStart = ptrCur + 1;
        }

        ++ptrCur;
    } // end of while (...

    if ('\0' == *ptrCur)
    {
        if (ptrCur > ptrStart)
        {
            std::string str;
            str.insert(0, ptrStart);
            float val = 0.0f;
            if (!IsDigitStr(str.c_str(), val))
            {
                char flag = (char)(*(ptrStart - 1));
                vecRes.push_back(std::make_pair(flag, str));
            }
        }
    }
    return true;
}

typedef enum _NodeType
{
    NODE_TYPE_OP,
    NODE_TYPE_NUM,
    NODE_TYPE_VAR,
    NODE_TYPE_EXPAND_VAR,
    NODE_TYPE_PARAM,
    NODE_TYPE_SPECIAL
} NodeType;

//��׺���ʽ����
bool ExpTree::CreateExpTree(const char* str)
{
    bool b = _ScanNode(str);
    if (!b)
    {
        return false;
    }
    std::stack<ExpNode*> node_stack;
    std::stack<ExpNode*> op_stack;
    ExpNode temp_node;
    temp_node.type = NODE_TYPE_OP;
    temp_node.op = '#';
    op_stack.push(&temp_node);
    for (int i = 0; i < m_count; i++)
    {
        if (m_nodes[i].type == NODE_TYPE_NUM || m_nodes[i].type == NODE_TYPE_VAR ||
            m_nodes[i].type == NODE_TYPE_PARAM || m_nodes[i].type == NODE_TYPE_SPECIAL)
        {
            m_nodes[i].left = NULL;
            m_nodes[i].right = NULL;
            node_stack.push(&m_nodes[i]);
        }
        else if (m_nodes[i].type == NODE_TYPE_OP)
        {
            switch (m_nodes[i].op)
            {
                case '(':
                {
                    op_stack.push(&m_nodes[i]);
                }
                break;
                case ')':
                {
                    while (op_stack.top()->op != '(')
                    {
                        ExpNode* op = op_stack.top();
                        op_stack.pop();
                        ExpNode* right = node_stack.top();
                        node_stack.pop();
                        ExpNode* left = node_stack.top();
                        node_stack.pop();
                        op->left = left;
                        op->right = right;
                        node_stack.push(op);
                    }
                    if (op_stack.empty())
                    {
                        return false;
                    }
                    op_stack.pop(); //����ջ��������
                }
                break;
                case '+':
                case '-':
                case '*':
                case '/':
                case '^':
                {
                    while (_CmpSymbolPri(op_stack.top()->op, m_nodes[i].op) >= 0)
                    {
                        ExpNode* op = op_stack.top();
                        op_stack.pop();
                        ExpNode* right = node_stack.top();
                        node_stack.pop();
                        ExpNode* left = node_stack.top();
                        node_stack.pop();
                        op->left = left;
                        op->right = right;
                        node_stack.push(op);
                    }
                    op_stack.push(&m_nodes[i]);
                }
                break;
                default:
                    return false;
                    break;
            }
        }
        else
        {
            return false;
        }
    }
    while (op_stack.top()->op != '#')
    {
        ExpNode* op = op_stack.top();
        op_stack.pop();
        ExpNode* right = node_stack.top();
        node_stack.pop();
        ExpNode* left = node_stack.top();
        node_stack.pop();
        op->left = left;
        op->right = right;
        node_stack.push(op);
    }
    op_stack.pop();
    if (node_stack.size() != 1)
    {
        return false;
    }
    m_root = node_stack.top();
    node_stack.pop();
    return true;

}

void ExpTree::Clear()
{
    m_root = NULL;
    m_count = 0;

    m_self = PERSISTID();
    m_skill = PERSISTID();
    m_target = PERSISTID();
    m_target_skill = PERSISTID();
    m_pKernel = NULL;

    memset(m_nodes, 0, sizeof(ExpNode) * MAX_NODE);
    memset(m_param, 0, sizeof(float) * MAX_PARAM);
}

float ExpTree::Value()
{
    return _ValNode(m_root);
}

//����
float ExpTree::_Cacul(float a, char op, float b)
{
    float ret = 0.00f;
    switch (op)
    {
        case '+':
            ret = a + b;
            break;
        case '-':
            ret = a - b;
            break;
        case '*':
            ret = a * b;
            break;
        case '/':
        {
            if (!FloatEqualZero(b))
            {
                ret = a / b;
            }
        }
        break;
        case '^':
            ret = std::pow(a, b);
            break;
        default:
            break;
    }
    return ret;
}

//��ֵ����
float ExpTree::_ValNode(ExpNode* cur)
{
    if (cur->type == NODE_TYPE_OP)
    {
        return _Cacul(_ValNode(cur->left), cur->op, _ValNode(cur->right));

    }
    else
    {
        if (cur->type == NODE_TYPE_NUM)
        {
            return cur->fValue * cur->sign;
        }
        else if (cur->type == NODE_TYPE_VAR || cur->type == NODE_TYPE_EXPAND_VAR || cur->type == NODE_TYPE_PARAM || cur->type == NODE_TYPE_SPECIAL)
        {
            return _GetParamValue(cur->chValue) * cur->sign;
        }
        else
        {
            return 0.00f;
        }
    }
}
int ExpTree::_CmpSymbolPri(const char& ispop, const char& icpop)
{
    int pri_isp = -1;
    int pri_icp = -1;
    for (int i = 0, j = 0; i < symbols; i++)
    {
        if (pri_table[i].op == ispop)
        {
            pri_isp = pri_table[i].isp;
            ++j;
            if (j == 2)
            {
                break;
            }
        }
        if (pri_table[i].op == icpop)
        {
            pri_icp = pri_table[i].icp;
            ++j;
            if (j == 2)
            {
                break;
            }
        }
    }
    return pri_isp - pri_icp;
}

bool ExpTree::_ScanNumber(const char* str, int len, float& val, int& bytes)
{
    if (len < 0)
    {
        return false;
    }
    val = 0.00f;
    bool hasDot = false;
    float w = 1;
    int i = 0;
    if (str[0] == '.')
    {
        hasDot = true;
        val = 0;
    }
    else
    {
        val = (float)(str[0] - '0');
        hasDot = false;
    }
    i++;
    for (; i < len && i < MAX_NODE_EXP - 1; i++)
    {
        if (IsDigit(str[i]) || str[i] == '.')
        {
            if (str[i] == '.')
            {
                if (hasDot)
                {
                    return false;;
                }
                hasDot = true;
            }
            else
            {
                if (hasDot)
                {
                    w *= 0.1f;
                    val += (str[i] - '0') * w;
                }
                else
                {
                    val = val * 10 + str[i] - '0';
                }
            }
        }
        else
        {
            break;
        }
    }
    //��ȥ��һ���ַ�
    if (i >= MAX_NODE_EXP - 1)
    {
        return false;
    }
    bytes = i - 1;
    return true;
}

bool ExpTree::_ScanSymbol(const char* str, int len, char* buf, int& bytes)
{


    //���������ַ�'@' '#' '$' '%' ��һ����ĸ�����
    if (len < 2)
    {
        return false;
    }
    int i = 0;
    buf[i] = str[i];
    i++;
    //���ַ���������ĸ�������ֻ��»���
    if (IsLetterUnderline(str[i]))
    {

        buf[i] = str[i];
        i++;
        for (; i < len && i < MAX_NODE_EXP - 1 ; i++)
        {
            //�����ַ�
            if (IsLetterUnderline(str[i]) || IsDigit(str[i]))
            {
                buf[i] = str[i];
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        return false;
    }
    if (i >= MAX_NODE_EXP - 1)
    {
        return false;
    }
    buf[i] = '\0';
    bytes = i - 1;
    return true;
}

bool ExpTree::_ScanExpandSymbol(const char* str, int len, char* buf, int& bytes)
{
    //���������ַ�'&' ���� ��һ����ĸ�����
    if (len < 3)
    {
        return false;
    }
    int i = 0;
    //'&'����
    buf[i] = str[i];
    i++;
    //���ַ���������ĸ�������ֻ��»���
    if (IsLetterUnderline(str[i]))
    {

        buf[i] = str[i];
        i++;
        for (; i < len && i < MAX_NODE_EXP - 1 ; i++)
        {
            //�����ַ�
            if (IsLetterUnderline(str[i]) || IsDigit(str[i]))
            {
                buf[i] = str[i];
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        return false;
    }
    if (i >= MAX_NODE_EXP - 1)
    {
        return false;
    }
    buf[i] = '\0';
    bytes = i - 1;
    return true;
}

bool ExpTree::_ScanRandom(const char* str, int len, char* buf, int& bytes)
{

    //R[M,N] or R[M]
    if (len < 6)
    {
        return false;
    }
    int i = 0;
    //R
    buf[i] = str[i];
    i++;
    //[
    if (str[i] != '[')
    {
        return false;
    }
    buf[i] = str[i];
    i++;
    bool find = false;
    for (; i < len && i < MAX_NODE_EXP - 2; i++)
    {
        buf[i] = str[i];
        if (buf[i] == ']')
        {
            find = true;
            break;
        }
    }
    if (!find)
    {
        return false;
    }
    //Խ����
    if (i >= MAX_NODE_EXP - 2)
    {
        return false;
    }
    buf[i] = str[i];
    i++;
    buf[i] = '\0';
    bytes = i - 1;
    return true;

}

bool ExpTree::_ScanParam(const char* str, int len, char* buf, int& bytes)
{
    if (len < 2)
    {
        return false;
    }
    int i = 0;
    buf[i] = str[i];
    i++;
    buf[i] = str[i];
    i++;
    buf[i] = '\0';
    bytes = i - 1;
    return true;
}

float ExpTree::_QueryPropValue(IKernel* pKernel, char chr, const char* prop_name)
{
    PERSISTID obj;
    float ret = 0.00f;
    if (chr == '@')
    {
        obj = m_self;
    }
    else if (chr == '#')
    {
        obj = m_skill;
    }
    else if (chr == '$')
    {
        obj = m_target;
    }
    else if (chr == '%')
    {
        obj = m_target_skill;
    }
    ret = Cau::QueryPropValue(pKernel, obj, prop_name);

    return ret;
}

float ExpTree::_GetParamValue(const char* szFormula)
{
    float res = 0.0f;
    int len = static_cast<int>(strnlen(szFormula, MAX_NODE_EXP - 1));
    if (len == 0 || len >= MAX_NODE_EXP - 1)
    {
        return res;
    }
    if (IsDigitStr(szFormula, res))
    {
        return res;
    }
    else
    {
        res = 0.0f;
    }

    if (szFormula[0] == '@' || szFormula[0] == '#' || szFormula[0] == '$' || szFormula[0] == '%')
    {
        char var[MAX_NODE_EXP];
        int i = 0;
        //ȥ��ͷ@ # $ %
        while (i < len - 1)
        {
            var[i] = szFormula[i + 1];
            i++;
        }
        var[i] = 0;
        res = _QueryPropValue(m_pKernel, szFormula[0], var);
    }
//     else if (szFormula[0] == '&')
//     {
//         char var[MAX_NODE_EXP];
//         //ȥ�� &
//         int i = 0;
//         //��ȡ����
//         while (i < len - 1)
//         {
//             var[i] = szFormula[i + 1];
//             i++;
//         }
//         var[i] = 0;
//         res = _QueryExpandPropValue(m_pKernel, var, 3);
//     }
    //�ж��Ƿ���������صı���
    else if (szFormula[0] == 'P')
    {
        int index = szFormula[1] - '1';

        if (index < MAX_PARAM)
        {
            res = m_param[index];
        }
    }

    //�ж��Ƿ����������صı�������ʽR[P1,P2]��R[P1]
    else if (szFormula[0] == 'R')
    {
        //�����Ƿ��ж���
        const char* cfind = strchr(szFormula, ',');
        if (cfind == NULL)
        {
            //û�ж��ţ�ȡһ�������������
            char var[MAX_NODE_EXP];
            int i = 0;
            while (i < len - 3)
            {
                var[i] = szFormula[2 + i];
                i++;
            }
            var[i] = 0;
            res = (float)util_random_float((float)_GetParamValue(var));
        }
        else
        {
            //�ж��ţ�ȡ����������������
            char var[MAX_NODE_EXP];
            int split = (int)(cfind - szFormula);
            int i = 0;
            while (i < split - 2)
            {
                var[i] = szFormula[2 + i];
                i++;
            }
            var[i] = 0;
            float min = _GetParamValue(var);
            i = 0;
            while (i < len - split - 2)
            {
                var[i] = szFormula[split + 1 + i];
                i++;
            }
            var[i] = 0;
            float max = _GetParamValue(var);
            if (max < min)
            {
                //ת��Ϊ�Ϸ��ķ�Χ
                float temp = min;
                min = max;
                max = temp;
            }
            res = min + (float)util_random_float((float)(max - min));
        }
    }
	//�ж��Ƿ�ȡINTֵ����ʽI[P]
	else if (szFormula[0] == 'I')
	{
		//û�ж��ţ�ȡһ�������������
		char var[MAX_NODE_EXP];
		int i = 0;
		while (i < len - 3)
		{
			var[i] = szFormula[2 + i];
			i++;
		}
		var[i] = 0;
		res = (float)floor((float)_GetParamValue(var));
	}
	//�ж��Ƿ�ȡ��Сֵ
	else if (szFormula[0] == 'M')
	{
		//�����Ƿ��ж���
		const char* cfind = strchr(szFormula, ',');
		if (cfind == NULL)
		{
			//û�ж��ţ�ȡһ�������������
			char var[MAX_NODE_EXP];
			int i = 0;
			while (i < len - 3)
			{
				var[i] = szFormula[2 + i];
				i++;
			}
			var[i] = 0;
			res = (float)_GetParamValue(var);
		}
		else
		{
			//�ж��ţ�ȡ����������������
			char var[MAX_NODE_EXP];
			int split = (int)(cfind - szFormula);
			int i = 0;
			while (i < split - 2)
			{
				var[i] = szFormula[2 + i];
				i++;
			}
			var[i] = 0;
			float tmp1 = _GetParamValue(var);
			i = 0;
			while (i < len - split - 2)
			{
				var[i] = szFormula[split + 1 + i];
				i++;
			}
			var[i] = 0;
			float tmp2 = _GetParamValue(var);
			res = __min(tmp1, tmp2);
		}
	}
	//�ж��Ƿ�ȡ���ֵ
	else if (szFormula[0] == 'N')
	{
		//�����Ƿ��ж���
		const char* cfind = strchr(szFormula, ',');
		if (cfind == NULL)
		{
			//û�ж��ţ�ȡһ�������������
			char var[MAX_NODE_EXP];
			int i = 0;
			while (i < len - 3)
			{
				var[i] = szFormula[2 + i];
				i++;
			}
			var[i] = 0;
			res = (float)_GetParamValue(var);
		}
		else
		{
			//�ж��ţ�ȡ����������������
			char var[MAX_NODE_EXP];
			int split = (int)(cfind - szFormula);
			int i = 0;
			while (i < split - 2)
			{
				var[i] = szFormula[2 + i];
				i++;
			}
			var[i] = 0;
			float tmp1 = _GetParamValue(var);
			i = 0;
			while (i < len - split - 2)
			{
				var[i] = szFormula[split + 1 + i];
				i++;
			}
			var[i] = 0;
			float tmp2 = _GetParamValue(var);
			res = __max(tmp1, tmp2);
		}
	}
    return res;
}
bool ExpTree::_ScanNode(const char* str)
{
    int len = (int)strnlen(str, MAX_EXP - 1);
    if (len == MAX_EXP - 1 || len == 0)
    {
        return false;
    }
    int i, sign;
    //inNum��ǵ�ǰ�Ƿ�������������
    bool inNum;
    for (i = 0, sign = 1, inNum = true; i < len; i++)
    {
        if (OpNumber(str[i]))
        {
            if (inNum)
            {
                int bytes = 0;
                bool b = false;
                if (IsDigit(str[i]) || str[i] == '.')
                {
                    m_nodes[m_count].type = NODE_TYPE_NUM;
                    b = _ScanNumber(&str[i], len - i, m_nodes[m_count].fValue, bytes);
                }
                else if (str[i] == '@' || str[i] == '#' || str[i] == '$' || str[i] == '%')
                {
                    m_nodes[m_count].type = NODE_TYPE_VAR;
                    b = _ScanSymbol(&str[i], len - i, m_nodes[m_count].chValue, bytes);
                }
                else if (str[i] == '&')
                {
                    m_nodes[m_count].type = NODE_TYPE_EXPAND_VAR;
                    b = _ScanExpandSymbol(&str[i], len - i, m_nodes[m_count].chValue, bytes);
                }
                else if (str[i] == 'R' || str[i] == 'I'|| str[i] == 'M' || str[i] == 'N')
                {
                    m_nodes[m_count].type = NODE_TYPE_SPECIAL;
                    b = _ScanRandom(&str[i], len - i, m_nodes[m_count].chValue, bytes);
                }
                else if (str[i] == 'P')
                {
                    m_nodes[m_count].type = NODE_TYPE_PARAM;
                    b = _ScanParam(&str[i], len - i, m_nodes[m_count].chValue, bytes);
                }
                else
                {
                    return false;
                }
                if (!b)
                {
                    return false;
                }
                i += bytes;
                m_nodes[m_count].sign = sign;
                m_count ++;
                if (m_count > MAX_NODE)
                {
                    return false;
                }
                sign = 1;
                inNum = false;
            }
            else
            {
                return false;
            }
        }
        else
        {
            switch (str[i])
            {
                case '(':
                case ')':
                {
                    m_nodes[m_count].type = NODE_TYPE_OP;
                    m_nodes[m_count].op = str[i];
                    m_count ++;
                    if (m_count > MAX_NODE)
                    {
                        return false;
                    }
                }
                break;
                case '+':
                case '-':
                case '*':
                case '/':
                case '^':
                {
                    if (inNum)
                    {
                        if (str[i] != '+' && str[i] != '-')
                        {
                            throw true;
                        }
                        while (str[i] == '+' || str[i] == '-')
                        {
                            if (str[i] == '-')
                            {
                                sign *= -1;
                            }
                            i++;
                        }
                        i--;
                    }
                    else
                    {
                        m_nodes[m_count].type = NODE_TYPE_OP;
                        m_nodes[m_count].op = str[i];
                        m_count ++;
                        if (m_count > MAX_NODE)
                        {
                            return false;
                        }
                        inNum = true;
                    }
                }
                break;
                default:
                    return false;
                    break;
            }
        }
    }
    return m_count > 0;
}

float ExpTree::CalculateParam(const char* szFormula, const float(&arg)[MAX_PARAM])
{
    if (szFormula == NULL)
    {
        return 0.00f;
    }
    size_t len = strnlen(szFormula, MAX_EXP - 1);
    if (len == 0 || len == MAX_EXP - 1)
    {
        return 0.00f;
    }
    memset(m_param, 0, sizeof(m_param));
    memcpy(m_param, arg, sizeof(float) * MAX_PARAM);
    bool b = CreateExpTree(szFormula);
    if (!b)
    {
        return 0.00f;
    }
    return (float)Value();
}

float ExpTree::CalculateBuff(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const char* szFormula)
{
    if (szFormula == NULL)
    {
        return 0.00f;
    }
    size_t len = strnlen(szFormula, MAX_EXP - 1);
    if (len == 0 || len >= MAX_EXP - 1)
    {
        return 0.00f;
    }
    m_self = self;
    m_target = sender;
    m_pKernel = pKernel;
    bool b = CreateExpTree(szFormula);
    if (!b)
    {
        return 0.00f;
    }
    return (float)Value();
}

float ExpTree::CalculateModify(IKernel* pKernel, const PERSISTID& self, const char* szFormula)
{
    if (szFormula == NULL)
    {
        return 0.00f;
    }
    size_t len = strnlen(szFormula, MAX_EXP - 1);
    if (len == 0 || len >= MAX_EXP - 1)
    {
        return 0.00f;
    }
    m_self = self;
    m_pKernel = pKernel;
    bool b = CreateExpTree(szFormula);
    if (!b)
    {
        return 0.00f;
    }
    return (float)Value();
}

float ExpTree::CalculateEvent(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill,
                              const PERSISTID& target, const PERSISTID& target_skill, const char* szFormula)
{
    if (szFormula == NULL)
    {
        return 0.00f;
    }
    size_t len = strnlen(szFormula, MAX_EXP - 1);
    if (len == 0 || len >= MAX_EXP - 1)
    {
        return 0.00f;
    }
    m_self = self;
    m_skill = skill;
    m_target = target;
    m_target_skill = target_skill;
    m_pKernel = pKernel;
    bool b = CreateExpTree(szFormula);
    if (!b)
    {
        return 0.00f;
    }
    return (float)Value();
}
float ExpTree::CalculateItem(IKernel* pKernel, const PERSISTID& item, const char* szFormula, const float(&arg)[MAX_PARAM])
{
    m_self = item;
    m_pKernel = pKernel;
    return CalculateParam(szFormula, arg);
}