/*
 * @Author       : LuHeQiu
 * @Date         : 2022-01-21 22:14:20
 * @LastEditTime : 2022-03-03 16:27:11
 * @LastEditors  : DeaneChen
 * @Description  : 
 * @FilePath     : \motor-controller-with-foc\Software\MainController\Hardware\drv8303.h
 * @HomePage     : https://www.luheqiu.com
 */
#ifndef __DRV8303_H
#define __DRV8303_H

#include "main.h"
#include "spi.h"
#include "usbd_cdc_if.h"

typedef uint8_t   DRV8303_8BitsType;
typedef uint16_t DRV8303_16BitsType;

#define SPI_DEBUG(buf)      CDC_Transmit_FS((DRV8303_8BitsType*)buf,strlen(buf))
#define SPI_REPORT(buf,len) CDC_Transmit_FS((DRV8303_8BitsType*)buf,len)

#define DRV8303_NUM 2   /* DRV8303数量 */

#define DRV8303_WRITE_BYTE(pData,size)  spi.Transmit8Bits(pData,size)  /* 发送数据的函数接口 */
#define DRV8303_READ_BYTE(pData,size)   spi.Receive8Bits(pData,size)   /* 读取数据的函数接口 */

#define DRV8303_0_CS()   HAL_GPIO_WritePin(M0_CS_GPIO_Port, M0_CS_Pin, GPIO_PIN_RESET)   /* 片选 */
#define DRV8303_0_NCS()  HAL_GPIO_WritePin(M0_CS_GPIO_Port, M0_CS_Pin, GPIO_PIN_SET)     /* 取消片选 */

#define DRV8303_1_CS()   HAL_GPIO_WritePin(M1_CS_GPIO_Port, M1_CS_Pin, GPIO_PIN_RESET)   /* 片选 */
#define DRV8303_1_NCS()  HAL_GPIO_WritePin(M1_CS_GPIO_Port, M1_CS_Pin, GPIO_PIN_SET)     /* 取消片选 */

/* ADDRESS 0x02 */

/* 栅极电流大小设置，设定了DRV8303驱动MOS管栅极的引脚的输入输出电流限制 */
/* 引脚：GH_A，GH_B，GH_C，GL_A，GL_B，GL_C */
/* 该设置影响了栅极驱动电流，从而影响MOS管栅极的输入寄生电容充电时间，影响开关速度 */
/* GATE_DRIVE_PEAK_CURRENT_1A7 : 最大输出电流1.7A  最大输入电流2.3A */
/* GATE_DRIVE_PEAK_CURRENT_0A7 : 最大输出电流0.7A  最大输入电流1.0A */
/* GATE_DRIVE_PEAK_CURRENT_0A25: 最大输出电流0.25A 最大输入电流0.5A */
/* 默认为0 */
typedef enum{
    GATE_DRIVE_PEAK_CURRENT_1A7 = 0,
    GATE_DRIVE_PEAK_CURRENT_0A7,
    GATE_DRIVE_PEAK_CURRENT_0A25
} drv8303_gate_current_e;

/* 三相PWM控制模式 */
/* 三相栅极驱动可以提供30mA的平均驱动电流 */ 
/* 当所选的MOSFET器件栅极寄生电容为25nC时支持最大200KHz的开关频率 */
/* PWM6_INPUTS_MODE : 6路PWM波控制，由前级控制器负责高低半桥的开关 */
/* PWM3_INPUTS_MODE : 3路PWM波控制，由前级控制器负责高半桥的开关，DRV8303自动对低半桥执行反逻辑控制 */
/* 默认为0 */
typedef enum{
    PWM6_INPUTS_MODE = 0,
    PWM3_INPUTS_MODE
} drv8303_pwm_mode_e;

/* 过流保护(Overcurrent Protection)和上报(Reporting)模式设置 */
/* 为了保护功率级不因电流过大而损坏，在DRV8303中实现了VDS传感电路。 */
/* 在超过电压阈值时触发过流保护功能。电压阈值是通过SPI寄存器编程的。 */
/* 过流保护只能作为一种保护方案使用;它不能作为一个精确的电流调节方案。 VDS行程点的跨通道公差可达20%。 */
/* I = U / R_DS(ON) 过流阈值为电压阈值除以MOSFET导通电阻 */
/* CURRENT_LIMIT : 电流限流模式，过流时限制电流但并不关闭设备，过流信息从nOCTW引脚和内部寄存器上报 */
/* OOC_LATCH_SHUTDOWNC : 过流锁存关闭模式，过流时关闭对应的半桥MOS，过流信息从nFault引脚和内部寄存器上报 */
/* 此时需要对寄存器gate_reset位写1或者在EN_GATE引脚输出reset复位脉冲进行状态复位，恢复控制 */
/* REPORT_ONLY : 只上报模式，过流时不作处理，只从nOCTW引脚和内部状态寄存器上报过流信息 */
/* OC_DISABLE : 关闭过流检测*/
/* 默认为0 */
typedef enum{
    CURRENT_LIMIT = 0,
    OC_LATCH_SHUTDOWN,
    REPORT_ONLY,
    OC_DISABLE
} drv8303_ocp_mode_e;

