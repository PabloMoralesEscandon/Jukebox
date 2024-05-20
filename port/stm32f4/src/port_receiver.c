/**
 * @file port_receiver.c
 * @brief File containing functions related to the HW of the receiver.
 *
 * This file defines an internal struct which contains the HW information of the receiver.
 *
 * @author Pablo Morales
 * @author Noel Solis
 * @date 12-2-2024
 */

/* Includes ------------------------------------------------------------------*/
#include "port_receiver.h"

/* Global variables ------------------------------------------------------------*/

port_receiver_hw_t receivers_arr[] = {
    [receiver_0_ID] = {.p_port = receiver_0_GPIO, .pin = receiver_0_PIN, .flag_pressed = false}
};

void port_receiver_init(uint32_t receiver_id){
    GPIO_TypeDef *p_port = receivers_arr[receiver_id].p_port;
    uint8_t pin = receivers_arr[receiver_id].pin;
    port_system_gpio_config(p_port, pin, GPIO_MODE_IN, GPIO_PUPDR_NOPULL);
    port_system_gpio_config_exti(p_port, pin, 0x0B);
    port_system_gpio_exti_enable(pin, 0x01, 0x00);
}

bool port_receiver_is_pressed(uint32_t receiver_id){
    return receivers_arr[receiver_id].flag_pressed;
}