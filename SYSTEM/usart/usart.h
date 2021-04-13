#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//Mini STM32¿ª·¢°å
//´®¿Ú1³õÊ¼»¯		   
//ÕýµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.csom
//ÐÞ¸ÄÈÕÆÚ:2011/6/14
//°æ±¾£ºV1.4
//°æÈ¨ËùÓÐ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ÕýµãÔ­×Ó 2009-2019
//All rights reserved
//********************************************************************************
//V1.3ÐÞ¸ÄËµÃ÷ 
//Ö§³ÖÊÊÓ¦²»Í¬ÆµÂÊÏÂµÄ´®¿Ú²¨ÌØÂÊÉèÖÃ.
//¼ÓÈëÁË¶ÔprintfµÄÖ§³Ö
//Ôö¼ÓÁË´®¿Ú½ÓÊÕÃüÁî¹¦ÄÜ.
//ÐÞÕýÁËprintfµÚÒ»¸ö×Ö·û¶ªÊ§µÄbug
//V1.4ÐÞ¸ÄËµÃ÷
//1,ÐÞ¸Ä´®¿Ú³õÊ¼»¯IOµÄbug
//2,ÐÞ¸ÄÁËUSART_RX_STA,Ê¹µÃ´®¿Ú×î´ó½ÓÊÕ×Ö½ÚÊýÎª2µÄ14´Î·½
//3,Ôö¼ÓÁËUSART_REC_LEN,ÓÃÓÚ¶¨Òå´®¿Ú×î´óÔÊÐí½ÓÊÕµÄ×Ö½ÚÊý(²»´óÓÚ2µÄ14´Î·½)
//4,ÐÞ¸ÄÁËEN_USART1_RXµÄÊ¹ÄÜ·½Ê½
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//¶¨Òå×î´ó½ÓÊÕ×Ö½ÚÊý 200
#define EN_USART1_RX 			1		//Ê¹ÄÜ£¨1£©/½ûÖ¹£¨0£©´®¿Ú1½ÓÊÕ
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.Ä©×Ö½ÚÎª»»ÐÐ·û 
extern u16 USART_RX_STA;         		//½ÓÊÕ×´Ì¬±ê¼Ç	
//Èç¹ûÏë´®¿ÚÖÐ¶Ï½ÓÊÕ£¬Çë²»Òª×¢ÊÍÒÔÏÂºê¶¨Òå
void uart_init(u32 bound);

//USART6 ADDD
#define USART6_REC_LEN  			200  	//¶¨Òå×î´ó½ÓÊÕ×Ö½ÚÊý 200
#define EN_USART6_RX 			1		//Ê¹ÄÜ£¨1£©/½ûÖ¹£¨0£©´®¿Ú6½ÓÊÕ
extern u8  USART6_RX_BUF[USART6_REC_LEN]; //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.Ä©×Ö½ÚÎª»»ÐÐ·û 
extern u16 USART6_RX_STA;         		//½ÓÊÕ×´Ì¬±ê¼
void uart6_init(u32 bound);

#endif


