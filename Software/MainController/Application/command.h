/*
 * @Author       : LuHeQiu
 * @Date         : 2022-02-26 18:13:44
 * @LastEditTime : 2022-03-02 16:55:58
 * @LastEditors  : DeaneChen
 * @Description  : 指令解析模块头文件
 * @FilePath     : \motor-controller-with-foc\Software\MainController\Application\command.h
 * @HomePage     : https://www.luheqiu.com
 */
#ifndef __COMMAND_H
#define __COMMAND_H

#include "main.h"

typedef char      COMMAND_charType;
typedef uint8_t   COMMAND_8BitsType;
typedef uint16_t  COMMAND_16BitsType;
typedef uint32_t  COMMAND_32BitsType;

#define COMMAND_NAME_LEN  20   //命令名的最大字符长度 (包含表字符串结束的'\0'转义字符在内，因此实际长度更小)
#define COMMAND_ARG_NUM   10   //命令中允许的最大参数个数
#define COMMAND_BUF_LEN   100  //命令字串的最大长度


/**
 * @brief  命令字串回调函数，用于获取带解析的命令字串，可以再USB、USART、CAN等接收函数内部调用
 *         该项目中再USB接收函数内部被调用
 * @param  Buf 带解析字串的首地址
 * @param  Len 字串长度
 */
void LoadCommandBuf(COMMAND_8BitsType *Buf, COMMAND_16BitsType Len);


typedef struct {
    const COMMAND_charType *commandName;                   //命令字符串
    COMMAND_16BitsType maxArgs;                            //最大参数数目
    void (*handle)(COMMAND_8BitsType* argBuf, COMMAND_8BitsType argNum, COMMAND_16BitsType* argsIndex);  //命令回调函数
    const COMMAND_charType *help;                          //帮助信息
} command_list_t;


/*    命令缓冲区状态          */
/* CMD_NULL: 命令空或解析完成 */
/* CMD_BUFF: 命令缓冲存在     */
typedef enum{
  CMD_NULL = 0,
  CMD_BUFF
}command_state_e;

typedef struct {
    COMMAND_8BitsType  buf[COMMAND_BUF_LEN];       //命令字符串缓冲区
    COMMAND_16BitsType bufLen;                     //命令字符串长度
    command_state_e    state;                      //命令解析状态
    COMMAND_16BitsType argIndex[COMMAND_ARG_NUM];  //命令参数索引 
} command_buf_t;


/**
 * @brief  解析命令，将缓冲区的命令字符流转换为带参数数组的命令字结构体，然后从命令列表中查找对应命令并调用
 * @param  
 * @retval 
 */
void ParsingCommand(void);

#endif
