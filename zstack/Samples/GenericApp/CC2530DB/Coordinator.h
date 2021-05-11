/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
* 文件名  ： Coordinator
* 作者    ： Fuxinjie
* 版本    ： V1.0.0
* 时间    ： 2021/5/11
* 简要    ： Coordinator的头文件  
********************************************************************
* 副本
*
*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

#ifndef COORDINATOR_H
#define COORDINATOR_H

/* 头文件 ----------------------------------------------------------------*/
#include "ZComDef.h"
/* 宏定义 ----------------------------------------------------------------*/
#define GENERICAPP_ENDPOINT 10//端口在这里使用的是端口号10，可用的范围为（1~240）
#define GENERICAPP_PROFID 0x0F04
#define GENERICAPP_DEVICEID 0x0001
#define GENERICAPP_DEVICE_VERSION   0
#define GENERICAPP_FLAGS  0
#define GENERICAPP_MAX_CLUSTERS    1 //定义一个簇的大小，一个簇的大小为N
#define GENERICAPP_CLUSTERID 1//定义一个簇中的一个一个命令ID
/* 结构体或枚举 ----------------------------------------------------------*/
/* 函数声明---------------------------------------------------------------*/
/* 外部变量引用 ----------------------------------------------------------*/ 
extern void GenericApp_Init(byte task_id ) ;
extern UINT16 GenericApp_ProcessEvent(byte task_id,UINT16 events ) ;
#endif
