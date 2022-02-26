/*
 * @Author       : LuHeQiu
 * @Date         : 2022-01-21 22:14:12
 * @LastEditTime : 2022-02-26 18:26:49
 * @LastEditors  : DeaneChen
 * @Description  : 
 * @FilePath     : \motor-controller-with-foc\Software\MainController\Hardware\drv8303.c
 * @HomePage     : https://www.luheqiu.com
 */
#include "drv8303.h"

#define DRV8303_CONFIG  GATE_DRIVE_PEAK_CURRENT_1A7, PWM6_INPUTS_MODE, CURRENT_LIMIT, OC_0V138, REPORT_OT_AND_nOCTW, GAIN_10, CYCLE_BY_CYCLE

void LoadConfig_0(void);
void LoadConfig_1(void);

drv8303_t drv8303[DRV8303_NUM] = {{DRV8303_CONFIG, LoadConfig_0},
                                  {DRV8303_CONFIG, LoadConfig_1} };


void LoadConfig_0(void){

    /* drv8303序号 */
    DRV8303_8BitsType drv8303_index = 0;

    /* 配置变量与命令变量 */
    DRV8303_16BitsType config[2];
    DRV8303_8BitsType command[2];

    /* 配置格式化 */
    config[0] = (drv8303[drv8303_index].oc_adj_set << 6 + drv8303[drv8303_index].ocp_mode << 4 + drv8303[drv8303_index].pwm_mode << 3 + drv8303[drv8303_index].gate_current) & 0x07FB;
    config[1] = (drv8303[drv8303_index].oc_toff << 6 + drv8303[drv8303_index].gain << 2 + drv8303[drv8303_index].octw_mode) & 0x004F;

    /* 设置DRV8303的控制寄存器1 */
    command[0] = 0x01 << 7 + 0x02 << 3 + (config[0] & 0x0700) >> 8;
    command[1] = config[0] & 0x00FF;
    DRV8303_0_CS();
    DRV8303_WRITE_BYTE(command, 2);
    DRV8303_0_NCS();

    /* 设置DRV8303的控制寄存器2 */
    command[0] = 0x01 << 7 + 0x03 << 3 + (config[1] & 0x0700) >> 8;
    command[1] = config[1] & 0x00FF;
    DRV8303_0_CS();
    DRV8303_WRITE_BYTE(command, 2);
    DRV8303_0_NCS();

}


void LoadConfig_1(void){

    /* drv8303序号 */
    DRV8303_8BitsType drv8303_index = 1;

    /* 配置变量与命令变量 */
    DRV8303_16BitsType config[2];
    DRV8303_8BitsType command[2];

    /* 配置格式化 */
    config[0] = (drv8303[drv8303_index].oc_adj_set << 6 + drv8303[drv8303_index].ocp_mode << 4 + drv8303[drv8303_index].pwm_mode << 3 + drv8303[drv8303_index].gate_current) & 0x07FB;
    config[1] = (drv8303[drv8303_index].oc_toff << 6 + drv8303[drv8303_index].gain << 2 + drv8303[drv8303_index].octw_mode) & 0x004F;

    command[0] = 0x01 << 7 + 0x02 << 3 + (config[0] & 0x0700) >> 8;
    command[1] = config[0] & 0x00FF;
    DRV8303_1_CS();
    DRV8303_WRITE_BYTE(command, 2);
    DRV8303_1_NCS();

    command[0] = 0x01 << 7 + 0x03 << 3 + (config[1] & 0x0700) >> 8;
    command[1] = config[1] & 0x00FF;
    DRV8303_1_CS();
    DRV8303_WRITE_BYTE(command, 2);
    DRV8303_1_NCS();
    
}

