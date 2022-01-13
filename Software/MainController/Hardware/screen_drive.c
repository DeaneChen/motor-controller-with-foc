/*
 * @Author       : LuHeQiu
 * @Date         : 2021-12-28 15:19:50
 * @LastEditTime : 2022-01-13 02:52:14
 * @LastEditors  : DeaneChen
 * @Description  : 
 * @FilePath     : \MainController\Hardware\screen_drive.c
 * @HomePage     : https://www.luheqiu.com
 */
#include "screen_drive.h"

/* OLED 128x64 -- BEGIN */
void OLED_Init(void);
void OLED_WriteByte(ScreenDrive8BitsType dataType,ScreenDrive8BitsType *pData,ScreenDrive16BitsType size);
#if (OLED_BUFFER_ENABLE==1)
void OLED_Draw(ScreenDrive16BitsType x, ScreenDrive16BitsType y, ScreenDrive8BitsType isLight);
#else
void OLED_Draw(ScreenDrive16BitsType x, ScreenDrive16BitsType y, ScreenDrive8BitsType data);
#endif
void OLED_ClearScreen(void);

#if (OLED_BUFFER_ENABLE==1)
ScreenDrive8BitsType oled_buffer[OLED_X_WIDTH][OLED_Y_WIDTH/8]; 
#endif

screen_t oled = {0,OLED_X_WIDTH,OLED_Y_WIDTH,OLED_Init,OLED_WriteByte,OLED_Draw};

void OLED_Init(void)
{
  //--关闭OLED显示

  //--设置行显示首地址
  //--设置对比度
  // 对比度:255
  //--设置水平翻转 
  //--设置垂直反转
  //--设置反色显示
  //--设置复用比为64行全部显示
  //--1/64 duty
  
  //--设置RAM行偏移 (0x00~0x3F)
  // 不偏移
  //--设置时钟及其分频比
  // 100 Frames/Sec
  //--set pre-charge period
  // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  //--设置COM硬件引脚映射
  // 不映射
  //--设置参考电压
  // VCC?

  //--设置内存地址模式 (0x00/0x01/0x02)
  // 页寻址模式
  //--set Charge Pump enable/disable
  //--set(0x10) disable
  // 设置全屏显示 (0xa4/0xa5)
  //--开启OLED屏幕
  //--开启显示

  // ScreenDrive8BitsType initData[29]={ 0xAE,
  //                                   0x40,0x81,0xFF,colume_dir_cmd,row_dir_cmd,0xA6,0xA8,0x3f,
  //                                   0xD3,0x00,0xd5,0x80,0xD9,0xF1,0xDA,0x12,0xDB,0x40,
  //                                   0x20,0x02,0x8D,0x14,0xA4,0xA6,0xAF,0xAF};


  ScreenDrive8BitsType colume_dir_cmd = 0xA0,row_dir_cmd = 0xC0,dis_rever_cmd=0xA6;
  #if (IS_COLUME_REVERSE==1)
     colume_dir_cmd = 0xA1;
  #endif
  #if (IS_ROW_REVERSE==1)
     row_dir_cmd = 0xC8;
  #endif
  #if (IS_DISPLAY_REVERSE==1)
     dis_rever_cmd = 0xA7;
  #endif
  

  ScreenDrive8BitsType initData[26]={ 0xAE,
                                      0x40,0x81,0xCF,colume_dir_cmd,row_dir_cmd,dis_rever_cmd,0xA8,0x3f,
                                      0xD3,0x00,0xd5,0x80,0xD9,0xF1,0xDA,0x12,0xDB,0x40,
                                      0x20,0x02,0x8D,0x14,0xA4,0xAF,0xAF};

  // ScreenDrive8BitsType initData[29]={0xAE,0x00,0x10,0x40,0x81,0xCF,0xA1,0xC8,0xA6,0xA8,
  //                                    0x3f,0xD3,0x00,0xd5,0x80,0xD9,0xF1,0xDA,0x12,0xDB,
  //                                    0x40,0x20,0x02,0x8D,0x14,0xA4,0xA6,0xAF,0xAF};
  
  OLED_Delay_ms(100); //建议上电时序-SEG/COM延时

  oled.WriteByteWithCD(0, initData, 26);

  OLED_ClearScreen();
  
}  

void OLED_WriteByte(ScreenDrive8BitsType dataType,ScreenDrive8BitsType *pData,ScreenDrive16BitsType size){
  OLED_CS();
  Screen_WriteByte(dataType, pData, size);
  OLED_NCS();
}

void OLED_ClearScreen(void)  
{  
  ScreenDrive8BitsType cmd[3]={0xb0,0x10,0x00};
  ScreenDrive8BitsType data  = 0x00;
  for(ScreenDrive8BitsType i=0;i<oled.y_width/8;i++){
    cmd[0]=0xb0+i;
    oled.WriteByteWithCD(OLED_CMD,cmd,3);
    for(ScreenDrive8BitsType n=0;n<oled.x_width;n++)oled.WriteByteWithCD(OLED_DATA,&data,1);
  }
}

#if (OLED_BUFFER_ENABLE==1)
void OLED_Draw(ScreenDrive16BitsType x, ScreenDrive16BitsType y,  ScreenDrive8BitsType isLight){
  
  if(x>oled.x_width) x %= (oled.x_width);
  if(y>oled.y_width) y %= (oled.y_width);
  
  ScreenDrive8BitsType cmd[3]={0xb0+y/8,((x&0xf0)>>4)|0x10,(x&0x0f)};
  ScreenDrive8BitsType data = 0x00;
  if(isLight){
    oled_buffer[x][y/8] |= (0x01<<(y%8));
    data  = oled_buffer[x][y/8];
  }else{
    oled_buffer[x][y/8] &= (0xFF ^ (0x01<<(y%8)));
    data  = oled_buffer[x][y/8];
  }
 
  
  oled.WriteByteWithCD(OLED_CMD,cmd,3);
  oled.WriteByteWithCD(OLED_DATA,&data,1);
}
#else
void OLED_Draw(ScreenDrive16BitsType x, ScreenDrive16BitsType y, ScreenDrive8BitsType data){
  ScreenDrive8BitsType cmd[3]={0xb0+y,((x&0xf0)>>4)|0x10,(x&0x0f)};
  ScreenDrive8BitsType data  = data;
  
  oled.WriteByteWithCD(OLED_CMD,cmd,3);
  oled.WriteByteWithCD(OLED_DATA,&data,1);
}
#endif

/* OLED 128x64 -- END */

