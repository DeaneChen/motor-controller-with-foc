/*
 * @Author       : LuHeQiu
 * @Date         : 2022-01-21 22:14:12
 * @LastEditTime : 2022-03-31 18:55:00
 * @LastEditors  : DeaneChen
 * @Description  : 
 * @FilePath     : \motor-controller-with-foc\Software\MainController\Hardware\drv8303.c
 * @HomePage     : https://www.luheqiu.com
 */
#include "drv8303.h"
#include "function.h"

#define DRV8303_CONFIG  GATE_DRIVE_PEAK_CURRENT_0A7, PWM6_INPUTS_MODE, CURRENT_LIMIT, OC_0V138, REPORT_OT_AND_nOCTW, GAIN_10, CYCLE_BY_CYCLE

void LoadConfig_0(void);
void LoadConfig_1(void);
void ReadRegister_0(DRV8303_8BitsType regIndex, DRV8303_8BitsType *regData);
void ReadRegister_1(DRV8303_8BitsType regIndex, DRV8303_8BitsType *regData);

drv8303_t drv8303[DRV8303_NUM] = {{DRV8303_CONFIG, LoadConfig_0, ReadRegister_0 },
                                  {DRV8303_CONFIG, LoadConfig_1, ReadRegister_1 }};


void ConfigToCommand(DRV8303_8BitsType drv8303Index,DRV8303_8BitsType* command){
    
    /* 配置变量 */
    DRV8303_16BitsType config[2];

    /* 配置格式化 */
    config[0] = ( (drv8303[drv8303Index].oc_adj_set << 6) + (drv8303[drv8303Index].ocp_mode << 4) + (drv8303[drv8303Index].pwm_mode << 3) + (drv8303[drv8303Index].gate_current) & 0x07FB);
    config[1] = ( (drv8303[drv8303Index].oc_toff << 6) + (drv8303[drv8303Index].gain << 2) + (drv8303[drv8303Index].octw_mode) & 0x004F);

    /* 设置DRV8303的控制寄存器1 */
    command[0] = (0x00 << 7) + (0x02 << 3) + ((config[0] & 0x0700) >> 8);
    command[1] = config[0] & 0x00FF;
    /* 设置DRV8303的控制寄存器2 */
    command[2] = (0x00 << 7) + (0x03 << 3) + ((config[1] & 0x0700) >> 8);
    command[3] = config[1] & 0x00FF;

}

void LoadConfig_0(void){

    /* 配置变量与命令变量 */
    DRV8303_8BitsType command[4];
    DRV8303_8BitsType response[4];

    /* 配置字 转 通信指令 */
    ConfigToCommand(0, command);

    /* 设置DRV8303的控制寄存器1 */
    DRV8303_0_CS();
    DRV8303_WRITE_BYTE(command, 2);
    DRV8303_0_NCS();
    DRV8303_0_CS();
    DRV8303_READ_BYTE(response, 2);
    DRV8303_0_NCS();

    /* 设置DRV8303的控制寄存器2 */
    DRV8303_0_CS();
    DRV8303_WRITE_BYTE(command+2, 2);
    DRV8303_0_NCS();
    DRV8303_0_CS();
    DRV8303_READ_BYTE(response+2, 2);
    DRV8303_0_NCS();


    /* 数据上报 */
    for (DRV8303_8BitsType i=0; i < 4;i++){
        DRV8303_8BitsType buf[5]= "0x   ";
        if(i==3){
            buf[4] = '\n';
        }
        itoa(response[i], buf + 2, 16);
        SPI_REPORT(buf, 5);
    }

}


void LoadConfig_1(void){

    /* 配置变量与命令变量 */
    DRV8303_8BitsType command[4];
    DRV8303_8BitsType response[5];

    /* 配置字 转 通信指令 */
    ConfigToCommand(1, command);

    DRV8303_1_CS();
    DRV8303_WRITE_BYTE(command, 2);
    DRV8303_1_NCS();
    DRV8303_1_CS();
    DRV8303_READ_BYTE(response, 2);
    DRV8303_1_NCS();
    //response[2] = '\n';   
    //SPI_REPORT(response,3);

    DRV8303_1_CS();
    DRV8303_WRITE_BYTE(command+2, 2);
    DRV8303_1_NCS();
    DRV8303_1_CS();
    DRV8303_READ_BYTE(response+2, 2);
    DRV8303_1_NCS();

    for (DRV8303_8BitsType i=0; i < 4;i++){
        DRV8303_8BitsType buf[5]= "0x   ";
        if(i==3){
            buf[4] = '\n';
        }
        itoa(response[i], buf + 2, 16);
        SPI_REPORT(buf, 5);
    }
    
}

void ReadRegister_0(DRV8303_8BitsType regIndex, DRV8303_8BitsType* regData){
    
    /* 配置变量与命令变量 */
    DRV8303_8BitsType command[2];
    DRV8303_8BitsType response[3];

    command[0] = 0x80 | ((regIndex & 0x03) << 3);
    command[1] = 0x00;

    /* 读取寄存器的值 */
    DRV8303_0_CS();
    DRV8303_WRITE_BYTE(command, 2);
    DRV8303_0_NCS();
    DRV8303_0_CS();
    DRV8303_READ_BYTE(response, 2);
    DRV8303_0_NCS();
    /* 数据上报 */
    for (DRV8303_8BitsType i=0; i < 2;i++){
        DRV8303_8BitsType buf[5]= "0x   ";
        if(i==1){
            buf[4] = '\n';
        }
        itoa(response[i], buf + 2, 16);
        SPI_REPORT(buf, 5);
    }

    regData[0] = response[0];
    regData[1] = response[1];
}

void ReadRegister_1(DRV8303_8BitsType regIndex, DRV8303_8BitsType* regData){
    
    /* 配置变量与命令变量 */
    DRV8303_8BitsType command[2];
    DRV8303_8BitsType response[3];

    command[0] = 0x80 | ((regIndex & 0x03) << 3);
    command[1] = 0x00;

    /* 读取寄存器的值 */
    DRV8303_1_CS();
    DRV8303_WRITE_BYTE(command, 2);
    DRV8303_1_NCS();
    DRV8303_1_CS();
    DRV8303_READ_BYTE(response, 2);
    DRV8303_1_NCS();
    /* 数据上报 */
    for (DRV8303_8BitsType i=0; i < 2;i++){
        DRV8303_8BitsType buf[5]= "0x   ";
        if(i==1){
            buf[4] = '\n';
        }
        itoa(response[i], buf + 2, 16);
        SPI_REPORT(buf, 5);
    }

    regData[0] = response[0];
    regData[1] = response[1];
}
