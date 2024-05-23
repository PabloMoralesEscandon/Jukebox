/**
 * @file main.c
 * @brief Main file.
 * @author Sistemas Digitales II
 * @date 2023-10-01
 */


/* Includes ------------------------------------------------------------------*/
/* Standard C libraries */
#include <stdio.h> // printf

#include <string.h>

/* HW libraries */

#include "port_system.h"

#include "fsm_button.h"

#include "port_button.h"

#include "fsm_usart.h"

#include "port_usart.h"

#include "fsm_buzzer.h"

#include "port_buzzer.h"

#include "melodies.h"

#include "fsm_jukebox.h"

#include "fsm_nec.h"

#include "port_nec.h"

#include "port_lcd.h"

/* Defines ------------------------------------------------------------------*/

#define ON_OFF_PRESS_TIME_MS 1000

#define NEXT_SONG_BUTTON_TIME_MS 500

/*-----------------------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;

// void SystemClock_Config(void);

void Error_Handler(void);

static void MX_I2C1_Init(void);

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* Init board */
    port_system_init();

    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    // SystemClock_Config();

    // /* USER CODE BEGIN SysInit */

    // /* USER CODE END SysInit */

    // /* Initialize all configured peripherals */
    MX_I2C1_Init();
    /* USER CODE BEGIN 2 */
    HD44780_Init(2);
    HD44780_Clear();
    HD44780_SetCursor(0,0);
    HD44780_PrintStr("JUKEBOX :)");

    fsm_t* p_fsm_user_button = fsm_button_new(BUTTON_0_DEBOUNCE_TIME_MS, BUTTON_0_ID);

    fsm_t* p_fsm_user_usart = fsm_usart_new(USART_0_ID);

    fsm_t* p_fsm_user_buzzer = fsm_buzzer_new(BUZZER_0_ID);

    fsm_t* p_fsm_user_jukebox = fsm_jukebox_new(p_fsm_user_button, ON_OFF_PRESS_TIME_MS, p_fsm_user_usart, p_fsm_user_buzzer, NEXT_SONG_BUTTON_TIME_MS);

    // fsm_t* p_fsm_user_NEC = fsm_NEC_new(NEC_0_ID);
    // fsm_NEC_t *p_fsm = (fsm_NEC_t *)(p_fsm_user_NEC);
    // int message;

    /* Infinite loop */
    while (1)
    {
        // fsm_fire(p_fsm_user_NEC);
        fsm_fire(p_fsm_user_button);
        fsm_fire(p_fsm_user_usart);
        fsm_fire(p_fsm_user_buzzer);
        fsm_fire(p_fsm_user_jukebox);
        // if(p_fsm->decoded){
        //     message = (int)fsm_NEC_get_message(p_fsm_user_NEC);
        //     printf("%d\n", message);
        // }

    } // End of while(1)
    // Nunca deberíamos llegar aquí
    fsm_destroy(p_fsm_user_button);
    fsm_destroy(p_fsm_user_usart);
    fsm_destroy(p_fsm_user_buzzer);
    fsm_destroy(p_fsm_user_jukebox);
    
    return 0;
}

// void SystemClock_Config(void){
//   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

//   /** Configure the main internal regulator output voltage
//   */
//   __HAL_RCC_PWR_CLK_ENABLE();
//   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

//   /** Initializes the RCC Oscillators according to the specified parameters
//   * in the RCC_OscInitTypeDef structure.
//   */
//   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//   RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//   RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
//   RCC_OscInitStruct.PLL.PLLM = 16;
//   RCC_OscInitStruct.PLL.PLLN = 336;
//   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
//   RCC_OscInitStruct.PLL.PLLQ = 2;
//   RCC_OscInitStruct.PLL.PLLR = 2;
//   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//   {
//     Error_Handler();
//   }

//   /** Initializes the CPU, AHB and APB buses clocks
//   */
//   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                               |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
//   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

//   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
//   {
//     Error_Handler();
//   }
// }

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif
