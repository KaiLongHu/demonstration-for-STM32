#include "mma8452q.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"  


u8     BUF[8];                   //�������ݻ����� 
int    dis_data;                 //����
float  x,y,z;					 //��źϳɵ�����		


u8 MMA_Init()
{
IIC_Init();
  MMA_Write_Byte(CTRL_REG1,0x01);//active mode on
	MMA_Write_Byte(CTRL_REG2,0x02);//�߷ֱ���ģʽ
	MMA_Write_Byte(CTRL_REG3,0X00);//������3��δ֪��������û����
	MMA_Write_Byte(CTRL_REG4,0X00);
	MMA_Write_Byte(CTRL_REG5,0X00);
	
	
return 0;
}




u8 MMA_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=MMA_Read_Len(MMA_ADDR,OUT_X_MSB_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}



u8 MMA_Write_Byte(u8 reg,u8 data) 				 
{ 
    IIC_Start(); 
	IIC_Send_Byte((MMA_ADDR<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	IIC_Send_Byte(data);//��������
	if(IIC_Wait_Ack())	//�ȴ�ACK
	{
		IIC_Stop();	 
		return 1;		 
	}		 
    IIC_Stop();	 
	return 0;
}



//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
u8 MMA_Read_Byte(u8 reg)
{
	u8 res;
    IIC_Start(); 
	IIC_Send_Byte((MMA_ADDR<<1)|0);//����������ַ+д����	
	IIC_Wait_Ack();		//�ȴ�Ӧ�� 
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ��
    IIC_Start();
	IIC_Send_Byte((MMA_ADDR<<1)|1);//����������ַ+������	
    IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	res=IIC_Read_Byte(0);//��ȡ����,����nACK 
    IIC_Stop();			//����һ��ֹͣ���� 
	return res;		
}

u8 MMA_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(); 
	IIC_Send_Byte((MMA_ADDR<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(buf[i]);	//��������
		if(IIC_Wait_Ack())		//�ȴ�ACK
		{
			IIC_Stop();	 
			return 1;		 
		}		
	}    
    IIC_Stop();	 
	return 0;	
} 
//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//    ����,�������
u8 MMA_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	IIC_Start(); 
	
	IIC_Send_Byte((MMA_ADDR<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ��
    IIC_Start();
	IIC_Send_Byte((MMA_ADDR<<1)|1);//����������ַ+������	
    IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(0);//������,����nACK 
		else *buf=IIC_Read_Byte(1);		//������,����ACK  
		len--;
		buf++; 
	}    
    IIC_Stop();	//����һ��ֹͣ���� 
	return 0;	
}


//***********************************************************************************************************
//***********************************************************************************************************
//***********************************************************************************************************

float Get_x(void)
{   
    dis_data = (BUF[0]<<8)|BUF[1];  //�ϳ�����   
	dis_data >>= 4;					//���ݺϳɣ�OUT_X_MSB :[7-0],OUT_X_LSB:[7-2],�ʺϳɺ��������Ҫ������λ
	x = (float)dis_data;
	return x;
}

//========�õ�Y�������====================================
float Get_y(void)
{   
    dis_data=(BUF[2]<<8)|BUF[3];  //�ϳ�����   
	dis_data>>=4;				  //���ݺϳɣ�OUT_X_MSB :[7-0],OUT_X_LSB:[7-2],�ʺϳɺ��������Ҫ������λ
	y=(float)dis_data;
	return y;
}
//========�õ�Z�������====================================
float Get_z(void)
{  
    dis_data=(BUF[4]<<8)|BUF[5];    //�ϳ�����  
    dis_data>>=4;					//���ݺϳɣ�OUT_X_MSB :[7-0],OUT_X_LSB:[7-2],�ʺϳɺ��������Ҫ������λ
	z=(float)dis_data;
	return z;
}


