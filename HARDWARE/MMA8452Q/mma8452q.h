#ifndef __MMA8452Q_H
#define __MMA8452Q_H
#include "myiic.h"   	//u8 defined in this file

//这下面三行默认的是SA0是1
#define deviceRead_REG  0x3B
#define deviceWrite_REG 0x3A
 #define MMA_ADDR 0x1D
 
 
#define OUT_X_MSB_REG 0x01
#define OUT_Y_MSB_REG 0x03
#define OUT_Z_MSB_REG 0x05

//下面三行是从简单的MMA历程上复制来的
#define CTRL_REG1             0x2A
#define MMA8451_XYZ_DATA_CFG          0x0E
#define ACTIVE_MASK           0x01



#define CTRL_REG2             0x2B
#define CTRL_REG3             0x2C
#define CTRL_REG4             0x2D
#define CTRL_REG5             0x2E
#define WHO_AM_I              0X0D

u8 MMA_Init(void);
u8 MMA_Write_Byte(u8 reg,u8 data);
u8 MMA_Read_Byte(u8 reg);
u8 MMA_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf);
u8 MMA_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf);
u8 MMA_Get_Accelerometer(short *ax,short *ay,short *az);















#endif
