#ifndef __DATAPROCESSING_H
#define __DATAPROCESSING_H
#include "myiic.h"   	//u8 defined in this file




u8 get_0GStable_data(short *ax,short *ay,short *az);

void flash_write(float *a,u16 data_len,u32 startadr);
void flash_read(float *b,u16 num,u32 startadr);//num数据个数







#endif
