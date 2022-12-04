/*
 * @Author       : LuHeQiu
 * @Date         : 2022-01-14 20:09:32
 * @LastEditTime : 2022-12-03 17:14:43
 * @LastEditors  : LuHeQiu
 * @Description  : 
 * @FilePath     : \motor-controller-with-foc\Software\MainController\Application\function.c
 * @HomePage     : https://www.luheqiu.com
 */

#include "function.h"

/**
 * @brief: 增量式PID
 * @param:
 * @retval: None
 */
void INCPID_Update(INCPIDController_t *PID,float target,float input){
    
    //误差值计算
    float error=target-input;
    
    //误差值存储
    PID->prevError=PID->lastError;
    PID->lastError=error;
    
    //PID输出计算
    float output =( PID->output + PID->pidParam.kp*( error-PID->lastError)
                                + PID->pidParam.ki*( error+PID->lastError)*0.5f
                                + PID->pidParam.kd*((error-PID->lastError)-(PID->lastError-PID->prevError)) );
    
    //PID输出幅限
    output = Constrain(output,PID->outMINLimit,PID->outMAXLimit);
    
    //PID输出更新
    PID->output = output;
}

/**
 * @brief: 位置式PID
 * @param:
 * @retval: None
 */
float POSPID_Update(POSPIDController_t *PID,float target,float input,float dt){
    
    //误差值计算
    float error=target-input;
    
    //比例项
    float pTerm=PID->pidParam.kp*error;
    
    //积分项
    PID->iTerm+=(PID->pidParam.ki*(error+PID->lastError)*0.5f*dt);
    PID->iTerm=Constrain(PID->iTerm,0-PID->integrationLimit,PID->integrationLimit);
    
    //微分项
    float dTerm=PID->pidParam.kd*(error-PID->lastError)/dt;
    
    //误差值存储
    PID->lastError=error;
    
    //PID输出计算
    float output = pTerm + PID->iTerm + dTerm;
    
    //输出值滤波
	output = PID->FilterPercent * output + (1 - PID->FilterPercent)* PID->output;
    
    //PID输出幅限
    output = Constrain(output,PID->outMINLimit,PID->outMAXLimit);
    
    //PID输出更新
    PID->output = output;
    
    return output;
}



