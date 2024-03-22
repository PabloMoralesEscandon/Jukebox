/**
 * @file fsm_button.c
 * @brief Button FSM main file.
 * @author Pablo Morales
 * @author Noel Solis
 * @date 12-2-2024
 */

/* Includes ------------------------------------------------------------------*/
#include "fsm_button.h"
#include "port_button.h"


/* State machine input or transition functions */

/// @brief Check if the button has been pressed
/// @param p_this pointer to an fsm_t struct that contains an fsm_button_t
/// @return true if pressed, false if not
static bool check_button_pressed(fsm_t *p_this){
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    return port_button_is_pressed(p_fsm->button_id);

}

/// @brief Check if the button has been released
/// @param p_this pointer to an fsm_t struct that contains an fsm_button_t
/// @return true if released, false if not
static bool check_button_released(fsm_t *p_this){
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    return !port_button_is_pressed(p_fsm->button_id);
}

/// @brief Check if the debounce time has elapsed
/// @param p_this pointer to an fsm_t struct that contains an fsm_button_t
/// @return true if it has elapsed, false if not
static bool check_timeout(fsm_t *p_this){
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    if(port_button_get_tick() > p_fsm->next_timeout)
        return true;
    return false;
}

/* State machine output or action functions */

/// @brief Store the system tick when the button was pressed
/// @param p_this pointer to an fsm_t struct that contains an fsm_button_t
static void do_store_tick_pressed(fsm_t *p_this){
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    p_fsm->tick_pressed = port_button_get_tick();
    p_fsm->next_timeout = p_fsm->tick_pressed + p_fsm->debounce_time;
}

/// @brief Store the duration of the button press
/// @param p_this pointer to an fsm_t struct that contains an fsm_button_t
static void do_set_duration(fsm_t *p_this){
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    p_fsm->duration = port_button_get_tick() - p_fsm->tick_pressed;
    p_fsm->next_timeout = port_button_get_tick() + p_fsm->debounce_time;
}

static fsm_trans_t fsm_trans_button[] = {
    {BUTTON_RELEASED, check_button_pressed, BUTTON_PRESSED_WAIT, do_store_tick_pressed },
    {BUTTON_PRESSED_WAIT, check_timeout, BUTTON_PRESSED, NULL },
    {BUTTON_PRESSED, check_button_released, BUTTON_RELEASED_WAIT, do_set_duration },
    {BUTTON_RELEASED_WAIT, check_timeout, BUTTON_RELEASED, NULL },
    {-1, NULL, -1, NULL },
}; /*!< Array that contains the transitions table for the FSM */

uint32_t fsm_button_get_duration(fsm_t *p_this){
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    return p_fsm->duration;
}

void fsm_button_reset_duration(fsm_t *p_this){
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    p_fsm->duration = 0;
}

void fsm_button_init(fsm_t *p_this, uint32_t debounce_time, uint32_t button_id){
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    fsm_init(p_this, fsm_trans_button);
    p_fsm->debounce_time = debounce_time;
    p_fsm->button_id = button_id;
    p_fsm->tick_pressed = 0;
    p_fsm->duration = 0;
    port_button_init(button_id);
}

/* Other auxiliary functions */

fsm_t *fsm_button_new(uint32_t debounce_time, uint32_t button_id){
    fsm_t *p_fsm = malloc(sizeof(fsm_button_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_button_init(p_fsm, debounce_time, button_id);
    return p_fsm;
}

