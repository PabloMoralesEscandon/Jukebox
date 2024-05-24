/**
 * @file fsm_nec.h
 * @brief Header for fsm_NEC.c file.
 * @author Pablo Morales
 * @author Noel Solis
 * @date 5-2-2024
 */

#ifndef FSM_NEC_H_
#define FSM_NEC_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include "fsm.h"

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
/// @brief Enumerates the NEC FSM states
enum FSM_NEC {
    NEC_WAIT = 0,    /*!< Initial state. NEC is released and can switch to pressed */
    NEC_START,    /*!< NEC is pressed and the anti-debounce is active */
    NEC_HOLD,         /*!< NEC is being pressed and can switch to released */
    NEC_DECODE    /*!< NEC is released and the anti-debounce is active */
};

/* Typedefs --------------------------------------------------------------------*/
/// @brief Structure that defines a NEC FSM
typedef struct{
    fsm_t f;             /*!< FSM for the NEC */
    uint32_t NEC_id;     /*!< NEC identifier */
    bool decoded;
    uint32_t message;
} fsm_NEC_t;

/* Function prototypes and explanation -------------------------------------------------*/

/// @brief Creates new NEC FSM
/// @param debounce_time  Debounce time in ms
/// @param NEC_id NEC identifier
/// @return pointer to new FSM NEC
fsm_t *fsm_NEC_new(uint32_t NEC_id);

/// @brief Initializes FSM NEC
/// @param p_this pointer to an fsm_t struct that contains an fsm_NEC_t
/// @param debounce_time Debounce time in ms
/// @param NEC_id NEC identifier 
void fsm_NEC_init(fsm_t *p_this, uint32_t NEC_id);

/// @brief Checks if the NEC FSM is active or not
/// @param p_this pointer to an fsm_t struct that contains an fsm_NEC_t
/// @return true if active, false if not
bool fsm_NEC_check_activity(fsm_t *p_this);

/// @brief Get the NEC decoded message
/// @param p_this Pointer to an fsm_t struct that contains an fsm_NEC_t
/// @return The decoded message
uint32_t fsm_NEC_get_message(fsm_t *p_this);




#endif /*FSM_NEC_H_*/
