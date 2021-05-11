/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
* 文件名  ： Enddevice
* 作者    ： Fuxinjie
* 版本    ： V1.0.0
* 时间    ： 2021/5/11
* 简要    ： 终端节点  
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
#include "GenericApp.h"
#include "DebugTrace.h"
#if !defined( WIN32 )
#include "OnBoard.h"
#endif
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"
// This list should be filled with Application specific Cluster IDs.这个列表应该用特定于应用程序的集群ID填充。 
const cId_t GenericApp_ClusterList[GENERICAPP_MAX_CLUSTERS] =
{
    GENERICAPP_CLUSTERID    //簇ID
 };//GenericApp_ClusterList就是一个簇，他有GENERICAPP_MAX_CLUSTERS条命令，下面一一列出命令的ID号
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
endPointDesc_t GenericApp_epDesc;//定义变量：节点描述符GenericApp_epDesc
byte GenericApp_TaskID;     //定义变量：任务优先级GenericApp_TaskID
byte GenericApp_TransID;    //定义变量：数据发送序列号GenericApp_TransID
devStates_t GenericApp_NwkState;//存储网络状态的变量
/* 函数 ------------------------------------------------------------------*/
void GenericApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );
void GenericApp_SendTheMessage(void);


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 函数名：GenericApp_Init
 * 参数：uint8 task_id
 * 作者：fangwei
 * 返回：void
 * 描述：通用应用程序初始化
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


void GenericApp_Init( uint8 task_id )           //初始化通用应用程序
{
 
    GenericApp_TaskID = task_id;               //初始化任务优先级
 
    GenericApp_NwkState = DEV_INIT;           //初始化网络状态
 
    GenericApp_TransID = 0;                  //将发送数据包的序列号初始化
 
    GenericApp_epDesc.endPoint = GENERICAPP_ENDPOINT;    //应用程序的端口号
 
    GenericApp_epDesc.task_id = &GenericApp_TaskID;     //节点描述符的任务ID
 
    GenericApp_epDesc.simpleDesc                 //简单描述符
           
        = (SimpleDescriptionFormat_t *)&GenericApp_SimpleDesc;
  
    GenericApp_epDesc.latencyReq = noLatencyReqs;      //延时策略
 
    afRegister( &GenericApp_epDesc );
   
      //使用afRegister函数将节点描述符进行注册
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* 函数名：GenericApp_ProcessEvent
* 参数：uint8 task_id, uint16 events
* 返回：uint16 
* 描述：任务处理函数
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
uint16 GenericApp_ProcessEvent( uint8 task_id, uint16 events )
{
    afIncomingMSGPacket_t *MSGpkt;
//定义一个指向接收消息结构体的指针
    if ( events & SYS_EVENT_MSG )
    {
        //检索收到的命令，没有收到返回NULL
        MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    //接受了消息队列上的消息，包含了指向接收到的无线数据包的指针
        while ( MSGpkt )
            //如果不为空时，判断消息的类型
    
        {
      
            switch ( MSGpkt->hdr.event )//这里是判断SYS_EVENT_MSG事件类型，不同的SYS_EVENT_MSG类型需要不同的处理。
      
            {
       
      
                case ZDO_STATE_CHANGE:	//加入网络后触发ZDO_STATE_CHANGE事件
       
                    GenericApp_NwkState = (devStates_t)(MSGpkt->hdr.status); //获得当前的状态
        
                    if(GenericApp_NwkState == DEV_END_DEVICE)   //若是终端节点加入网络
       
                    {
          
                        GenericApp_SendTheMessage();//调用发送信息的函数，发送数据
       
                    }
        
                    break;
       
            default:
          
                break;
      
            }
      
            osal_msg_deallocate( (uint8 *)MSGpkt );//释放内存
     
            MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    
        }
    
        return (events ^ SYS_EVENT_MSG); // 返回未处理的事件 
 
    }
  
    return 0;

}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* 函数名  ： GenericApp_SendTheMessage
* 参数    ： void
* 返回    ： void
* 作者    ： Shenfuming
* 时间    ： 2021/5/11
* 描述    ： 数据发送函数
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//数据信息处理器回调。此函数处理任何输入的数据-可能来自其他设备。因此，根据集群ID，执行预期的操作。 

void GenericApp_SendTheMessage(void)
{
    unsigned char theMessageData[4]="LED";//存放要发送的数据
    afAddrType_t my_DstAddr;

    my_DstAddr.addrMode =(afAddrMode_t)Addr16Bit;//发送地址模式设置为单播
    my_DstAddr.endPoint =GENERICAPP_ENDPOINT;
//初始化端口号
    my_DstAddr.addr.shortAddr =0x0000;
//直接指定协调器的网络地址
    AF_DataRequest( &my_DstAddr,&GenericApp_epDesc,
//调用数据发送函数AF_DataRequst进行无线数据的发送
                   GENERICAPP_CLUSTERID,
                   3,
                   theMessageData,//数据包
                   &GenericApp_TransID,//发送 ID
                   AF_DISCV_ROUTE,//选项
                   AF_DEFAULT_RADIUS);//发送半径 

    HalLedBlink (HAL_LED_2,0,50,500);
//使终端节点led3闪烁
}
