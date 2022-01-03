/*
 * @Author       : LuHeQiu
 * @Date         : 2021-12-28 15:20:02
 * @LastEditTime : 2022-01-03 14:12:02
 * @LastEditors  : DeaneChen
 * @Description  : 
 * @FilePath     : \MainController\Hardware\screen_drive.h
 * @HomePage     : https://www.luheqiu.com
 */
#ifndef __SCREEN_DRIVE_H__
#define __SCREEN_DRIVE_H__

#include "main.h"
#include "spi.h"

/*  变量类型与所需的函数接口配置 - BEGIN  */
typedef uint8_t  ScreenDrive8BitsType;
typedef uint16_t ScreenDrive16BitsType;

#define Screen_WriteByte(dataType,pData,size)  spi.Transmit8BitsWithCD(dataType,pData,size)  /* 发送数据的函数接口 */
/*  变量类型与所需的函数接口配置 - END  */


/*  屏幕设备专有定义配置 - BEGIN  */
/* OLED 128x64 */
#define OLED_CMD  0	/* 写命令 */
#define OLED_DATA 1	/* 写数据 */
#define OLED_CS()   HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET)   /* 片选 */
#define OLED_NCS()  HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET)     /* 取消片选 */
/*  屏幕设备专有定义配置 - END  */


/**
 * @brief  屏幕结构体
 */
typedef struct{

  ScreenDrive8BitsType device_type;
  
  /**
   * @brief  屏幕初始化函数
   * @param  none
   * @retval none
   */
  void (*Init)(void);

  /**
   * @brief  以阻塞的方式向屏幕发送一段带控制字的数据
   * @param  {8Bits} dataType 数据控制字。可在八位数据前增加一个控制字，1或0。
   *                            例如在3-wire SPI的OLED通讯中，每帧有九位：D/C#，D7~D0
   * @param  {8Bits} *pData   数据指针。指向所存的数据数组
   * @param  {16Bits} Size    数据大小。警告：该处未作溢出检测！
   * @retval none
   */
  void (*WriteByteWithCD)(ScreenDrive8BitsType dataType, ScreenDrive8BitsType *pData, ScreenDrive16BitsType size);

  /**
   * @brief  在屏幕坐标x，y处绘制一个点
   * @param  {16Bits} x
   * @param  {16Bits} y
   * @retval none
   */
  void (*Draw)(ScreenDrive16BitsType x, ScreenDrive16BitsType y);
    
}screen_t;


/* 声明为外部变量，便于调用 */
/* OLED显示屏对象 */
extern screen_t oled;    

#endif
