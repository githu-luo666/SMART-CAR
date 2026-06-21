# 🏁 第二十届全国大学生智能汽车竞赛 —— 微缩光电组

<div align="center">
  <img src="https://img.shields.io/badge/Competition-20th_SmartCar-blue" alt="20th">
  <img src="https://img.shields.io/badge/Group-Micro_Photoelectric-brightgreen" alt="Group">
  <img src="https://img.shields.io/badge/MCU-CYT4BBmini-orange" alt="MCU">
</div>

> **谨以此代码纪念第二十届智能车竞速赛，致敬无数个在实验室调试代码、挑灯夜战的日日夜夜。**

---

## 📖 项目简介

本项目是参与 **第二十届全国大学生智能汽车竞赛（微缩光电组）** 的嵌入式控制程序。

该程序基于逐飞科技推出的 **LQ_CYT4BBminiMBV2** 核心板开发，利用光电传感器（红外/CCD/CMOS）对赛道元素进行识别，通过 PID 控制算法实现小车的快速寻迹与稳定过弯。代码包含了底层驱动配置、图像处理逻辑及核心控制策略。

## 🛠 硬件平台

- **主控芯片**：Infineon CYT4BB (LQ_CYT4BBminiMBV2 核心板)
- **传感器方案**：微缩光电传感器（红外对管 / 线性CCD）
- **动力部分**：微型直流减速电机 + 舵机转向
- **调试工具**：逐飞科技无线调试器 / 串口上位机

## 🗂 工程目录结构

```text
.
├── User/                   # 用户主程序入口
│   ├── main.c              # 主循环
│   └── isr.c               # 中断服务函数
├── Hardware/               # 底层硬件驱动
│   ├── GPIO/               # 引脚配置
│   ├── ADC/                # 传感器数据采集
│   ├── PWM/                # 电机与舵机驱动
│   └── UART/               # 串口调试输出
├── Control/                # 核心控制算法
│   ├── PID.c               # 位置式/增量式PID
│   └── Steering.c          # 转向与速度解算
├── Image/                  # 图像/信号处理
│   └── Process.c           # 二值化与赛道中线提取
├── Config/                 # 宏定义与参数配置
└── README.md               # 项目说明文档
