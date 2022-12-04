/*
 * @Author       : LuHeQiu
 * @Date         : 2022-01-21 21:17:31
 * @LastEditTime : 2022-12-04 18:59:36
 * @LastEditors  : LuHeQiu
 * @Description  : 
 * @FilePath     : \motor-controller-with-foc\Software\MainController\Application\foc.c
 * @HomePage     : https://www.luheqiu.com
 */
#include "foc.h"
#include "function.h"
#include "tim.h"
#include "math.h"

#ifdef FOC_USE_CAL_CORE_FLOAT
    #define SQRT3_NUM   1.73205080757f
    #define SQRT3_2_NUM 0.86602540378f
    #define SQRT3_3_NUM 0.57735026919f
#else
    #define SQRT3_NUM 17320508f
#endif

static float curTheta = 0.0f;

void FOC_ControllerTask(void){

    Vector2 U_target_qd = {0.1, 0.0};
    Vector2 U_target_ab;
    Vector3 T_target_uvw;
    curTheta += 0.06283185307f;
    if(curTheta > 31.4159265f){
        curTheta = 0.0f;
    }
    RevParkTransformaion(&U_target_qd, curTheta, &U_target_ab);
    SVPWM(&U_target_ab, 0.8f, &T_target_uvw);

    if (T_target_uvw.x1 > SVPWM_PERIOD)
        T_target_uvw.x1 = SVPWM_PERIOD;
    if (T_target_uvw.x2 > SVPWM_PERIOD)
        T_target_uvw.x2 = SVPWM_PERIOD;
    if (T_target_uvw.x3 > SVPWM_PERIOD)
        T_target_uvw.x3 = SVPWM_PERIOD;

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint16_t)T_target_uvw.x1);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint16_t)T_target_uvw.x2);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (uint16_t)T_target_uvw.x3);
    
}


/**
 * @brief  Clarke线性变换，将一个三相电流向量分解为二相正交向量
 *         基本公式为：
 *                      | I_alpha |       |  1    -1/2     -1/2     |       | Ia |
 *                      | I_beta  |   =   |  0   3^0.5/2  -3^0.5/2  |   x   | Ib |
 *                                                                          | Ic |
 * @param  input 被变换的三相电流向量，是一个三维的向量
 * @param  output 变换得到的二相正交电流向量，是一个二维的向量
 * @retval none
 */
void ClarkeTransformaion(Vector3 *input, Vector2 *output){
    Vector2* current_ab = output;
    /* 使用乘法 替代 除法 */
    current_ab->x1 = input->x1 - input->x2 * 0.5f         - input->x3 * 0.5f;
    current_ab->x2 = 0.0f      + input->x2 * SQRT3_2_NUM  - input->x3 * SQRT3_2_NUM;
}

/**
 * @brief  Park线性变换，将一个二相静止坐标系变换到二相旋转坐标系，实质上只是一个旋转矩阵。
 *         基本公式为：
 *                      | I_d |   =   |  cos(theta)  sin(theta)  |   x   | I_alpha | 
 *                      | I_q |       | -sin(theta)  cos(theta)  |       | I_beta  |
 * 
 * @param  input 被变换的二相静止电流向量
 * @param  theta 角度
 * @param  output 变换得到的二相旋转电流向量
 * @retval none
 */
void ParkTransformaion(Vector2 *input, Degree theta, Vector2 *output){
    Vector2* current_dq = output;
    float costheta = CosByLut(theta);
    float sintheta = SinByLut(theta);
    current_dq->x1 =  input->x1 * costheta + input->x2 * sintheta;
    current_dq->x2 = -input->x1 * sintheta + input->x2 * costheta;
}

/**
 * @brief  Park反变换，将一个二相旋转坐标系变换到二相静止坐标系，实质上只是一个逆向的旋转矩阵。
 *         基本公式为：
 *                      | U_alpha |   =   |  cos(theta) -sin(theta)  |   x   | U_d | 
 *                      | U_beta  |       |  sin(theta)  cos(theta)  |       | U_q |
 */
void RevParkTransformaion(Vector2 *input, Degree theta ,Vector2 *output){
    ParkTransformaion(input, 0 - theta, output);
}



//static const FOC8BitsType sector_code[7] = {0b100, 0b110, 0b010, 0b011, 0b001, 0b101, 0b100}; /* 扇区电压基向量编码表（已弃用） */
/**
 * @brief  SVPWM计算函数、根据静止二相坐标系下的电压向量，计算出PWM中心对齐模式下U、V、W三相的接通时间，即脉冲宽度。
 *         该函数需要一个标记PWM周期长度的宏定义 SVPWM_PERIOD ，在本例中PWM频率为15KHz、周期为5600个单位。
 *         注意：仅仅适用于PWM中心对齐模式的应用场景。
 * @param  input 被变换的二相静止电压向量
 * @param  modulation SVPWM调制度，表示了期望输出的大小
 * @param  output 输出的SVPWM三相脉冲宽度（PWM中心对齐模式下）
 * @retval 
 */
