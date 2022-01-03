/*
 * @Author       : LuHeQiu
 * @Date         : 2021-12-28 15:19:50
 * @LastEditTime : 2021-12-28 18:24:22
 * @LastEditors  : DeaneChen
 * @Description  : 
 * @FilePath     : \MainController\Hardware\screen_drive.c
 * @HomePage     : https://www.luheqiu.com
 */
#include "screen_drive.h"

void OLED_Init(void);
void OLED_WriteByte(ScreenDrive8BitsType dataType,ScreenDrive8BitsType *pData,ScreenDrive16BitsType size);
void OLED_Draw(ScreenDrive8BitsType x, ScreenDrive8BitsType y);
void OLED_ClearScreen(void);

screen_t oled = {0,OLED_Init,OLED_WriteByte,OLED_Draw};

void OLED_Init(void)
{
  //--turn off oled panel
  //---set low column address
  //---set high column address
  //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  //--set contrast control register
  // Set SEG Output Current Brightness
  //--Set SEG/Column Mapping     0xa0×óÓÒ·´ÖÃ 0xa1Õý³£
  //Set COM/Row Scan Direction   0xc0ÉÏÏÂ·´ÖÃ 0xc8Õý³£
  //--set normal display
  //--set multiplex ratio(1 to 64)

  //--1/64 duty
  //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  //-not offset
  //--set display clock divide ratio/oscillator frequency
  //--set divide ratio, Set Clock as 100 Frames/Sec
  //--set pre-charge period
  //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  //--set com pins hardware configuration
  //
  //--set vcomh  

  //Set VCOM Deselect Level
  //-Set Page Addressing Mode (0x00/0x01/0x02)
  //
  //--set Charge Pump enable/disable
  //--set(0x10) disable
  // Disable Entire Display On (0xa4/0xa5)
  // Disable Inverse Display On (0xa6/a7) 
  //--turn on oled panel
  //--display ON 
  ScreenDrive8BitsType initData[29]={0xAE,0x00,0x10,0x40,0x81,0xCF,0xA1,0xC8,0xA6,0xA8,
                                     0x3f,0xD3,0x00,0xd5,0x80,0xD9,0xF1,0xDA,0x12,0xDB,
                                     0x40,0x20,0x02,0x8D,0x14,0xA4,0xA6,0xAF,0xAF};

  oled.WriteByteWithCD(0, initData, 29);
  //  for(ScreenDrive8BitsType i=0;i<29;i++){
  //    Screen_SCK_RESET
  //  }
  OLED_ClearScreen();
  
  //OLED_Clear();
  //OLED_Set_Pos(0,0); 	
}  

void OLED_WriteByte(ScreenDrive8BitsType dataType,ScreenDrive8BitsType *pData,ScreenDrive16BitsType size){
  OLED_CS();
  Screen_WriteByte(dataType, pData, size);
  OLED_NCS();
}

void OLED_ClearScreen(void)  
{  
  ScreenDrive8BitsType cmd[3]={0xb0,0x00,0x10};
  ScreenDrive8BitsType data  = 0x00;
  for(ScreenDrive8BitsType i=0;i<8;i++){
    cmd[0]=0xb0+i;
    oled.WriteByteWithCD(OLED_CMD,cmd,3);
    for(ScreenDrive8BitsType n=0;n<128;n++)oled.WriteByteWithCD(OLED_DATA,&data,1);
  }
}

void OLED_Draw(ScreenDrive8BitsType x, ScreenDrive8BitsType y){
  ScreenDrive8BitsType cmd[3]={0xb0+y,((x&0xf0)>>4)|0x10,(x&0x0f)};
  ScreenDrive8BitsType data  = 0x10;
  
  oled.WriteByteWithCD(OLED_CMD,cmd,3);
  oled.WriteByteWithCD(OLED_DATA,&data,1);
}

