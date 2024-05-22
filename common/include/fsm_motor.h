/**
 * @file fsm_motor.h
 * @brief Header for fsm_motor.c file.
 * @author Pablo Morales
 * @author Noel Solís
 * @date 12-5-24
 */

#ifndef FSM_MOTOR_H_
#define FSM_MOTOR_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include <fsm.h>
/* HW dependent includes */


/* Defines and enums ----------------------------------------------------------*/
/* Enums */
enum FSM_MOTOR {
  WAIT_START = 0,
  WORK
};

enum USER_ACTIONS {
  STOP = 0,
  BEGIN
};

/* Typedefs --------------------------------------------------------------------*/

/// @brief Structure that defines a USART FSM
typedef struct{
    fsm_t f;                    /*!< FSM for the motor */
    uint8_t 	motor_id;      /*!< Used motor ID */
    uint8_t 	user_action;    /*!< Current User Action */
} fsm_motor_t;

/* Function prototypes and explanation -------------------------------------------------*/


void 	fsm_motor_set_action (fsm_t *p_this, uint8_t action);

uint8_t fsm_motor_get_action (fsm_t *p_this);

fsm_t   *fsm_motor_new (uint32_t motor_id);

void 	fsm_motor_init (fsm_t *p_this, uint32_t motor_id);

bool 	fsm_motor_check_activity (fsm_t *p_this);

#endif /* FSM_MOTOR_H_ */