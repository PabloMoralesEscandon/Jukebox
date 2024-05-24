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

#include "port_lcd.h"

/* Defines ------------------------------------------------------------------*/
#define MAX(a, b) ((a) > (b) ? (a) : (b)) /*!< Macro to get the maximum of two values. */
#define MIN(a, b) ((a) < (b) ? (a) : (b)) /*!< Macro to get the minimum of two values. */

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
void _send(fsm_t *p_fsm_usart, char* message){
    printf(message);
    fsm_usart_set_out_data(p_fsm_usart, message);

}


void _show_song(char* song_name){
    port_lcd_clear();
    port_lcd_set_cursor(0,0);
    port_lcd_print_str("NOW PLAYING:");
    port_lcd_set_cursor(0,1);
    port_lcd_print_str(song_name);
}

void _show_state(char* state){
    port_lcd_clear();
    port_lcd_set_cursor(0,0);
    port_lcd_print_str(state);
}

void _show_vol(char* volume){
    port_lcd_clear();
    port_lcd_set_cursor(0,0);
    port_lcd_print_str("VOLUME:");
    port_lcd_set_cursor(0,1);
    port_lcd_print_str(volume);
    port_lcd_print_str("%");
}

static uint32_t _random(uint32_t min, uint32_t max){
   return (uint32_t)min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void _set_next_song(fsm_jukebox_t * p_fsm_jukebox){
    fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, STOP);
    (p_fsm_jukebox->melody_idx) +=1;
    if((p_fsm_jukebox->melody_idx >= MELODIES_MEMORY_SIZE) ||
    (p_fsm_jukebox->melodies[p_fsm_jukebox->melody_idx].melody_length <= 0)){
        p_fsm_jukebox->melody_idx = 0;
    }
    p_fsm_jukebox->p_melody = p_fsm_jukebox->melodies[p_fsm_jukebox->melody_idx].p_name;
    char msg[USART_OUTPUT_BUFFER_LENGTH];
    sprintf(msg, "Now playing: %s :) \n", p_fsm_jukebox->p_melody);
    _send(p_fsm_jukebox->p_fsm_usart, msg);
    fsm_buzzer_set_melody(p_fsm_jukebox->p_fsm_buzzer, &p_fsm_jukebox->melodies[p_fsm_jukebox->melody_idx]);
    fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, PLAY);
    _show_song(p_fsm_jukebox->p_melody);
}

void _execute_command(fsm_jukebox_t * p_fsm_jukebox, char * p_command, char * p_param){
    if(!strcmp(p_command,"play")){
        fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, PLAY);
        _show_song(p_fsm_jukebox->p_melody);
        return;
    }
    if(!strcmp(p_command,"stop")){
        fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, STOP);
        _show_state("STOP");
        return;
    }
    if(!strcmp(p_command,"pause")){
        fsm_buzzer_set_action(p_fsm_jukebox->p_fsm_buzzer, PAUSE);
        _show_state("PAUSE");
        return;
    }
    if(!strcmp(p_command,"speed")){
        double param = atof(p_param);
        fsm_buzzer_set_speed(p_fsm_jukebox->p_fsm_buzzer, MAX(param, 0.1));
        (p_fsm_jukebox->volume) = MAX(param, 0.1);
        return;
    }
    if(!strcmp(p_command,"volume")){
        double param = atof(p_param);
        fsm_buzzer_set_volume(p_fsm_jukebox->p_fsm_buzzer, MIN(param, 1.0));
        (p_fsm_jukebox->volume) = MIN(param, 1.0);
        char buffer[3];
        sprintf(buffer, "%d", (int)((p_fsm_jukebox->volume)*100));
        _show_vol(buffer);
        char msg[USART_OUTPUT_BUFFER_LENGTH];
        sprintf(msg, "Current volume: %s%%\n", buffer);
        _send(p_fsm_jukebox->p_fsm_usart, msg);
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
            _show_song(p_fsm_jukebox->p_melody);
            return;
        }
        _send(p_fsm_jukebox->p_fsm_usart, "Error: Melody not found :(\n");
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
        uint32_t melody_selected = _random(0,3);
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
        return;
    }
    if(p_fsm_jukebox->game_state==GAMING) {
        char msg[USART_OUTPUT_BUFFER_LENGTH];
        if(!strcmp(p_command,p_fsm_jukebox->p_melody)){
            sprintf(msg, "The correct answer was %s\n", p_fsm_jukebox->p_melody);
            fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, msg);
            sprintf(msg, "So your guess is correct! :)\n");
            fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, msg);
            p_fsm_jukebox->game_state=GAMING;
            return;
        } else{
            sprintf(msg, "The correct answer was %s\n", p_fsm_jukebox->p_melody);
            fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, msg);
            sprintf(msg, "So your guess is incorrect! :(\n");
            fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, msg);
            sprintf(msg, "Remember you can give up at any time with the command <<GIVE UP>>\n");
            fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, msg);  
            return;
        }
    }
    if((!strcmp(p_command,"give"))&&(!strcmp(p_param,"up"))){
        p_fsm_jukebox->game_state=WAITING;
        char msg[USART_OUTPUT_BUFFER_LENGTH];
        sprintf(msg, "The correct answer was %s\n", p_fsm_jukebox->p_melody);
        fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, msg);
        sprintf(msg, "Im dissapointed in you for not keeping on trying :(\n");
        fsm_usart_set_out_data(p_fsm_jukebox->p_fsm_usart, msg);

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
    _send(p_fsm->p_fsm_usart, "Jukebox ON :) \n");
    fsm_buzzer_set_speed(p_fsm->p_fsm_buzzer, 1.0);
    fsm_buzzer_set_melody(p_fsm->p_fsm_buzzer, &(p_fsm->melodies[0]));
    fsm_buzzer_set_action(p_fsm->p_fsm_buzzer, PLAY);
    port_lcd_clear();
    port_lcd_set_cursor(0, 0);
    port_lcd_print_str("JUKEBOX ON");
    port_lcd_set_cursor(0, 1);
    port_lcd_print_str(":D");
    port_lcd_backlight();
}

