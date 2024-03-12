/**
 * @file fsm_usart.h
 * @brief Header for fsm_usart.c file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

#ifndef FSM_USART_H_
#define FSM_USART_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include <fsm.h>

/* HW dependent includes */
#include "port_usart.h"

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
enum FSM_USART{
    WAIT_DATA = 0,
    SEND_DATA
};

/* Typedefs --------------------------------------------------------------------*/
typedef struct{
    fsm_t f;
    bool data_received;
    char in_data [USART_INPUT_BUFFER_LENGTH];
    char out_data [USART_OUTPUT_BUFFER_LENGTH];
    uint8_t usart_id;

} fsm_usart_t;

/* Function prototypes and explanation -------------------------------------------------*/

fsm_t * fsm_usart_new(uint32_t usart_id);

void fsm_usart_init(fsm_t *p_this, uint32_t usart_id);

bool fsm_usart_check_data_received(fsm_t *p_this);

void fsm_usart_get_in_data(fsm_t *p_this, char *p_data);

void fsm_usart_set_out_data(fsm_t *p_this, char *p_data);

void fsm_usart_reset_input_data(fsm_t *p_this);

bool fsm_usart_check_activity(fsm_t *p_this);

void fsm_usart_disable_rx_interrupt(fsm_t *p_this);

void fsm_usart_disable_tx_interrupt(fsm_t *p_this);

void fsm_usart_enable_rx_interrupt(fsm_t *p_this);

void fsm_usart_enable_tx_interrupt(fsm_t *p_this);


#endif /* FSM_USART_H_ */
