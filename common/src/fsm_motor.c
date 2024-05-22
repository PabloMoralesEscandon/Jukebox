/**
 * @file fsm_motor.c
 * @brief Buzzer melody player FSM main file.
 * @author Pablo Morales
 * @author Noel Solís
 * @date 12-5-24
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C libraries */
#include <stdlib.h>
/* Other libraries */
#include "port_motor.h"
#include "fsm_motor.h"
/* State machine input or transition functions */


/* State machine output or action functions */
static void _start_motor(fsm_t *p_this, double freq){   
    fsm_motor_t *p_fsm = (fsm_motor_t *)(p_this);

    port_motor_set_frequency(p_fsm->motor_id, freq);
}

static bool check_motor_start(fsm_t *p_this) {
    fsm_motor_t *p_fsm = (fsm_motor_t *)(p_this);

    return (p_fsm->user_action == 1);

}

static bool check_motor_stop(fsm_t *p_this){
    fsm_motor_t *p_fsm = (fsm_motor_t *)(p_this);

    return p_fsm->user_action == 0;
}


static void do_motor_start(fsm_t *p_this, double freq){
    fsm_motor_t *p_fsm = (fsm_motor_t *)(p_this);
    _start_motor(p_this,freq);
}

static void do_motor_stop (fsm_t *p_this){
    fsm_motor_t *p_fsm = (fsm_motor_t *)(p_this);
    port_motor_stop(p_fsm->motor_id);

}

static fsm_trans_t fsm_trans_motor[] = {
    {WAIT_START, check_motor_start, WORK, do_motor_start},
    {WORK , check_motor_stop, WAIT_START, do_motor_stop },
    {-1, NULL, -1, NULL },
};


/* Public functions */


fsm_t* fsm_motor_new(uint32_t motor_id){
    fsm_t *p_fsm = malloc(sizeof(fsm_motor_t));
    fsm_motor_init(p_fsm, motor_id);
    return p_fsm;
}

void fsm_motor_init(fsm_t *p_this, uint32_t motor_id){
    fsm_motor_t *p_fsm = (fsm_motor_t *)(p_this);
    fsm_init(p_this, fsm_trans_motor);
    p_fsm->motor_id = motor_id;
    p_fsm->user_action = 0;
    port_motor_init(motor_id);


}

void fsm_motor_set_action(fsm_t *p_this, uint8_t action){
    fsm_motor_t *p_fsm = (fsm_motor_t *)(p_this);
    switch (action)
    {
    case STOP:
        p_fsm->user_action = action;
        break;
    
    case BEGIN:
        p_fsm->user_action = action;
        break;
    
    default:
        break;
    }
    return;
}


uint8_t fsm_motor_get_action(fsm_t *p_this){
    fsm_motor_t *p_fsm = (fsm_motor_t *)(p_this);
    return p_fsm->user_action;
}

bool fsm_motor_check_activity(fsm_t *p_this) {
    fsm_motor_t *p_fsm = (fsm_motor_t *)(p_this);
    return p_fsm->user_action==BEGIN;

}