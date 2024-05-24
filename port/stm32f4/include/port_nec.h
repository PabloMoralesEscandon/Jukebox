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

/// @brief Defines a NEC receiver hardware
typedef struct{
    GPIO_TypeDef *p_port;   /*!< Pointer to the GPIO struct to which the receiver is connected */
    uint8_t pin;            /*!< Pin to which the NEC is connectedted */
    uint32_t buffer;        /*!< Buffer for the decoded message */
    uint8_t idx;            /*!< Buffer current index */
    bool timeout;           /*!< FLag to indicate if there has been a timeout */
    bool event;             /*!< Flag to indicate if there has been an event*/
    bool decode;            /*< Flag to indicate the receiver is decoding */ 
} port_NEC_hw_t;         


/* Global variables */

/// @brief Array of elements with the hw info of the NECs
extern port_NEC_hw_t NECs_arr[];

/* Function prototypes and explanation -------------------------------------------------*/

/// @brief Iniatilizes NEC receiver
/// @param NEC_id 
void port_NEC_init(uint32_t NEC_id);

/// @brief Sets NEC timer duration to check for 1 or 0
/// @param NEC_id NEC receiver id
/// @param duration_ms The timer duration in ms
void port_NEC_set_timer_duration(uint32_t NEC_id, uint32_t duration_ms);

/// @brief Descodes wether symbol was 1 or 0
/// @param NEC_id NEC receiver id
void port_NEC_decode(uint32_t NEC_id);

/// @brief Get if an event took place
/// @param NEC_id NEC receiver id
/// @return value of event flag
bool port_NEC_event(uint32_t NEC_id);

/// @brief Get if the receiver is decoding
/// @param NEC_id NEC receiver id
/// @return value of the decoding flag
bool port_NEC_decoding(uint32_t NEC_id);

/// @brief Get fully decoded message
/// @param NEC_id NEC receiver id
/// @return fully decoded 32 bit message
uint32_t port_NEC_get_message(uint32_t NEC_id);

/// @brief Sets the event flag
/// @param NEC_id NEC receiver id
/// @param value 0 or 1
void port_NEC_set_event(uint32_t NEC_id, bool value);

/// @brief Sets the decode flag
/// @param NEC_id NEC receiver id
/// @param value 0 or 1
void port_NEC_set_decode(uint32_t NEC_id, bool value);

#endif