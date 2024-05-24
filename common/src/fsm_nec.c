/**
 * @file fsm_NEC.c
 * @brief NEC FSM main file.
 * @author Pablo Morales
 * @author Noel Solis
 * @date 12-2-2024
 */

/* Includes ------------------------------------------------------------------*/
#include "fsm_nec.h"
#include "port_nec.h"


/* State machine input or transition functions */

/// @brief Check if the NEC has been pressed
/// @param p_this pointer to an fsm_t struct that contains an fsm_NEC_t
/// @return true if pressed, false if not
static bool check_NEC_event(fsm_t *p_this){
    fsm_NEC_t *p_fsm = (fsm_NEC_t *)(p_this);
    return port_NEC_event(p_fsm->NEC_id);

}

/// @brief Check if the NEC has been pressed
/// @param p_this pointer to an fsm_t struct that contains an fsm_NEC_t
/// @return true if pressed, false if not
static bool check_NEC_decode(fsm_t *p_this){
    fsm_NEC_t *p_fsm = (fsm_NEC_t *)(p_this);
    return port_NEC_decoding(p_fsm->NEC_id);

}

/* State machine output or action functions */

/// @brief Store the system tick when the NEC was pressed
/// @param p_this pointer to an fsm_t struct that contains an fsm_NEC_t
static void do_wait(fsm_t *p_this){
    fsm_NEC_t *p_fsm = (fsm_NEC_t *)(p_this);
    port_NEC_set_event(p_fsm->NEC_id, false);
}

static void do_decode(fsm_t *p_this){
    fsm_NEC_t *p_fsm = (fsm_NEC_t *)(p_this);
    port_NEC_set_decode(p_fsm->NEC_id, true);
}

static void do_store(fsm_t *p_this){
    fsm_NEC_t *p_fsm = (fsm_NEC_t *)(p_this);
    port_NEC_set_decode(p_fsm->NEC_id, false);
    port_NEC_set_event(p_fsm->NEC_id, false);
    (p_fsm->message) = port_NEC_get_message(p_fsm->NEC_id);
    (p_fsm->decoded) = true;
}

static fsm_trans_t fsm_trans_NEC[] = {
    {NEC_WAIT, check_NEC_event, NEC_START, do_wait },
    {NEC_START, check_NEC_event, NEC_HOLD, do_wait },
    {NEC_HOLD, check_NEC_event, NEC_DECODE, do_decode },
    {NEC_DECODE, check_NEC_decode, NEC_WAIT, do_store },
    {-1, NULL, -1, NULL },
}; /*!< Array that contains the transitions table for the FSM */

void fsm_NEC_init(fsm_t *p_this, uint32_t NEC_id){
    fsm_NEC_t *p_fsm = (fsm_NEC_t *)(p_this);
    fsm_init(p_this, fsm_trans_NEC);
    p_fsm->NEC_id = NEC_id;
    p_fsm->message = 0;
    p_fsm->decoded = false;
    port_NEC_init(NEC_id);
}

uint32_t fsm_NEC_get_message(fsm_t *p_this){
    fsm_NEC_t *p_fsm = (fsm_NEC_t *)(p_this);
    return (p_fsm->message);
    (p_fsm->decoded) = false;
}

bool fsm_NEC_check_activity(fsm_t *p_this) {
    fsm_NEC_t *p_fsm = (fsm_NEC_t *)(p_this);
    return (p_fsm->decoded);
}



/* Other auxiliary functions */

fsm_t *fsm_NEC_new(uint32_t NEC_id){
    fsm_t *p_fsm = malloc(sizeof(fsm_NEC_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_NEC_init(p_fsm, NEC_id);
    return p_fsm;
}

