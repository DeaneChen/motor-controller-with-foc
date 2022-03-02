/*
 * @Author       : LuHeQiu
 * @Date         : 2022-01-12 19:50:44
 * @LastEditTime : 2022-03-01 17:07:47
 * @LastEditors  : DeaneChen
 * @Description  : 
 * @FilePath     : \motor-controller-with-foc\Software\MainController\Application\lgui.c
 * @HomePage     : https://www.luheqiu.com
 */

#include "lgui.h"
#include "stdarg.h"
#include "function.h"
#include "font.h"


/* 取大数 */
#ifndef Max
  #define Max(A,B)     ((A)>=(B)?(A):(B))
#endif

 /* 四舍五入 */
#ifndef Round
  #define Round(A)     ( ((A)-(int)(A))>=0.5f ? (int)(A+1) : (int)(A))
#endif

char *itoa(int value, char *string, int radix);


void ShowPicture(LGUI16BitsType x, LGUI16BitsType y, const LGUI8BitsType *p, LGUI16BitsType img_x, LGUI16BitsType img_y, LGUI8BitsType img_depth);
void ShowChar(LGUI16BitsType x, LGUI16BitsType y, LGUI8BitsType character, LGUI8BitsType fontsize, LGUI16BitsType fontcolor, LGUI8BitsType isCover);
void ShowString(LGUI16BitsType x, LGUI16BitsType y, LGUI8BitsType fontsize, LGUI16BitsType fontcolor, LGUI8BitsType isCover, LGUI8BitsType *str, ...);
void Fill(LGUI16BitsType sx, LGUI16BitsType sy, LGUI16BitsType ex, LGUI16BitsType ey, LGUI16BitsType color);

LGUI_t lgui = {ShowPicture,ShowChar,ShowString,Fill};

void ShowPicture(LGUI16BitsType x, LGUI16BitsType y, const LGUI8BitsType *p, LGUI16BitsType img_x, LGUI16BitsType img_y, LGUI8BitsType img_depth){
    
    if(img_depth == 1){
        LGUI16BitsType tx = 0, ty = 0;
        LGUI8BitsType temp;
        for (tx = 0; tx < img_x; tx+=8){
            for (ty = 0; ty < img_y;ty++){
                temp = *(p + (tx / 8) + ty * (img_x / 8));
                for (LGUI8BitsType t = 0; t < 8;t++){
                    LGUI_DRAWPOINT(x + tx + t, y + ty, temp & (0x01 << t));
                }
            }
        }
    }

}

void ShowChar(LGUI16BitsType x, LGUI16BitsType y, LGUI8BitsType character, LGUI8BitsType fontsize,  LGUI16BitsType fontcolor, LGUI8BitsType isCover){
    LGUI8BitsType temp;
    LGUI8BitsType pos, t;
    character = character - ' ';
 
    for(pos=0;pos<fontsize;pos++)
    {
        if(fontsize==8)temp=ASCII_0806[character][pos];
        if(fontsize==12)temp=ASCII_1206[character][pos];
        else if(fontsize==16)temp=ASCII_1608[character][pos];
        if(!fontcolor)
            temp = ~temp;
        for(t=0;t<Max(6,fontsize/2);t++)
        {   
            if(isCover){
                LGUI_DRAWPOINT(x + t, y - (fontsize - 1) + pos, temp & 0x01);
            }else{
                if(temp & 0x01)LGUI_DRAWPOINT(x + t, y - (fontsize - 1) + pos, 1);
            } 
            
            temp>>=1; 
        }
    }
}

