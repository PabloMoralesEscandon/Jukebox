/**
 * @file port_usart.c
 * @brief Portable functions to interact with the USART FSM library.
 * @author Pablo Morales
 * @author Noel Solis
 * @date 04-3-2024
*/
/* Includes ------------------------------------------------------------------*/
/* Standard C libraries */
#include <string.h>
#include <stdlib.h>

/* HW dependent libraries */
#include "port_system.h"
#include "port_usart.h"

/* Global variables */

port_usart_hw_t usart_arr[] = {
    [USART_0_ID] = {
        .p_usart = USART_0,
        .p_port_tx = USART_0_GPIO_TX,
        .p_port_rx = USART_0_GPIO_RX,
        .pin_tx = USART_0_PIN_TX,
        .pin_rx = USART_0_PIN_RX,
        .alt_func_tx = USART_0_AF_TX,
        .alt_func_rx = USART_0_AF_RX,
        .i_idx = 0,
        .read_complete = false,
        .o_idx = 0,
        .write_complete = false
    }
    
};

/* Private functions */

/// @brief Resets buffer
/// @param buffer Pointer to buffer
/// @param length Buffer length
void _reset_buffer(char* buffer, uint32_t length){
    memset(buffer, EMPTY_BUFFER_CONSTANT, length);
}

/* Public functions */


void port_usart_init(uint32_t usart_id)
{
    USART_TypeDef *p_usart = usart_arr[usart_id].p_usart;
    GPIO_TypeDef *p_port_tx = usart_arr[usart_id].p_port_tx;
    GPIO_TypeDef *p_port_rx = usart_arr[usart_id].p_port_rx;
    uint8_t pin_tx = usart_arr[usart_id].pin_tx;
    uint8_t pin_rx = usart_arr[usart_id].pin_rx;
    uint8_t alt_func_tx = usart_arr[usart_id].alt_func_tx;
    uint8_t alt_func_rx = usart_arr[usart_id].alt_func_rx;

    // Configuration of tx and rx GPIO
    port_system_gpio_config(p_port_tx, pin_tx, GPIO_MODE_ALTERNATE, GPIO_PUPDR_PUP); 
    port_system_gpio_config(p_port_rx, pin_rx, GPIO_MODE_ALTERNATE, GPIO_PUPDR_PUP); 
    port_system_gpio_config_alternate(p_port_tx, pin_tx, alt_func_tx);
    port_system_gpio_config_alternate(p_port_rx, pin_rx, alt_func_rx);

    // Enable USART clock
    if (p_usart == USART3) RCC -> APB1ENR |= RCC_APB1ENR_USART3EN;
    if (p_usart == USART1) RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;
    if (p_usart == USART1) RCC -> APB2ENR |= RCC_APB2ENR_USART6EN;  
    
    // Disable USART
    p_usart -> CR1 &= ~USART_CR1_UE;

    // Set data length to 8 bits
    p_usart -> CR1 &= ~USART_CR1_M;

    // Set stop bits to 1 bit
    p_usart -> CR2 &= ~USART_CR2_STOP;

    // Set parity bit to no parity
    p_usart -> CR1 &= ~USART_CR1_PCE;

    // Set oversampling to 16
    p_usart -> CR1 &= ~USART_CR1_OVER8;

    /*
        Set Baudrate to 9600:
        USARTDIV = 16MHz / 8x(2-0)x9600 = 0d104.166
        0d104 = 0x68
        0d.166 = 0x2
        USARTDIV = 0x0682
    */
    p_usart -> BRR = 0x0683;

    // Enable tx and rx
    p_usart -> CR1 = USART_CR1_TE | USART_CR1_RE;

    // Disable rx interrupts
    port_usart_disable_rx_interrupt(usart_id);

    // Disable tx interrupts
    port_usart_disable_tx_interrupt(usart_id);

    // Clear rx interrupt flags
    p_usart -> SR &= ~USART_SR_RXNE;

    // Clear tx interrupt flags
    p_usart -> SR &= ~USART_SR_TXE;

    // Enable USART interrupts globally
    if (p_usart == USART3){
        NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0));
        NVIC_EnableIRQ(USART3_IRQn);
    }
    if (p_usart == USART1){
        NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0));
        NVIC_EnableIRQ(USART1_IRQn);
    }
    if (p_usart == USART6){
        NVIC_SetPriority(USART6_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 1));
        NVIC_EnableIRQ(USART6_IRQn);
    }

    // Enable the USART
    p_usart -> CR1 |= USART_CR1_UE;

    // Clear buffers
    _reset_buffer(usart_arr[usart_id].output_buffer, USART_OUTPUT_BUFFER_LENGTH);
    _reset_buffer(usart_arr[usart_id].input_buffer, USART_INPUT_BUFFER_LENGTH);

}

