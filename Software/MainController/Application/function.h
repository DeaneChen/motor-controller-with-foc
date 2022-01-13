/*
 * @Author       : LuHeQiu
 * @Date         : 2022-01-12 22:33:07
 * @LastEditTime : 2022-01-12 22:34:59
 * @LastEditors  : DeaneChen
 * @Description  : 
 * @FilePath     : \MainController\Application\function.h
 * @HomePage     : https://www.luheqiu.com
 */
#ifndef __FUNCTION_H
#define __FUNCTION_H

#define Abs(value)                 (((value)>=0)?(value):(0-(value)))
#define Constrain(input,low,high)  ((input)<(low)?(low):((input)>(high)?(high):(input))) 
#define Max(A,B)                   ((A)>=(B)?(A):(B))
#define Min(A,B)                   ((A)<=(B)?(A):(B))

#endif
