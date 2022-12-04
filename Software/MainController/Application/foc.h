/*
 * @Author       : LuHeQiu
 * @Date         : 2022-01-21 21:17:42
 * @LastEditTime : 2022-12-04 18:30:20
 * @LastEditors  : LuHeQiu
 * @Description  : 
 * @FilePath     : \motor-controller-with-foc\Software\MainController\Application\foc.h
 * @HomePage     : https://www.luheqiu.com
 */
#ifndef __FOC_H
#define __FOC_H

#include "main.h"

#define FOC_USE_CAL_CORE_FLOAT       /* 使用浮点计算核心 */
// #define FOC_USE_CAL_CORE_INT32    /* 使用int32计算核心（暂未设计） */
 
#define SVPWM_PERIOD (PWM_PEROID)      /* SVPWM的周期总长 当前值为5600 */

typedef uint8_t FOC8BitsType;

typedef int8_t  FOC8BitsNumber;
typedef int16_t FOC16BitsNumber;
typedef int32_t FOC32BitsNumber;

typedef struct Vector3{
#ifdef FOC_USE_CAL_CORE_FLOAT
    float x1, x2, x3;
#else
    FOC32BitsNumber x1, x2, x3;
#endif
}Vector3;


typedef struct Vector2{
#ifdef FOC_USE_CAL_CORE_FLOAT
    float x1, x2;
#else
    FOC32BitsNumber x1, x2;
#endif
}Vector2;

#ifdef FOC_USE_CAL_CORE_FLOAT
    typedef float Degree;
#else
    typedef int32_t Degree;
#endif


void FOC_ControllerTask(void);

/**
 * @brief  Clarke线性变换，将一个三相电流向量分解为二相正交向量
 * @param  input 被变换的三相电流向量，是一个三维的向量
 * @param  output 变换得到的二相正交电流向量，是一个二维的向量
 * @retval none
 */
void ClarkeTransformaion(Vector3 *input, Vector2 *output);

/**
 * @brief  Park线性变换，将一个二相静止坐标系变换到二相旋转坐标系
 * @param  input 被变换的二相静止电流向量
 * @param  theta 角度
 * @param  output 变换得到的二相旋转电流向量
 * @retval none
 */
void ParkTransformaion(Vector2 *input, Degree theta, Vector2 *output);

/**
 * @brief  Park反变换，将一个二相旋转坐标系变换到二相静止坐标系
 * @param  input 被变换的二相旋转电流向量
 * @param  theta 角度
 * @param  output 变换得到的二相静止电流向量
 * @retval none
 */
void RevParkTransformaion(Vector2 *input, Degree theta ,Vector2 *output);

/**
 * @brief  SVPWM计算函数、根据静止二相坐标系下的电压向量，计算出PWM中心对齐模式下U、V、W三相的接通时间，即脉冲宽度。
 *         该函数需要一个标记PWM周期长度的宏定义 SVPWM_PERIOD ，在本例中PWM频率为15KHz、周期为5600个单位。
 *         注意：仅仅适用于PWM中心对齐模式的应用场景。
 * @param  input 被变换的二相静止电压向量
 * @param  modulation SVPWM调制度，表示了期望输出的大小
 * @param  output 输出的SVPWM三相脉冲宽度（PWM中心对齐模式下）
 * @retval 
 */
void SVPWM(Vector2* input, float modulation, Vector3* output);

/* 基于幅角计算的 SVPWM 算法，并不打算使用，仅仅用作耗时对比 */
void SVPWM2(Vector2* input, float modulation, Vector3* output);

#endif
