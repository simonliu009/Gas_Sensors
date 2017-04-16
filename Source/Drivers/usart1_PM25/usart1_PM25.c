#include "main.h"

//     硬件接线 
//     Pin1-VCC  -> 5V
//     Pin2-GND  -> GND
//     Pin4-RXD  -> PA9
//     Pin5-TXD  -> PA10

uint8_t USART_RX_STA;
uint8_t USART_RX_BUF[64];  //接收数据缓冲,最大64个字节

/**
  * @brief  USART1初始化
**/
void USART1_PM25_Init(void)
{
		//GPIO端口设置
		GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);   //使能USART1时钟

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);     //使能GPIOA时钟

    //USART1_TX   GPIOA.9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9

		//USART1_RX     GPIOA.10
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA.10
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//浮空输入
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10 
	
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);  //复用串口功能1
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);
		
		//USART初始化设置

    USART_InitStructure.USART_BaudRate = 9600; //串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一位停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
		USART_Init(USART1, &USART_InitStructure); //初始化串口1
		
		//USART1 NVIC 设置
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority=1 ;//抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据指定参数初始化NVIC
                    
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口1接收中断
		USART_ITConfig(USART1, USART_IT_ORE, ENABLE);
		USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

