<!--
 * @Author       : LuHeQiu
 * @Date         : 2021-12-08 13:26:15
 * @LastEditTime : 2021-12-13 01:12:02
 * @LastEditors  : DeaneChen
 * @Description  : 
 * @FilePath     : \motor-controller-with-foc\README.en.md
 * HomePage     : https://luheqiu.com
-->
<div align="center">
    <br/>
    <h1>
        <a href="http://www.luheqiu.com" target="_blank" rel="noopener noreferrer">
        <img src="https://cxbox.luheqiu.com/Static/image/LuHeQiu_LOGO(1920x1080)HD.png" alt="LuHeQiu Logo">
        </a>
    </h1>
    <strong>路何求创想工坊出品 | 硬件&算法工坊</strong>
</div>

# 【FOC+STM32】Dual-Channel Controller of BLDC Motor with FOC - README

<p align="center">
    <img src="https://img.shields.io/badge/Release-v0.1.0-blue">
    <img src="https://img.shields.io/badge/dynamic/json?style=flat-square&label=Gitee%20stars&url=https://gitee.com/api/v5/repos/luheqiu/motor-controller-with-foc&query=$.stargazers_count">
    <img src="https://img.shields.io/badge/Platform-KEIL & AD-lightgray">
    <img src="https://img.shields.io/badge/Progress-疫情延期中-red">
    <img src="https://img.shields.io/badge/Language-C-brightgreen">
</p>

#### INTRODUCTION
The main control chip of the controller is STM32F405RGT6. And the controller is based on the HAL library and the FreeRTOS. It reserves one CAN, USART, SWD, and USB interface, which is convenient for communication and control engineering applications. The controller provides dual-channel brushless motor control, and reserves the encoder interface and voltage sampling interface respectively, which is suitable for control applications or algorithm verification of inductive FOC and non-inductive FOC. Besides, the control board can also be suitable for vector control of asynchronous motors.

> **Note: This project is a hardware design and algorithm verification project. It is currently in the design and verification stage. Complete engineering application tests and extreme parameter stress tests have not been carried out. Please do not use it directly to avoid safety issues and property losses. The subsequent verification will be updated synchronously**

