#include "main.h"
/******************************************************************************
;  *----------------------摘要描述---------------------------------
;  *   注:       本例程测试EXTI外部中断
;  *  	 PA4 EXTI输入, PA10 LED输出    
;  *     PA4 上升沿中断，输入一次翻转对应IO一次
******************************************************************************/
u32 KeyTime=0;

void cs32_gpio_init(void);
void cs_eval_led_toggle(void);
void delay_1ms(uint16_t TimeDelay);

#define  KEY_IN  __GPIO_INPUT_PIN_GET(GPIOA, GPIO_PIN_4)

bool keyflag;

int main(void)
{	
   cs32_gpio_init();
	
   while(1)
   {

   	  	   
	 if(KEY_IN != RESET)
    {
		delay_1ms(10);
		if(KEY_IN != RESET)
		{
	     if(!keyflag)
		 {
		   cs_eval_led_toggle();	 
		 }			 
	 	  keyflag=1;		
		}
    }
	else 
	{
	      keyflag=0;
		  pmu_deep_sleep_mode_enter(PMU_LDO_LOW_POWER, PMU_DSM_ENTRY_WFI);
	}
  
	   
   }
}
void cs32_gpio_init(void)
{
   nvic_config_t nvic_config_struct;
	
  __RCU_DEF_INIT();                            /*初始化外设时钟*/
  __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOA);    /*使能GPIOA时钟*/	
  __RCU_APB2_CLK_ENABLE(RCU_APB2_PERI_SYSCFG); /*使能SYSCFG时钟*/	
 
  syscfg_exti_line_config(SYSCFG_EXTI_PORT_PA, SYSCFG_EXTI_PIN_4); // Connect EXTI4 Line to PA4 pin	
  /*配置GPIOA_10 输出模式*/
  gpio_mode_set(GPIOA, GPIO_PIN_10, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));   
  /*配置GPIOA_10 输出低*/
  __GPIO_PIN_SET(GPIOA, GPIO_PIN_10);
  /*配置GPIOA_4 输入模式*/
  gpio_mode_set(GPIOA, GPIO_PIN_4, GPIO_MODE_IN_FLOAT);  
	/*配置外部中断 模式*/
  EXTI->INTEN |= 0x00000010;	  /*中断线Line4使能*/ 
  EXTI->EVTEN &= 0xffffffef;	  /*事件线Line4不使能*/ 
  EXTI->RTEN  |= 0x00000010;	  /*上升沿中断使能*/ 
  EXTI->SWTIEN|= 0x00000010;	  /*使能Line4软件触发中断/事件使能*/ 

  // Enable and set EXTI4_15 Interrupt
   nvic_config_struct.IRQn = IRQn_EXTI4_15;
   nvic_config_struct.priority = 0x00;
   nvic_config_struct.enable_flag = ENABLE;
   nvic_init(&nvic_config_struct);	
}
void cs_eval_led_toggle(void)
{	
	GPIOA->DO ^= GPIO_PIN_10;
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
/**
  * @fn void EXTI4_15_IRQHandler(void)
  * @brief  This function handles External lines 4 to 15 interrupt request.
  * @param  None
  * @return None
  */
void EXTI4_15_IRQHandler(void)
{
    if(__EXTI_FLAG_STATUS_GET(EXTI_LINE_4) != RESET)
    {
        /* Clear the EXTI line 4  bit */
        __EXTI_FLAG_CLEAR(EXTI_LINE_4);
    }
}




