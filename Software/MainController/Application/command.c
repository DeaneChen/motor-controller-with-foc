/*
 * @Author       : LuHeQiu
 * @Date         : 2022-02-26 18:13:38
 * @LastEditTime : 2022-02-26 18:57:24
 * @LastEditors  : DeaneChen
 * @Description  : 指令解析模块源文件
 * @FilePath     : \motor-controller-with-foc\Software\MainController\Application\command.c
 * @HomePage     : https://www.luheqiu.com
 */
#include "command.h"

/* 命令列表 */
command_list_t command[1] = {{"version",0}};

/* 带解析命令字串Buf */
COMMAND_8BitsType *CommandBuf;
COMMAND_16BitsType ComandBufLen;

void GetCommandBuf(COMMAND_8BitsType *Buf, COMMAND_16BitsType Len){
    CommandBuf = Buf;
    ComandBufLen = Len;
}
