/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
* �ļ���  �� Coordinator
* ����    �� Fuxinjie
* �汾    �� V1.0.0
* ʱ��    �� 2021/5/11
* ��Ҫ    �� Э����  
********************************************************************
* ����
*
*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/* ͷ�ļ� ----------------------------------------------------------------*/
#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"
#include <string.h>
#include "Coordinator.h"
#include "DebugTrace.h"


#if ! defined ( WIN32 )
#include "OnBoard.h"
#endif


#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"

const cId_t GenericApp_ClusterList[GENERICAPP_MAX_CLUSTERS] =
//GENERICAPP MAX CLUSTERS����Coordinator.h�ļ��ж����
{
GENERICAPP_CLUSTERID
};//GenericApp_ClusterList����һ���أ�����GENERICAPP_MAX_CLUSTERS���������һһ�г������ID��

//Ϊ�˸�Э��ջ�������ݵĶ����ʽ����һ��

const SimpleDescriptionFormat_t GenericApp_SimpleDesc =
{

    GENERICAPP_ENDPOINT,//�˿ں�
    GENERICAPP_PROFID,//Ӧ�ù淶ID
    GENERICAPP_DEVICEID,//Ӧ���豸ID
    GENERICAPP_DEVICE_VERSION,//Ӧ���豸�汾��
    GENERICAPP_FLAGS,

    GENERICAPP_MAX_CLUSTERS,//����ذ������������
    (cId_t *)GenericApp_ClusterList,//������б� ����GENERICAPP_MAX_CLUSTERS����������
    0,(cId_t *)NULL

};
//���豸������
/*
�������
*/
endPointDesc_t GenericApp_epDesc;//�ڵ������� endPointDesc_t�ṹ���ﶨ����ԡ�_t����β
byte GenericApp_TaskID;
//�������ȼ�
byte GenericApp_TransID;
//���ݷ������к�




/* ���� ------------------------------------------------------------------*/

void GenericApp_MessageMSGCB(afIncomingMSGPacket_t *pckt);//������Ϣ������
void GenericApp_SendTheMessage(void);//�������ݷ��ͺ���

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ������  �� GenericApp_Init
* ����    �� byte task_id
* ����    �� void
* ����    �� Shenfuming
* ʱ��    �� 2021/5/11
* ����    �� �����ʼ������
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GenericApp_Init( byte task_id )    //�����ʼ����������ʽ��Ϊ�̶�
{
  
    GenericApp_TaskID=task_id;      
  
    GenericApp_TransID = 0;
  
    GenericApp_epDesc.endPoint = GENERICAPP_ENDPOINT;
  
    GenericApp_epDesc.task_id = &GenericApp_TaskID;
  
    GenericApp_epDesc.simpleDesc
            
        = (SimpleDescriptionFormat_t *)&GenericApp_SimpleDesc;
  
    GenericApp_epDesc.latencyReq = noLatencyReqs;
  
  
    afRegister( &GenericApp_epDesc );   //�Ǽ�ע��˵�����
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * �������� GenericApp_ProcessEvent 
 * ������ byte task_id,UINT16 events 
 * ���أ� void 
 * ���ߣ� fangwei
 * ������ ��Ϣ������
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


UINT16 GenericApp_ProcessEvent(byte task_id,UINT16 events) 

{
     afIncomingMSGPacket_t*MSGpkt;  //����һ��ָ�������Ϣ�ṹ���ָ��
     if (events & SYS_EVENT_MSG )
     {
         MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive(GenericApp_TaskID );
    //��������Ϣ�����ϵ���Ϣ��������ָ����յ����������ݰ���ָ��

         while ( MSGpkt )
        {
             switch ( MSGpkt->hdr.event )
             {
             case AF_INCOMING_MSG_CMD:    //�Խ��ܵ����ݽ����ж�
                 GenericApp_MessageMSGCB( MSGpkt );   //�������ݵĴ���ִ����Ϣ�ص�����
                 break;
             default:
                 break;
             }
             osal_msg_deallocate ( (uint8 *) MSGpkt );   //��ռ�ݵĶ��ڴ��ͷ�
             MSGpkt = (afIncomingMSGPacket_t *) osal_msg_receive(GenericApp_TaskID );
               //������һ����Ϣ���ٴ���Ϣ�����������Ϣ

         return (events ^ SYS_EVENT_MSG) ;     //����δ������¼�
     }
         return 0;
 }
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ��������GenericApp_MessageMSGCB
* ������afIncomingMSGPacket_t *pkt
* ���أ�void
* ��������Ϣ������
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GenericApp_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{
  unsigned char buffer[4]=" ";
  switch ( pkt->clusterId )
  {
    case GENERICAPP_CLUSTERID:
      // "the" message
        osal_memcpy(buffer,pkt->cmd.Data,3);//�����ܵ����ݿ�����������buffer
        if ( (buffer[0] == 'L')|| (buffer[1] == 'E') || (buffer[2]== 'D') )

        {

            HalLedBlink(HAL_LED_2,0,50,500);//LED2��˸��HalLedBlink��������˸
            
}

        else
        {

           HalLedSet(HAL_LED_2,HAL_LED_MODE_ON) ;//SET�����˵���Ϩ��ת��

        }

        break;
  }
}
