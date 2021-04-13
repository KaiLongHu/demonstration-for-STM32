#include "dataprocessing.h"
#include "mma8452q.h"
#include "delay.h"
#include "w25qxx.h"

short atcx,atcy,atcz;
int time1;
short tem1=0,tem2=0,tem3=0;


//取3S中30次加速度值取平均

//*********************************************************************************
//u8 get_0GStable_data(short *asx,short *asy,short *asz)//asx 加速度stable x轴,均值法
//{
//MMA_Get_Accelerometer(&atcx,&atcy,&atcz);
//	while(time1!=30)
//		{
//			tem1+=atcx;
//			tem2+=atcy;
//			tem3+=atcz;
//		  time1++;
//			delay_ms(100);
//	  }
//			*asx=tem1/30;
//			*asy=tem2/30;
//			*asz=tem3/30;
//		return 0;
//}

u8 get_0GStable_data(short *asx,short *asy,short *asz)//不取均值ASZ的值不正确
{
MMA_Get_Accelerometer(&atcx,&atcy,&atcz);
	
			*asx=atcx;
			*asy=atcy;
			*asz=atcz;
	return 0;
}

union
{
float x;
	u8 s[4];
}FTC;



void flash_write(float *a,u16 data_len,u32 startadr)
{
u16 i;
	for(i=0;i<data_len;i++)
	{
		FTC.x=a[i];
		W25QXX_Write(FTC.s,startadr+4*i,4);
	}
}


void flash_read(float *b,u16 num,u32 startadr)
{
	u16 i;
	for(i=0;i<num;i++)
	{
		W25QXX_Read(FTC.s,startadr+4*i,4);
		b[i]=FTC.x;
	}

}