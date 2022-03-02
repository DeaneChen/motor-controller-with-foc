/*
 * @Author       : LuHeQiu
 * @Date         : 2022-01-12 19:52:03
 * @LastEditTime : 2022-03-01 17:07:43
 * @LastEditors  : DeaneChen
 * @Description  : 
 * @FilePath     : \motor-controller-with-foc\Software\MainController\Application\lgui.h
 * @HomePage     : https://www.luheqiu.com
 */

#ifndef __LGUI_H
#define __LGUI_H

#include "main.h"
#include "screen_drive.h"


/*  变量类型与所需的函数接口配置 - BEGIN  */
typedef uint8_t   LGUI8BitsType;
typedef uint16_t  LGUI16BitsType;
#define LGUI_DRAWPOINT(x, y, c)  oled.Draw(x, y, c)

/*  变量类型与所需的函数接口配置 - END  */

typedef struct
{
    //void (*ShowPoint)(LGUI16BitsType x, LGUI16BitsType y, LGUI8BitsType c);

    /**
     * @brief  绘制一张图片
     * @param  x 绘制坐标点x
     * @param  y 绘制坐标点y
     * @param  p 图片数据首地址
     * @param  img_x 图片尺寸x
     * @param  img_y 图片尺寸y
     * @param  img_depth 图片色深
     * @retval 
     */
    void (*ShowPicture)(LGUI16BitsType x, LGUI16BitsType y, const LGUI8BitsType *p, LGUI16BitsType img_x, LGUI16BitsType img_y, LGUI8BitsType img_depth);

    /**
     * @brief  显示一个字符,字符左下角为(x,y)
     * @param  x 显示坐标点x
     * @param  y 显示坐标点y
     * @param  character 显示的字符
     * @param  fontsize 字体大小
     * @param  fontcolor 字体颜色
     * @param  isCover 是否覆盖
     * @retval 
     */
    void (*ShowChar)(LGUI16BitsType x, LGUI16BitsType y, LGUI8BitsType character, LGUI8BitsType fontsize, LGUI16BitsType fontcolor, LGUI8BitsType isCover);

    

    /**
     * @brief  显示一个格式化字符串,字符左下角为(x,y)
     * @param  x 显示坐标点x
     * @param  y 显示坐标点y
     * @param  fontsize 字体大小
     * @param  fontcolor 字体颜色
     * @param  isCover 是否覆盖
     * @param  str 格式化字符
     *              %s:字符串
     *              %d:数字 %3d:三位宽数字且前缀补空格 %04d:四位宽数字且前缀补0
     *              %f:浮点 %3f:三位宽数字且前缀补空格 %04f:四位宽数字且前缀补0 %.2f:保留两位小数
     *                   对于直接使用%.2f这类格式化字符串的变量，将不保留整数部分的0 
     *                   例如 0.03 将以 ".03" 的形式输出以节省空间，如果需要输出0.03请使用 %01.2f   
     *              %%:输出字符百分号“%”本身 
     * @retval 
     */
    void (*ShowString)(LGUI16BitsType x, LGUI16BitsType y, LGUI8BitsType fontsize, LGUI16BitsType fontcolor, LGUI8BitsType isCover, LGUI8BitsType *str, ...);

    /**
     * @brief  填充一块区域
     * @param  sx 左上开始坐标(sx,sy)
     * @param  sy
     * @param  ex 右下结束坐标(ex,ey)
     * @param  ey
     * @param  color 填充颜色
     * @retval 
     */
    void (*Fill)(LGUI16BitsType sx, LGUI16BitsType sy, LGUI16BitsType ex, LGUI16BitsType ey, LGUI16BitsType color);

}LGUI_t;

extern LGUI_t lgui;

#endif

