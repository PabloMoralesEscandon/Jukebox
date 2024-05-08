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

/* Global variables */

port_buzzer_hw_t buzzers_arr[] = {
  [BUZZER_0_ID] = {.p_port = BUZZER_0_GPIO,
                  .pin = BUZZER_0_PIN,
                  .alt_func =  ALT_FUNC2_TIM3,
                  .note_end = false
                  }
};

/* Private functions */

static void _timer_duration_setup(uint32_t buzzer_id)
{
  if (buzzer_id == BUZZER_0_ID)
  {
    // Enable the timer clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    // Set clock source to internal
    TIM2->CR1 = 0x0000;
    // Set counter to 0
    TIM2->CNT = 0;
    // Enable autoreload preload
    TIM2->CR1 |= 0x0080;
    // Clear the update interrupt flag
    TIM2->SR = ~TIM_SR_UIF;
    // Enable update interrupt
    TIM2->DIER |= TIM_DIER_UIE;
    /* Configure interruptions */
    NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0)); 
    NVIC_EnableIRQ(TIM2_IRQn);
    // Start counting
    TIM2->CR1 |= TIM_CR1_CEN;                                                        
  }
}

static void _timer_pwm_setup(uint32_t buzzer_id){
  if (buzzer_id == BUZZER_0_ID)
  {
    // Enable the timer clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    // Set clock source to internal
    TIM3->CR1 = 0x0000;
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
    // Enable autoreload preload
    TIM3->CR1 |= 0x0080;
    // Clear the update interrupt flag
    TIM3->SR = ~TIM_SR_UIF;
    // Enable update interrupt
    TIM3->DIER |= TIM_DIER_UIE;
    /* Configure interruptions */
    NVIC_SetPriority(TIM3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0)); 
    NVIC_EnableIRQ(TIM3_IRQn);
    // Start counting
    TIM3->CR1 |= TIM_CR1_CEN;                                                        
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
  double sysclk_as_double = (double)(SystemCoreClock/1000);
  double ms_as_double = (double)duration_ms;
  double ARR = 65535;
  uint16_t rounded_ARR;
  double PSC;
  uint16_t rounded_PSC;
  // Calculate the PSC value for max ARR value
  PSC = ((sysclk_as_double * ms_as_double) / (ARR + 1)) - 1;
  // Round PSC value
  PSC = round(PSC);
  // Calculate the PSC value for max ARR value
  ARR = ((sysclk_as_double * ms_as_double) / (PSC + 1)) - 1;
  // Round ARR value
  ARR = round(ARR);

  // Check if ARR is greater than its maximum value
  if(ARR > 65535){
    PSC++;
    // Calculate the PSC value for max ARR value
    ARR = ((sysclk_as_double * ms_as_double) / (PSC + 1)) - 1;
    // Round ARR value
    ARR = round(ARR);
  }

  switch (buzzer_id)
  {
    case 0:
      // Load autoreload register
      TIM2->ARR = (uint16_t)ARR;
      // Load prescaler register
      TIM2->PSC = (uint16_t)PSC;
      // Values are loaded into active registers
      TIM2->EGR = TIM_EGR_UG;
      break;
    
    default:
      break;
  }
  buzzers_arr[buzzer_id].note_end = false;
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
  
  double sysclk_as_double = (double)(SystemCoreClock/1000);
  double pwm_period = 1 / frequency_hz;
  double ARR = 65535;
  uint16_t rounded_ARR;
  double PSC;
  uint16_t rounded_PSC;
  double pwm_dc;
  // Calculate the PSC value for max ARR value
  PSC = ((sysclk_as_double * pwm_period) / (ARR + 1)) - 1;
  // Round PSC value
  PSC = round(PSC);
  // Calculate the PSC value for max ARR value
  ARR = ((sysclk_as_double * pwm_period) / (PSC + 1)) - 1;
  // Round ARR value
  ARR = round(ARR);

  // Check if ARR is greater than its maximum value
  if(ARR > 65535){
    PSC++;
    // Calculate the PSC value for max ARR value
    ARR = ((sysclk_as_double * pwm_period) / (PSC + 1)) - 1;
    // Round ARR value
    ARR = round(ARR);
  }

  switch (buzzer_id)
  {
    case 0:
      // Load autoreload register
      TIM3->ARR = (uint16_t)ARR;
      // Load prescaler register
      TIM3->PSC = (uint16_t)PSC;
      // Values are loaded into active registers
      TIM3->EGR = TIM_EGR_UG;
      // Set PWM width
      pwm_dc = BUZZER_PWM_DC / 100;
      TIM3->CCR1 = (uint16_t)round(ARR * pwm_dc);
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

      break;
    
    default:
      break;
  }
  
}