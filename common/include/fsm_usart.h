/**
 * @file fsm_usart.h
 * @brief Header for fsm_usart.c file.
 * @author Pablo Morales
 * @author Noel Solís
 * @date 26-2-2024
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
/// @brief Enumerates the UART FSM states
enum FSM_USART{
    WAIT_DATA = 0, /*!< Initial state. Data has been sent or read */
    SEND_DATA      /*!< Sending data */
};

/* Typedefs --------------------------------------------------------------------*/
/// @brief Structure that defines a USART FSM
typedef struct{
    fsm_t f;                                        /*!< FSM for the UART */
    bool data_received;                             /*!< Flag to indicate data has been received */
    char in_data [USART_INPUT_BUFFER_LENGTH];       /*!< Input data */
    char out_data [USART_OUTPUT_BUFFER_LENGTH];     /*!< Output data */
    uint8_t usart_id;                               /*!< UASRT identifier */

} fsm_usart_t;

/* Function prototypes and explanation -------------------------------------------------*/

/// @brief Creates a new UART FSM
/// @param usart_id Unique identifier for the UART
/// @return Pointer to the new UART FSM
fsm_t * fsm_usart_new(uint32_t usart_id);

/// @brief Iniatializes UART FSM
/// @param p_this Pointer to an fsm struct that corresponds to an UART
/// @param usart_id Unique identifier for the UART
void fsm_usart_init(fsm_t *p_this, uint32_t usart_id);

/// @brief Check if data has been received
/// @param p_this Pointer to an fsm struct that corresponds to an UART
/// @return True if data has been received, false if not
bool fsm_usart_check_data_received(fsm_t *p_this);

/// @brief Get received data
/// @param p_this Pointer to an fsm struct that corresponds to an UART
/// @param p_data Pointer to which the data will be copied
void fsm_usart_get_in_data(fsm_t *p_this, char *p_data);

/// @brief Copies data to he buffer
/// @param p_this Pointer to an fsm struct that corresponds to an UART
/// @param p_data Pointer to the data
void fsm_usart_set_out_data(fsm_t *p_this, char *p_data);

/// @brief Resets the input buffer
/// @param p_this Pointer to an fsm struct that corresponds to an UART
void fsm_usart_reset_input_data(fsm_t *p_this);

/// @brief Check wether the UART is active or not
/// @param p_this Pointer to an fsm struct that corresponds to an UART
/// @return true if active, flase if not
bool fsm_usart_check_activity(fsm_t *p_this);

/// @brief Disable RX interruptions of the UART
/// @param p_this Pointer to an fsm struct that corresponds to an UART
void fsm_usart_disable_rx_interrupt(fsm_t *p_this);

/// @brief Disable TX interruptions for the UART
/// @param p_this Pointer to an fsm struct that corresponds to an UART
void fsm_usart_disable_tx_interrupt(fsm_t *p_this);

/// @brief Enable RX interruptions for the UART
/// @param p_this Pointer to an fsm struct that corresponds to an UART
void fsm_usart_enable_rx_interrupt(fsm_t *p_this);

/// @brief Enable TX interruptions for the UART
/// @param p_this Pointer to an fsm struct that corresponds to an UART
void fsm_usart_enable_tx_interrupt(fsm_t *p_this);


#endif /* FSM_USART_H_ */
