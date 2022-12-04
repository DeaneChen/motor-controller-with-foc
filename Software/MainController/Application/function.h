/*
 * @Author       : LuHeQiu
 * @Date         : 2022-01-12 22:33:07
 * @LastEditTime : 2022-12-04 18:19:14
 * @LastEditors  : LuHeQiu
 * @Description  : 
 * @FilePath     : \motor-controller-with-foc\Software\MainController\Application\function.h
 * @HomePage     : https://www.luheqiu.com
 */
#ifndef __FUNCTION_H
#define __FUNCTION_H

#include "main.h"
#include "usart.h"

typedef  int32_t Fun32BitsNumber;

#define Abs(value)                 (((value)>=0)?(value):(0-(value)))
#define Constrain(input,low,high)  ((input)<(low)?(low):((input)>(high)?(high):(input))) 
#define Max(A,B)                   ((A)>=(B)?(A):(B))
#define Min(A,B)                   ((A)<=(B)?(A):(B))

#define PI    3.14159265f

/* extern float SIN_TABLE[]; */

// PID Part
typedef struct{
    float kp,ki,kd;         //PID参数
}PIDParam_t;

typedef struct{

    PIDParam_t pidParam;      //PID参数
    
    float lastError;        //上次误差
    float prevError;        //前次误差
    float output;           //输出
    float outMINLimit;      //输出最大值限制
    float outMAXLimit;      //输出最小值限制
    
}INCPIDController_t;          

typedef struct{

    PIDParam_t pidParam;      //PID参数
    
    float lastError;        //上次误差
    
    float iTerm;            //积分项
	float integrationLimit; //积分幅限
    
	float FilterPercent;    //一阶低通滤波系数(0,1]
    float output;           //输出
    float outMINLimit;      //输出最大值限制
    float outMAXLimit;      //输出最小值限制
    
}POSPIDController_t;  

void INCPID_Update(INCPIDController_t *PID,float target,float input);

/**
 * @brief  位置式PID
 * @param  PID     PID控制器指针
 * @param  target  给定值
 * @param  input   当前值
 * @param  dt      控制周期
 * @retval 
 */
float POSPID_Update(POSPIDController_t *PID,float target,float input,float dt);


/**
* @brief  atoi ( ascii to integer) 为把字符串转换成整型数的一个函数
* @param  nptr 字符串指针
* @retval 被转换完成的整型数
*/
int atoi(const char *nptr);

/**
* @brief  itoa ( integer to ascii) 为把整型数转换为字符串的一个函数
* @param  value 待转换的
* @param  string 存储转换结果的指针，未作溢出处理，请自己确保开辟的空间足够
* @param  radix 转换的进制，例如 10 为按照10进制转换。
* @retval 转换成的字符串(不推荐使用)
*/
char *itoa(int value, char *string, int radix);

///**
// * @brief  字符串比较函数
// * @param  str1 字符串1
// * @param  str2 字符串2
// * @retval 根据第一个字符串与第二个字符串相等、大于或小于，返回0、大于0或小于0
// */
//int strcmp(const unsigned char *str1, const unsigned char *str2);

///**
// * @brief  获取字符串长度函数
// * @param  str 字符串
// * @retval 字符串长度
// */
//int strlen(const unsigned char *str);


/**
 * @brief  基于查找表(look-up table)的sin函数，使用一个大小为256的表扩展映射四倍用于sin函数查找。
 *         (sin/cos共用同一张容量为256个float的表)
 * @param  theta 弧度制角度
 * @retval sin(theta)
 */
float SinByLut(float theta);

/**
 * @brief  基于查找表(look-up table)的线性插值版sin函数，使用一个大小为256的表扩展映射四倍用于sin函数查找。
 *         (sin/cos共用同一张容量为256个float的表)
 * @param  theta 弧度制角度
 * @retval sin(theta)
 */
float SinByLinearLut(float theta);

/**
 * @brief  基于查找表(look-up table)的cos函数，使用一个大小为256的表扩展映射四倍用于cos函数查找。
 *         (sin/cos共用同一张容量为256个float的表)
 * @param  theta 弧度制角度
 * @retval cos(theta)
 */
float CosByLut(float theta);

/**
 * @brief  基于查找表(look-up table)的线性插值版cos函数，使用一个大小为256的表扩展映射四倍用于cos函数查找。
 *         (sin/cos共用同一张容量为256个float的表)
 * @param  theta 弧度制角度
 * @retval sin(theta)
 */
float CosByLinearLut(float theta);

#endif