void ShowString(LGUI16BitsType x, LGUI16BitsType y, LGUI8BitsType fontsize, LGUI16BitsType fontcolor, LGUI8BitsType isCover, LGUI8BitsType *data, ...){
    const char *s;
    int d;
    float f;
    LGUI16BitsType cx=x, cy=y;
    char buf[16];
    va_list ap;
    va_start(ap, data);

    while(*data!=0){      /* 判断是否达到字符尾 */
        if(*data=='\\'){  /* "\" */
            switch (*++data)
            {
            case 'r':     /* 回车 */
                cx = x;
                data++;
                break;
            case 'n':     /* 换行 */
                y += fontsize;
                data++;
                break;

            default:
                data++;
                break;
            }
        }else if (*data=='%')  /* "%"  识别到%格式化字符串 */  
        {   
            char isFormat;
            int format,nearformat,isZeroPre;
            int *formatPoint = &format;
            format = 0, nearformat = 6;
            isZeroPre = 0;
            do{
                isFormat = 0;
                switch (*++data)
                {
                case 's': /* 字符串 */  

                    /* 获取%s对应的字符串指针 */
                    s = va_arg(ap, const char *);  

                    for (; *s; s++)
                    {
                        lgui.ShowChar(cx, cy, *s, fontsize, fontcolor, isCover);
                        cx += Max(6, fontsize / 2);
                    }
                    data++;
                    break;
                case 'd': /* 数字 */       

                    /* 获取%d对应的整形数字指针 */      
                    d = va_arg(ap, int);   

                    /* 将int按照10进制转换为字符串 */      
                    itoa(d, buf, 10);            

                    if(format!=0){   

                        /* 获取字符串长度 */           
                        int length;
                        for (length = 0; *(buf + length); length++)
                            ;

                        /* 如果格式化宽度大于字符长度则现输出前缀，
                           前缀的长度 = 格式化宽度 - 字符宽度 */
                        if(format > length){     

                            /* 前缀默认为空格 */
                            unsigned char pre = ' ';   

                            /* 若声明前缀为0则修改为0 */
                            if(isZeroPre)             
                                pre = '0';

                             /* 输出前缀 */
                            for (; format>length;format--){
                                lgui.ShowChar(cx, cy, pre, fontsize, fontcolor, isCover);
                                cx += Max(6, fontsize / 2);
                            }
                        }
                            
                    }

                    /* 正常输出剩余字符 */
                    for (s = buf; *s; s++){
                        lgui.ShowChar(cx, cy, *s, fontsize, fontcolor, isCover);
                        cx += Max(6, fontsize / 2);
                    }
                    data++;
                    break;
                
                case 'f': /* 浮点 */

                    /* 获取%f对应的整形数字指针。
                       由于float形参在通过省略号参数时回北转化为double
                       故此处提取变量时，使用double类型 */  

                    f = va_arg(ap, double);

                    /* nearformat为近端格式，例如%4.3f中
                         4为缺省格式参数format
                         3为近端格式参数nearformat 
                       近端格式参数默认值为6，即对于%3f这类无‘.’格式时，nearformat=6
                       
                       nearformat用于表征保留的小数位数，故默认值6表示默认保留小数6位
                       保留方法为，首先扩大响应的10^n倍，然后视为int，再转换为字符串。
                       例如12.345 
                       nearformat = 2 , 则先扩大10^2倍得到1234.5，根据四舍五入得到1235
                       1235转换为字符串"1235" */


                    for (int i = 0; i < nearformat;i++){
                        f *= 10;
                    }
                    itoa( Round(f), buf, 10);

                    /* 将后续定义的变量固定在该{}块内作为局部变量，防止干扰外部代码 */
                    {
                        /* 获取字符串长度 */
                        int length;
                        for (length = 0; *(buf + length); length++)
                            ;

                        /* 格式化浮点数共有三部分字符串组成：整数，小数点和小数部分 */
                        /* 对于%4.3f，则整数部分为4 小数部分为3。（当整数部分小数实际整数长度时，以实际为准）
                            因此可以推出格式化长度为7，若字符串实际长度小于7，则应补充前缀
                            format为整数部分，补充前缀或者输出实际整数位数后，会逐渐递减 */
                        
                        unsigned char pre = ' ';

                        if(format > (length - nearformat)){   /* 即 format + nearformat > length */
                            
                            if(isZeroPre)
                                pre = '0';

                            for (; format> (length - nearformat) ;format--){

                                /* 当format=0时，意味整数部分输出结束，应输出一个小数点 */
                                if(format==0){
                                    lgui.ShowChar(cx, cy, '.', fontsize, fontcolor, isCover);
                                    cx += Max(6, fontsize / 2);

                                    /* 小数部分，前缀强制为0 */
                                    pre = '0';
                                }

                                lgui.ShowChar(cx, cy, pre, fontsize, fontcolor, isCover);
                                cx += Max(6, fontsize / 2);
                            }
                        }
                                

                        /* 正常输出剩余字符 */
                        for (s = buf; *s; s++,format--){
                            if(format==0){
                                lgui.ShowChar(cx, cy, '.', fontsize, fontcolor, isCover);
                                cx += Max(6, fontsize / 2);                   
                                pre = '0';
                            }
                            lgui.ShowChar(cx, cy, *s, fontsize, fontcolor, isCover);
                            cx += Max(6, fontsize / 2);
                        }
                        data++;
                        break;
                    }

                case '%': /* 输出百分号“%” */
                    lgui.ShowChar(cx, cy, '%', fontsize, fontcolor, isCover);
                    cx += Max(6, fontsize / 2);
                    data++;
                    break;

                case '.':

                    /* .格式化前缀，将format保留，并使得格式写入指针指向 nearformat
                       这意味后续的格式化数字将被写入nearformat */

                    nearformat = 0;
                    formatPoint = &nearformat;
                    isFormat = 1;
                    break;

                    /* 0格式化前缀，若0出现在开始，即format为空时，0被解释为修改前缀空格为“0” ，例如%02d
                       若0出现在format内部，例如%10d 或者 %10.10f , 0被解释为位宽的一部分，即数字10*/
                case '0':
                    if(format==0){
                        isZeroPre = 1;
                        isFormat = 1;
                        break;
                    }

                    /* 数字格式化前缀，被解释为位宽的一部分 */
                case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':  case '9':
                    (*formatPoint) *= 10;
                    (*formatPoint) += (*data) - '0';
                    isFormat = 1;
                    
                    break;

                default:
                    data++;
                    break;
                }

            /*  遇到%后视为遇到格式化字符，每遇到0-9的数字则继续读取，直到遇到d f s等字符，否则结束 */
            } while (isFormat != 0);

        }else{
            lgui.ShowChar(cx, cy, *data++, fontsize, fontcolor, isCover);
            cx+=Max(6,fontsize/2);
        }
        
    }
}

void Fill(LGUI16BitsType sx, LGUI16BitsType sy, LGUI16BitsType ex, LGUI16BitsType ey, LGUI16BitsType color){
    
    
    LGUI16BitsType tx = 0, ty = 0;
    for (tx = sx; tx < ex; tx++){
        for (ty = sy; ty < ey;ty++){
            LGUI_DRAWPOINT(tx, ty, color);
        }
    }
    
}

char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 1000000000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

