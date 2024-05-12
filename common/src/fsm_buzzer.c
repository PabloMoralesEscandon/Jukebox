/**
 * @file fsm_buzzer.c
 * @brief Buzzer melody player FSM main file.
 * @author Pablo Morales
 * @author Noel Solís
 * @date 12-5-24
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C libraries */
#include <stdlib.h>
/* Other libraries */
#include "port_buzzer.h"
#include "fsm_buzzer.h"
#include "melodies.h"
/* State machine input or transition functions */


/* State machine output or action functions */
static void _start_note 	(fsm_t *p_this, double freq, uint32_t duration)
{   
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);

    duration = (uint32_t)((double)duration/p_fsm->player_speed);
    port_buzzer_set_note_frequency(p_fsm->buzzer_id, freq);
    port_buzzer_set_note_duration(p_fsm->buzzer_id, duration);
}



static bool check_melody_start 	(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    return !p_fsm->p_melody==null;
}
static bool check_player_start(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    return !p_fsm->p_melody==null;
}

static bool check_end_melody(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);

    return (p_fsm->note_index>=p_fsm->p_melody->melody_length*);


}
 
static bool check_note_end 	(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    return port_buzzer_get_note_timeout(p_fsm->buzzer_id);;
}

static bool check_pause(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);

    return p_fsm->user_action == 2;
}

static bool check_play_note(fsm_t *p_this) 
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);

    return (p_fsm->user_action == 2)&&(!(p_fsm->note_index>=p_fsm->p_melody->melody_length*));

}

static bool check_player_start(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);

    return check_melody_start(p_this);
}

static bool check_player_stop(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);

    return p_fsm->user_action == 0;
}

static bool check_resume(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);

    return p_fsm->user_action == 1;
}



static void do_end_melody 	(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    port_buzzer_stop(p_fsm->buzzer_id);
    p_fsm->note_index = 0;
    p_fsm->user_action = 0;
}

static void do_melody_start(fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    freq = (p_fsm->p_melody->p_notes)*;
    duration = (p_fsm->p_melody->p_durations)*;
    _start_note(p_this,freq, duration);
    p_fsm->note_index++;

}

static void do_note_end (fsm_t *p_this)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);


}


/* Public functions */


fsm_t *fsm_buzzer_new(uint32_t buzzer_id)
{
    fsm_t *p_fsm = malloc(sizeof(fsm_buzzer_t));
    fsm_buzzer_init(p_fsm, buzzer_id);
    return p_fsm;
}

void fsm_buzzer_init(fsm_t *p_this, uint32_t buzzer_id)
{
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    fsm_init(p_this, fsm_trans_buzzer);

    /* TO-DO alumnos */
    
}