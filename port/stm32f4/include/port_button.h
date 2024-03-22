/**
 * @file port_button.h
 * @brief Header for port_button.c file.
 * @author Pablo Morales
 * @author Noel Solis
 * @date 12-2-2024
 */

#ifndef PORT_BUTTON_H_
#define PORT_BUTTON_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* HW dependent includes */
#include "port_system.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
/// @brief Button 0 identifier
#define BUTTON_0_ID 0

/// @brief Button 0 GPIO port
#define BUTTON_0_GPIO GPIOC

/// @brief Button 0 pin
#define BUTTON_0_PIN 13

/// @brief Button 0 debounce time in ms
#define BUTTON_0_DEBOUNCE_TIME_MS 110

/* Typedefs --------------------------------------------------------------------*/
/// @brief Structure that defines the HW of a button
typedef struct{
    GPIO_TypeDef *p_port;   /*!< Pointer tohe GPIO struct to which the button is connected */
    uint8_t pin;            /*!< Pin to which the button is connected */
    bool flag_pressed;      /*!< Flag to indicate wether the button is pressed or not */
} port_button_hw_t;

/* Global variables */

/// @brief Array of elements that represent the hw charasteristics of the buttons
extern port_button_hw_t buttons_arr[];

/* Function prototypes and explanation -------------------------------------------------*/

/// @brief Initialiazes
/// @param button_id 
void port_button_init(uint32_t button_id);

/// @brief Get system tick in ms
/// @return Current system tick
uint32_t port_button_get_tick();

/// @brief Get status of the button
/// @param button_id id of the target
/// @return true if pressed, false if not
bool port_button_is_pressed(uint32_t button_id);

#endif
