/**
 ******************************************************************************
 * @file           : interr.c
 * @brief          : Interrupt Service Routines (ISR) for STM32F4xx microcontrollers.
 * 
 * @author Pablo Morales
 * @author Noel Solis
 * @date 12-2-2024
 ******************************************************************************
 */


// Include headers of different port elements:
#include "port_system.h"
#include "port_button.h"
#include "port_usart.h"
//#include "port_buzzer.h"

/**
 * @brief Interrupt service routine for the System tick timer (SysTick).
 * 
 * @note This ISR is called when the SysTick timer generates an interrupt.
 * The program flow jumps to this ISR and increments the tick counter by one millisecond.
 */
void SysTick_Handler(void){
  port_system_set_millis(port_system_get_millis() + 1);
}

/// @brief Handles Px10 to Px15 interrupts
/// @param void
void EXTI15_10_IRQHandler(void){
  /* ISR user button */
  if ( EXTI->PR & BIT_POS_TO_MASK(buttons_arr[BUTTON_0_ID].pin)){
    buttons_arr[BUTTON_0_ID].flag_pressed = !port_system_gpio_read(buttons_arr[BUTTON_0_ID].p_port, buttons_arr[BUTTON_0_ID].pin);
    EXTI->PR |= BIT_POS_TO_MASK(buttons_arr[BUTTON_0_ID].pin);
  }
}

/// @brief Handles UART3 interrupts
/// @param  void
void USART3_IRQHandler(void){
  USART_TypeDef *p_usart = usart_arr[USART_0_ID].p_usart;
  if((p_usart -> SR && USART_SR_RXNE) && (p_usart -> CR1 && USART_CR1_RXNEIE)){
    port_usart_store_data(USART_0_ID);
  }
  if((p_usart -> SR && USART_SR_TXE) && (p_usart -> CR1 && USART_CR1_TXEIE)){
    port_usart_write_data(USART_0_ID);
  }
}