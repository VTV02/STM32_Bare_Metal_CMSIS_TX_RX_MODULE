#include <stdio.h>
#include<stdint.h>
#include <uart_rx.h>
#include<stm32f411xe.h>
#define GPIOAEN			(1U<<0)

char key;
int main(void)
{
	RCC->AHB1ENR|=(1U<<0);
	GPIOA->MODER|=(1U<<10);
	GPIOA->MODER&=~(1U<<11);
	uart2_txrx_init();
	while(1)
	{
		key = uart2_read();
    /*receive '1', turn on LED PA5 */
		if(key=='1')
		{
			GPIOA->ODR|=(1U<<5);
		}
    /*receive '0', turn off LED PA5*/
		if(key=='0')
		{
			GPIOA->ODR&=~(1U<<5);
		}

	}
}

