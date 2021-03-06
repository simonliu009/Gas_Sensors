#include "main.h"

//     硬件接线 
//     Pin1-TXD  -> PC11
//     Pin2-RXD  -> PC10
//     Pin3-VCC  -> VDD
//     Pin4-GND  -> GND

uint8_t USART4_RX_STA;
uint8_t USART4_RX_BUF[64];  //接收数据缓冲,最大64个字节

unsigned char T6703_ReadGas_Cmd[]={0x15,0x04,0x13,0x8b,0x00,0x01,0x46,0x70};

/**
  * @brief  USART4初始化
**/
void USART4_CO2_T6703_Init(void)
{
		//GPIO端口设置
		GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART4,ENABLE);   //使能USART4时钟

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);     //使能GPIOC时钟

    //USART4_TX   GPIOC.10
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PC.10
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC.10

		//USART4_RX     GPIOC.11
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PC.11
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//浮空输入
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC.11
	
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_0);  //复用串口功能0
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_0);
		
		//USART初始化设置

    USART_InitStructure.USART_BaudRate = 19200; //串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一位停止位
    USART_InitStructure.USART_Parity = USART_Parity_Even;//偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
		USART_Init(USART4, &USART_InitStructure); //初始化串口4
		
		//USART4 NVIC 设置
		NVIC_InitStructure.NVIC_IRQChannel = USART3_8_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority=3 ;//抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据指定参数初始化NVIC
                    
		USART_ITConfig(USART4, USART_IT_RXNE, ENABLE);//开启串口4接收中断
		USART_ITConfig(USART4, USART_IT_ORE, ENABLE);
		USART_Cmd(USART4, ENABLE);                    //使能串口4 

}

/**
  * @brief  USART4发送一个字符串
**/
void USART4_SendStr(USART_TypeDef* USARTx, uint8_t *Data,uint32_t len)
{
    uint32_t i;
		USART_ClearFlag(USART4,USART_FLAG_TC); 
    for(i=0; i<len; i++)
    {                                         
        USART_SendData(USARTx, *Data++);
        while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
    }
}

