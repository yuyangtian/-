#include"�﷨������.h"
#include"���������.h"

//#define Tree_trace(x) PrintSyntaxTree(x,1);
#pragma warning(disable: 4996)
#pragma warning(disable: 4703)
double Parameter = 0,
Origin_x = 0, Origin_y = 0,
Scale_x = 1, Scale_y = 1,
Rot_angle = 0;

static Token token;//����һ���Ǻ�

				   //������������
static void FetchToken();//���ôʷ���������GetToken���ѵõ��ĵ�ǰ��¼��������������õ��ļǺ��ǷǷ�����errtoken����ָ��һ���﷨����
static void MatchToken(enum Token_Type The_Token);//ƥ�䵱ǰ�Ǻ�
static void SyntaxError(int case_of);//�����﷨������ӳ��򡣸��ݴ�������ʴ�ӡ�����Ϣ������ֹ�������С��������ʿ��Ը��ݴ��β�ͬ�����֣�SyntaxError��1���ʷ���   SyntaxError��2���﷨��
static void ErrMsg(unsigned LineNo, char *descrip, char *string);//��ӡ������Ϣ
static void PrintSyntaxTree(struct ExprNode *root, int indent);//���������ӡ�﷨��
															   //���ս���ݹ��ӳ������� ��2��
															   //��1�࣬�﷨�������������﷨������������ӳ�������Ϊ����->void���͵ĺ��������ս���ĵݹ��ӳ�������
static void Program();//����
static void Statement();//���
static void OriginStatement();//Origin���
static void RotStatement();//Rot���
static void ScaleStatement();//Scale���
static void ForStatement();//For���
						   //��2�࣬�﷨����+�����﷨������˱��ʽ�����Ϊ����ֵΪָ���﷨���ڵ��ָ��ĺ�����
static struct ExprNode *Expression();//���ʽ����Ԫ�Ӽ�������ʽ
static struct ExprNode *Term();//�˳�������ʽ
static struct ExprNode *Factor();//һԪ�Ӽ�������ʽ
								 //��������Ӷ������������˼Ӽ�����˳��ŵ����ȼ����⡣���⼸�����̵ķ��������У�ʼ�մ���fsys������ֵ����֤�����ڳ�����������������ķ��ţ�ʹ�������̵��Խ�����ȥ��
static struct ExprNode *Component();//��������ʽ
static struct ExprNode *Atom();//ԭ�ӱ��ʽ

							   //����ӿ�����
extern void Parser(char *SrcFilePtr);

//�﷨�����캯������
static struct ExprNode *MakeExprNode(enum Token_Type opcode, ...);

//ͨ���ʷ��������ӿ�GetToken���һ���Ǻ�
static void FetchToken()
{
	token = GetToken();
	if (token.type == ERRTOKEN) SyntaxError(1); //����õ��ļǺ��ǷǷ�����errtoken����ָ��һ���﷨����
}

//ƥ�䵱ǰ�Ǻ�
static void MatchToken(enum Token_Type The_Token)
{
	if (token.type != The_Token)
	{
		SyntaxError(2);//��ʧ�ܣ�ָ���﷨����
	}
	FetchToken();//���ɹ�����ȡ��һ��
}

//�﷨������
static void SyntaxError(int case_of)
{
	switch (case_of)
	{
	case 1: ErrMsg(LineNo, (char*)"����Ǻ� ", token.lexeme);
		break;
	case 2: ErrMsg(LineNo, (char*)"����Ԥ�ڼǺ�", token.lexeme);
		break;
	}
}

//��ӡ������Ϣ
void ErrMsg(unsigned LineNo, char *descrip, char *string)
{
	char msg[256];
	memset(msg, 0, 256);
	sprintf(msg, "��%d�� : %s %s",LineNo,descrip,string);

	MessageBoxA(NULL, msg, "error", MB_OK);

	CloseScanner();
	exit(1);
}