void SVPWM(Vector2 *input, float modulation, Vector3 *output){
    
    /* 判断扇区 */
    float alpha = input->x1;
    float beta  = input->x2;
    float beta_div_sqrt3 = beta * SQRT3_3_NUM;  /* 计算 beta/sqrt(3) */
    FOC8BitsNumber sector;

    /* beta > 0 在 1、2、3 扇区 */
    /* beta < 0 在 4、5、6 扇区 */
    /* alpha 与 -beta/sqrt(3) 和 beta/sqrt(3) 的关系可以区分 1 4、2 5、3 6 扇区 */
    if(beta>=0){
        if(alpha>beta_div_sqrt3){
            sector = 1;
        }else{
            if(alpha>-beta_div_sqrt3){
                sector = 2;
            }else{
                sector = 3;
            }
        }
    }else{
        if(alpha>=-beta_div_sqrt3){
            sector = 6;
        }else{
            if(alpha>beta_div_sqrt3){
                sector = 5;
            }else{
                sector = 4;
            }
        }
    }

    /* 计算电压基矢量 */
    /* 计算SVPWM脉冲宽度：先计算零矢量T7，然后依次累加出另外两个基矢量 */
    float U_1, U_2;
    float Tu, Tv, Tw;
    switch (sector) {
        /* 扇区1  变换矩阵为             */
        /*        |  1  -3^0.5/3    |    */
        /*        |  0   3^0.5*2/3  |    */
        case 1:
            U_1 = alpha - beta_div_sqrt3;   /* U 0 0 */
            U_2 = 2.0f * beta_div_sqrt3;    /* U V 0 */
            Tw = (1.0f - modulation * (U_1 + U_2)) * SVPWM_PERIOD * 0.5f;
            Tv = modulation * U_2 * SVPWM_PERIOD + Tw;
            Tu = modulation * U_1 * SVPWM_PERIOD + Tv;
            break;

        /* 扇区2  变换矩阵为             */
        /*        |  1   3^0.5/3    |    */
        /*        | -1   3^0.5/3    |    */
        case 2:
            U_1 = alpha + beta_div_sqrt3;  /* U V 0 */
            U_2 = -alpha + beta_div_sqrt3; /* 0 V 0 */
            Tw = (1.0f - modulation * (U_1 + U_2)) * SVPWM_PERIOD * 0.5f;
            Tu = modulation * U_1 * SVPWM_PERIOD + Tw;
            Tv = modulation * U_2 * SVPWM_PERIOD + Tu;
            break;

        /* 扇区3  变换矩阵为             */
        /*        |  0   3^0.5*2/3  |    */
        /*        | -1  -3^0.5/3    |    */
        case 3:
            U_1 = 2.0f * beta_div_sqrt3;   /* 0 V 0 */
            U_2 = -alpha - beta_div_sqrt3; /* 0 V W */
            Tu = (1.0f - modulation * (U_1 + U_2)) * SVPWM_PERIOD * 0.5f;
            Tw = modulation * U_2 * SVPWM_PERIOD + Tu;
            Tv = modulation * U_1 * SVPWM_PERIOD + Tw;
            break;

        /* 扇区4  变换矩阵为             */
        /*        | -1   3^0.5/3    |    */
        /*        |  0  -3^0.5*2/3  |    */
        case 4:
            U_1 = -alpha + beta_div_sqrt3; /* 0 V W */
            U_2 = -2.0f * beta_div_sqrt3;  /* 0 0 W */
            Tu = (1.0f - modulation * (U_1 + U_2)) * SVPWM_PERIOD * 0.5f;
            Tv = modulation * U_1 * SVPWM_PERIOD + Tu;
            Tw = modulation * U_2 * SVPWM_PERIOD + Tv;
            break;

        /* 扇区5  变换矩阵为             */
        /*        | -1  -3^0.5/3    |    */
        /*        |  1  -3^0.5/3    |    */
        case 5:
            U_1 = -alpha - beta_div_sqrt3; /* 0 0 W */
            U_2 = alpha - beta_div_sqrt3;  /* U 0 W */
            Tv = (1.0f - modulation * (U_1 + U_2)) * SVPWM_PERIOD * 0.5f;
            Tu = modulation * U_2 * SVPWM_PERIOD + Tv;
            Tw = modulation * U_1 * SVPWM_PERIOD + Tu;
            break;

        /* 扇区6  变换矩阵为             */
        /*        |  0  -3^0.5*2/3  |    */
        /*        |  1   3^0.5/3    |    */
        case 6:
            U_1 = -2.0f * beta_div_sqrt3; /* U 0 W */
            U_2 = alpha + beta_div_sqrt3; /* U 0 0 */
            Tv = (1.0f - modulation * (U_1 + U_2)) * SVPWM_PERIOD * 0.5f;
            Tw = modulation * U_1 * SVPWM_PERIOD + Tv;
            Tu = modulation * U_2 * SVPWM_PERIOD + Tw;
            break;

        default:
            U_1 = 0.0f; /* 0 0 0 */
            U_2 = 0.0f; /* 0 0 0 */
            Tu = (1.0f - modulation * (U_1 + U_2)) * SVPWM_PERIOD * 0.5f;
            Tv = modulation * U_1 * SVPWM_PERIOD + Tu;
            Tw = modulation * U_2 * SVPWM_PERIOD + Tv;
            break;
    }

    output->x1 = Tu;
    output->x2 = Tv;
    output->x3 = Tw;
    return;
}





