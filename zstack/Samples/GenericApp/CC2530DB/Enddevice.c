/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
* �ļ���  �� Enddevice
* ����    �� Fuxinjie
* �汾    �� V1.0.0
* ʱ��    �� 2021/5/11
* ��Ҫ    �� �ն˽ڵ�  
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
#include "GenericApp.h"
#include "DebugTrace.h"
#if !defined( WIN32 )
#include "OnBoard.h"
#endif
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"
// This list should be filled with Application specific Cluster IDs.����б�Ӧ�����ض���Ӧ�ó���ļ�ȺID��䡣 
const cId_t GenericApp_ClusterList[GENERICAPP_MAX_CLUSTERS] =
{
    GENERICAPP_CLUSTERID    //��ID
 };//GenericApp_ClusterList����һ���أ�����GENERICAPP_MAX_CLUSTERS���������һһ�г������ID��
const SimpleDescriptionFormat_t GenericApp_SimpleDesc =
{
  
    GENERICAPP_ENDPOINT,              //  int Endpoint;
    GENERICAPP_PROFID,                //  uint16 AppProfId[2];
    GENERICAPP_DEVICEID,              //  uint16 AppDeviceId[2];
    GENERICAPP_DEVICE_VERSION,        //  int   AppDevVer:4;
    GENERICAPP_FLAGS,                 //  int   AppFlags:4;
    0,          
    (cId_t *)NULL, 
    GENERICAPP_MAX_CLUSTERS,          
    (cId_t *)GenericApp_ClusterList, 
};
endPointDesc_t GenericApp_epDesc;//����������ڵ�������GenericApp_epDesc
byte GenericApp_TaskID;     //����������������ȼ�GenericApp_TaskID
byte GenericApp_TransID;    //������������ݷ������к�GenericApp_TransID
devStates_t GenericApp_NwkState;//�洢����״̬�ı���
/* ���� ------------------------------------------------------------------*/
void GenericApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );
void GenericApp_SendTheMessage(void);


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ��������GenericApp_Init
 * ������uint8 task_id
 * ���ߣ�fangwei
 * ���أ�void
 * ������ͨ��Ӧ�ó����ʼ��
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


void GenericApp_Init( uint8 task_id )           //��ʼ��ͨ��Ӧ�ó���
{
 
    GenericApp_TaskID = task_id;               //��ʼ���������ȼ�
 
    GenericApp_NwkState = DEV_INIT;           //��ʼ������״̬
 
    GenericApp_TransID = 0;                  //���������ݰ������кų�ʼ��
 
    GenericApp_epDesc.endPoint = GENERICAPP_ENDPOINT;    //Ӧ�ó���Ķ˿ں�
 
    GenericApp_epDesc.task_id = &GenericApp_TaskID;     //�ڵ�������������ID
 
    GenericApp_epDesc.simpleDesc                 //��������
           
        = (SimpleDescriptionFormat_t *)&GenericApp_SimpleDesc;
  
    GenericApp_epDesc.latencyReq = noLatencyReqs;      //��ʱ����
 
    afRegister( &GenericApp_epDesc );
   
      //ʹ��afRegister�������ڵ�����������ע��
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ��������GenericApp_ProcessEvent
* ������uint8 task_id, uint16 events
* ���أ�uint16 
* ��������������
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
uint16 GenericApp_ProcessEvent( uint8 task_id, uint16 events )
{
    afIncomingMSGPacket_t *MSGpkt;
//����һ��ָ�������Ϣ�ṹ���ָ��
    if ( events & SYS_EVENT_MSG )
    {
        //�����յ������û���յ�����NULL
        MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    //��������Ϣ�����ϵ���Ϣ��������ָ����յ����������ݰ���ָ��
        while ( MSGpkt )
            //�����Ϊ��ʱ���ж���Ϣ������
    
        {
      
            switch ( MSGpkt->hdr.event )//�������ж�SYS_EVENT_MSG�¼����ͣ���ͬ��SYS_EVENT_MSG������Ҫ��ͬ�Ĵ���
      
            {
       
      
                case ZDO_STATE_CHANGE:	//��������󴥷�ZDO_STATE_CHANGE�¼�
       
                    GenericApp_NwkState = (devStates_t)(MSGpkt->hdr.status); //��õ�ǰ��״̬
        
                    if(GenericApp_NwkState == DEV_END_DEVICE)   //�����ն˽ڵ��������
       
                    {
          
                        GenericApp_SendTheMessage();//���÷�����Ϣ�ĺ�������������
       
                    }
        
                    break;
       
            default:
          
                break;
      
            }
      
            osal_msg_deallocate( (uint8 *)MSGpkt );//�ͷ��ڴ�
     
            MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    
        }
    
        return (events ^ SYS_EVENT_MSG); // ����δ������¼� 
 
    }
  
    return 0;

}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ������  �� GenericApp_SendTheMessage
* ����    �� void
* ����    �� void
* ����    �� Shenfuming
* ʱ��    �� 2021/5/11
* ����    �� ���ݷ��ͺ���
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//������Ϣ�������ص����˺��������κ����������-�������������豸����ˣ����ݼ�ȺID��ִ��Ԥ�ڵĲ����� 

void GenericApp_SendTheMessage(void)
{
    unsigned char theMessageData[4]="LED";//���Ҫ���͵�����
    afAddrType_t my_DstAddr;

    my_DstAddr.addrMode =(afAddrMode_t)Addr16Bit;//���͵�ַģʽ����Ϊ����
    my_DstAddr.endPoint =GENERICAPP_ENDPOINT;
//��ʼ���˿ں�
    my_DstAddr.addr.shortAddr =0x0000;
//ֱ��ָ��Э�����������ַ
    AF_DataRequest( &my_DstAddr,&GenericApp_epDesc,
//�������ݷ��ͺ���AF_DataRequst�����������ݵķ���
                   GENERICAPP_CLUSTERID,
                   3,
                   theMessageData,//���ݰ�
                   &GenericApp_TransID,//���� ID
                   AF_DISCV_ROUTE,//ѡ��
                   AF_DEFAULT_RADIUS);//���Ͱ뾶 

    HalLedBlink (HAL_LED_2,0,50,500);
//ʹ�ն˽ڵ�led3��˸
}
