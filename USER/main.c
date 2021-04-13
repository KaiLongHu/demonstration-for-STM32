#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
//#include "mpu6050.h"
//#include "inv_mpu.h"
//#include "inv_mpu_dmp_motion_driver.h" 
#include "mma8452q.h"
#include "dataprocessing.h"
#include "arm_math.h"
//#include "timer.h"
#include "arm_const_structs.h"
#include "spi.h"    
#include "w25qxx.h" //flash
#include "sram.h"   //sram
#include "malloc.h" //memory management
#include "sdio_sdcard.h"  
#include "ff.h"  
#include "exfuns.h"  
#include "usmart.h"  
#include "fontupd.h"
#include "text.h"	
#include "piclib.h"
#include "usbh_usr.h" 




////test for fatfs
FRESULT res1;
FIL file1;
UINT BWW;
float buf1[50];


USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;


//用户测试主程序
//返回值:0,正常
//       1,有问题
u8 USH_User_App(void)//usb插入后会进入这个函数
{ 
	u32 total,free;
	u8 res=0;
	Show_Str(30,140,200,16,"设备连接成功!.",16,0);	 
	res=exf_getfree("2:",&total,&free);
	
	res1=f_open (&file1, "2:/myown11.txt", FA_CREATE_ALWAYS|FA_WRITE);
	f_write(&file1, "78787888888", 50,&BWW);	
	f_close(&file1);
	
	
	
	if(res==0)
	{
		POINT_COLOR=BLUE;//设置字体为蓝色	   
		LCD_ShowString(30,160,200,16,16,"FATFS OK!");	
		LCD_ShowString(30,180,200,16,16,"U Disk Total Size:     MB");	 
		LCD_ShowString(30,200,200,16,16,"U Disk  Free Size:     MB"); 	    
		LCD_ShowNum(174,180,total>>10,5,16); //显示U盘总容量 MB
		LCD_ShowNum(174,200,free>>10,5,16);	
	} 
 
	while(HCD_IsDeviceConnected(&USB_OTG_Core))//设备连接成功，进入死循环
	{	
		LED1=!LED1;
		delay_ms(200);
	}
	POINT_COLOR=RED;//设置字体为红色	   
	Show_Str(30,140,200,16,"设备连接中...",16,0);
	LCD_Fill(30,160,239,220,WHITE);
	return res;
} 






int main(void)
{        
	
	
	short aacx,aacy,aacz;		//加速度传感器原始数据
	float sox,soy,soz;    //stable oringal data for xyz
	float cx,cy,cz;        //changing data 	
	float bufx[30];				
	float bufy[30];
	float bufz[30];
	double x;
	double y;
	double z; 
  int USBFLAG;
	

	u8 t;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(9600);		//初始化串口1波特率为9600
	uart6_init(9600);			//初始化串口6波特率为9600
	
	LED_Init();				//初始化与LED连接的硬件接口
	KEY_Init();				//按键
	////为了防止干扰串口先注释
  //LCD_Init();				//初始化LCD 
	//#################################
	W25QXX_Init();			//SPI FLASH初始化
	MMA_Init();         //MMA初始化
	
	
	usmart_dev.init(84); 	//初始化USMART	 
	my_mem_init(SRAMIN);	//初始化内部内存池	
 	exfuns_init();			//为fatfs相关变量申请内存 
	piclib_init();			//初始化画图
  	f_mount(fs[0],"0:",1); 	//挂载SD卡  
  	f_mount(fs[1],"1:",1); 	//挂载SD卡  
  	f_mount(fs[2],"2:",1); 	//挂载U盘
//##########################################
//TLCD 显示屏设置  
//	POINT_COLOR=RED;    
// 	while(font_init()) 				//检查字库，正常的返回值是0
//	{	    
//		LCD_ShowString(60,50,200,16,16,"Font Error!");
//		delay_ms(200);				  
//		LCD_Fill(60,50,240,66,WHITE);//清除显示	     
//		delay_ms(200);				  
//	}
//	Show_Str(30,50,200,16,"探索者STM32F407开发板",16,0);				    	 
//	Show_Str(30,70,200,16,"USB U盘实验",16,0);					    	 
//	Show_Str(30,90,200,16,"2014年7月22日",16,0);	    	 
//	Show_Str(30,110,200,16,"正点原子@ALIENTEK",16,0); 
//	Show_Str(30,140,200,16,"设备连接中...",16,0);			 	
//##########################################	
	//初始化USB主机
  //为了防止干扰串口先注释
	//USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_cb);  
//##########################################
	
	
	//文件书写test
//		res1=f_open (&file1, "2:/myown11.txt", FA_CREATE_ALWAYS|FA_WRITE);
//	f_write(&file1, "78787888888", 50,&BWW);	
//	f_close(&file1);
//##########################################
	
//	while(USBFLAG)//USBFLAG激活优盘程序
//	{
//		USBH_Process(&USB_OTG_Core, &USB_Host);//会进入USER_APP中
//		delay_ms(1);
//		t++;
//		if(t==200)
//		{
//			LED0=!LED0;
//			t=0;
//		}
//	}	
//USART_SendData(USART1, 0x01);
//	printf("\r\n您发送的消息为:\r\n");
//USART1 发送数据
	u8 len;
	u8 p;
	//print fpga sensor 8bit data from USART1 
	while(1)
	{
 		if(USART6_RX_STA&0x8000)// bit 15 is 1, rec complete
		{					   
			len=USART6_RX_STA&0x3fff;//得到此次接收到的数据长度
			
			for(p=0;p<len;p++)
			{
				USART_SendData(USART1, USART6_RX_BUF[p]);         //向串口1发送串口6储存的数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			USART6_RX_STA=0;
		}
//##########################################	
	
	//STM32  LED test
//		LED0=1;
//##########################################
	//*******************************************************************************************************
	// MMA part
//	int i=0;
//	for(i=0;i<30;i++)//开始时取3S稳定
//	{
//	MMA_Get_Accelerometer(&aacx,&aacy,&aacz);
//	bufx[i]=aacx;
//	bufy[i]=aacy;
//	bufz[i]=aacz;
//	x+=bufx[i];
//	y+=bufy[i];
//	z+=bufz[i];			
//	delay_ms(10);	
//				//				printf("\r\n%d]:                      %f                    %f            %f",i,bufx[i],bufy[i],bufz[i]);
//				//			printf("\r\n:                      %f                    %f            %f",x,y,z);
//	}
//	
////soN数据是平稳状态下数据
//					sox=x/30;
//					soy=y/30;
//					soz=z/30;
////##########################################				
//	while(1)
//	{			
//		MMA_Get_Accelerometer(&aacx,&aacy,&aacz);//芯片数据读取
//				cx=(float)(aacx-sox)/16;	//应该是除以1693，为了方便显示现在除以16(近似扩大100倍）															
//				cy=(float)(aacy-soy)/16;
//				cz=(float)(aacz-soz)/16;
//		
//			printf("\r\n:    %f        %f            %f",cx,cy,cz);		
//		delay_ms(150);
//  }
////###################################################################################################	
}
	}
