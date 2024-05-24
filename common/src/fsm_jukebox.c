/**
 * @file fsm_jukebox.c
 * @brief Jukebox FSM main file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

/* Includes ------------------------------------------------------------------*/
// Standard C includes

#include <string.h> // strcmp

#include <stdio.h>  // sprintf

#include <stdlib.h>

// Other includes

#include <fsm.h>

#include "fsm_jukebox.h"

#include "fsm_button.h"

#include "fsm_usart.h"

#include "fsm_buzzer.h"

#include "port_system.h"

#include "port_usart.h"

/* Defines ------------------------------------------------------------------*/
#define MAX(a, b) ((a) > (b) ? (a) : (b)) /*!< Macro to get the maximum of two values. */

/* Private functions */
/**
 * @brief Parse the message received by the USART.
 * 
 * Given data received by the USART, this function parses the message and extracts the command and the parameter (if available).
 * 
 * > 1. Split the message by space using function `strtok()` \n
 * > 2. If there's a token (command), copy it to the `p_command` variable. Otherwise, return `false` \n
 * > 3. Extract the parameter (if available). To do so, get the next token using function `strtok()`. If there's a token, copy it to the `p_param` variable. Otherwise, copy an empty string to the `p_param` variable \n
 * > 4. Return `true` indicating that the message has been parsed correctly \n
 * 
 * @param p_message Pointer to the message received by the USART.
 * @param p_command Pointer to store the command extracted from the message. 
 * @param p_param Pointer to store the parameter extracted from the message. 
 * @return true if the message has been parsed correctly 
 * @return false if the message has not been parsed correctly 
 */
bool _parse_message(char *p_message, char *p_command, char *p_param)
{
    char *p_token = strtok(p_message, " "); // Split the message by space

    // If there's a token (command), copy it to the command variable
    if (p_token != NULL)
    {
        strcpy(p_command, p_token);
    }
    else
    {
        // No command found, you might return an error or handle it as needed
        // The USART driver of the computer sends an empty at initialization, so we will ignore it
        return false;
    }

    // Extract the parameter (if available)
    p_token = strtok(NULL, " "); // Get the next token

    if (p_token != NULL)
    {
        strcpy(p_param, p_token);
    }
    else
    {
        strcpy(p_param, " "); // NO param found
    }
    return true;
}

void _set_next_song(fsm_jukebox_t * p_fsm_jukebox){
    fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, STOP);
    (p_fsm_jukebox->melody_idx) +=1;
    if((p_fsm_jukebox->melody_idx >= MELODIES_MEMORY_SIZE) ||
    (p_fsm_jukebox->melodies[p_fsm_jukebox->melody_idx].melody_length <= 0)){
        p_fsm_jukebox->melody_idx = 0;
    }
    p_fsm_jukebox->p_melody = p_fsm_jukebox->melodies[p_fsm_jukebox->melody_idx].p_name;
    printf("Now playing: %s :) \n", p_fsm_jukebox->p_melody);
    fsm_buzzer_set_melody(p_fsm_jukebox->p_fsm_buzzer, &p_fsm_jukebox->melodies[p_fsm_jukebox->melody_idx]);
    fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, PLAY);
}

