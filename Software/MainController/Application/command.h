/*
 * @Author       : LuHeQiu
 * @Date         : 2022-02-26 18:13:44
 * @LastEditTime : 2022-02-26 18:52:26
 * @LastEditors  : DeaneChen
 * @Description  : 指令解析模块头文件
 * @FilePath     : \motor-controller-with-foc\Software\MainController\Application\command.h
 * @HomePage     : https://www.luheqiu.com
 */
#ifndef __COMMAND_H
#define __COMMAND_H

#include "main.h"

typedef uint8_t    COMMAND_8BitsType;
typedef uint16_t  COMMAND_16BitsType;

/**
 * @brief  命令字串回调函数，用于获取带解析的命令字串，可以再USB、USART、CAN等接收函数内部调用
 *         该项目中再USB接收函数内部被调用
 * @param  Buf 带解析字串的首地址
 * @param  Len 字串长度
 */
void GetCommandBuf(COMMAND_8BitsType *Buf, COMMAND_16BitsType Len);


typedef struct {
        COMMAND_8BitsType  const *commandName;     //命令字符串
        COMMAND_16BitsType maxArgs;                //最大参数数目
        void (*handle)(int argc, void* cmdArg);    //命令回调函数
        COMMAND_8BitsType  *help;                  //帮助信息
}command_list_t;


#endif