void port_usart_get_from_input_buffer(uint32_t usart_id, char* p_buffer){
    memcpy(p_buffer, usart_arr[usart_id].input_buffer, USART_INPUT_BUFFER_LENGTH);
}

bool port_usart_get_txr_status(uint32_t usart_id){
    return ((usart_arr[usart_id].p_usart -> SR) & USART_SR_TXE);
}

void port_usart_copy_to_output_buffer(uint32_t usart_id, char *p_data, uint32_t length){
    memcpy(usart_arr[usart_id].output_buffer, p_data, USART_OUTPUT_BUFFER_LENGTH);
}

void port_usart_reset_input_buffer(uint32_t usart_id){
    _reset_buffer(usart_arr[usart_id].input_buffer, USART_INPUT_BUFFER_LENGTH);
    usart_arr[usart_id].read_complete = false;
}

void port_usart_reset_output_buffer(uint32_t usart_id){
    _reset_buffer(usart_arr[usart_id].output_buffer, USART_OUTPUT_BUFFER_LENGTH);
    usart_arr[usart_id].write_complete = false;
}

bool port_usart_rx_done(uint32_t usart_id){
    return usart_arr[usart_id].read_complete;
}

bool port_usart_tx_done(uint32_t usart_id){
    return usart_arr[usart_id].write_complete;
}

void port_usart_store_data(uint32_t usart_id){
    char data = usart_arr[usart_id].p_usart -> DR;
    if (data != END_CHAR_CONSTANT){
        if(usart_arr[usart_id].i_idx >= USART_INPUT_BUFFER_LENGTH){
            usart_arr[usart_id].i_idx = 0;
        }
        usart_arr[usart_id].input_buffer[usart_arr[usart_id].i_idx] = data;
        usart_arr[usart_id].i_idx += 1;
    } else{
        usart_arr[usart_id].read_complete = true;
        usart_arr[usart_id].i_idx = 0;
    }
}

void port_usart_write_data(uint32_t usart_id){
    if ((usart_arr[usart_id].o_idx == USART_OUTPUT_BUFFER_LENGTH - 1) || (usart_arr[usart_id].output_buffer[usart_arr[usart_id].o_idx] == END_CHAR_CONSTANT)){
        usart_arr[usart_id].p_usart -> DR = usart_arr[usart_id].output_buffer[usart_arr[usart_id].o_idx];
        port_usart_disable_tx_interrupt(usart_id);
        usart_arr[usart_id].o_idx = 0;
        usart_arr[usart_id].write_complete = true;
    } else if(usart_arr[usart_id].output_buffer[usart_arr[usart_id].o_idx] != EMPTY_BUFFER_CONSTANT){
        usart_arr[usart_id].p_usart -> DR = usart_arr[usart_id].output_buffer[usart_arr[usart_id].o_idx];
        usart_arr[usart_id].o_idx += 1;
    }
}

void port_usart_disable_rx_interrupt(uint32_t usart_id){
    usart_arr[usart_id].p_usart -> CR1 &= ~USART_CR1_RXNEIE;
}

void port_usart_disable_tx_interrupt(uint32_t usart_id){
    usart_arr[usart_id].p_usart -> CR1 &= ~(USART_CR1_TXEIE | USART_CR1_TCIE);
}

void port_usart_enable_rx_interrupt(uint32_t usart_id){
    usart_arr[usart_id].p_usart -> CR1 |= USART_CR1_RXNEIE;
}

void port_usart_enable_tx_interrupt(uint32_t usart_id){
    usart_arr[usart_id].p_usart -> CR1 |= (USART_CR1_TXEIE | USART_CR1_TCIE);
}