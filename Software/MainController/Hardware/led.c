/*
 * @Author       : LuHeQiu
 * @Date         : 2022-01-12 17:34:34
 * @LastEditTime : 2022-01-12 18:12:10
 * @LastEditors  : DeaneChen
 * @Description  : 
 * @FilePath     : \MainController\Hardware\led.c
 * @HomePage     : https://www.luheqiu.com
 */
#include "led.h"


void ON(void);
void OFF(void);
void Shift(void);
void SetBrightness(LED8BitsType brightness);

LED_t led = {0, 4, ON, OFF, Shift, SetBrightness};


void ON(void){
    led.isLight = 1;
    HAL_TIM_Base_Start_IT(&htim6);
}
void OFF(void){
    led.isLight = 0;
    HAL_TIM_Base_Stop_IT(&htim6);
    HAL_GPIO_WritePin(FnLED_GPIO_Port, FnLED_Pin, GPIO_PIN_SET);
}
void Shift(void){
    led.isLight = !led.isLight;
    if(led.isLight){
    HAL_TIM_Base_Start_IT(&htim6);
    }else{
    HAL_TIM_Base_Stop_IT(&htim6);
        HAL_GPIO_WritePin(FnLED_GPIO_Port, FnLED_Pin, GPIO_PIN_SET);
    }
}

void SetBrightness(LED8BitsType brightness){
    if(brightness >= 100){
        brightness = 100;
    }else{
        led.brightness = brightness;
    }
}

/**
 * @brief  LED亮度控制中断
 * @param  none
 * @retval none
 */
void LED_PeriodElapsedCallback(void){
    static LED8BitsType led_bright_tick = 0;
    led_bright_tick += 1;
    if(led_bright_tick>=100){
        led_bright_tick = 0;
    }
    if(led_bright_tick == led.brightness){
        HAL_GPIO_WritePin(FnLED_GPIO_Port, FnLED_Pin, GPIO_PIN_SET);
    }else if(led_bright_tick == 0){
        HAL_GPIO_WritePin(FnLED_GPIO_Port, FnLED_Pin, GPIO_PIN_RESET);
    }
}
