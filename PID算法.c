#include "test_c.h"
typedef struct PID
{
 double SetPoint;    //设定目标  
 double Proportion;  //比例常数 
 double Integral;    //积分常数 
 double Derivative;  //微分常数 
 double LastError;   // Error[-1]
 double PrevError;   // Error[-2]
 double SumError;    // sums of Errors
}PID;
/*===============PID计算函数====================*/
double PIDCalc( PID *pp, double NextPoint )
{
    double dError, Error;
    Error = pp->SetPoint - NextPoint; // 偏差
    pp->SumError += Error; // 积分
    dError = pp->LastError - pp->PrevError; // 当前微分
    pp->PrevError = pp->LastError;
    pp->LastError = Error;
    return (pp->Proportion * Error// 比例项
    + pp->Integral * pp->SumError // 积分项
    + pp->Derivative * dError );  // 微分项
}
/*===============PID结构体变量初始化函数====================*/
void PIDInit (PID *pp)
{
memset ( pp,0,sizeof(PID));     // PID 结构体成员清零, 这种方法是对较大的结构体后数组清零的方法
}
/*===============读取输入变量函数（在此设定为固定值100）====================*/
double sensor (void)  
{
return 100.0;
}
/*====================输出变量控制函数======================*/
void actuator(double rDelta)  
{
}
int main(void)
{
    PID sPID; // PID Control Structure
    double rOut; // PID Response (Output)
    double rIn; // PID Feedback (Input)
    PIDInit ( &sPID ); // Initialize Structure
    sPID.Proportion = 0.5;   // 设定比例常数 0.5 
    sPID.Integral =   0.5;   // 设定积分常数 0.5
    sPID.Derivative = 0.0; // 设定微分常数 0.0
    sPID.SetPoint = 100.0; // 设定目标值 100.0
    for (;;)
    {                                   // Mock Up of PID Processing
        rIn = sensor ();                // 读取输入变量函数(Read Input )
        rOut = PIDCalc ( &sPID,rIn );   // PID计算函数(Perform PID Interation)
        actuator ( rOut );              // 输出变量控制函数(Effect Needed Changes)
    }
}
//void Swp_Num(int *i, int *j)
//{
//  int temp = 0;
//  temp = *i;
//  *i = *j;
//  *j = temp;
//}
