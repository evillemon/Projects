/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
* 文件名  ： Coordinator
* 作者    ： Fuxinjie
* 版本    ： V1.0.0
* 时间    ： 2021/5/11
* 简要    ： 协调器  
********************************************************************
* 副本
*
*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/* 头文件 ----------------------------------------------------------------*/
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
//GENERICAPP MAX CLUSTERS是在Coordinator.h文件中定义宏
{
GENERICAPP_CLUSTERID
};//GenericApp_ClusterList就是一个簇，他有GENERICAPP_MAX_CLUSTERS条命令，下面一一列出命令的ID号

//为了跟协议栈里面数据的定义格式保持一致

const SimpleDescriptionFormat_t GenericApp_SimpleDesc =
{

    GENERICAPP_ENDPOINT,//端口号
    GENERICAPP_PROFID,//应用规范ID
    GENERICAPP_DEVICEID,//应用设备ID
    GENERICAPP_DEVICE_VERSION,//应用设备版本号
    GENERICAPP_FLAGS,

    GENERICAPP_MAX_CLUSTERS,//输入簇包含的命令个数
    (cId_t *)GenericApp_ClusterList,//输入簇列表 可有GENERICAPP_MAX_CLUSTERS个输入命令
    0,(cId_t *)NULL

};
//简单设备描述符
/*
定义变量
*/
endPointDesc_t GenericApp_epDesc;//节点描述符 endPointDesc_t结构体里定义的以“_t”结尾
byte GenericApp_TaskID;
//任务优先级
byte GenericApp_TransID;
//数据发送序列号




/* 函数 ------------------------------------------------------------------*/

void GenericApp_MessageMSGCB(afIncomingMSGPacket_t *pckt);//声明消息处理函数
void GenericApp_SendTheMessage(void);//声明数据发送函数

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* 函数名  ： GenericApp_Init
* 参数    ： byte task_id
* 返回    ： void
* 作者    ： Shenfuming
* 时间    ： 2021/5/11
* 描述    ： 任务初始化函数
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GenericApp_Init( byte task_id )    //任务初始化函数，格式较为固定
{
  
    GenericApp_TaskID=task_id;      
  
    GenericApp_TransID = 0;
  
    GenericApp_epDesc.endPoint = GENERICAPP_ENDPOINT;
  
    GenericApp_epDesc.task_id = &GenericApp_TaskID;
  
    GenericApp_epDesc.simpleDesc
            
        = (SimpleDescriptionFormat_t *)&GenericApp_SimpleDesc;
  
    GenericApp_epDesc.latencyReq = noLatencyReqs;
  
  
    afRegister( &GenericApp_epDesc );   //登记注册端点描述
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 函数名： GenericApp_ProcessEvent 
 * 参数： byte task_id,UINT16 events 
 * 返回： void 
 * 作者： fangwei
 * 描述： 消息处理函数
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


UINT16 GenericApp_ProcessEvent(byte task_id,UINT16 events) 

{
     afIncomingMSGPacket_t*MSGpkt;  //定义一个指向接收消息结构体的指针
     if (events & SYS_EVENT_MSG )
     {
         MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive(GenericApp_TaskID );
    //接受了消息队列上的消息，包含了指向接收到的无线数据包的指针

         while ( MSGpkt )
        {
             switch ( MSGpkt->hdr.event )
             {
             case AF_INCOMING_MSG_CMD:    //对接受的数据进行判断
                 GenericApp_MessageMSGCB( MSGpkt );   //无线数据的处理，执行信息回调函数
                 break;
             default:
                 break;
             }
             osal_msg_deallocate ( (uint8 *) MSGpkt );   //将占据的堆内存释放
             MSGpkt = (afIncomingMSGPacket_t *) osal_msg_receive(GenericApp_TaskID );
               //处理完一个消息后，再从消息队列里接收消息

         return (events ^ SYS_EVENT_MSG) ;     //返回未处理的事件
     }
         return 0;
 }
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* 函数名：GenericApp_MessageMSGCB
* 参数：afIncomingMSGPacket_t *pkt
* 返回：void
* 描述：消息处理函数
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GenericApp_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{
  unsigned char buffer[4]=" ";
  switch ( pkt->clusterId )
  {
    case GENERICAPP_CLUSTERID:
      // "the" message
        osal_memcpy(buffer,pkt->cmd.Data,3);//将接受的数据拷贝到缓冲区buffer
        if ( (buffer[0] == 'L')|| (buffer[1] == 'E') || (buffer[2]== 'D') )

        {

            HalLedBlink(HAL_LED_2,0,50,500);//LED2闪烁，HalLedBlink定义了闪烁
            
}

        else
        {

           HalLedSet(HAL_LED_2,HAL_LED_MODE_ON) ;//SET定义了点亮熄灭翻转等

        }

        break;
  }
}
