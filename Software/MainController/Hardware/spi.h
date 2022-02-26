/*
 * @Author       : LuHeQiu
 * @Date         : 2021-12-28 17:18:55
 * @LastEditTime : 2022-01-21 22:31:28
 * @LastEditors  : DeaneChen
 * @Description  : 
 * @FilePath     : \motor-controller-with-foc\Software\MainController\Hardware\spi.h
 * @HomePage     : https://www.luheqiu.com
 */
#ifndef __SPI_H
#define __SPI_H

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

  
  /**
   * @brief  总线状态，忙时将无法使用总线
   */
  SPI_State_e spi_state;    

  /**
   * @brief  以阻塞的方式发送带一位数据控制字的八位数据（共计9位）
   * @param  {8Bits} dataType 数据控制字。可在八位数据前增加一个控制字，1或0。
   *                            例如在3-wire SPI的OLED通讯中，每帧有九位：D/C#，D7~D0
   * @param  {8Bits} *pData   数据指针。指向所存的数据数组
   * @param  {16Bits} Size    数据大小。警告：该处未作溢出检测！
   * @retval {8bits} 状态码 0:成功传输 1:总线忙
   */
  SPI8BitsType (*Transmit8BitsWithCD)(SPI8BitsType dataType, SPI8BitsType *pData, SPI16BitsType size);


  /**
   * @brief  以阻塞的方式发送八位数据
   * @param  {8Bits} *pData   数据指针。指向所存的数据数组
   * @param  {16Bits} Size    数据大小。警告：该处未作溢出检测！
   * @retval {8bits} 状态码 0:成功传输 1:总线忙
   */
  SPI8BitsType (*Transmit8Bits)(SPI8BitsType *pData, SPI16BitsType size);

}SPI_t;


/* 声明为外部变量，便于外部调用 */
/* SPI总线对象 */
extern SPI_t spi;


#endif
