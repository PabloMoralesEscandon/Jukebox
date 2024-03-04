/**
 * @file port_button.c
 * @brief File containing functions related to the HW of the button.
 *
 * This file defines an internal struct which contains the HW information of the button.
 *
 * @author Pablo Morales
 * @author Noel Solis
 * @date 12-2-2024
 */

/* Includes ------------------------------------------------------------------*/
#include "port_button.h"

/* Global variables ------------------------------------------------------------*/

port_button_hw_t buttons_arr[] = {
    [BUTTON_0_ID] = {.p_port = BUTTON_0_GPIO, .pin = BUTTON_0_PIN, .flag_pressed = false}
};

void port_button_init(uint32_t button_id){
    GPIO_TypeDef *p_port = buttons_arr[button_id].p_port;
    uint8_t pin = buttons_arr[button_id].pin;

    /* TO-DO alumnos */
    port_system_gpio_config(p_port, pin, GPIO_MODE_IN, GPIO_PUPDR_NOPULL);
    port_system_gpio_config_exti(p_port, pin, 0x0B);
    port_system_gpio_exti_enable(pin, 0x01, 0x00);
}

bool port_button_is_pressed(uint32_t button_id){
    return buttons_arr[button_id].flag_pressed;
}

uint32_t port_button_get_tick(){
    return port_system_get_millis();
}
