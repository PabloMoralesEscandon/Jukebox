/**
 * @file fsm_jukebox.h
 * @brief Header for fsm_jukebox.c file.
 * @author Pablo Morales
 * @author Noel Solis
 * @date 2-5-24
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

/// @brief Enumerator for the Jukebox finite state machine
enum FSM_JUKEBOX{
    OFF = 0,
    START_UP,
    WAIT_COMMAND,
    SLEEP_WHILE_OFF,
    SLEEP_WHILE_ON,
    SHUT_OFF
};

enum GAME_STATE{
    WAITING = 0,
    GAMING,
};


/* Typedefs ------------------------------------------------------------------*/

/// @brief Structure that contains the information of a melody
typedef struct{
    fsm_t f;    /*!< jukebox fsm struct */
    melody_t melodies[MELODIES_MEMORY_SIZE];    /*!< Melodies array */
    uint8_t melody_idx; /*!< Index to current note in melody */
    char *p_melody; /*!< Melody name pointer */
    fsm_t *p_fsm_button;    /*!< buttons fsm */
    uint32_t on_off_press_time_ms;  /*!< Time to press to turn off and on in milis */
    fsm_t *p_fsm_usart; /*!< usart's fsm */
    fsm_t *p_fsm_buzzer;    /*!<buzzer's fsm */
    uint32_t next_song_press_time_ms;   /*!< Time to press for next song in milis */
    double speed;   /*!< Reproduction Speed */
    double volume;  /*!< Reproduction Volume */
    uint8_t game_state; /*!< Guessing game state */
} fsm_jukebox_t;

/* Function prototypes and explanation ---------------------------------------*/

/// @brief Create a new jukebox FSM. 
/// @param p_fsm_button Pointer to the button FSM 
/// @param on_off_press_time_ms Button press time in milliseconds to turn the system ON or OFF 
/// @param p_fsm_usart Pointer to the USART FSM 
/// @param p_fsm_buzzer Pointer to the buzzer FSM. 
/// @param next_song_press_time_ms Button press time in milliseconds to change to the next song.
/// @return A pointer to the button FSM 
fsm_t * fsm_jukebox_new(fsm_t *p_fsm_button, uint32_t on_off_press_time_ms, fsm_t *p_fsm_usart, fsm_t *p_fsm_buzzer, uint32_t next_song_press_time_ms);

/// @brief Initialize a jukebox FSM. 
/// @param p_this Pointer to the jukebox FSM 
/// @param p_fsm_button Pointer to the button FSM 
/// @param on_off_press_time_ms Button press time in milliseconds to turn the system ON or OFF 
/// @param p_fsm_usart Pointer to the USART FSM 
/// @param p_fsm_buzzer Pointer to the buzzer FSM. 
/// @param next_song_press_time_ms Button press time in milliseconds to change to the next song.
void fsm_jukebox_init(fsm_t *p_this, fsm_t *p_fsm_button, uint32_t on_off_press_time_ms, fsm_t *p_fsm_usart, fsm_t *p_fsm_buzzer, uint32_t next_song_press_time_ms);

#endif /* FSM_JUKEBOX_H_ */