/* 基于幅角计算的 SVPWM 算法，理论验证通过，但代码未自己核对，可能存在BUG */
/* 该算法仅仅用作耗时对比 */
#define PI2 6.2831853f
#define PI_3 1.047197551f

void SVPWM2(Vector2 *input, float modulation, Vector3 *output){
    
    /* 判断扇区 */
    float alpha = input->x1;
    float beta  = input->x2;
    float theta = atan2f(beta, alpha);
    float len = beta * beta + alpha * alpha;
    len = sqrtf(len);

    if (theta < 0)
        theta = theta + PI2;

    FOC8BitsNumber sector = (FOC8BitsNumber)(theta / PI_3) + 1;

    if (sector > 6)
        sector = 6;

    float omega = (sector - 1) * 1.047197551f; /* 计算扇区旋转角 */

    /* 计算电压基矢量 */
    /* 计算SVPWM脉冲宽度：先计算零矢量T7，然后依次累加出另外两个基矢量 */
    float U_1, U_2;
    float Tu, Tv, Tw;

    U_1 = SinByLinearLut(PI_3 - theta + omega) * 2 * SQRT3_3_NUM * len;
    U_2 = SinByLinearLut(theta - omega) * 2 * SQRT3_3_NUM * len;

    switch (sector) {

        case 1:
            Tw = (1.0f - modulation * (U_1 + U_2)) * SVPWM_PERIOD * 0.5f;
            Tv = modulation * U_2 * SVPWM_PERIOD + Tw;
            Tu = modulation * U_1 * SVPWM_PERIOD + Tv;
            break;

        case 2:
            Tw = (1.0f - modulation * (U_1 + U_2)) * SVPWM_PERIOD * 0.5f;
            Tu = modulation * U_1 * SVPWM_PERIOD + Tw;
            Tv = modulation * U_2 * SVPWM_PERIOD + Tu;
            break;

        case 3:
            Tu = (1.0f - modulation * (U_1 + U_2)) * SVPWM_PERIOD * 0.5f;
            Tw = modulation * U_2 * SVPWM_PERIOD + Tu;
            Tv = modulation * U_1 * SVPWM_PERIOD + Tw;
            break;

        case 4:
            Tu = (1.0f - modulation * (U_1 + U_2)) * SVPWM_PERIOD * 0.5f;
            Tv = modulation * U_1 * SVPWM_PERIOD + Tu;
            Tw = modulation * U_2 * SVPWM_PERIOD + Tv;
            break;

        case 5:
            Tv = (1.0f - modulation * (U_1 + U_2)) * SVPWM_PERIOD * 0.5f;
            Tu = modulation * U_2 * SVPWM_PERIOD + Tv;
            Tw = modulation * U_1 * SVPWM_PERIOD + Tu;
            break;

        case 6:
            Tv = (1.0f - modulation * (U_1 + U_2)) * SVPWM_PERIOD * 0.5f;
            Tw = modulation * U_1 * SVPWM_PERIOD + Tv;
            Tu = modulation * U_2 * SVPWM_PERIOD + Tw;
            break;

        default:
            U_1 = 0.0f; /* 0 0 0 */
            U_2 = 0.0f; /* 0 0 0 */
            Tu = (1.0f - modulation * (U_1 + U_2)) * SVPWM_PERIOD * 0.5f;
            Tv = modulation * U_1 * SVPWM_PERIOD + Tu;
            Tw = modulation * U_2 * SVPWM_PERIOD + Tv;
            break;
    }

    output->x1 = Tu;
    output->x2 = Tv;
    output->x3 = Tw;
    return;
}