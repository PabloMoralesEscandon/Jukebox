/**
 * @file port_receiver.h
 * @brief Header for port_receiver.c file.
 * @author Pablo Morales
 * @author Noel Solis
 * @date 12-2-2024
 */

#ifndef PORT_receiver_H_
#define PORT_receiver_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* HW dependent includes */
#include "port_system.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
/// @brief receiver 0 identifier
#define receiver_0_ID 0

/// @brief receiver 0 GPIO port
#define receiver_0_GPIO GPIOC

/// @brief receiver 0 pin
#define receiver_0_PIN 14

/// @brief receiver 0 debounce time in ms
#define receiver_0_DEBOUNCE_TIME_MS 110

/* Typedefs --------------------------------------------------------------------*/
/// @brief Structure that defines the HW of a receiver
typedef struct{
    GPIO_TypeDef *p_port;   /*!< Pointer to the GPIO struct to which the receiver is connected */
    uint8_t pin;            /*!< Pin to which the receiver is connected */
    bool flag_pressed;      /*!< Flag to indicate wether the receiver is pressed or not */
} port_receiver_hw_t;

/* Global variables */

/// @brief Array of elements that represent the hw charasteristics of the receivers
extern port_receiver_hw_t receivers_arr[];

/* Function prototypes and explanation -------------------------------------------------*/

/// @brief Initialiazes
/// @param receiver_id 
void port_receiver_init(uint32_t receiver_id);

/// @brief Get system tick in ms
/// @return Current system tick
uint32_t port_receiver_get_tick();

/// @brief Get status of the receiver
/// @param receiver_id id of the target
/// @return true if pressed, false if not
bool port_receiver_is_pressed(uint32_t receiver_id);

#endif
