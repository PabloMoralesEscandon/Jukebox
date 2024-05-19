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
static void _start_note 	(fsm_t *p_this, double freq, uint32_t duration){   
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);

    duration = (uint32_t)((double)duration/p_fsm->player_speed);
    port_buzzer_set_note_frequency(p_fsm->buzzer_id, freq);
    port_buzzer_set_note_duration(p_fsm->buzzer_id, duration);
}



static bool check_melody_start 	(fsm_t *p_this){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    return !((p_fsm->p_melody)==NULL);
}

static bool check_player_start(fsm_t *p_this){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    return !((p_fsm->p_melody)==NULL)&&(p_fsm->user_action==1);
}

static bool check_end_melody(fsm_t *p_this){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);

    return (p_fsm->note_index>=p_fsm->p_melody->melody_length);


}
 
static bool check_note_end 	(fsm_t *p_this){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    return port_buzzer_get_note_timeout(p_fsm->buzzer_id);;
}

static bool check_pause(fsm_t *p_this){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);

    return p_fsm->user_action == 2;
}

static bool check_play_note(fsm_t *p_this) {
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);

    return (p_fsm->user_action == 1)&&(!(p_fsm->note_index>=p_fsm->p_melody->melody_length));

}

static bool check_player_stop(fsm_t *p_this){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);

    return p_fsm->user_action == 0;
}

static bool check_resume(fsm_t *p_this){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);

    return p_fsm->user_action == 1;
}



static void do_end_melody 	(fsm_t *p_this){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    port_buzzer_stop(p_fsm->buzzer_id);
    p_fsm->note_index = 0;
    p_fsm->user_action = 0;
}

static void do_melody_start(fsm_t *p_this){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    double freq = *(p_fsm->p_melody->p_notes);
    uint32_t duration = *(p_fsm->p_melody->p_durations);
    _start_note(p_this,freq, duration);
    p_fsm->note_index++;

}

static void do_note_end (fsm_t *p_this){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    port_buzzer_stop(p_fsm->buzzer_id);

}

static void do_pause (fsm_t *p_this){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    port_buzzer_stop(p_fsm->buzzer_id);

}

static void do_play_note(fsm_t *p_this){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    double freq = (p_fsm->p_melody->p_notes[p_fsm->note_index]);
    uint32_t duration = (p_fsm->p_melody->p_durations[p_fsm->note_index]);
    _start_note(p_this,freq, duration);
    p_fsm->note_index++;

}

static void do_player_start(fsm_t *p_this){
    do_melody_start(p_this);
}

static void do_player_stop(fsm_t *p_this) {
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    port_buzzer_stop(p_fsm->buzzer_id);
    p_fsm->note_index = 0;
}

static fsm_trans_t fsm_trans_buzzer[] = {
    {WAIT_START, check_player_start, WAIT_NOTE, do_player_start},
    {WAIT_NOTE , check_note_end, PLAY_NOTE, do_note_end },
    {PLAY_NOTE, check_player_stop, WAIT_START, do_player_stop },
    {PLAY_NOTE, check_play_note, WAIT_NOTE, do_play_note },
    {PLAY_NOTE, check_pause, PAUSE_NOTE, do_pause },
    {PLAY_NOTE, check_end_melody, WAIT_MELODY, do_end_melody },
    {PAUSE_NOTE, check_resume, PLAY_NOTE, NULL },
    {WAIT_MELODY, check_melody_start, WAIT_NOTE, do_melody_start },
    {-1, NULL, -1, NULL },
};


/* Public functions */


fsm_t* fsm_buzzer_new(uint32_t buzzer_id){
    fsm_t *p_fsm = malloc(sizeof(fsm_buzzer_t));
    fsm_buzzer_init(p_fsm, buzzer_id);
    return p_fsm;
}

void fsm_buzzer_init(fsm_t *p_this, uint32_t buzzer_id){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    fsm_init(p_this, fsm_trans_buzzer);

    /* TO-DO alumnos */
    
    p_fsm->buzzer_id = buzzer_id;
    p_fsm->p_melody = NULL;
    p_fsm->note_index = 0;
    p_fsm->user_action = 0;
    p_fsm->player_speed = 1.0;
    port_buzzer_init(buzzer_id);


}


void fsm_buzzer_set_melody(fsm_t *p_this, const melody_t *p_melody){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    p_fsm->p_melody = (melody_t *)p_melody;

}


void fsm_buzzer_set_speed(fsm_t *p_this, double speed){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    p_fsm->player_speed = speed;
}

void fsm_buzzer_set_action(fsm_t *p_this, uint8_t action){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    p_fsm->user_action = action;
    if(action==0) p_fsm->note_index=0;
}


uint8_t fsm_buzzer_get_action(fsm_t *p_this){
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    return p_fsm->user_action;
}

bool fsm_buzzer_check_activity(fsm_t *p_this) {
    fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
    return p_fsm->user_action==PLAY;

}