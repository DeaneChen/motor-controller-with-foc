/*
 * @Author       : LuHeQiu
 * @Date         : 2021-12-28 17:18:55
 * @LastEditTime : 2022-01-03 14:11:41
 * @LastEditors  : DeaneChen
 * @Description  : 
 * @FilePath     : \MainController\Hardware\spi.h
 * @HomePage     : https://www.luheqiu.com
 */
#ifndef __SPI_H__
#define __SPI_H__

#include "main.h"

#define SPI_SCK_SET()        HAL_GPIO_WritePin(SPI_SCK_GPIO_Port, SPI_SCK_Pin, GPIO_PIN_SET)
#define SPI_SCK_RESET()      HAL_GPIO_WritePin(SPI_SCK_GPIO_Port, SPI_SCK_Pin, GPIO_PIN_RESET)
#define SPI_SDO_SET()        HAL_GPIO_WritePin(SPI_SDO_GPIO_Port, SPI_SDO_Pin, GPIO_PIN_SET)
#define SPI_SDO_RESET()      HAL_GPIO_WritePin(SPI_SDO_GPIO_Port, SPI_SDO_Pin, GPIO_PIN_RESET)

typedef  uint8_t  SPI8BitsType;
typedef uint16_t  SPI16BitsType;

/*    SPI总线状态     */
/* SPI_FREE: 总线空闲 */
/* SPI_BUSY: 总线忙   */
typedef enum{
  SPI_FREE,
  SPI_BUSY
}SPI_State_e;


typedef struct{
  SPI_State_e spi_state;    //总线状态

  /**
   * @brief  以阻塞的方式发送带一位数据控制字的八位数据（共计9位）
   * @param  {8Bits} dataType 数据控制字。可在八位数据前增加一个控制字，1或0。
   *                            例如在3-wire SPI的OLED通讯中，每帧有九位：D/C#，D7~D0
   * @param  {8Bits} *pData   数据指针。指向所存的数据数组
   * @param  {16Bits} Size    数据大小。警告：该处未作溢出检测！
   * @retval none
   */
  void (*Transmit8BitsWithCD)(SPI8BitsType dataType, SPI8BitsType *pData, SPI16BitsType size);
}SPI_t;


/* 声明为外部变量，便于外部调用 */
/* SPI总线对象 */
extern SPI_t spi;


#endif
