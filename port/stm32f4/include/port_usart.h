/**
 * @file port_usart.h
 * @brief Header for port_usart.c file.
 * @author Pablo Morales
 * @author Noel Solis
 * @date 04-3-2024
*/
#ifndef PORT_USART_H_
#define PORT_USART_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* HW dependent includes */
#include "stm32f4xx.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
/// @brief USART 0 identifier
#define USART_0_ID 0

/// @brief USART 0 used
#define USART_0 USART3

/// @brief USART 0 GPIO port for TX
#define USART_0_GPIO_TX GPIOB

/// @brief USART 0 GPIO port for RX
#define USART_0_GPIO_RX GPIOC

/// @brief USART 0 pin for TX
#define USART_0_PIN_TX 10

/// @brief USART 0 pin for RX 
#define USART_0_PIN_RX 11

/// @brief USART 0 alternate function for TX
#define USART_0_AF_TX 7

/// @brief USART 0 alternate function for RX
#define USART_0_AF_RX 7
 
/// @brief USART input data length
#define USART_INPUT_BUFFER_LENGTH 10

/// @brief USART output data length
#define USART_OUTPUT_BUFFER_LENGTH 100
 
/// @brief Char that indicates buffer is empty
#define EMPTY_BUFFER_CONSTANT 0x0

/// @brief Char that indicates data ends
#define END_CHAR_CONSTANT 0xA

/* Typedefs --------------------------------------------------------------------*/
/// @brief Structure that defines HW of a UART
typedef struct{
    USART_TypeDef* p_usart;                             /*!< Pointer to USART struct */
    GPIO_TypeDef* p_port_tx;                            /*!< TX GPIO port */
    GPIO_TypeDef* p_port_rx;                            /*!< RX GPIO port */
    uint8_t pin_tx;                                     /*!< TX pin */
    uint8_t pin_rx;                                     /*!< RX pin */
    uint8_t alt_func_tx;                                /*!< Alternate function for the TX pin */
    uint8_t alt_func_rx;                                /*!< Alternate function for the RX pin */
    char input_buffer [USART_INPUT_BUFFER_LENGTH];      /*!< Input buffer */
    uint8_t i_idx;                                      /*!< Input buffer index */
    bool read_complete;                                 /*!< Flag to indicate if read is complete */
    char output_buffer [USART_OUTPUT_BUFFER_LENGTH];    /*!< Output buffer */
    uint8_t o_idx;                                      /*!< Output buffer index  */
    bool write_complete;                                /*!< Flag to indicate if write is complete */
} port_usart_hw_t;

/* Global variables */
/// @brief Array of UART elements
extern port_usart_hw_t usart_arr[];

/* Function prototypes and explanation -------------------------------------------------*/

/// @brief Initializes USART
/// @param usart_id USART identifier
void port_usart_init(uint32_t usart_id);

/// @brief Checks if TX has ended
/// @param usart_id USART identifier
/// @return True if TX han ended, false if not
bool port_usart_tx_done(uint32_t usart_id);

/// @brief Chec if RX has ended
/// @param usart_id USART identifier
/// @return True if RX has ended, false if not
bool port_usart_rx_done(uint32_t usart_id);

/// @brief Copies data in the input buffer
/// @param usart_id USART identifier
/// @param p_buffer Pointer to where data will be copied
void port_usart_get_from_input_buffer(uint32_t usart_id, char *p_buffer);

/// @brief Checks if USART can recieve data
/// @param usart_id USART identifier
/// @return TXE flag
bool port_usart_get_txr_status(uint32_t usart_id);

/// @brief Copy data to output buffer
/// @param usart_id USART identifier
/// @param p_data Pointer to he data
/// @param length Length of the data
void port_usart_copy_to_output_buffer(uint32_t usart_id, char *p_data, uint32_t length);

/// @brief Resets input buffer
/// @param usart_id USART identifier
void port_usart_reset_input_buffer(uint32_t usart_id);

/// @brief Resets output buffer
/// @param usart_id USART identifier
void port_usart_reset_output_buffer(uint32_t usart_id);

/// @brief Reads data from data register and stores it in input buffer
/// @param usart_id USART identifier
void port_usart_store_data(uint32_t usart_id);

/// @brief Writes data from output buffer to the data register
/// @param usart_id USART identifier
void port_usart_write_data(uint32_t usart_id);

/// @brief Disables USART RX interrupts
/// @param usart_id USART identifier
void port_usart_disable_rx_interrupt(uint32_t usart_id);

/// @brief Disables USART TX interrupts
/// @param usart_id USART identifier
void port_usart_disable_tx_interrupt(uint32_t usart_id);

/// @brief Enables USART RX interrupts
/// @param usart_id USART identifier
void port_usart_enable_rx_interrupt(uint32_t usart_id);

/// @brief Enables USART TX interrupts
/// @param usart_id USART identifier
void port_usart_enable_tx_interrupt(uint32_t usart_id);

#endif