/**
 * @file port_buzzer.c
 * @brief Portable functions to interact with the Buzzer melody player FSM library.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */
/* Includes ------------------------------------------------------------------*/
/* Standard C libraries */

#include <math.h>

/* HW dependent libraries */

#include "port_buzzer.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */

#define ALT_FUNC2_TIM3 2

#define TIM_AS_PWM1_MASK 96

#define ARR_MAX 65535

/* Global variables */

port_buzzer_hw_t buzzers_arr[] = {
  [BUZZER_0_ID] = {.p_port = BUZZER_0_GPIO,
                   .pin = BUZZER_0_PIN,
                   .alt_func =  ALT_FUNC2_TIM3,
                   .note_end = false
                  }
};

/* Private functions */

/// @brief  Enables TIMER 2 to count notes duartion
/// @param buzzer_id The unique identifier of the buzzer
static void _timer_duration_setup(uint32_t buzzer_id)
{
  if (buzzer_id == BUZZER_0_ID)
  {
    // Enable the timer clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    // Set clock source to internal
    TIM2->CR1 &= ~TIM_CR1_CEN;
    // Set counter to 0
    TIM2->CNT = 0;
    // Enable autoreload preload
    TIM2->CR1 |= TIM_CR1_ARPE;
    // Clear the update interrupt flag
    TIM2->SR = ~TIM_SR_UIF;
    // Enable update interrupt
    TIM2->DIER |= TIM_DIER_UIE;
    /* Configure interruptions */
    NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0)); 
    NVIC_EnableIRQ(TIM2_IRQn);                                                        
  }
}

/// @brief Enables TIMER 3 in PWM note to play frequencies trough buzzer
/// @param buzzer_id The unique identifier of the buzzer
static void _timer_pwm_setup(uint32_t buzzer_id){
  if (buzzer_id == BUZZER_0_ID)
  {
    // Enable the timer clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    // Set clock source to internal
    TIM3->CR1 &= ~TIM_CR1_CEN;
    // Enable autoreload preload
    TIM3->CR1 |= TIM_CR1_ARPE;
    // Set counter to 0
    TIM3->CNT = 0;
    // Set ARR and PSC to 0
    TIM3->ARR = 0;
    TIM3->PSC = 0;
    TIM2->EGR = TIM_EGR_UG;
    // Disable output compare of channel 1
    TIM3->CCER &= ~TIM_CCER_CC1E;
    // Set mode to PWM1
    TIM3->CCMR1 |= TIM_AS_PWM1_MASK;
    // Enable preload
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;                                                      
  }   
}

/* Public functions -----------------------------------------------------------*/

void port_buzzer_init(uint32_t buzzer_id)
{
  port_buzzer_hw_t buzzer = buzzers_arr[buzzer_id];
  GPIO_TypeDef *p_port = buzzer.p_port;
  uint8_t pin = buzzer.pin;
  uint8_t alt_func = buzzer.alt_func;

  // Configure GPIO and alt function
  port_system_gpio_config(p_port, pin, GPIO_MODE_ALTERNATE, GPIO_PUPDR_NOPULL);
  port_system_gpio_config_alternate(p_port, pin, alt_func);

  // Call local functions
  _timer_duration_setup(buzzer_id);
  _timer_pwm_setup(buzzer_id);
}

void port_buzzer_set_note_duration(uint32_t buzzer_id, uint32_t duration_ms){
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

  switch (buzzer_id)
  {
    case 0:
      // Disable timer
      TIM2->CR1 &= ~TIM_CR1_CEN;
      // Reset counter
      TIM2->CNT = 0;
      // Load autoreload register
      TIM2->ARR = (uint32_t)round(ARR);
      // Load prescaler register
      TIM2->PSC = (uint32_t)round(PSC);
      // Values are loaded into active registers
      TIM2->EGR = TIM_EGR_UG;
      //Se note end flag to false
      buzzers_arr[buzzer_id].note_end = false;
      // Enable timer
      TIM2->CR1 |= TIM_CR1_CEN;
      break;
    
    default:
      break;
  }
}

bool port_buzzer_get_note_timeout(uint32_t buzzer_id){
  return buzzers_arr[buzzer_id].note_end;
}

void port_buzzer_set_note_frequency(uint32_t buzzer_id, double frequency_hz){
  // Check if frequency is 0
  if(frequency_hz == 0){
     // Activate PWM mode
    port_buzzer_stop(buzzer_id);
    return;
  }

  double sysclk_as_double = (double)SystemCoreClock;
  double pwm_period = 1 / frequency_hz;
  double ARR = ARR_MAX;
  double PSC;
  // Calculate the PSC value for max ARR value
  PSC = round((sysclk_as_double * pwm_period) / (ARR + 1)) - 1;
  // Calculate the PSC value for max ARR value
  ARR = round((sysclk_as_double * pwm_period) / (PSC + 1)) - 1;

  // Check if ARR is greater than its maximum value
  if(ARR > ARR_MAX){
    PSC++;
    // Calculate the PSC value for max ARR value
    ARR = round((sysclk_as_double * pwm_period) / (PSC + 1)) - 1;
  }

  switch (buzzer_id)
  {
    case 0:
      // Disable timer
      TIM3->CR1 &= ~TIM_CR1_CEN;
      // Reset counter
      TIM3->CNT = 0;
      // Load autoreload register
      TIM3->ARR = (uint32_t)round(ARR);
      // Load prescaler register
      TIM3->PSC = (uint32_t)round(PSC);
      // Set PWM width
      double ccr1 = ARR * 0.5;
      ccr1 = round(ccr1);
      TIM3->CCR1 = (BUZZER_PWM_DC * (ARR + 1));
      // Values are loaded into active registers
      TIM3->EGR = TIM_EGR_UG;
      // Enable output compare
      TIM3->CCER |= TIM_CCER_CC1E;
      // Enable timer
      TIM3->CR1 |= TIM_CR1_CEN;

      break;
    
    default:
      break;
  }
}

void port_buzzer_stop(uint32_t buzzer_id){
  
  switch (buzzer_id)
  {
    case 0:
      // Disable timer
      TIM3->CR1 &= ~TIM_CR1_CEN;
      TIM2->CR1 &= ~TIM_CR1_CEN;

      break;
    
    default:
      break;
  }
  
}