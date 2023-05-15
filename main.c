#include "main.h"
/******************************************************************************
;  *----------------------摘要描述---------------------------------
;  *   注:      本例程测试EXTI外部事件唤醒SLEEP
;  *  	 PA4 EXTI外部事件输入, PA10 输出    
;  *     PA4 上升沿触发事件,唤醒SLEEP,翻转对应IO一次
******************************************************************************/
u32 KeyTime=0;
bool keyflag;
#define KEY_IN __GPIO_INPUT_PIN_GET(GPIOA, GPIO_PIN_4)

void cs32_gpio_init(void);
void cs_eval_led_toggle(void);
void delay_1ms(uint16_t TimeDelay);

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
		    pmu_deep_sleep_mode_enter(PMU_LDO_LOW_POWER, PMU_DSM_ENTRY_WFE);
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
  EXTI->INTEN &= 0xffffffef;	  /*中断线Line4不使能*/ 
  EXTI->EVTEN |= 0x00000010;	  /*事件线Line4使能*/ 
  EXTI->RTEN  |= 0x00000010;	  /*上升沿中断使能*/ 
  EXTI->SWTIEN|= 0x00000010;	  /*使能Line4软件触发中断/事件使能*/ 
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



