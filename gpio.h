/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H
#define __GPIO_H
/* Includes ------------------------------------------------------------------*/
#include "cs32f0xx.h"
#include "main.h"
/* Exported define ------------------------------------------------------------*/ 
#define GPIOA_8_High   __GPIO_PIN_SET(GPIOA, GPIO_PIN_8)
#define GPIOA_8_Low    __GPIO_PIN_RESET(GPIOA, GPIO_PIN_8)  
#define GPIOA_9_High   __GPIO_PIN_SET(GPIOA, GPIO_PIN_9)
#define GPIOA_9_Low    __GPIO_PIN_RESET(GPIOA, GPIO_PIN_9) 
#define GPIOA_10_High   __GPIO_PIN_SET(GPIOA, GPIO_PIN_10)   
#define GPIOA_10_Low    __GPIO_PIN_RESET(GPIOA, GPIO_PIN_10)

#define GPIOB_4_High   __GPIO_PIN_SET(GPIOB, GPIO_PIN_4)
#define GPIOB_4_Low    __GPIO_PIN_RESET(GPIOB, GPIO_PIN_4)  
#define GPIOB_5_High   __GPIO_PIN_SET(GPIOB, GPIO_PIN_5)  
#define GPIOB_5_Low    __GPIO_PIN_RESET(GPIOB, GPIO_PIN_5) 
#define GPIOB_8_High   __GPIO_PIN_SET(GPIOB, GPIO_PIN_8)  
#define GPIOB_8_Low    __GPIO_PIN_RESET(GPIOB, GPIO_PIN_8)

#define GPIOA_4_In     __GPIO_INPUT_PIN_GET(GPIOA, GPIO_PIN_4)
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void cs32_gpio_init(void);
void cs32_gpio_exti_init(void);
void cs32_gpio_output_test(void);
void cs32_gpio_input_test(void);
void delay_1ms(uint16_t TimeDelay);
#endif /* __GPIO_H */