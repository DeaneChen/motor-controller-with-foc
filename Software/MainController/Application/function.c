/*
 * @Author       : LuHeQiu
 * @Date         : 2022-01-14 20:09:32
 * @LastEditTime : 2022-03-02 17:01:58
 * @LastEditors  : DeaneChen
 * @Description  : 
 * @FilePath     : \motor-controller-with-foc\Software\MainController\Application\function.c
 * @HomePage     : https://www.luheqiu.com
 */

#include "function.h"

/**
 * @brief: 增量式PID
 * @param:
 * @retval: None
 */
void INCPID_Update(INCPIDController_t *PID,float target,float input){
    
    //误差值计算
    float error=target-input;
    
    //误差值存储
    PID->prevError=PID->lastError;
    PID->lastError=error;
    
    //PID输出计算
    float output =( PID->output + PID->pidParam.kp*( error-PID->lastError)
                                + PID->pidParam.ki*( error+PID->lastError)*0.5f
                                + PID->pidParam.kd*((error-PID->lastError)-(PID->lastError-PID->prevError)) );
    
    //PID输出幅限
    output = Constrain(output,PID->outMINLimit,PID->outMAXLimit);
    
    //PID输出更新
    PID->output = output;
}

/**
 * @brief: 位置式PID
 * @param:
 * @retval: None
 */
float POSPID_Update(POSPIDController_t *PID,float target,float input,float dt){
    
    //误差值计算
    float error=target-input;
    
    //比例项
    float pTerm=PID->pidParam.kp*error;
    
    //积分项
    PID->iTerm+=(PID->pidParam.ki*(error+PID->lastError)*0.5f*dt);
    PID->iTerm=Constrain(PID->iTerm,0-PID->integrationLimit,PID->integrationLimit);
    
    //微分项
    float dTerm=PID->pidParam.kd*(error-PID->lastError)/dt;
    
    //误差值存储
    PID->lastError=error;
    
    //PID输出计算
    float output = pTerm + PID->iTerm + dTerm;
    
    //输出值滤波
	output = PID->FilterPercent * output + (1 - PID->FilterPercent)* PID->output;
    
    //PID输出幅限
    output = Constrain(output,PID->outMINLimit,PID->outMAXLimit);
    
    //PID输出更新
    PID->output = output;
    
    return output;
}



/**
 * @brief  atoi ( ascii to integer) 为把字符串转换成整型数的一个函数
 * @param  nptr 字符串指针
 * @retval 
 */
int atoi (const char* nptr){
    int n = 0, sign;
    sign = (*nptr == '-') ? (-1) : (1);  //判断符号
    if ( (*nptr) == '+' || (*nptr) == '-' ) //跳过符号
        nptr++;
    for ( ; *nptr != '\0'; nptr++ ){
        if( (*nptr)<'0' || (*nptr)>'9' )
            break;
        n = 10 * n + ((*nptr) - '0');
    }

    return sign * n;
}


//int strcmp(const unsigned char *str1, const unsigned char *str2)
//{
//    while(*str1 && (*str1==*str2)){
//        ++str1;
//        ++str2;
//    }
//    return *str1 - *str2;
//}


//int strlen(const char *str)
//{
//    const char *cp =  str;
//    while (*cp++ );
//    return (cp - str - 1 );
//}

