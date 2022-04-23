#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX  100
typedef char Optertype;
typedef int  Opndtype;
typedef struct
{
    Optertype* base;
    Optertype* top;
    int stacksize;
} Opter;

typedef struct
{
    Opndtype* base;
    Opndtype* top;
    int stacksize;
} Opnd;

//操作数栈操作
void  InitOpnd(Opnd* S);//初始化操作数栈
void PushOpnd(Opnd* S, Opndtype Data);//数入栈
Opndtype PopOpnd(Opnd* S);//数出栈
Opndtype GetOpndTop(Opnd* S);//取栈顶
int FullOpnd(Opnd* S);//是否为满栈


//运算符操作
void  InitOpter(Opter* S);//初始化操作符号栈
void PushOpter(Opter* S, Optertype Data);//符号压栈
Optertype PopOpter(Opter* S);//符号出栈
Optertype GetOpterTop(Opter* S);//取符号栈顶
int FullOpter(Opter* S);//是否为满栈

void shuru(char* biaoda);  //输入算数表达式
int getIndex(char theta);   //获取theta所对应的索引
char getPriority(char theta1, char theta2);   //获取theta1与theta2之间的优先级
void caozuo(char* biaoda, Opnd* shu, Opter* fu);//算数操作主函数
void fuhao(Opnd* shu, Opter* fu, Optertype hao);//符号优先级判断和操作
void yunsuan(Opnd* shu, Opter* fu);//取两栈顶元素进行加减乘除

int main()
{
    Opnd caozuoshu;
    Opter caozuofu;
    Opnd* shu = &caozuoshu;
    Opter* fu = &caozuofu;
    char biaoda[MAX * 2];//表达式
    InitOpnd(shu);  //初始化
    InitOpter(fu);   //初始化
    PushOpter(fu, '#'); //先压入一个'#'
    shuru(biaoda);     //输入表达式
    caozuo(biaoda, shu, fu); //表达式操作
    printf("结果为%d", GetOpndTop(shu));
    return 0;
}

void  InitOpnd(Opnd* S) //初始化数字栈
{
    S->base = (Opndtype*)malloc(MAX * sizeof(Opndtype));
    if (S->base == NULL)
    {
        printf("申请失败\n");
        exit(-1);
    }
    S->stacksize = MAX;
    S->top = S->base - 1;
}

void PushOpnd(Opnd* S, Opndtype Data) //压入数字
{
    if (FullOpnd(S))
    {
        S->base = (Opndtype*)realloc(S->base, 2 * MAX * sizeof(Opndtype));
        if (S->base)  exit(-1);
        S->top = S->base + S->stacksize;
        S->stacksize += MAX;
    }
    else
    {
        *(++S->top) = Data;
    }
}

Opndtype GetOpndTop(Opnd* S)//取数字栈顶
{
    if (S->top < S->base)
    {
        return -1;
    }
    else
    {
        return  *S->top;
    }

}
Opndtype PopOpnd(Opnd* S)//数字出栈
{
    Opndtype yy = 0;
    if (S->top >= S->base)
    {
        yy = *S->top--;
        return yy;
    }
}

int FullOpnd(Opnd* S) //判断数字栈是否为空
{
    if (S->top - S->base == S->stacksize - 1)
        return 1;
    else
        return 0;
}

//符号栈管理
void  InitOpter(Opter* S)//初始符号栈
{
    S->base = (Optertype*)malloc(MAX * sizeof(Optertype));
    if (S->base == NULL)
    {
        printf("申请失败\n");
        exit(-1);
    }
    S->stacksize = MAX;
    S->top = S->base - 1;
}

void PushOpter(Opter* S, Optertype Data)//符号压入
{
    if (FullOpter(S))
    {
        S->base = (Optertype*)realloc(S->base, 2 * MAX * sizeof(Optertype));
        if (S->base)  exit(-1);
        S->top = S->base + S->stacksize;
        S->stacksize += MAX;
    }
    else
    {
        *(++S->top) = Data;
    }
}

Optertype PopOpter(Opter* S)//符号出栈
{
    Optertype yy = 0;
    if (S->top >= S->base)
    {
        yy = *S->top--;
        return yy;
    }
}

Optertype GetOpterTop(Opter* S)//取符号栈顶
{
    if (S->top < S->base)
    {
        return -1;
    }
    else
    {
        return  *S->top;
    }
}

int FullOpter(Opter* S)//判断符号栈满
{
    if (S->top - S->base == S->stacksize - 1)
        return 1;
    else
        return 0;
}

void shuru(char* biaoda)//计算式输入
{
    printf("请输入你要输入的表达式(请以‘#’结尾)：\n");
    gets(biaoda);

}

int getIndex(char theta)   //获取theta所对应的索引
{
    int index = 0;
    switch (theta)
    {
    case '+':
        index = 0;
        break;
    case '-':
        index = 1;
        break;
    case '*':
        index = 2;
        break;
    case '/':
        index = 3;
        break;
    case '(':
        index = 4;
        break;
    case ')':
        index = 5;
        break;
    case '#':
        index = 6;
    default:
        break;
    }
    return index;
}

char getPriority(char theta1, char theta2)   //获取theta1与theta2之间的优先级
{
    const char priority[][7] =     //算符间的优先级关系
    {
        { '>','>','<','<','<','>','>' },
        { '>','>','<','<','<','>','>' },
        { '>','>','>','>','<','>','>' },
        { '>','>','>','>','<','>','>' },
        { '<','<','<','<','<','=','0' },
        { '>','>','>','>','0','>','>' },
        { '<','<','<','<','<','0','=' },
    };

    int index1 = getIndex(theta1);
    int index2 = getIndex(theta2);
    return priority[index1][index2];
}

void caozuo(char* biaoda, Opnd* shu, Opter* fu)//biaoda 为表达式， shu为数字栈指针， fu为符号栈指针
{
    char* shun = biaoda;
    //为了处理首位数字是负数
    Opndtype zi = 0;
    zi = atoi(shun);
    PushOpnd(shu, zi);
    while (*shun > '0' && *shun < '9') shun++;

    while (*shun != NULL) //处理余下字符表达式
    {
        if (*shun > '0' && *shun < '9')
        {
            zi = atoi(shun);
            PushOpnd(shu, zi);
            while (*shun > '0' && *shun < '9') shun++;
        }
        else
        {
            if (*shun == '-' && (*(shun - 1) < '0' || *(shun - 1) > '9'))//判断是减号还是负号
            {
                zi = atoi(shun);
                PushOpnd(shu, zi);
                shun++;
                while (*shun > '0' && *shun < '9') shun++;
            }
            fuhao(shu, fu, *shun);
            shun++;
        }
    }
}
void fuhao(Opnd* shu, Opter* fu, Optertype hao)//判断栈顶符号优先级
{
    switch (getPriority(GetOpterTop(fu), hao))
    {
    case '<':
        PushOpter(fu, hao);
        break;
    case '=':
        PopOpter(fu);
        break;
    case'>':
        yunsuan(shu, fu);
        fuhao(shu, fu, hao); //继续进行判断操作
        break;
    }
}
void yunsuan(Opnd* shu, Opter* fu)//当栈顶优先级较高时进行运算
{
    Opndtype a, b, c;
    Optertype d;
    b = PopOpnd(shu);
    a = PopOpnd(shu);
    switch (PopOpter(fu))
    {
    case'+':
        c = a + b;
        break;
    case'-':
        c = a - b;
        break;
    case'*':
        c = a * b;
        break;
    case'/':
        c = a / b;
        break;
    }
    PushOpnd(shu, c);
}