//���������ӡ�﷨������-��-��
void PrintSyntaxTree(struct ExprNode *root, int indent)
{
	int temp;
	for (temp = 1; temp <= indent; temp++) printf("\t");//����
	switch (root->OpCode)
	{
	case PLUS:		printf("%s\n", "+");								break;
	case MINUS:		printf("%s\n", "-");								break;
	case MUL:		printf("%s\n", "*");								break;
	case DIV:		printf("%s\n", "/");								break;
	case POWER:		printf("%s\n", "**");								break;
	case FUNC:		printf("%x\n", root->Content.CaseFunc.MathFuncPtr);	break;
	case CONST_ID:	printf("%f\n", root->Content.CaseConst);			break;
	case T:			printf("%s\n", "T");								break;
	default:		printf("Error Tree Node !\n");						exit(0);
	}
	if (root->OpCode == CONST_ID || root->OpCode == T) //Ҷ�ӽڵ㷵��
		return;//�����Ͳ���ֻ��Ҷ�ӽڵ� ��������ֵ����������ֵ��ַ
	if (root->OpCode == FUNC)//�ݹ��ӡһ�����ӽڵ�
		PrintSyntaxTree(root->Content.CaseFunc.Child, indent + 1);//�����к��ӽڵ��ָ��������ָ��
	else//�ݹ��ӡ�������ӽڵ�
	{//��Ԫ���㣺���Һ��ӵ��ڲ��ڵ�
		PrintSyntaxTree(root->Content.CaseOperater.Left, indent + 1);
		PrintSyntaxTree(root->Content.CaseOperater.Right, indent + 1);
	}
}

//��ͼ���Խ�������ڣ�����������ⲿ�ӿڣ�
void Parser(char *SrcFilePtr)//�﷨�����������
{
	if (!InitScanner(SrcFilePtr))//��ʼ���ʷ�������
	{
		printf("Open Source File Error !\n"); return;
	}
	FetchToken();//�Ȼ��һ���Ǻ�
	Program();//Ȼ�����Program�ݹ��ӳ��򣬵ݹ��½�����
	CloseScanner();//�رմʷ�������
	return;
}

//����Program�ĵݹ��ӳ���
static void Program()
{
	while (token.type != NONTOKEN)//�Ǻ����Ͳ�Ϊ��
	{
		Statement();//�����ж�����
		MatchToken(SEMICO);//ֱ��ƥ�䵽�ָ���
	}
}

//���Statment�ĵݹ��ӳ���
static void Statement()
{
	switch (token.type)
	{
		//��������������ͣ��ֱ����
	case ORIGIN: OriginStatement(); break;
	case SCALE: ScaleStatement(); break;
	case ROT:  RotStatement(); break;
	case FOR: ForStatement(); break;
	default: SyntaxError(2); //���򱨴�
	}
}

//���OriginStatement�ĵݹ��ӳ���
static void OriginStatement(void)
{
	struct ExprNode *tmp;//�﷨���ڵ������
	MatchToken(ORIGIN);
	MatchToken(IS);
	MatchToken(L_BRACKET);

	tmp = Expression();  //Tree_trace(tmp);

	Origin_x = GetExprValue(tmp);//��ȡ�������ƽ�ƾ���
	DelExprTree(tmp);//ɾ��һ����

	MatchToken(COMMA);

	tmp = Expression();   //Tree_trace(tmp);

	Origin_y = GetExprValue(tmp);//��ȡ�������ƽ�ƾ���
	DelExprTree(tmp);//ɾ��һ����

	MatchToken(R_BRACKET);
}

//���ScaleStatement�ĵݹ��ӳ���
static void ScaleStatement(void)
{
	struct ExprNode *tmp;
	MatchToken(SCALE);
	MatchToken(IS);
	MatchToken(L_BRACKET);

	tmp = Expression();  //Tree_trace(tmp);

	Scale_x = GetExprValue(tmp);//��ȡ������ı�������
	DelExprTree(tmp);

	MatchToken(COMMA);

	tmp = Expression();    //Tree_trace(tmp);

	Scale_y = GetExprValue(tmp);//��ȡ������ı�������
	DelExprTree(tmp);

	MatchToken(R_BRACKET);
}

//���RotStatement�ĵݹ��ӳ���
static void RotStatement(void)
{
	struct ExprNode *tmp;
	MatchToken(ROT);
	MatchToken(IS);

	tmp = Expression();  //Tree_trace(tmp);

	Rot_angle = GetExprValue(tmp);//�����ת�Ƕ�
	DelExprTree(tmp);
}