/* 过压阈值 */
/* 过流检测通过检测Vds实现，即漏源电压。漏源电流等于漏源电压除以MOS管导通电阻 */
/* 因此，I = U / R_DS(ON) 。其中R_DS(ON)为MOSFET导通电阻 */
/* 例如：对于6.5mohm导通电阻的MOSFET，设计过电流为20A，则电压阈值为20A x 0.0065 = 0.13V */
/* 特别的，对于1.679V以上的电压阈值，仅限在工作电压大于8V时可以被设置 */
typedef enum{
    OC_0V060 = 0,
    OC_0V068, OC_0V076, OC_0V086, OC_0V097, OC_0V109, OC_0V123, 
    OC_0V138, OC_0V155, OC_0V175, OC_0V197, OC_0V222, OC_0V250, 
    OC_0V282, OC_0V317, OC_0V358, OC_0V403, OC_0V454, OC_0V511, 
    OC_0V576, OC_0V648, OC_0V730, OC_0V822, OC_0V926, OC_1V043, 
    OC_1V175, OC_1V324, OC_1V491, OC_1V679, OC_1V892, OC_2V131, OC_2V400
} drv8303_oc_adj_set_e;

/* ADDRESS 0x03 */

/* 错误上报模式设置 */
/* REPORT_OT_AND_nOCTW : 同时在状态寄存器和nOCTW引脚上报信息 */
/* REPORT_OT_ONLY : 只在状态寄存器上报 */
/* REPORT_OC_ONLY : 只在nOCTW引脚上报 */
/* 默认为0 */
typedef enum{
    REPORT_OT_AND_nOCTW = 0,
    REPORT_OT_ONLY,
    REPORT_OC_ONLY
} drv8303_octw_mode_e;

/* 电流检测放大倍数 */
/* 电流检测放大电路的放大倍数设置 */
/* GAIN_10 : 放大倍数为10 */
/* GAIN_20 : 放大倍数为20 */
/* GAIN_40 : 放大倍数为40 */
/* GAIN_80 : 放大倍数为80 */
/* 例: 设置放大倍数为GAIN_40，电机A相电流10A，采样电阻1mohm，电阻两端电压0.01V，则采样电压0.4V*/
/* 默认为0 */
typedef enum{
    GAIN_10 = 0,
    GAIN_20,
    GAIN_40,
    GAIN_80
} drv8303_gain_e;

/* 电流限流的控制模式设置 */
/* 该设置项只在 过流保护模式设置 被设置为 电流限流模式 时有效*/
/* CYCLE_BY_CYCLE : 逐周期模式，检测到过流时关闭MOSFET直到下一个周期*/
/* OFF_TIME : 倒计时模式，检测到过流时关闭64us，如果另一个MOSFET又出现过流则复位倒计时 */
/* 默认为0 */
typedef enum{
    CYCLE_BY_CYCLE = 0,
    OFF_TIME
} drv8303_oc_toff_e;


typedef struct{
    drv8303_gate_current_e  gate_current;
    drv8303_pwm_mode_e      pwm_mode;
    drv8303_ocp_mode_e      ocp_mode;
    drv8303_oc_adj_set_e    oc_adj_set;
    drv8303_octw_mode_e     octw_mode;
    drv8303_gain_e          gain;
    drv8303_oc_toff_e       oc_toff;

    /**
     * @brief  载入DRV8303配置函数，对结构体变量依次赋值后需要调用该函数，将配置数据写入DRV8303硬件
     * @param  
     * @retval 
     */
    void (*LoadConfig)(void);


    /**
     * @brief  寄存器读取函数，可以读取DRV8303四个寄存器的值
     * @param  regIndex 寄存器序号，取值范围0~3。
     *                  | 寄存器序号 |       0            1            2           3
     *                  |  寄存器名  |  状态寄存器0  状态寄存器1  控制寄存器0  控制寄存器1
     * @param  regData  读取到的寄存器值
     * @retval 
     */
    void (*ReadRegister)(DRV8303_8BitsType regIndex, DRV8303_8BitsType *regData);

} drv8303_t;

extern drv8303_t drv8303[DRV8303_NUM];

#endif
