#include "EIE3810_CLOCK.h"
#include "EIE3810_USART.h"

int main(void)
{
	EIE3810_clock_tree_init();
	EIE3810_USART2_init(36, 9600);
	EIE3810_USART1_init(72, 9600);
	USART_print(1,"1234567890");
	USART_print(2,"115010184");
}