void _execute_command(fsm_jukebox_t * p_fsm_jukebox, char * p_command, char * p_param){
    if(!strcmp(p_command,"play")){
        fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, PLAY);
        return;
    }
    if(!strcmp(p_command,"stop")){
        fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, STOP);
        return;
    }
    if(!strcmp(p_command,"pause")){
        fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, PAUSE);
        return;
    }
    if(!strcmp(p_command,"speed")){
        double param = atof(p_param);
        fsm_buzzer_set_speed(p_fsm_jukebox->p_fsm_buzzer, MAX(param, 0.1));
        return;
    }
    if(!strcmp(p_command,"next")){
        _set_next_song(p_fsm_jukebox);
        return;
    }
    if(!strcmp(p_command,"select")){
        uint32_t melody_selected = atoi(p_param);
        if(p_fsm_jukebox->melodies[melody_selected].melody_length > 0){
            fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, STOP);
            p_fsm_jukebox->melody_idx = melody_selected;
            const melody_t* melody = &p_fsm_jukebox->melodies[p_fsm_jukebox->melody_idx];
            fsm_buzzer_set_melody(p_fsm_jukebox->p_fsm_buzzer, melody);
            p_fsm_jukebox->p_melody = p_fsm_jukebox->melodies[p_fsm_jukebox->melody_idx].p_name;
            fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, PLAY);
            return;
        }
        fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, "Error: Melody not found :(\n");
        printf("Error: Melody not found :(\n");
        return;
    }
    if(!strcmp(p_command,"info")){
        char msg[USART_OUTPUT_BUFFER_LENGTH];
        sprintf(msg, "Playing: %s\n", p_fsm_jukebox->p_melody);
        fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, msg);
        return;
    }
    if(!strcmp(p_command,"game")){
        char msg[USART_OUTPUT_BUFFER_LENGTH];
        sprintf(msg, "Gaming\n");
        fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, msg);
        uint32_t melody_selected = random(0,3)
        if(p_fsm_jukebox->melodies[melody_selected].melody_length > 0){
            fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, STOP);
            p_fsm_jukebox->melody_idx = melody_selected;
            const melody_t* melody = &p_fsm_jukebox->melodies[p_fsm_jukebox->melody_idx];
            fsm_buzzer_set_melody(p_fsm_jukebox->p_fsm_buzzer, melody);
            p_fsm_jukebox->p_melody = p_fsm_jukebox->melodies[p_fsm_jukebox->melody_idx].p_name;
            fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, PLAY);
            p_fsm_jukebox->game_state = GAMING;
            return;
        }
        
        //Falta implementar todo el juego, usando este comando para 
        //activarlo, tras iniciar el juego debería sonar un sonido 
        //genérico de aviso y posteriormente comenzar una canción
        //En cualquier momento tras el inicio de esta canción puedes
        //usar el comando pick 1, 2 etc para hacer tu guess de qué
        //canción se trata

        //Cuando estemos jugando tendremos que tener algun flag levantado
        //y guardar que cancion es la que ha tocado (o sacar la que está
        //siendo reproducida como con el comando de info que tenemos ya 
        //implementado). Si la acierta
        return;
    }
    if(!strcmp(p_command,"pick")){
        if(p_fsm_jukebox->game_state!=GAMING){
            sprintf(msg, "Begin gaming before trying to guess the song\n");
            fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, msg);
            return;
        }
        char msg[USART_OUTPUT_BUFFER_LENGTH];
        sprintf(msg, "You have guessed%s\n",p_command);
        fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, msg);
        bool check = !strcmp(p_param,p_fsm_jukebox->p_melody);
        if(check) {
            sprintf(msg, "The correct answer was %s\nSo your guess is correct! :)\n", p_fsm_jukebox->p_melody);
            fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, msg);
        }else {
            sprintf(msg, "The correct answer was %s\nSo your guess is incorrect! :(\n", p_fsm_jukebox->p_melody);
            fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, msg);  
        }

        return;
    }
    
    fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, "Error: Command not found :(\n");
    printf("Error: Command not found :(\n");
    fsm_usart_reset_input_data(p_fsm_jukebox->p_fsm_usart);
    return;
}

/* State machine input or transition functions */

static bool check_on(fsm_t * p_this){
    fsm_jukebox_t *p_fsm = (fsm_jukebox_t *)(p_this);
    uint32_t duration = fsm_button_get_duration(p_fsm->p_fsm_button);
    return ((duration > 0) && (duration > (p_fsm->on_off_press_time_ms)));
}

static bool check_off(fsm_t * p_this){
     return check_on(p_this);
}

static bool check_melody_finished(fsm_t * p_this){
    fsm_jukebox_t *p_fsm = (fsm_jukebox_t *)(p_this);
    return (fsm_buzzer_get_action(p_fsm->p_fsm_buzzer) == STOP);
}

static bool check_command_received(fsm_t * p_this){
    fsm_jukebox_t *p_fsm = (fsm_jukebox_t *)(p_this);
    if(fsm_usart_check_data_received(p_fsm->p_fsm_usart)){
        printf("Command received\n");
    }
    
    return fsm_usart_check_data_received(p_fsm->p_fsm_usart);
}

static bool check_next_song_button(fsm_t * p_this){
    fsm_jukebox_t *p_fsm = (fsm_jukebox_t *)(p_this);
    return (
        (fsm_button_get_duration(p_fsm->p_fsm_button) > 0) && 
        (fsm_button_get_duration(p_fsm->p_fsm_button) > p_fsm->next_song_press_time_ms) &&
        (fsm_button_get_duration(p_fsm->p_fsm_button) < p_fsm->on_off_press_time_ms)
    );
}
static bool check_activity(fsm_t * p_this){
    fsm_jukebox_t *p_fsm = (fsm_jukebox_t *)(p_this);
    return (
        (fsm_button_check_activity(p_fsm->p_fsm_button)) ||
        (fsm_usart_check_activity(p_fsm->p_fsm_usart)) ||
        (fsm_buzzer_check_activity(p_fsm->p_fsm_buzzer))
    );
}

static bool check_no_activity(fsm_t * p_this){
    return !check_activity(p_this);
}

/* State machine output or action functions */

static void do_start_up(fsm_t * p_this){
    fsm_jukebox_t *p_fsm = (fsm_jukebox_t *)(p_this);
    fsm_button_reset_duration(p_fsm->p_fsm_button);
    fsm_usart_enable_rx_interrupt(p_fsm->p_fsm_usart);
    printf("Jukebox ON :) \n");
    fsm_buzzer_set_speed(p_fsm->p_fsm_buzzer, 1.0);
    fsm_buzzer_set_melody(p_fsm->p_fsm_buzzer, &(p_fsm->melodies[0]));
    fsm_buzzer_set_action(p_fsm->p_fsm_buzzer, PLAY);
}

