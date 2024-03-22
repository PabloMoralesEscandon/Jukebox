/**
 * @file fsm_button.h
 * @brief Header for fsm_button.c file.
 * @author Pablo Morales
 * @author Noel Solis
 * @date 5-2-2024
 */

#ifndef FSM_BUTTON_H_
#define FSM_BUTTON_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include "fsm.h"

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
/// @brief Enumerates the Button FSM states
enum FSM_BUTTON {
    BUTTON_RELEASED = 0,    /*!< Initial state. Button is released and can switch to pressed */
    BUTTON_PRESSED_WAIT,    /*!< Button is pressed and the anti-debounce is active */
    BUTTON_PRESSED,         /*!< Button is being pressed and can switch to released */
    BUTTON_RELEASED_WAIT    /*!< Button is released and the anti-debounce is active */
};

/* Typedefs --------------------------------------------------------------------*/
/// @brief Structure that defines a Button FSM
typedef struct{
    fsm_t f;                /*!< FSM for the button */
    uint32_t debounce_time; /*!< Debounce time in ms */
    uint32_t next_timeout;  /*!< Time until next timeout in ms */
    uint32_t tick_pressed;  /*!< Tick the button is pressed */
    uint32_t duration;      /*!< Time the button is pressed */
    uint32_t button_id;     /*!< Button identifier */
} fsm_button_t;

/* Function prototypes and explanation -------------------------------------------------*/

/// @brief Creates new button FSM
/// @param debounce_time  Debounce time in ms
/// @param button_id Button identifier
/// @return pointer to new FSM button
fsm_t *fsm_button_new(uint32_t debounce_time, uint32_t button_id);

/// @brief Initializes FSM button
/// @param p_this pointer to an fsm_t struct that contains an fsm_button_t
/// @param debounce_time Debounce time in ms
/// @param button_id Button identifier 
void fsm_button_init(fsm_t *p_this, uint32_t debounce_time, uint32_t button_id);

/// @brief Get duration of last button press
/// @param p_this pointer to an fsm_t struct that contains an fsm_button_t
/// @return duration of the last button press
uint32_t fsm_button_get_duration(fsm_t *p_this);

/// @brief Reset the duration of the last button press
/// @param p_this pointer to an fsm_t struct that contains an fsm_button_t
void fsm_button_reset_duration(fsm_t *p_this);

/// @brief Checks if the button FSM is active or not
/// @param p_this pointer to an fsm_t struct that contains an fsm_button_t
/// @return true if active, false if not
bool fsm_button_check_activity(fsm_t *p_this);

#endif /*FSM_BUTTON_H_*/
