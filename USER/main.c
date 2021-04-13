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


//�û�����������
//����ֵ:0,����
//       1,������
u8 USH_User_App(void)//usb����������������
{ 
	u32 total,free;
	u8 res=0;
	Show_Str(30,140,200,16,"�豸���ӳɹ�!.",16,0);	 
	res=exf_getfree("2:",&total,&free);
	
	res1=f_open (&file1, "2:/myown11.txt", FA_CREATE_ALWAYS|FA_WRITE);
	f_write(&file1, "78787888888", 50,&BWW);	
	f_close(&file1);
	
	
	
	if(res==0)
	{
		POINT_COLOR=BLUE;//��������Ϊ��ɫ	   
		LCD_ShowString(30,160,200,16,16,"FATFS OK!");	
		LCD_ShowString(30,180,200,16,16,"U Disk Total Size:     MB");	 
		LCD_ShowString(30,200,200,16,16,"U Disk  Free Size:     MB"); 	    
		LCD_ShowNum(174,180,total>>10,5,16); //��ʾU�������� MB
		LCD_ShowNum(174,200,free>>10,5,16);	
	} 
 
	while(HCD_IsDeviceConnected(&USB_OTG_Core))//�豸���ӳɹ���������ѭ��
	{	
		LED1=!LED1;
		delay_ms(200);
	}
	POINT_COLOR=RED;//��������Ϊ��ɫ	   
	Show_Str(30,140,200,16,"�豸������...",16,0);
	LCD_Fill(30,160,239,220,WHITE);
	return res;
} 






int main(void)
{        
	
	
	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(9600);		//��ʼ������1������Ϊ9600
	uart6_init(9600);			//��ʼ������6������Ϊ9600
	
	LED_Init();				//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();				//����
	////Ϊ�˷�ֹ���Ŵ�����ע��
  //LCD_Init();				//��ʼ��LCD 
	//#################################
	W25QXX_Init();			//SPI FLASH��ʼ��
	MMA_Init();         //MMA��ʼ��
	
	
	usmart_dev.init(84); 	//��ʼ��USMART	 
	my_mem_init(SRAMIN);	//��ʼ���ڲ��ڴ��	
 	exfuns_init();			//Ϊfatfs��ر��������ڴ� 
	piclib_init();			//��ʼ����ͼ
  	f_mount(fs[0],"0:",1); 	//����SD��  
  	f_mount(fs[1],"1:",1); 	//����SD��  
  	f_mount(fs[2],"2:",1); 	//����U��
//##########################################
//TLCD ��ʾ������  
//	POINT_COLOR=RED;    
// 	while(font_init()) 				//����ֿ⣬�����ķ���ֵ��0
//	{	    
//		LCD_ShowString(60,50,200,16,16,"Font Error!");
//		delay_ms(200);				  
//		LCD_Fill(60,50,240,66,WHITE);//�����ʾ	     
//		delay_ms(200);				  
//	}
//	Show_Str(30,50,200,16,"̽����STM32F407������",16,0);				    	 
//	Show_Str(30,70,200,16,"USB U��ʵ��",16,0);					    	 
//	Show_Str(30,90,200,16,"2014��7��22��",16,0);	    	 
//	Show_Str(30,110,200,16,"����ԭ��@ALIENTEK",16,0); 
//	Show_Str(30,140,200,16,"�豸������...",16,0);			 	
//##########################################	
	//��ʼ��USB����
  //Ϊ�˷�ֹ���Ŵ�����ע��
	//USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_cb);  
//##########################################
	
	
	//�ļ���дtest
//		res1=f_open (&file1, "2:/myown11.txt", FA_CREATE_ALWAYS|FA_WRITE);
//	f_write(&file1, "78787888888", 50,&BWW);	
//	f_close(&file1);
//##########################################
	
//	while(USBFLAG)//USBFLAG�������̳���
//	{
//		USBH_Process(&USB_OTG_Core, &USB_Host);//�����USER_APP��
//		delay_ms(1);
//		t++;
//		if(t==200)
//		{
//			LED0=!LED0;
//			t=0;
//		}
//	}	
//USART_SendData(USART1, 0x01);
//	printf("\r\n�����͵���ϢΪ:\r\n");
//USART1 ��������
	u8 len;
	u8 p;
	//print fpga sensor 8bit data from USART1 
	while(1)
	{
 		if(USART6_RX_STA&0x8000)// bit 15 is 1, rec complete
		{					   
			len=USART6_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			
			for(p=0;p<len;p++)
			{
				USART_SendData(USART1, USART6_RX_BUF[p]);         //�򴮿�1���ʹ���6���������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
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
//	for(i=0;i<30;i++)//��ʼʱȡ3S�ȶ�
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
////soN������ƽ��״̬������
//					sox=x/30;
//					soy=y/30;
//					soz=z/30;
////##########################################				
//	while(1)
//	{			
//		MMA_Get_Accelerometer(&aacx,&aacy,&aacz);//оƬ���ݶ�ȡ
//				cx=(float)(aacx-sox)/16;	//Ӧ���ǳ���1693��Ϊ�˷�����ʾ���ڳ���16(��������100����															
//				cy=(float)(aacy-soy)/16;
//				cz=(float)(aacz-soz)/16;
//		
//			printf("\r\n:    %f        %f            %f",cx,cy,cz);		
//		delay_ms(150);
//  }
////###################################################################################################	
}
	}