static void do_start_jukebox(fsm_t * p_this){
    fsm_jukebox_t *p_fsm = (fsm_jukebox_t *)(p_this);
    (p_fsm->melody_idx) = 0;
    (p_fsm->p_melody) = ((p_fsm->melodies[0]).p_name);
}

static void do_stop_jukebox(fsm_t * p_this){
    fsm_jukebox_t *p_fsm = (fsm_jukebox_t *)(p_this);
    fsm_button_reset_duration(p_fsm->p_fsm_button);
    fsm_usart_disable_rx_interrupt(p_fsm->p_fsm_usart);
    fsm_usart_disable_tx_interrupt(p_fsm->p_fsm_usart);
    printf("Jukebox OFF :( \n");
    fsm_buzzer_set_action(p_fsm->p_fsm_buzzer, STOP);
}

static void do_load_next_song(fsm_t * p_this){
    fsm_jukebox_t *p_fsm = (fsm_jukebox_t *)(p_this);
    _set_next_song(p_fsm);
    fsm_button_reset_duration(p_fsm->p_fsm_button);
}

static void do_read_command(fsm_t * p_this){
    fsm_jukebox_t *p_fsm = (fsm_jukebox_t *)(p_this);
    char p_message[USART_INPUT_BUFFER_LENGTH];
    char p_command[USART_INPUT_BUFFER_LENGTH];
    char p_param[USART_INPUT_BUFFER_LENGTH];
    fsm_usart_get_in_data(p_fsm->p_fsm_usart, p_message);
    bool valid = _parse_message(p_message, p_command, p_param);
    if(valid){
        _execute_command(p_fsm, p_command, p_param);
    }
    fsm_usart_reset_input_data(p_fsm->p_fsm_usart);
    memset(p_message, EMPTY_BUFFER_CONSTANT, USART_INPUT_BUFFER_LENGTH);
}

static void do_sleep_off(fsm_t * p_this){
    port_system_sleep();
}

static void do_sleep_wait_command(fsm_t * p_this){
    port_system_sleep();
}

static void do_sleep_while_off(fsm_t * p_this){
    port_system_sleep();
}

static void do_sleep_while_on(fsm_t * p_this){
    port_system_sleep();
}

static uint32_t random(uint32_t min, uint32_t max){
   return (uint32_t)min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

static fsm_trans_t fsm_trans_jukebox[] = {
    {OFF, check_no_activity, SLEEP_WHILE_OFF, do_sleep_off},
    {SLEEP_WHILE_OFF, check_no_activity, SLEEP_WHILE_OFF, do_sleep_while_off},
    {SLEEP_WHILE_OFF, check_activity, OFF, NULL},
    {OFF, check_on, START_UP, do_start_up},
    {START_UP, check_melody_finished, WAIT_COMMAND, do_start_jukebox},
    {WAIT_COMMAND, check_off, OFF, do_stop_jukebox},
    {WAIT_COMMAND, check_next_song_button, WAIT_COMMAND, do_load_next_song},
    {WAIT_COMMAND, check_command_received, WAIT_COMMAND, do_read_command},
    {WAIT_COMMAND, check_no_activity, SLEEP_WHILE_ON, do_sleep_wait_command},
    {SLEEP_WHILE_ON, check_no_activity, SLEEP_WHILE_ON, do_sleep_while_on},
    {SLEEP_WHILE_ON, check_activity, WAIT_COMMAND, NULL},
    {-1, NULL, -1, NULL },
};

/* Public functions */
fsm_t *fsm_jukebox_new(fsm_t *p_fsm_button, uint32_t on_off_press_time_ms, fsm_t *p_fsm_usart, fsm_t *p_fsm_buzzer, uint32_t next_song_press_time_ms)
{
    fsm_t *p_fsm = malloc(sizeof(fsm_jukebox_t));

    fsm_jukebox_init(p_fsm, p_fsm_button, on_off_press_time_ms, p_fsm_usart, p_fsm_buzzer, next_song_press_time_ms);
    
    return p_fsm;
}

void fsm_jukebox_init(fsm_t *p_this, fsm_t *p_fsm_button, uint32_t on_off_press_time_ms, fsm_t *p_fsm_usart, fsm_t *p_fsm_buzzer, uint32_t next_song_press_time_ms){
    fsm_init(p_this, fsm_trans_jukebox);
    fsm_jukebox_t *p_fsm = (fsm_jukebox_t *)(p_this);
    p_fsm->p_fsm_button = p_fsm_button;
    p_fsm->on_off_press_time_ms = on_off_press_time_ms;
    p_fsm->p_fsm_usart = p_fsm_usart;
    p_fsm->p_fsm_buzzer = p_fsm_buzzer;
    p_fsm->next_song_press_time_ms = next_song_press_time_ms;
    p_fsm->game_state = WAITING;
    p_fsm->melody_idx = 0;
    memset(p_fsm->melodies, EMPTY_BUFFER_CONSTANT, sizeof(p_fsm->melodies));
    p_fsm->melodies[0] = scale_melody;
    p_fsm->melodies[1] = happy_birthday_melody;
    p_fsm->melodies[2] = tetris_melody;
    p_fsm->melodies[3] = megalovania_melody;
}

