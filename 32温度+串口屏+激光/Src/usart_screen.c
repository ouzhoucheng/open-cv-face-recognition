#include <stdio.h>
#include "usart_screen.h"
#include "usart.h"

char HMI_USART_SendByte(char byte)
{
    while((USART1->SR&0X40)==0);//ѭ������,ֱ���������
    USART1->DR = (uint8_t) byte;
	return byte;
}

void HMI_SendCmd(char *cmd) //STM32�봮����֮�䴮��ͨѶ��Э�� ָ��+0xff+0xff+0xff
{
    while (*cmd) {
        HMI_USART_SendByte(*cmd);
        cmd++;
    }
    HMI_USART_SendByte(0xff);
    HMI_USART_SendByte(0xff);
    HMI_USART_SendByte(0xff);
}


void HMI_SetWidgetVaule(char *name, char *vaule)//���ÿؼ���ֵ
{
    while (*name) {
        HMI_USART_SendByte(*name);
        name++;
    }
    HMI_USART_SendByte('=');
    HMI_USART_SendByte('\"');
    while (*vaule) {
        HMI_USART_SendByte(*vaule);
        vaule++;
    }
    HMI_USART_SendByte('\"');
    HMI_USART_SendByte(0xff);
    HMI_USART_SendByte(0xff);
    HMI_USART_SendByte(0xff);
}



