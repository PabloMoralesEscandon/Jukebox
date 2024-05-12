/**
 * @file port_buzzer.h
 * @brief Header for port_buzzer.c file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */
#ifndef PORT_BUZZER_H_
#define PORT_BUZZER_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */

#include <stdint.h>
#include <stdbool.h>

/* HW dependent includes */

#include "port_system.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */

#define BUZZER_0_ID 0
#define BUZZER_0_GPIO GPIOA
#define BUZZER_0_PIN 6
#define BUZZER_PWM_DC 0.5

/* Typedefs --------------------------------------------------------------------*/

typedef struct{
    GPIO_TypeDef *p_port;   /*!< Pointer to the GPIO struct to which the button is connected */
    uint8_t pin;            /*!< Pin to which the buzzer is connected */
    uint8_t alt_func;       /*!< Alternate function for PMW */
    bool note_end;          /*< Falg to indicate the note has finished >*/ 
} port_buzzer_hw_t;         


/* Global variables */

/// @brief Array of elements with the hw info of the buzzers
extern port_buzzer_hw_t buzzers_arr[];

/* Function prototypes and explanation -------------------------------------------------*/

/// @brief Sets up a buzzer for use
/// @param buzzer_id The unique identifier of the buzzer
void port_buzzer_init(uint32_t buzzer_id);

/// @brief Stops the PMW to stop the buzzer
/// @param buzzer_id The unique identifier of the buzzer
void port_buzzer_stop(uint32_t buzzer_id); 	

/// @brief Checks the note has ended flag
/// @param buzzer_id The unique identifier of the buzzer
/// @return True if note has ended, false if not
bool port_buzzer_get_note_timeout(uint32_t buzzer_id);

/// @brief Sets the timer that controls the note duration to the desired one
/// @param buzzer_id  The unique identifier of the buzzer
/// @param duration_ms Desired duration
void port_buzzer_set_note_duration(uint32_t buzzer_id, uint32_t duration_ms);

/// @brief Set PMW period to match desired frequency
/// @param buzzer_id The unique identifier of the buzzer
/// @param frequency_hz The desired frequency
void port_buzzer_set_note_frequency(uint32_t buzzer_id, double frequency_hz);

#endif