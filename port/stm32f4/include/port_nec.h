/**
 * @file port_NEC.h
 * @brief Header for port_NEC.c file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */
#ifndef PORT_NEC_H_
#define PORT_NEC_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */

#include <stdint.h>
#include <stdbool.h>

/* HW dependent includes */

#include "port_system.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */

#define NEC_0_ID 0
#define NEC_0_GPIO GPIOA
#define NEC_0_PIN 10

/* Typedefs --------------------------------------------------------------------*/

typedef struct{
    GPIO_TypeDef *p_port;   /*!< Pointer to the GPIO struct to which the button is conNECted */
    uint8_t pin;            /*!< Pin to which the NEC is conNECted */
    uint32_t buffer;
    uint8_t idx;
    bool timeout;
    bool event;
    bool decode;          /*< Flag to indicate a command has been received >*/ 
} port_NEC_hw_t;         


/* Global variables */

/// @brief Array of elements with the hw info of the NECs
extern port_NEC_hw_t NECs_arr[];

/* Function prototypes and explanation -------------------------------------------------*/

void port_NEC_init(uint32_t NEC_id);

void port_NEC_set_timer_duration(uint32_t NEC_id, uint32_t duration_ms);

void port_NEC_decode(uint32_t NEC_id);

bool port_NEC_event(uint32_t NEC_id);

bool port_NEC_decoding(uint32_t NEC_id);

uint32_t port_NEC_get_message(uint32_t NEC_id);

void port_NEC_set_event(uint32_t NEC_id, bool value);

void port_NEC_set_decode(uint32_t NEC_id, bool value);

#endif