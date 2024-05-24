/**
 * @file fsm_buzzer.h
 * @brief Header for fsm_buzzer.c file.
 * @author Pablo Morales
 * @author Noel Solís
 * @date 12-5-24
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
enum  	FSM_BUZZER {
  WAIT_START = 0,
  PLAY_NOTE,
  PAUSE_NOTE,
  WAIT_NOTE,
  WAIT_MELODY
};

enum  	USER_ACTIONS {
  STOP = 0,
  PLAY,
  PAUSE
};

/* Typedefs --------------------------------------------------------------------*/

/// @brief Structure that defines a USART FSM
typedef struct{
    fsm_t f;                    /*!< FSM for the buzzer */
    melody_t *p_melody;         /*!< Pointer to the current melody */
    uint32_t 	note_index;     /*!< Current Note Index */
    uint8_t 	buzzer_id;      /*!< Used buzzer ID */
    uint8_t 	user_action;    /*!< Current User Action */
    double player_speed;        /*!< Reproduction Speed */
    double player_volume;     /*!< Current volume */

} fsm_buzzer_t;

/* Function prototypes and explanation -------------------------------------------------*/

void    fsm_buzzer_set_melody (fsm_t *p_this, const melody_t *p_melody);

void 	fsm_buzzer_set_speed (fsm_t *p_this, double speed);

void fsm_buzzer_set_volume(fsm_t *p_this, double volume);

void 	fsm_buzzer_set_action (fsm_t *p_this, uint8_t action);

uint8_t fsm_buzzer_get_action (fsm_t *p_this);

fsm_t   *fsm_buzzer_new (uint32_t buzzer_id);

void 	fsm_buzzer_init (fsm_t *p_this, uint32_t buzzer_id);

bool 	fsm_buzzer_check_activity (fsm_t *p_this);

#endif /* FSM_BUZZER_H_ */