//���ForStatement�ĵݹ��ӳ���
//���Ҳ��ķ����ŵ�չ��->�����ս����ֱ��ƥ�䣬�������ս���͵�����Ӧ�ӳ���
//ForStatement��Ψһ�ķ��ս����Expression����������5����ͬλ�ã��ֱ����ѭ������ʼ����ֹ�������������ꡢ�����꣬��Ҫ5�����ڵ�ָ�뱣����5���﷨��
static void ForStatement(void)
{
	//eg:for T from 0 to 2*pi step pi/50 draw (t, -sin(t));
	double Start, End, Step;//��ͼ��㡢�յ㡢����
	struct ExprNode *start_ptr, *end_ptr, *step_ptr, *x_ptr, *y_ptr;

	MatchToken(FOR);
	MatchToken(T);
	MatchToken(FROM);//eg:for T from

	start_ptr = Expression(); //Tree_trace(start_ptr);//��ò��������ʽ���﷨��

	Start = GetExprValue(start_ptr);//������������ʽ��ֵ
	DelExprTree(start_ptr);//�ͷŲ�������﷨����ռ�ռ�

	MatchToken(TO);

	end_ptr = Expression(); //Tree_trace(end_ptr);//��������յ���ʽ�﷨��

	End = GetExprValue(end_ptr);//��������յ���ʽ��ֵ
	DelExprTree(end_ptr);//�ͷŲ����յ��﷨����ռ�ռ�

	MatchToken(STEP);

	step_ptr = Expression(); //Tree_trace(step_ptr);//��������������ʽ�﷨��

	Step = GetExprValue(step_ptr);//��������������ʽ��ֵ
	DelExprTree(step_ptr);//�ͷŲ��������﷨����ռ�ռ�

	MatchToken(DRAW);
	MatchToken(L_BRACKET);

	x_ptr = Expression(); //Tree_trace(x_ptr);

	MatchToken(COMMA);

	y_ptr = Expression();//Tree_trace(y_ptr);

	MatchToken(R_BRACKET);

	DrawLoop(Start, End, Step, x_ptr, y_ptr); //����ͼ��
	DelExprTree(x_ptr);//�ͷź������﷨����ռ�ռ�
	DelExprTree(y_ptr);//�ͷ��������﷨����ռ�ռ�
}

//����Ԫ�Ӽ����㣩���ʽExpression�ĵݹ��ӳ������ϱ߲�̫��ͬ���ǣ����ʽ��ҪΪ�乹���﷨��
//�Ѻ������Ϊ�﷨���ڵ��ָ�룬�ں���������2���﷨���ڵ��ָ��������ֱ���ΪExpression���Ҳ�������Term�����﷨���ڵ�ָ��
//���ʽӦ�����������Ż��޷��ſ�ͷ�������ɸ����ԼӼ������Ӷ��ɡ�
static struct ExprNode *Expression()//չ���Ҳ������ҹ����﷨��
{
	struct ExprNode *left, *right;//���������ڵ�ָ��
	Token_Type token_tmp;//��ǰ�Ǻ�

	left = Term();//������������õ����﷨�������������һ���˳�������ʽ
	while (token.type == PLUS || token.type == MINUS)
	{
		token_tmp = token.type;//��ǰ�Ǻ��Ǽ�/��
		MatchToken(token_tmp);//ƥ��Ǻ�
		right = Term();//�����Ҳ������õ����﷨�����Ҳ�������һ���˳�������ʽ
		left = MakeExprNode(token_tmp, left, right);//����������﷨�������Ϊ������
	}
	return left;//���ص��ǵ�ǰ�ǺŽڵ�
}

//�˳�������ʽTerm�ĵݹ��ӳ���
//���������ɸ������Գ˳������Ӷ���
static struct ExprNode *Term()
{
	struct ExprNode *left, *right;
	Token_Type token_tmp;
	left = Factor();
	while (token.type == MUL || token.type == DIV)
	{
		token_tmp = token.type;
		MatchToken(token_tmp);
		right = Factor();
		left = MakeExprNode(token_tmp, left, right);
	}
	return left;
}

