/**
 * @file fsm_buzzer.h
 * @brief Header for fsm_buzzer.c file.
 * @author Pablo Morales
 * @author Noel Solís
 * @date 2-5-24
 */

#ifndef FSM_BUZZER_H_
#define FSM_BUZZER_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include <fsm.h>
#include "melodies.h"
/* HW dependent includes */


/* Defines and enums ----------------------------------------------------------*/
/* Enums */

/// @brief States of the buzzer's FSM
enum  	FSM_BUZZER {
  WAIT_START = 0,
  PLAY_NOTE,
  PAUSE_NOTE,
  WAIT_NOTE,
  WAIT_MELODY
};

/// @brief Possible user actions
enum  	USER_ACTIONS {
  STOP = 0,
  PLAY,
  PAUSE
};

/* Typedefs --------------------------------------------------------------------*/

/// @brief Structure that defines a BUZZER FSM
typedef struct{
    fsm_t f;                    /*!< FSM for the buzzer */
    melody_t *p_melody;         /*!< Pointer to the current melody */
    uint32_t 	note_index;     /*!< Current Note Index */
    uint8_t 	buzzer_id;      /*!< Used buzzer ID */
    uint8_t 	user_action;    /*!< Current User Action */
    double player_speed;        /*!< Reproduction Speed */

} fsm_buzzer_t;

/* Function prototypes and explanation -------------------------------------------------*/

/// @brief Set the melody to play. 
/// @param p_this Pointer to an fsm_t struct than contains an fsm_buzzer_t struct 
/// @param p_melody Pointer to the melody to play 
void    fsm_buzzer_set_melody (fsm_t *p_this, const melody_t *p_melody);

/// @brief Set the speed of the player. 
/// @param p_this Pointer to an fsm_t struct than contains an fsm_buzzer_t struct 
/// @param speed Speed of the player 
void 	fsm_buzzer_set_speed (fsm_t *p_this, double speed);

/// @brief Set the action to perform on the player. 
/// @param p_this Pointer to an fsm_t struct than contains an fsm_buzzer_t struct 
/// @param action Action to perform on the player 
void 	fsm_buzzer_set_action (fsm_t *p_this, uint8_t action);

/// @brief Get the action of the user. 
/// @param p_this Pointer to an fsm_t struct than contains an fsm_buzzer_t struct 
/// @return Action to perform on the player 
uint8_t fsm_buzzer_get_action (fsm_t *p_this);

/// @brief Creates a new buzzer finite state machine. 
/// @param buzzer_id Unique buzzer identifier number
/// @return A pointer to the new buzzer finite state machine 
fsm_t   *fsm_buzzer_new (uint32_t buzzer_id);

/// @brief Initialize a buzzer finite state machine. 
/// @param p_this Pointer to an fsm_t struct than contains an fsm_buzzer_t struct 
/// @param buzzer_id Unique buzzer identifier number 
void 	fsm_buzzer_init (fsm_t *p_this, uint32_t buzzer_id);

/// @brief Check if the buzzer finite state machine is playing a melody. 
/// @param p_this 
/// @return True if the player is playing or paused. False if the player is stopped. 
bool 	fsm_buzzer_check_activity (fsm_t *p_this);

#endif /* FSM_BUZZER_H_ */