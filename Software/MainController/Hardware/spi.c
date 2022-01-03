/*
 * @Author       : LuHeQiu
 * @Date         : 2021-12-28 17:18:35
 * @LastEditTime : 2021-12-29 23:35:18
 * @LastEditors  : DeaneChen
 * @Description  : 
 * @FilePath     : \MainController\Hardware\spi.c
 * @HomePage     : https://www.luheqiu.com
 */
#include "spi.h"

void Transmit8BitsWithCD(SPI8BitsType dataType, SPI8BitsType *pData, SPI16BitsType size);

SPI_t spi = {SPI_FREE, Transmit8BitsWithCD};


/**
 * @brief  以阻塞的方式发送带一位数据控制字的八位数据（共计9位）
 * @param  {8Bits} dataType 数据控制字。可在八位数据前增加一个控制字，1或0。
 *                            例如在3-wire SPI的OLED通讯中，每帧有九位：D/C#，D7~D0
 * @param  {8Bits} *pData   数据指针。指向所存的数据数组
 * @param  {16Bits} Size    数据大小。警告：该处未作溢出检测！
 * @retval none
 */
void Transmit8BitsWithCD(SPI8BitsType dataType, SPI8BitsType *pData, SPI16BitsType size){

  /* 发送pData所指向的，大小位size的所数据 */
  for (SPI8BitsType dataBytes = 0; dataBytes < size; dataBytes++,pData++){
    
    /* 发送数据控制字 */
    SPI_SCK_RESET();
    if(dataType)
      SPI_SDO_SET();
    else
      SPI_SDO_RESET();
    SPI_SCK_SET();

    /* 发送八位数据控制字 */
    SPI8BitsType data = *pData;
    for (SPI8BitsType i = 0; i < 8; i++){
      SPI_SCK_RESET();
      if( data & 0x80 )
        SPI_SDO_SET();
      else
        SPI_SDO_RESET();
      SPI_SCK_SET();
      data <<= 1;
    }
  }

}


