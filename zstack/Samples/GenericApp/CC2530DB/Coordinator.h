/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
* �ļ���  �� Coordinator
* ����    �� Fuxinjie
* �汾    �� V1.0.0
* ʱ��    �� 2021/5/11
* ��Ҫ    �� Coordinator��ͷ�ļ�  
********************************************************************
* ����
*
*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

#ifndef COORDINATOR_H
#define COORDINATOR_H

/* ͷ�ļ� ----------------------------------------------------------------*/
#include "ZComDef.h"
/* �궨�� ----------------------------------------------------------------*/
#define GENERICAPP_ENDPOINT 10//�˿�������ʹ�õ��Ƕ˿ں�10�����õķ�ΧΪ��1~240��
#define GENERICAPP_PROFID 0x0F04
#define GENERICAPP_DEVICEID 0x0001
#define GENERICAPP_DEVICE_VERSION   0
#define GENERICAPP_FLAGS  0
#define GENERICAPP_MAX_CLUSTERS    1 //����һ���صĴ�С��һ���صĴ�СΪN
#define GENERICAPP_CLUSTERID 1//����һ�����е�һ��һ������ID
/* �ṹ���ö�� ----------------------------------------------------------*/
/* ��������---------------------------------------------------------------*/
/* �ⲿ�������� ----------------------------------------------------------*/ 
extern void GenericApp_Init(byte task_id ) ;
extern UINT16 GenericApp_ProcessEvent(byte task_id,UINT16 events ) ;
#endif
