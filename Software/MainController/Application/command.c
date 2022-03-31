/*
 * @Author       : LuHeQiu
 * @Date         : 2022-02-26 18:13:38
 * @LastEditTime : 2022-03-31 17:54:23
 * @LastEditors  : DeaneChen
 * @Description  : 指令解析模块源文件
 * @FilePath     : \motor-controller-with-foc\Software\MainController\Application\command.c
 * @HomePage     : https://www.luheqiu.com
 */
#include "command.h"
#include "function.h"
#include "usbd_cdc_if.h"

#include "drv8303.h"

/* 取大数 */
#ifndef Max
    #define Max(A,B)   ((A)>=(B)?(A):(B))
#endif

/* 取小数 */
#ifndef Min
    #define Min(A,B)   ((A)<=(B)?(A):(B))
#endif

#define COMMAND_DEBUG(buf) while(CDC_Transmit_FS((COMMAND_8BitsType*)buf,strlen(buf))!=USBD_OK)

command_buf_t commandBuf = {0};

void Version(COMMAND_8BitsType* argBuf, COMMAND_8BitsType argNum, COMMAND_16BitsType* argsIndex){
    COMMAND_DEBUG("FOC V1.2\n");
}

void Help(COMMAND_8BitsType* argBuf, COMMAND_8BitsType argNum, COMMAND_16BitsType* argsIndex);

void DRV8303(COMMAND_8BitsType* argBuf, COMMAND_8BitsType argNum, COMMAND_16BitsType* argsIndex){

    if(argNum<2){
        COMMAND_DEBUG("缺少参数\n");
        return;
    }
    
    /* 配置DRV8303 */
    if(!strcmp(argBuf+argsIndex[0],"-config")){

        /* 写寄存器 */
        if(!strcmp(argBuf+argsIndex[1],"-w")){
            if(argNum<3){
                COMMAND_DEBUG("缺少参数\n");
                return;
            }
            COMMAND_8BitsType drv8303Index = atoi(argBuf+argsIndex[2]);
            if(drv8303Index <= 1){
                drv8303[drv8303Index].LoadConfig();
            }else{
                COMMAND_DEBUG("DRV8303 is not exist\n");
            }

        /* 读寄存器 */
        }else if(!strcmp(argBuf+argsIndex[1],"-r")){
            if(argNum<4){
                COMMAND_DEBUG("缺少参数\n");
                return;
            }
            COMMAND_8BitsType drv8303Index = atoi(argBuf+argsIndex[2]);
            COMMAND_8BitsType regIndex = atoi(argBuf+argsIndex[3]);
            COMMAND_8BitsType regData[3];
            if(drv8303Index <= 1){
                if(regIndex <= 3){
                    drv8303[drv8303Index].ReadRegister(regIndex,regData);
                }else{
                    COMMAND_DEBUG("reg is not exist\n");
                }
            }else{
                COMMAND_DEBUG("DRV8303 is not exist\n");
            }
            
        }
    }else{
        COMMAND_DEBUG("未知参数\n");
    }
}

/* 命令列表 */
command_list_t command_list[] = {
/*       命令名       最大参数数量           命令函数         帮助信息         */
    {   "/drv8303",         4       ,        DRV8303        ,     "\t Config The DRV8303 \t /drv8303 -config [-w/r] [deviceID] [regID](optional) \r\n" },
    {   "/help"   ,         0       ,        Help           ,     "\t get the command help \r\n"       },
    {   "/version",         0       ,        Version        ,     "\t check the device version \r\n"       }
    };

/**
 * @brief  输出命令的帮助信息
 * @param  *
 * @retval *
 */
void Help(COMMAND_8BitsType* argBuf, COMMAND_8BitsType argNum, COMMAND_16BitsType* argsIndex){
  
    for(COMMAND_16BitsType i = 0; i < sizeof(command_list)/sizeof(command_list[0]); i++){
        
        COMMAND_DEBUG(command_list[i].commandName);

        COMMAND_DEBUG(command_list[i].help);

    }
}

/**
 * @brief  atoi ( ascii to integer) 为把字符串转换成整型数的一个函数
 * @param  nptr 字符串指针
 * @retval 被转换完成的整型数
 */

/*
int atoi (const char* nptr){
    int n, sign;
    sign = (*nptr == '-') ? (-1) : (1);  //判断符号
    if ( (*nptr) == '+' || (*nptr) == '-' ) //跳过符号
        nptr++;
    for ( ; *nptr != '\0'; nptr++ ){
        if( (*nptr)<'0' || (*nptr)>'9' )
            break;
        n = 10 * n + ((*nptr) - '0');
    }

    return sign * n;
}

*/

void LoadCommandBuf(COMMAND_8BitsType *Buf, COMMAND_16BitsType Len){
    
    /* 缓存长度溢出 */
    if ( Len > COMMAND_BUF_LEN ){
        return ;
    }

    /* 上一命令还未解析 */
    if ( commandBuf.state == CMD_BUFF ){
        return ;
    }

    /* 转存字符流 */
    for (COMMAND_16BitsType i = 0; i < Len; i++){
        commandBuf.buf[i] = *(Buf++);
    }
    commandBuf.bufLen = Len;
    commandBuf.state = CMD_BUFF;


    /* 调用命令解析器 */
    //ParsingCommand();
}



void ParsingCommand(void){

    /* 命令缓冲区为空 */
    if( commandBuf.state == CMD_NULL )
        return;

    /* 解析命令字 */
    COMMAND_16BitsType i = 0;
    COMMAND_charType commandName[COMMAND_NAME_LEN];
    for ( ; i < commandBuf.bufLen ; i++){
        if(commandBuf.buf[i]==' ' || commandBuf.buf[i]==0x0D || commandBuf.buf[i]==0x0A || commandBuf.buf[i]=='\0'){
            commandName[i] = '\0';
            break;
        }else{
            commandName[i] = commandBuf.buf[i];
        }
    }

    /* 解析参数表 */
    COMMAND_8BitsType isSpace = 0;  /* 空格标志 */
    COMMAND_8BitsType argsNum = 0;  /* 参数数目 */
    for ( ; i < commandBuf.bufLen ; i++){
    
        if(commandBuf.buf[i]==' '){
            isSpace = 1;
            commandBuf.buf[i] = '\0';
            continue;
        }else if( commandBuf.buf[i]==0x0D || commandBuf.buf[i]==0x0A || commandBuf.buf[i]=='\0' ){
            commandBuf.buf[i] = '\0';
            break;
        }else{
            if(isSpace){
                isSpace = 0;
                if(argsNum < COMMAND_ARG_NUM){
                    commandBuf.argIndex[argsNum++] = i;      
                }
                else{
                    COMMAND_DEBUG("参数表达到限制\n");
                    return ;      /* 参数表达到限制 */
                }
            }
        }
    }

    COMMAND_16BitsType j = 0;
    /* 查找命令 */
    for(j = 0; j < sizeof(command_list)/sizeof(command_list[0]); j++){
        
        /* 命令遍历比较 */
        if(!strcmp(commandName,command_list[j].commandName)){   
            if( argsNum > command_list[j].maxArgs ){
                COMMAND_DEBUG("参数过多\n");
            }
            else{
                command_list[j].handle(commandBuf.buf,argsNum,commandBuf.argIndex);
            }
            break;
        }
    }

    if( j >= sizeof(command_list) / sizeof(command_list[0]) ){
           COMMAND_DEBUG("未知指令\n");
    }

    commandBuf.state = CMD_NULL;
    
}

