/**
 * @file port_motor.c
 * @brief Portable functions to interact with the Motor melody player FSM library.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */
/* Includes ------------------------------------------------------------------*/
/* Standard C libraries */

#include <math.h>

/* HW dependent libraries */

#include "port_motor.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */

#define ALT_FUNC2_TIM9 3

#define TIM_AS_PWM1_MASK 96

#define ARR_MAX 65535

/* Global variables */

port_motor_hw_t motors_arr[] = {
  [MOTOR_0_ID] = {.p_port = MOTOR_0_GPIO,
                   .pin = MOTOR_0_PIN,
                   .alt_func =  ALT_FUNC2_TIM9,
                   .rot_end = false
                  }
};

/* Private functions */

/// @brief  Enables TIMER 2 to count notes duartion
/// @param motor_id The unique identifier of the motor
// static void _timer_duration_setup(uint32_t motor_id)
// {
//   if (motor_id == MOTOR_0_ID)
//   {
//     // Enable the timer clock
//     RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
//     // Set clock source to internal
//     TIM2->CR1 &= ~TIM_CR1_CEN;
//     // Set counter to 0
//     TIM2->CNT = 0;
//     // Enable autoreload preload
//     TIM2->CR1 |= TIM_CR1_ARPE;
//     // Clear the update interrupt flag
//     TIM2->SR = ~TIM_SR_UIF;
//     // Enable update interrupt
//     TIM2->DIER |= TIM_DIER_UIE;
//     /* Configure interruptions */
//     NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0)); 
//     NVIC_EnableIRQ(TIM2_IRQn);                                                        
//   }
// }

/// @brief Enables TIMER 5 in PWM to activate motor
/// @param motor_id The unique identifier of the motor
static void _timer_pwm_setup(uint32_t motor_id){
  if (motor_id == MOTOR_0_ID)
  {
    // Enable the timer clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM9EN;
    // Set clock source to internal
    TIM9->CR1 &= ~TIM_CR1_CEN;
    // Enable autoreload preload
    TIM9->CR1 |= TIM_CR1_ARPE;
    // Set counter to 0
    TIM9->CNT = 0;
    // Set ARR and PSC to 0
    TIM9->ARR = 0;
    TIM9->PSC = 0;
    // TIM2->EGR = TIM_EGR_UG;
    // Disable output compare of channel 1
    TIM9->CCER &= ~TIM_CCER_CC1E;
    // Set mode to PWM1
    TIM9->CCMR1 |= TIM_AS_PWM1_MASK;
    // Enable preload
    TIM9->CCMR1 |= TIM_CCMR1_OC1PE;                                                      
  }   
}

/* Public functions -----------------------------------------------------------*/

void port_motor_init(uint32_t motor_id)
{
  port_motor_hw_t motor = motors_arr[motor_id];
  GPIO_TypeDef *p_port = motor.p_port;
  uint8_t pin = motor.pin;
  uint8_t alt_func = motor.alt_func;

  // Configure GPIO and alt function
  port_system_gpio_config(p_port, pin, GPIO_MODE_ALTERNATE, GPIO_PUPDR_NOPULL);
  port_system_gpio_config_alternate(p_port, pin, alt_func);

  // Call local functions
  // _timer_duration_setup(motor_id);
  _timer_pwm_setup(motor_id);
}

// void port_motor_set_note_duration(uint32_t motor_id, uint32_t duration_ms){
//   double sysclk_as_double = (double)SystemCoreClock;
//   double ms_as_double = (double)duration_ms;
//   double s_as_double = ms_as_double/1000;
//   double ARR = ARR_MAX;
//   double PSC;
//   // Calculate the PSC value for max ARR value
//   PSC = round((sysclk_as_double * s_as_double) / (ARR + 1)) - 1;

//   // Calculate the PSC value for max ARR value
//   ARR = round((sysclk_as_double * s_as_double) / (PSC + 1)) - 1;

//   // Check if ARR is greater than its maximum value
//   if(ARR > ARR_MAX){
//     PSC++;
//     // Calculate the PSC value for max ARR value
//     ARR = round((sysclk_as_double * s_as_double) / (PSC + 1)) - 1;
//   }

//   switch (motor_id)
//   {
//     case 0:
//       // Disable timer
//       TIM2->CR1 &= ~TIM_CR1_CEN;
//       // Reset counter
//       TIM2->CNT = 0;
//       // Load autoreload register
//       TIM2->ARR = (uint32_t)round(ARR);
//       // Load prescaler register
//       TIM2->PSC = (uint32_t)round(PSC);
//       // Values are loaded into active registers
//       TIM2->EGR = TIM_EGR_UG;
//       //Se note end flag to false
//       motors_arr[motor_id].rot_end = false;
//       // Enable timer
//       TIM2->CR1 |= TIM_CR1_CEN;
//       break;
    
//     default:
//       break;
//   }
// }

bool port_motor_get_note_timeout(uint32_t motor_id){
  return motors_arr[motor_id].rot_end;
}

void port_motor_set_frequency(uint32_t motor_id, double frequency_hz){
  // Check if frequency is 0
  if(frequency_hz == 0){
     // Activate PWM mode
    port_motor_stop(motor_id);
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

  switch (motor_id)
  {
    case 0:
      // Disable timer
      TIM9->CR1 &= ~TIM_CR1_CEN;
      // Reset counter
      TIM9->CNT = 0;
      // Load autoreload register
      TIM9->ARR = (uint32_t)round(ARR);
      // Load prescaler register
      TIM9->PSC = (uint32_t)round(PSC);
      // Set PWM width
      double ccr1 = ARR * 0.5;
      ccr1 = round(ccr1);
      TIM9->CCR1 = (MOTOR_PWM_DC * (ARR + 1));
      // Values are loaded into active registers
      TIM9->EGR = TIM_EGR_UG;
      // Enable output compare
      TIM9->CCER |= TIM_CCER_CC1E;
      // Enable timer
      TIM9->CR1 |= TIM_CR1_CEN;

      break;
    
    default:
      break;
  }
}

void port_motor_stop(uint32_t motor_id){
  
  switch (motor_id)
  {
    case 0:
      // Disable timer
      TIM9->CR1 &= ~TIM_CR1_CEN;
      // TIM2->CR1 &= ~TIM_CR1_CEN;

      break;
    
    default:
      break;
  }
  
}