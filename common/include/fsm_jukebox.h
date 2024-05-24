/**
 * @file fsm_jukebox.h
 * @brief Header for fsm_jukebox.c file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */
#ifndef FSM_JUKEBOX_H_
#define FSM_JUKEBOX_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */

#include <stdint.h>

/* Other includes */

#include <fsm.h>

#include "melodies.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */

#define MELODIES_MEMORY_SIZE 10

/* Enums */

enum FSM_JUKEBOX{
    OFF = 0,
    START_UP,
    WAIT_COMMAND,
    SLEEP_WHILE_OFF,
    SLEEP_WHILE_ON,
    SHUT_OFF
};

/* Typedefs ------------------------------------------------------------------*/

typedef struct{
    fsm_t f;
    melody_t melodies[MELODIES_MEMORY_SIZE];
    uint8_t melody_idx;
    char *p_melody;
    fsm_t *p_fsm_button;
    uint32_t on_off_press_time_ms;
    fsm_t *p_fsm_usart;
    fsm_t *p_fsm_buzzer;
    uint32_t next_song_press_time_ms;
    double speed;
    double volume;
} fsm_jukebox_t;

/* Function prototypes and explanation ---------------------------------------*/

fsm_t * fsm_jukebox_new(fsm_t *p_fsm_button, uint32_t on_off_press_time_ms, fsm_t *p_fsm_usart, fsm_t *p_fsm_buzzer, uint32_t next_song_press_time_ms);

void fsm_jukebox_init(fsm_t *p_this, fsm_t *p_fsm_button, uint32_t on_off_press_time_ms, fsm_t *p_fsm_usart, fsm_t *p_fsm_buzzer, uint32_t next_song_press_time_ms);

#endif /* FSM_JUKEBOX_H_ */