//һԪ�Ӽ�����Factor�ĵݹ��ӳ���
//�����������һ����ʶ����һ�����֣�����һ�����������������ӱ��ʽ
static struct ExprNode *Factor()
{
	struct ExprNode *left, *right;
	if (token.type == PLUS)//ƥ��һԪ������
	{
		MatchToken(PLUS);
		right = Factor();
	}
	else if (token.type == MINUS)//���ʽ�˻�Ϊ�����Ҳ������ı��ʽ
	{
		MatchToken(MINUS);
		right = Factor();
		left = new ExprNode;
		left->OpCode = CONST_ID;
		left->Content.CaseConst = 0.0;
		right = MakeExprNode(MINUS, left, right);
	}
	else right = Component();//ƥ����ս��Component
	return right;
}

//��������ʽComponent�ĵݹ��ӳ���
static struct ExprNode *Component()//�ҽ��
{
	struct ExprNode *left, *right;
	left = Atom();
	if (token.type == POWER)
	{
		MatchToken(POWER);
		right = Component();//�ݹ����Component��ʵ��POWER���ҽ��
		left = MakeExprNode(POWER, left, right);
	}
	return left;
}

//ԭ�ӱ��ʽAtom�ĵݹ��ӳ��򣬰����ָ��� ���� ���� ����
static struct ExprNode *Atom()
{
	struct Token t = token;
	struct ExprNode *address, *tmp;
	switch (token.type)
	{
	case CONST_ID:
		MatchToken(CONST_ID);
		address = MakeExprNode(CONST_ID, t.value);
		break;
	case T:
		MatchToken(T);
		address = MakeExprNode(T);
		break;
	case FUNC:
		MatchToken(FUNC);
		MatchToken(L_BRACKET);
		tmp = Expression();  //Tree_trace(tmp);
		address = MakeExprNode(FUNC, t.FuncPtr, tmp);
		MatchToken(R_BRACKET);
		break;
	case L_BRACKET:
		MatchToken(L_BRACKET);
		address = Expression(); // Tree_trace(address);
		MatchToken(R_BRACKET);
		break;
	default:
		SyntaxError(2);
	}
	return address;
}

//�����﷨����һ���ڵ㣬���տɱ�����б�
static struct ExprNode * MakeExprNode(enum Token_Type opcode, ...)//ע�����������һ���ɱ�����ĺ���
{
	struct ExprNode *ExprPtr = new(struct ExprNode);//Ϊ�½ڵ㿪�ٿռ�
	ExprPtr->OpCode = opcode;//��ڵ�д��Ǻ����
	va_list ArgPtr;//ָ��ɱ亯���Ĳ�����ָ��
	va_start(ArgPtr, opcode);//��ʼ��va_list��������һ������Ҳ���ǹ̶�����Ϊopcode
	switch (opcode)//���ݼǺŵ�����첻ͬ�Ľڵ�
	{
	case CONST_ID://�����ڵ�
		ExprPtr->Content.CaseConst = (double)va_arg(ArgPtr, double);//���ؿɱ�������ɱ���������ǳ���
		break;
	case T://����T�ڵ�
		ExprPtr->Content.CaseParmPtr = &Parameter;//���ؿɱ�������ɱ���������ǲ���T
		break;
	case FUNC://�������ýڵ�
		ExprPtr->Content.CaseFunc.MathFuncPtr = (FuncPtr)va_arg(ArgPtr, FuncPtr);//�ɱ���������Ƕ�Ӧ������ָ��
		ExprPtr->Content.CaseFunc.Child = (struct ExprNode *)va_arg(ArgPtr, struct ExprNode *);//�ɱ���������ǽڵ�
		break;
	default://��Ԫ����ڵ�
		ExprPtr->Content.CaseOperater.Left = (struct ExprNode *)va_arg(ArgPtr, struct ExprNode *);//�ɱ���������ǽڵ�
		ExprPtr->Content.CaseOperater.Right = (struct ExprNode *)va_arg(ArgPtr, struct ExprNode *);//ͬ��
		break;
	}
	va_end(ArgPtr);//�����ɱ�����Ļ�ȡ

	return ExprPtr;
}