/**
* @brief  atoi ( ascii to integer) 为把字符串转换成整型数的一个函数
* @param  nptr 字符串指针
* @retval 
*/
int atoi (const char* nptr){
   int n = 0, sign;
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


char *itoa(int value, char *string, int radix)
{
    int     i, d;
    char    *ptr = string;
    char    buf[33];

    /* This implementation only works for decimal numbers. */
    if (radix > 16 || radix < 2 )
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

//    for (i = 1000000000; i > 0; i /= 10)
//    {
//        d = value / i;

//        if (d || flag)
//        {
//            *ptr++ = (char)(d + 0x30);
//            value -= (d * i);
//            flag = 1;
//        }
//    }

    for (i = 0; i < 33 && value != 0 ; i++){
        d = value % radix;
        char c = d < 10 ? d + '0' : d - 10 + 'A';
        buf[i] = c;
        value /= radix;
    }

    if (i > 32 )
    {
       *ptr = 0;
       return string;
    }

   for (--i; i >= 0;--i){
       *ptr++ = buf[i];
   }

    /* Null terminate the string. */
    *ptr = 0;

   return string;

} /* NCL_Itoa */

//int strcmp(const unsigned char *str1, const unsigned char *str2)
//{
//    while(*str1 && (*str1==*str2)){
//        ++str1;
//        ++str2;
//    }
//    return *str1 - *str2;
//}


//int strlen(const char *str)
//{
//    const char *cp =  str;
//    while (*cp++ );
//    return (cp - str - 1 );
//}


float SIN_TABLE[] = 
{ 0.00000000f, 0.00613588f, 0.01227154f, 0.01840673f, 0.02454123f, 0.03067480f, 0.03680722f, 0.04293826f, 
  0.04906767f, 0.05519524f, 0.06132074f, 0.06744392f, 0.07356456f, 0.07968244f, 0.08579731f, 0.09190896f, 
  0.09801714f, 0.10412163f, 0.11022221f, 0.11631863f, 0.12241068f, 0.12849811f, 0.13458071f, 0.14065824f, 
  0.14673047f, 0.15279719f, 0.15885814f, 0.16491312f, 0.17096189f, 0.17700422f, 0.18303989f, 0.18906866f, 
  0.19509032f, 0.20110463f, 0.20711138f, 0.21311032f, 0.21910124f, 0.22508391f, 0.23105811f, 0.23702361f, 
  0.24298018f, 0.24892761f, 0.25486566f, 0.26079412f, 0.26671276f, 0.27262136f, 0.27851969f, 0.28440754f, 
  0.29028468f, 0.29615089f, 0.30200595f, 0.30784964f, 0.31368174f, 0.31950203f, 0.32531029f, 0.33110631f, 
  0.33688985f, 0.34266072f, 0.34841868f, 0.35416353f, 0.35989504f, 0.36561300f, 0.37131719f, 0.37700741f, 
  0.38268343f, 0.38834505f, 0.39399204f, 0.39962420f, 0.40524131f, 0.41084317f, 0.41642956f, 0.42200027f, 
  0.42755509f, 0.43309382f, 0.43861624f, 0.44412214f, 0.44961133f, 0.45508359f, 0.46053871f, 0.46597650f, 
  0.47139674f, 0.47679923f, 0.48218377f, 0.48755016f, 0.49289819f, 0.49822767f, 0.50353838f, 0.50883014f, 
  0.51410274f, 0.51935599f, 0.52458968f, 0.52980362f, 0.53499762f, 0.54017147f, 0.54532499f, 0.55045797f, 
  0.55557023f, 0.56066158f, 0.56573181f, 0.57078075f, 0.57580819f, 0.58081396f, 0.58579786f, 0.59075970f, 
  0.59569930f, 0.60061648f, 0.60551104f, 0.61038281f, 0.61523159f, 0.62005721f, 0.62485949f, 0.62963824f, 
  0.63439328f, 0.63912444f, 0.64383154f, 0.64851440f, 0.65317284f, 0.65780669f, 0.66241578f, 0.66699992f, 
  0.67155895f, 0.67609270f, 0.68060100f, 0.68508367f, 0.68954054f, 0.69397146f, 0.69837625f, 0.70275474f, 
  0.70710678f, 0.71143220f, 0.71573083f, 0.72000251f, 0.72424708f, 0.72846439f, 0.73265427f, 0.73681657f, 
  0.74095113f, 0.74505779f, 0.74913639f, 0.75318680f, 0.75720885f, 0.76120239f, 0.76516727f, 0.76910334f, 
  0.77301045f, 0.77688847f, 0.78073723f, 0.78455660f, 0.78834643f, 0.79210658f, 0.79583690f, 0.79953727f, 
  0.80320753f, 0.80684755f, 0.81045720f, 0.81403633f, 0.81758481f, 0.82110251f, 0.82458930f, 0.82804505f, 
  0.83146961f, 0.83486287f, 0.83822471f, 0.84155498f, 0.84485357f, 0.84812034f, 0.85135519f, 0.85455799f, 
  0.85772861f, 0.86086694f, 0.86397286f, 0.86704625f, 0.87008699f, 0.87309498f, 0.87607009f, 0.87901223f, 
  0.88192126f, 0.88479710f, 0.88763962f, 0.89044872f, 0.89322430f, 0.89596625f, 0.89867447f, 0.90134885f, 
  0.90398929f, 0.90659570f, 0.90916798f, 0.91170603f, 0.91420976f, 0.91667906f, 0.91911385f, 0.92151404f, 
  0.92387953f, 0.92621024f, 0.92850608f, 0.93076696f, 0.93299280f, 0.93518351f, 0.93733901f, 0.93945922f, 
  0.94154407f, 0.94359346f, 0.94560733f, 0.94758559f, 0.94952818f, 0.95143502f, 0.95330604f, 0.95514117f, 
  0.95694034f, 0.95870347f, 0.96043052f, 0.96212140f, 0.96377607f, 0.96539444f, 0.96697647f, 0.96852209f, 
  0.97003125f, 0.97150389f, 0.97293995f, 0.97433938f, 0.97570213f, 0.97702814f, 0.97831737f, 0.97956977f, 
  0.98078528f, 0.98196387f, 0.98310549f, 0.98421009f, 0.98527764f, 0.98630810f, 0.98730142f, 0.98825757f, 
  0.98917651f, 0.99005821f, 0.99090264f, 0.99170975f, 0.99247953f, 0.99321195f, 0.99390697f, 0.99456457f, 
  0.99518473f, 0.99576741f, 0.99631261f, 0.99682030f, 0.99729046f, 0.99772307f, 0.99811811f, 0.99847558f, 
  0.99879546f, 0.99907773f, 0.99932238f, 0.99952942f, 0.99969882f, 0.99983058f, 0.99992470f, 0.99998118f, 1.0f };


#define PI_2 1.5707963268f

float SinByLut(float theta) {
    
    /* 将 theta 映射到 sin 的第一最小正周期内[0,2pi) */
    Fun32BitsNumber cycleIndex = (Fun32BitsNumber)(theta * 0.1591549431f * 1024) & 0x3FF;

    /* [0,pi/2) 映射查表 */
    float value;
    if (cycleIndex < 256) {
        value = SIN_TABLE[cycleIndex];
    } else if (cycleIndex < 512) {
        value = SIN_TABLE[512 - cycleIndex];
    } else if (cycleIndex < 768) {
        value = -SIN_TABLE[cycleIndex - 512];
    } else {
        value = -SIN_TABLE[1024 - cycleIndex];
    }

    return value;
}

float CosByLut(float theta) {
    return SinByLut(theta + PI_2);
}

float SinByLinearLut(float theta) {

    /* 将 theta 映射到 sin 的第一最小正周期内[0,2pi) */
    float thetaInBase = theta * 0.159154943092f;  /* 乘以 1/2pi */
    if (theta >= 0.0f) {
        thetaInBase = (thetaInBase) - (Fun32BitsNumber)(thetaInBase);
    } else {
        thetaInBase = (thetaInBase) - (Fun32BitsNumber)(thetaInBase) - 1;
    }
    /* 将 2pi 空间 映射到 1024 空间 */
    thetaInBase = thetaInBase * 1024.0f;

    Fun32BitsNumber cycleIndex = (Fun32BitsNumber)(thetaInBase)&0x3FF;
    float indexDiv = thetaInBase - cycleIndex;

    float value;
    if (cycleIndex < 256) {
        value = SIN_TABLE[cycleIndex] * (1 - indexDiv) + SIN_TABLE[cycleIndex + 1] * (indexDiv);
    } else if (cycleIndex < 512) {
        value = SIN_TABLE[512 - cycleIndex] * (1 - indexDiv) + SIN_TABLE[512 - cycleIndex - 1] * (indexDiv);
    } else if (cycleIndex < 768) {
        value = -(SIN_TABLE[cycleIndex - 512] * (1 - indexDiv) + SIN_TABLE[cycleIndex - 512 + 1] * (indexDiv));
    } else {
        value = -(SIN_TABLE[1024 - cycleIndex] * (1 - indexDiv) + SIN_TABLE[1024 - cycleIndex - 1] * (indexDiv));
    }

    return value;
}

float CosByLinearLut(float theta) {
    return SinByLinearLut(theta + PI_2);
}
