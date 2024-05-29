#include<stm32f411xe.h>
#include <uart_rx.h>

#define SYS_FREQ         16000000
#define APB1_CLK		 SYS_FREQ
#define UART_BAURATE	 115200
#define UART2EN			(1U<<17)
#define GPIOAEN			(1U<<0)
#define CR1_TE			(1U<<3)
#define CR1_RE			(1U<<2)
#define CR1_EN			(1U<<13)
#define SR_TXE			(1U<<7)
#define SR_RXNE			(1U<<5)
void write_string(int wordlength,char* word);
static uint16_t compute_uart_bd(uint32_t PeriphClk,uint32_t BauRate);
static void uart_set_baurate(USART_TypeDef *USARTx,uint32_t PeriphClk, uint32_t BauRate);
char uart2_read(void);

void uart2_txrx_init(void)
{
	/***************************Configure uart gpio pin*****************************/
	/*Enable clock access to gpioa*/
	RCC->AHB1ENR|=GPIOAEN;
	/*Set PA2 as alternate function mode*/
	GPIOA->MODER|=(1U<<5);
	GPIOA->MODER&=~(1U<<4);
	/*Set PA2 as alternate function type UART_TX (AF07)*/
	/*PA2 configure AFRL2 with AF07*/
	GPIOA->AFR[0]|=(1U<<8);
	GPIOA->AFR[0]|=(1U<<9);
	GPIOA->AFR[0]|=(1U<<10);
	GPIOA->AFR[0]&=~(1U<<11);

	/*Set PA3 as alternate function */
	GPIOA->MODER|=(1U<<7);
	GPIOA->MODER&=~(1U<<6);
	/*Set PA3 as alternate function type UART_RX(AF07)*/
	GPIOA->AFR[0]|=(1U<<12);
	GPIOA->AFR[0]|=(1U<<13);
	GPIOA->AFR[0]|=(1U<<14);
	GPIOA->AFR[0]&=~(1U<<15);

	/***************************Configure uart module*******************************/
	/*Enable clock access to uart2*/
	RCC->APB1ENR|=UART2EN;
	/*Configure baudrate*/
	uart_set_baurate(USART2,APB1_CLK, UART_BAURATE);

	/*Configure transfer direction*/
	USART2->CR1|=CR1_TE;
	/*Configure receiver direction*/
	USART2->CR1|=CR1_RE;
	/*Enable uart module*/
	USART2->CR1|=CR1_EN;

}
char uart2_read(void)
{
	/*Make sure the receiver data register is not empty*/
	while(!(USART2->SR&SR_RXNE));
	return USART2->DR;
}

void uart2_write(int ch)
{
	/*Make sure the transmit data register is empty*/
	while(!(USART2->SR & SR_TXE));
	/*Write to transmit data register*/
	USART2->DR=(ch & 0xFF);
}

void write_string(int wordlength,char* word)
{
	for(int i=0;i<wordlength;i++)
	{
		uart2_write(word[i]);
	}

}

static void uart_set_baurate(USART_TypeDef *USARTx,uint32_t PeriphClk, uint32_t BauRate)
{
	USARTx->BRR=compute_uart_bd(PeriphClk,BauRate);

}
static uint16_t compute_uart_bd(uint32_t PeriphClk,uint32_t BauRate)
{
	return (PeriphClk+(BauRate/2U))/BauRate;
}
