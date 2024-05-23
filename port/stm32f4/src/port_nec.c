/**
 * @file port_NEC.c
 * @brief Portable functions to interact with the NEC melody player FSM library.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */
/* Includes ------------------------------------------------------------------*/
/* Standard C libraries */

#include <math.h>

/* HW dependent libraries */

#include "port_nec.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */

#define ARR_MAX 65535

/* Global variables */

port_NEC_hw_t NECs_arr[] = {
  [NEC_0_ID] = {
                    .p_port = NEC_0_GPIO,
                    .pin = NEC_0_PIN,
                    .idx =  0,
                    .event = false,
                    .decode = false
                }
};

/* Private functions */

/// @brief  Enables TIMER 2 to count notes duartion
/// @param NEC_id The unique identifier of the NEC
static void _timer_duration_setup(uint32_t NEC_id)
{
  if (NEC_id == NEC_0_ID)
  {
    // Enable the timer clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    // Set clock source to internal
    TIM4->CR1 &= ~TIM_CR1_CEN;
    // Set counter to 0
    TIM4->CNT = 0;
    // Enable autoreload preload
    TIM4->CR1 |= TIM_CR1_ARPE;
    // Clear the update interrupt flag
    TIM4->SR = ~TIM_SR_UIF;
    // Enable update interrupt
    TIM4->DIER |= TIM_DIER_UIE;
    /* Configure interruptions */
    NVIC_SetPriority(TIM4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0)); 
    NVIC_EnableIRQ(TIM4_IRQn);                                                        
  }
}

/* Public functions -----------------------------------------------------------*/

void port_NEC_init(uint32_t NEC_id)
{
  port_NEC_hw_t NEC = NECs_arr[NEC_id];
  GPIO_TypeDef *p_port = NEC.p_port;
  uint8_t pin = NEC.pin;

  // Configure GPIO and alt function
  port_system_gpio_config(p_port, pin, GPIO_MODE_IN, GPIO_PUPDR_PUP);
  port_system_gpio_config_exti(p_port, pin, 0x0B);
  port_system_gpio_exti_enable(pin, 0x01, 0x00);

  // Call local functions
  _timer_duration_setup(NEC_id);
}

void port_NEC_set_timer_duration(uint32_t NEC_id, uint32_t duration_ms){
  double sysclk_as_double = (double)SystemCoreClock;
  double ms_as_double = (double)duration_ms;
  double s_as_double = ms_as_double/1000;
  double ARR = ARR_MAX;
  double PSC;
  // Calculate the PSC value for max ARR value
  PSC = round((sysclk_as_double * s_as_double) / (ARR + 1)) - 1;

  // Calculate the PSC value for max ARR value
  ARR = round((sysclk_as_double * s_as_double) / (PSC + 1)) - 1;

  // Check if ARR is greater than its maximum value
  if(ARR > ARR_MAX){
    PSC++;
    // Calculate the PSC value for max ARR value
    ARR = round((sysclk_as_double * s_as_double) / (PSC + 1)) - 1;
  }

  switch (NEC_id)
  {
    case 0:
      // Disable timer
      TIM4->CR1 &= ~TIM_CR1_CEN;
      // Reset counter
      TIM4->CNT = 0;
      // Load autoreload register
      TIM4->ARR = (uint32_t)round(ARR);
      // Load prescaler register
      TIM4->PSC = (uint32_t)round(PSC);
      // Values are loaded into active registers
      TIM4->EGR = TIM_EGR_UG;
      // Enable timer
      TIM4->CR1 |= TIM_CR1_CEN;
      break;
    
    default:
      break;
  }
}

void port_NEC_decode(uint32_t NEC_id){
  if(NECs_arr[NEC_id].timeout){
    //1
    NECs_arr[NEC_id].buffer |= (1u << NECs_arr[NEC_id].idx);
  } else{
    //0
    NECs_arr[NEC_id].buffer &= ~(1u << NECs_arr[NEC_id].idx);
  }
  NECs_arr[NEC_id].timeout = false;
  NECs_arr[NEC_id].event = false;
  port_NEC_set_timer_duration(NEC_id, 1.2);
}

bool port_NEC_event(uint32_t NEC_id){
    return NECs_arr[NEC_id].event;
}

bool port_NEC_decoding(uint32_t NEC_id){
    return NECs_arr[NEC_id].decode;
}

uint32_t port_NEC_get_message(uint32_t NEC_id){
    return NECs_arr[NEC_id].buffer;
}

void port_NEC_set_event(uint32_t NEC_id, bool value){
    NECs_arr[NEC_id].event = value;
}

void port_NEC_set_decode(uint32_t NEC_id, bool value){
    NECs_arr[NEC_id].decode = value;
}