static void do_start_jukebox(fsm_t * p_this){
    fsm_jukebox_t *p_fsm = (fsm_jukebox_t *)(p_this);
    (p_fsm->melody_idx) = 0;
    (p_fsm->p_melody) = ((p_fsm->melodies[0]).p_name);
}

static void do_shut_off(fsm_t * p_this){
    fsm_jukebox_t *p_fsm = (fsm_jukebox_t *)(p_this);
    fsm_button_reset_duration(p_fsm->p_fsm_button);
    _send(p_fsm->p_fsm_usart, "Jukebox OFF :( \n");
    fsm_buzzer_set_speed(p_fsm->p_fsm_buzzer, 1.0);
    p_fsm->melody_idx = 0;
    fsm_buzzer_set_melody(p_fsm->p_fsm_buzzer, &(p_fsm->melodies[7])); // Elegir canción de apagado
    fsm_buzzer_set_action(p_fsm->p_fsm_buzzer, PLAY);
    port_lcd_clear();
    port_lcd_set_cursor(0, 0);
    port_lcd_print_str("JUKEBOX OFF");
    port_lcd_set_cursor(0, 1);
    port_lcd_print_str(":(");
    port_lcd_backlight();
}

static void do_stop_jukebox(fsm_t * p_this){
    fsm_jukebox_t *p_fsm = (fsm_jukebox_t *)(p_this);
    fsm_button_reset_duration(p_fsm->p_fsm_button);
    fsm_usart_disable_rx_interrupt(p_fsm->p_fsm_usart);
    fsm_usart_disable_tx_interrupt(p_fsm->p_fsm_usart);
    port_lcd_clear();
    port_lcd_no_backlight();
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
    // port_system_sleep();
    port_lcd_clear();
    port_lcd_set_cursor(0, 0);
    port_lcd_print_str("Zzz");
    //port_system_sleep();
}

static void do_sleep_while_off(fsm_t * p_this){
    
    port_system_sleep();
}

static void do_sleep_while_on(fsm_t * p_this){

    port_system_sleep();
}

static fsm_trans_t fsm_trans_jukebox[] = {
    {OFF, check_no_activity, SLEEP_WHILE_OFF, do_sleep_off},
    {SLEEP_WHILE_OFF, check_no_activity, SLEEP_WHILE_OFF, do_sleep_while_off},
    {SLEEP_WHILE_OFF, check_activity, OFF, NULL},
    {OFF, check_on, START_UP, do_start_up},
    {START_UP, check_melody_finished, WAIT_COMMAND, do_start_jukebox},
    {WAIT_COMMAND, check_off, SHUT_OFF, do_shut_off},
    {SHUT_OFF, check_melody_finished, OFF, do_stop_jukebox},
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
    p_fsm->melodies[4] = sailor_melody;
    p_fsm->melodies[5] = espana_melody;
    p_fsm->melodies[6] = mario_melody;
    p_fsm->melodies[7] = iscale_melody;
    p_fsm->volume = 0.5;
}

