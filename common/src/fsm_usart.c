/**
 * @file fsm_usart.c
 * @brief USART FSM main file.
 * @author Pablo Morales
 * @author Noel Solís
 * @date 26-2-2024
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C libraries */
#include <string.h>
#include <stdlib.h>

/* Other libraries */
#include "port_usart.h"
#include "fsm_usart.h"

/* State machine input or transition functions */

/// @brief Checks if there is received data
/// @param p_this Pointer to an fsm struct that corresponds to an UART
/// @return True if there is received data, false if not
static bool check_data_rx(fsm_t *p_this){
    fsm_usart_t *p_fsm = (fsm_usart_t *)(p_this);
    return port_usart_rx_done(p_fsm->usart_id);
}

/// @brief Checks if there is data to be sent
/// @param p_this Pointer to an fsm struct that corresponds to an UART
/// @return True if there is data to be sent, false if not
static bool check_data_tx(fsm_t *p_this){
    fsm_usart_t *p_fsm = (fsm_usart_t *)(p_this);
    return (p_fsm->out_data[0])!=EMPTY_BUFFER_CONSTANT;
}

/// @brief Checks if data is sent
/// @param p_this Pointer to an fsm struct that corresponds to an UART
/// @return True if data is sent, false if not
static bool check_tx_end(fsm_t *p_this){
    fsm_usart_t *p_fsm = (fsm_usart_t *)(p_this);
    return port_usart_tx_done(p_fsm->usart_id);
}

/* State machine output or action functions */

/// @brief Gets received data
/// @param p_this Pointer to an fsm struct that corresponds to an UART
static void do_get_data_rx(fsm_t *p_this){
    fsm_usart_t *p_fsm = (fsm_usart_t *)(p_this);
    port_usart_get_from_input_buffer(p_fsm->usart_id, p_fsm->in_data);
    port_usart_reset_input_buffer(p_fsm->usart_id);
    p_fsm->data_received = true;
}

/// @brief Sets data to be sent
/// @param p_this Pointer to an fsm struct that corresponds to an UART
static void do_set_data_tx(fsm_t *p_this){
    fsm_usart_t *p_fsm = (fsm_usart_t *)(p_this);
    port_usart_reset_output_buffer(p_fsm->usart_id);
    port_usart_copy_to_output_buffer(p_fsm->usart_id, p_fsm->out_data, USART_OUTPUT_BUFFER_LENGTH);
    while(!port_usart_get_txr_status(p_fsm->usart_id)){}
    port_usart_write_data(p_fsm->usart_id);
    port_usart_enable_tx_interrupt(p_fsm->usart_id);
}

/// @brief Finishes sending data
/// @param p_this Pointer to an fsm struct that corresponds to an UART
static void do_tx_end(fsm_t *p_this){
    fsm_usart_t *p_fsm = (fsm_usart_t *)(p_this);
    port_usart_reset_output_buffer(p_fsm->usart_id);
    memset(p_fsm->out_data, EMPTY_BUFFER_CONSTANT, USART_OUTPUT_BUFFER_LENGTH);
}

/// @brief Array containing the transition table for the UART FSM
static fsm_trans_t fsm_trans_usart[] = {
    {WAIT_DATA, check_data_tx, SEND_DATA, do_set_data_tx},
    {WAIT_DATA, check_data_rx, WAIT_DATA, do_get_data_rx},
    {SEND_DATA, check_tx_end, WAIT_DATA, do_tx_end},
    {-1, NULL, -1, NULL },
};

/* Public functions */
void fsm_usart_get_in_data(fsm_t *p_this, char *p_data){
    fsm_usart_t *p_fsm = (fsm_usart_t *)(p_this);
    memcpy(p_data, p_fsm->in_data, USART_INPUT_BUFFER_LENGTH);
}

void fsm_usart_set_out_data(fsm_t *p_this, char *p_data){
    fsm_usart_t *p_fsm = (fsm_usart_t *)(p_this);
    // Ensure to reset the output data before setting a new one
    memset(p_fsm->out_data, EMPTY_BUFFER_CONSTANT, USART_OUTPUT_BUFFER_LENGTH);
    memcpy(p_fsm->out_data, p_data, USART_OUTPUT_BUFFER_LENGTH);
}


fsm_t *fsm_usart_new(uint32_t usart_id){
    fsm_t *p_fsm = malloc(sizeof(fsm_usart_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_usart_init(p_fsm, usart_id);
    return p_fsm;
}

void fsm_usart_init(fsm_t *p_this, uint32_t usart_id){
    fsm_usart_t *p_fsm = (fsm_usart_t *)(p_this);
    
    fsm_init(p_this, fsm_trans_usart);
    p_fsm->usart_id = usart_id;
    p_fsm->data_received = false;
    memset(p_fsm->in_data, EMPTY_BUFFER_CONSTANT, USART_INPUT_BUFFER_LENGTH);
    memset(p_fsm->out_data, EMPTY_BUFFER_CONSTANT, USART_OUTPUT_BUFFER_LENGTH);
    port_usart_init(p_fsm->usart_id);
}

bool fsm_usart_check_data_received(fsm_t *p_this){
    fsm_usart_t *p_fsm = (fsm_usart_t *)(p_this);
    return p_fsm->data_received;
}

void fsm_usart_reset_input_data(fsm_t *p_this){
    fsm_usart_t *p_fsm = (fsm_usart_t *)(p_this);
    memset(p_fsm->in_data, EMPTY_BUFFER_CONSTANT, USART_INPUT_BUFFER_LENGTH);
    p_fsm->data_received = false;
}

void fsm_usart_disable_rx_interrupt(fsm_t *p_this){
    fsm_usart_t *p_fsm = (fsm_usart_t *)(p_this);
    port_usart_disable_rx_interrupt(p_fsm->usart_id);
}

void fsm_usart_disable_tx_interrupt(fsm_t *p_this){
    fsm_usart_t *p_fsm = (fsm_usart_t *)(p_this);
    port_usart_disable_tx_interrupt(p_fsm->usart_id);
}

void fsm_usart_enable_rx_interrupt(fsm_t *p_this){
    fsm_usart_t *p_fsm = (fsm_usart_t *)(p_this);
    port_usart_enable_rx_interrupt(p_fsm->usart_id);
}

void fsm_usart_enable_tx_interrupt(fsm_t *p_this){
    fsm_usart_t *p_fsm = (fsm_usart_t *)(p_this);
    port_usart_enable_tx_interrupt(p_fsm->usart_id);
}

bool fsm_usart_check_activity(fsm_t *p_this){
    fsm_usart_t *p_fsm = (fsm_usart_t *)(p_this);
    return ((p_fsm->f).current_state==SEND_DATA)||(p_fsm->data_received);
}