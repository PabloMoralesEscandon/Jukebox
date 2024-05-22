/**
 * @file port_motor.h
 * @brief Header for port_motor.c file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */
#ifndef PORT_MOTOR_H_
#define PORT_MOTOR_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */

#include <stdint.h>
#include <stdbool.h>

/* HW dependent includes */

#include "port_system.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */

#define MOTOR_0_ID 0
#define MOTOR_0_GPIO GPIOA
#define MOTOR_0_PIN 2
#define MOTOR_PWM_DC 0.5

/* Typedefs --------------------------------------------------------------------*/

typedef struct{
    GPIO_TypeDef *p_port;   /*!< Pointer to the GPIO struct to which the button is connected */
    uint8_t pin;            /*!< Pin to which the motor is connected */
    uint8_t alt_func;       /*!< Alternate function for PMW */
    bool rot_end;          /*< Falg to indicate the rotation has finished >*/ 
} port_motor_hw_t;         


/* Global variables */

/// @brief Array of elements with the hw info of the motors
extern port_motor_hw_t motors_arr[];

/* Function prototypes and explanation -------------------------------------------------*/

/// @brief Sets up a motor for use
/// @param motor_id The unique identifier of the motor
void port_motor_init(uint32_t motor_id);

/// @brief Stops the PMW to stop the motor
/// @param motor_id The unique identifier of the motor
void port_motor_stop(uint32_t motor_id); 	

/// @brief Checks the note has ended flag
/// @param motor_id The unique identifier of the motor
/// @return True if note has ended, false if not
bool port_motor_get_timeout(uint32_t motor_id);

/// @brief Sets the timer that controls the note duration to the desired one
/// @param motor_id  The unique identifier of the motor
/// @param duration_ms Desired duration
void port_motor_set_duration(uint32_t motor_id, uint32_t duration_ms);

/// @brief Set PMW period to match desired frequency
/// @param motor_id The unique identifier of the motor
/// @param frequency_hz The desired frequency
void port_motor_set_frequency(uint32_t motor_id, double frequency_hz);

#endif