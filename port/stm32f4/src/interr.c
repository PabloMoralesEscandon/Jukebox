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
//#include "port_usart.h"
//#include "port_buzzer.h"

/**
 * @brief Interrupt service routine for the System tick timer (SysTick).
 * 
 * @note This ISR is called when the SysTick timer generates an interrupt.
 * The program flow jumps to this ISR and increments the tick counter by one millisecond.
 */
void SysTick_Handler(void)
{
  port_system_set_millis(port_system_get_millis() + 1);
}

void EXTI15_10_IRQHandler(void){
  /* ISR user button */
  if ( EXTI->PR & BIT_POS_TO_MASK(buttons_arr[BUTTON_0_ID].pin)){
    if (buttons_arr[0].p_port->IDR &  0x2000)
      buttons_arr[0].flag_pressed = true;
    else
      buttons_arr[0].flag_pressed = false;
  }
} 