/*
 * @Author       : LuHeQiu
 * @Date         : 2022-01-12 17:34:46
 * @LastEditTime : 2022-01-21 21:22:27
 * @LastEditors  : DeaneChen
 * @Description  : 
 * @FilePath     : \motor-controller-with-foc\Software\MainController\Hardware\led.h
 * @HomePage     : https://www.luheqiu.com
 */
#ifndef __LED_H
#define __LED_H

#include "main.h"
#include "tim.h"

/*  变量类型与所需的函数接口配置 - BEGIN  */
typedef uint8_t  LED8BitsType;

/*  变量类型与所需的函数接口配置 - END  */

typedef struct{

    /**
     * @brief  查看LED是否已经点亮,请勿写入该变量
     */
    LED8BitsType isLight;

    /**
     * @brief  查看LED亮度,请勿写入该变量
     */
    LED8BitsType brightness;

    /**
     * @brief  开启LED灯
     */
    void (*ON)(void);

    /**
     * @brief  关闭LED灯FF
     */
    void (*OFF)(void);

    /**t
     * @brief  切换LED灯状态
     */
    void (*Shift)(void);

    /**
     * @brief  设置LED灯亮度
     * @param  {8bits} 亮度值(0%~100%)
     * @retval 
     */
    void (*SetBrightness)(LED8BitsType brightness);

}LED_t;

extern LED_t led;

/**
 * @brief  LED亮度控制中断
 * @param  none
 * @retval none
 */
void LED_PeriodElapsedCallback(void);

#endif
