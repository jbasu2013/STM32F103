#include "stm32f103xb.h"

int main(){
	SystemCoreClockUpdate();

    const char msg[] = "Hello World!\n";     
    int length = sizeof(msg)/sizeof(msg[0]);

    // Enable clock to PortA (TX and RX pins of USART1)
    RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN;
    // Enable clock to USART1
    RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;

    // Set pin PA9 (TX1) to AF Push-pull (see Reference manual page 166)
    // CNF1 CNF0 Mode1 Mode0  = 1011 = 0xB at pin9
    // Read -> Clear CNF and MODE for Pin9 -> Set 0xB -> Write
    GPIOA -> CRH = ((GPIOA -> CRH & ~(GPIO_CRH_CNF9_Msk | GPIO_CRH_MODE9_Msk)) | (0xB << GPIO_CRH_MODE9_Pos));
    
    // Initialise USART1 for transmission
    // Enable USART1
    USART1 -> CR1 |= USART_CR1_UE; 
    // Set M to 0 - 1 start 8 data n stop bits
    USART1 -> CR1 & ~USART_CR1_M;
    // Set Stop to 00 - 1 stop bit
    USART1 -> CR2 & ~USART_CR2_STOP;
    // Set Baud rate to 9600 - 72e6 / (16*468.75) -> Mantissa 468, Fraction 0.75 -> 0xC
    USART1 -> BRR = ((468 << USART_BRR_DIV_Mantissa_Pos) | 0xC);
    // Set TE to 1
    USART1 -> CR1 |= USART_CR1_TE;

    // Infinite loop
	while(1){
        // loop over message
        for(int i = 0; i < length; i++){
            // wait until finished
            while((USART1 -> SR & USART_SR_TC_Msk) == 0) {}
            // send character
            USART1 -> DR = msg[i];
        }
	}
	return 0;
}
