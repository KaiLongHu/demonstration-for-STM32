#include "mma8452q.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"  


u8     BUF[8];                   //接收数据缓存区 
int    dis_data;                 //变量
float  x,y,z;					 //存放合成的数据		


u8 MMA_Init()
{
IIC_Init();
  MMA_Write_Byte(CTRL_REG1,0x01);//active mode on
	MMA_Write_Byte(CTRL_REG2,0x02);//高分辨率模式
	MMA_Write_Byte(CTRL_REG3,0X00);//下面这3行未知，测试有没有用
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
	IIC_Send_Byte((MMA_ADDR<<1)|0);//发送器件地址+写命令	
	if(IIC_Wait_Ack())	//等待应答
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答 
	IIC_Send_Byte(data);//发送数据
	if(IIC_Wait_Ack())	//等待ACK
	{
		IIC_Stop();	 
		return 1;		 
	}		 
    IIC_Stop();	 
	return 0;
}



//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
u8 MMA_Read_Byte(u8 reg)
{
	u8 res;
    IIC_Start(); 
	IIC_Send_Byte((MMA_ADDR<<1)|0);//发送器件地址+写命令	
	IIC_Wait_Ack();		//等待应答 
    IIC_Send_Byte(reg);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答
    IIC_Start();
	IIC_Send_Byte((MMA_ADDR<<1)|1);//发送器件地址+读命令	
    IIC_Wait_Ack();		//等待应答 
	res=IIC_Read_Byte(0);//读取数据,发送nACK 
    IIC_Stop();			//产生一个停止条件 
	return res;		
}

u8 MMA_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(); 
	IIC_Send_Byte((MMA_ADDR<<1)|0);//发送器件地址+写命令	
	if(IIC_Wait_Ack())	//等待应答
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答
	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(buf[i]);	//发送数据
		if(IIC_Wait_Ack())		//等待ACK
		{
			IIC_Stop();	 
			return 1;		 
		}		
	}    
    IIC_Stop();	 
	return 0;	
} 
//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
u8 MMA_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	IIC_Start(); 
	
	IIC_Send_Byte((MMA_ADDR<<1)|0);//发送器件地址+写命令	
	if(IIC_Wait_Ack())	//等待应答
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答
    IIC_Start();
	IIC_Send_Byte((MMA_ADDR<<1)|1);//发送器件地址+读命令	
    IIC_Wait_Ack();		//等待应答 
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(0);//读数据,发送nACK 
		else *buf=IIC_Read_Byte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
    IIC_Stop();	//产生一个停止条件 
	return 0;	
}


//***********************************************************************************************************
//***********************************************************************************************************
//***********************************************************************************************************

float Get_x(void)
{   
    dis_data = (BUF[0]<<8)|BUF[1];  //合成数据   
	dis_data >>= 4;					//数据合成，OUT_X_MSB :[7-0],OUT_X_LSB:[7-2],故合成后的数据需要右移两位
	x = (float)dis_data;
	return x;
}

//========得到Y轴的数据====================================
float Get_y(void)
{   
    dis_data=(BUF[2]<<8)|BUF[3];  //合成数据   
	dis_data>>=4;				  //数据合成，OUT_X_MSB :[7-0],OUT_X_LSB:[7-2],故合成后的数据需要右移两位
	y=(float)dis_data;
	return y;
}
//========得到Z轴的数据====================================
float Get_z(void)
{  
    dis_data=(BUF[4]<<8)|BUF[5];    //合成数据  
    dis_data>>=4;					//数据合成，OUT_X_MSB :[7-0],OUT_X_LSB:[7-2],故合成后的数据需要右移两位
	z=(float)dis_data;
	return z;
}


