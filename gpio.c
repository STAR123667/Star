#include "gpio.h"
bool led_flag;
void cs32_gpio_init(void)
{
  __RCU_DEF_INIT();                            /*初始化外设时钟*/

  __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOA);    /*使能GPIOA时钟*/
  __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOB);    /*使能GPIOB时钟*/
  __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOC);    /*使能GPIOC时钟*/
  __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOF);    /*使能GPIOF时钟*/

  /*配置GPIOA GPIOB输出模式*/
  gpio_mode_set(GPIOA, GPIO_PIN_8, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));   
  gpio_mode_set(GPIOA, GPIO_PIN_9, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));  
  gpio_mode_set(GPIOA, GPIO_PIN_10, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));  

  gpio_mode_set(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));  
  gpio_mode_set(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));  
  gpio_mode_set(GPIOB, GPIO_PIN_8, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));  

  gpio_mode_set(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));  
  /*配置GPIOA_4 输入模式*/
  gpio_mode_set(GPIOA, GPIO_PIN_4, GPIO_MODE_IN_PD);  

  /*配置GPIOA B输出低*/
  __GPIO_PIN_SET(GPIOA, GPIO_PIN_8);
  __GPIO_PIN_SET(GPIOA, GPIO_PIN_9);
  __GPIO_PIN_SET(GPIOA, GPIO_PIN_10);
  __GPIO_PIN_SET(GPIOB, GPIO_PIN_4);
  __GPIO_PIN_SET(GPIOB, GPIO_PIN_5);
  __GPIO_PIN_SET(GPIOB, GPIO_PIN_8);
  __GPIO_PIN_SET(GPIOA, GPIO_PIN_3);
}
void cs32_gpio_exti_init(void)
{
    nvic_config_t nvic_config_struct;

    __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOA);    /*使能GPIOA时钟*/  
    __RCU_APB2_CLK_ENABLE(RCU_APB2_PERI_SYSCFG); // Enable SYSCFG clock

    gpio_mode_set(GPIOA, GPIO_PIN_4, GPIO_MODE_IN_PD);      //GPIOA PIN4 Down Input//
    syscfg_exti_line_config(SYSCFG_EXTI_PORT_PA, SYSCFG_EXTI_PIN_4); // Connect EXTI13 Line to PA4 pin

    // Configure EXTI4 line RISING//
    __EXTI_INTR_ENABLE(EXTI_LINE_4);                          // 使能LINE4//
    __EXTI_EDGE_ENABLE(EXTI_EDGE_RISING, EXTI_LINE_4);        // 上升沿触发中断//

    // Enable and set EXTI4_15 Interrupt
    nvic_config_struct.IRQn = IRQn_EXTI4_15;
    nvic_config_struct.priority = 0x00;
    nvic_config_struct.enable_flag = ENABLE;
    nvic_init(&nvic_config_struct);
}
/* GPIO 引脚输出测试*/
void cs32_gpio_output_test(void)
{
      GPIOA_8_High;
      GPIOA_9_High;
      GPIOA_10_High;
      GPIOB_4_High;
      GPIOB_5_High;
      GPIOB_8_High;

      delay_1ms(1000);
	
      GPIOA_8_Low;
      GPIOA_9_Low;
      GPIOA_10_Low;
      GPIOB_4_Low;
      GPIOB_5_Low;
      GPIOB_8_Low;

      delay_1ms(1000);
}
/* GPIO 引脚输入测试*/
void cs32_gpio_input_test(void)
{
     bool key_flag;

     if(GPIOA_4_In!=0)
     { 
        delay_1ms(10); 
       if(GPIOA_4_In!=0)
       {     
        if(!key_flag)
        {
          if(!led_flag)
        { 
            GPIOA->DO &= 0xfffffeff;
            led_flag=1;
        }
        else 
        {
            GPIOA->DO |= 0x00000100; 
            led_flag=0;
        }
        } 
             key_flag =1; 
       }
     }
     else 
     {  
             key_flag =0; 
     }
}
/**********  软件普通延时  **********/
/**
  * @fn void delay_1ms(uint16_t TimeDelay)
  * @brief  Simple delay 1ms      
  * @param  None
  * @return None
  */
void delay_1ms(uint16_t TimeDelay)
{
    int i,j;
    for(i=0;i<TimeDelay*35;i++)
    {
        for(j=0;j<38;j++)
        __NOP();
    }
}


