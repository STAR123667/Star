#include "main.h"
uint32_t KeyTime;  //保存KEY按下时间
uint16_t KeyCount; //记录KEY按下次数
uint16_t VBat_Start; //电池状态次数
uint16_t TimeCount3s;//时间计数
extern bool Led_Flag;      //LED标志//
extern bool Phase_Flag;    //相序标志//
extern uint8_t Phase_Start;//相序状态//
extern bool PhaseLine_Flag;
struct Flag
{ 
   uint8_t c:1;
   uint8_t Power_Run:1;
}Key_flag;		
void Get_Phase(void);
int main()
{	
   COMP_Cmd(COMP_Selection_COMP1, DISABLE); 
   COMP_Cmd(COMP_Selection_COMP2, DISABLE);		
   PWR_PVDCmd(DISABLE);	
   PWR_BackupAccessCmd(DISABLE);	
   RTC->CR = RTC->CR & 0xffffff7f;	
   RCC_LSICmd(DISABLE);
   while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) !=RESET);	
	
   Power_En_Init();       //电源引脚初始化
   Buzzer_Init();         //蜂鸣器引脚初始化
   Vbat_Init();           //电池采样引脚初始化
   Time14_Init();   	  //定时器14 500US刷新A B 引脚初始化
   Time17_Init();	      //定时器17 参考信号引脚初始化
   Time6_Init();          //定时器6  计时1MS
   Time3_Init();	      //定时器3  计时100US 
   Sensor_Init();         //运放输入引脚初始化 	
	
   VBat_Start = ADC_Get_VBat();	        //获取电池电压2000次，使开机电池处于稳定状态。	
   // IWDG_Config(IWDG_Prescaler_64,909);  //看门狗配置3S  	
   
   while(1)
   {   
           // 喂狗
             // IWDG_Feed();

            if(!Phase_Flag)
			{   
		      Scan_Key();        //按键输入	  
			}       
	        else 
			{  
		      Get_Phase();	     
			}
	          Get_Battery();     //电池电压获取,低于6V报警
   }
}
void Scan_Key(void)
{
	      uint8_t j,i;	
	  	
          if( Power_En_In != Bit_SET)	  
		  {
            Key_flag.c =0;			
		  }   
  
          if( Power_En_In == Bit_SET && Key_flag.c == 0 )	 
	      {		  
            Delay_ms(100);  
	      if( Power_En_In == Bit_SET && Key_flag.c == 0)	  
	      {          
           Key_flag.c =1;
		  }
		  }		  
		  if(Key_flag.c !=0)
		  { 
		   Power_En_H;    /* 电源上电 */	
		   Delay_ms(10); 
		   KeyTime++;   
		  }
          
		   if(Key_flag.c !=1)
		  {
//			IWDG_Feed();  		
		   if(KeyTime >1 && KeyTime <=100)  
		  {
			  
		  if(!PhaseLine_Flag)
		  {		
		    KeyCount = KeyCount+1;
		    switch (KeyCount)
            {
          	case 1:
			{ 
			  Power_En_H;    /* 电源上电 */	           			  
			 if(Key_flag.Power_Run==0)
			 {		  
			   for(j=0; j<200; j++)
               {
	            TIM_Cmd(TIM15, ENABLE);
                TIM_CtrlPWMOutputs(TIM15, ENABLE);  
                Delay_ms(1);
               }	
                TIM_Cmd(TIM15, DISABLE);
                TIM_CtrlPWMOutputs(TIM15, DISABLE); 
			 
                Key_flag.Power_Run =1;		   
                Led_Flag =1;
                Led_Out_Low();		
			    Delay_ms(1000);
			    Led_Out_Close();
				// IWDG_Feed();  
			     for(i=1;i<=5;i++)
			     {
				   // IWDG_Feed();  
    			   PowerUpStart1(i);   
				   Delay_ms(400); 
                   Get_Battery(); 	
			     }
                  TIM_Cmd(TIM6, ENABLE); 
			  }		     
			  Led_Flag =1;
			  Led_Out_Low();			 
			  KeyTime = 0;	  			         
              TimeCount3s = 0; 		
              // TIM_Cmd(TIM6, ENABLE);   			  
			  TIM_Cmd(TIM14, DISABLE); 			  
			 } 
              break;
          	case 2:
			{ 
			  Power_En_H;    /* 电源上电 */	  
			  Led_Out_Hight(); 
			  KeyCount =0; 
			  KeyTime =0;		
			  TimeCount3s =0;
			  // TIM_Cmd(TIM6, ENABLE);   
              TIM_Cmd(TIM14, DISABLE); 
		    }
          		break;
          	default:
          		break;
            }
		 }
			else 
			{ 
			  KeyTime=0; 
			  KeyCount=0;
		      return;
			}
	   }			
		   else if(KeyTime > 100) 
		   {  
	       Led_Out_Close(); 
		   TIM_Cmd(TIM6, DISABLE);  
           TIM_Cmd(TIM14, DISABLE); 		   
		   KeyTime =0;	  
		   KeyCount=0; 
		   Key_flag.Power_Run =0;			  
		   TimeCount3s =0;	
		   Power_En_L;
		   }		   
           else 
           {  
	      			
            if(TIM_GetFlagStatus(TIM6, TIM_FLAG_Update) == SET)  
	        {
			    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
			    TimeCount3s ++; 
                if(TimeCount3s == 1000) 		          //1秒除电源指示灯亮，其他不亮
                { 	
                Led_Out_Close();						
                Power_Led_Open;
				TimeCount3s=0;
                Time3_Init();
                TIM_Cmd(TIM6, DISABLE);  
				TIM_Cmd(TIM14, ENABLE);     //定时器14 使能。A B间隔扫描	   		   
				Led_Flag=0;  	
				
					
                RCC_HSICmd(ENABLE);
				while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) !=SET);
                /* Enable Prefetch Buffer and set Flash Latency */
		        FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;
		        /* HCLK = SYSCLK */
		        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
		        /* PCLK = HCLK */
		        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;
		        // PLL configuration = (HSI/2) * 12 = 48 MHz
		        RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_2); // 8M/2 * 2 = 8M
		        /* Enable PLL */
		        RCC->CR |= RCC_CR_PLLON;
		        /* Wait till PLL is ready */
		        while ((RCC->CR & RCC_CR_PLLRDY) == 0)
		        {
		        }
		        /* Select PLL as system clock source */
		        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // PLL 做系统时钟
		        /* Wait till PLL is used as system clock source */
		        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
		        {
		        }	
			  			
				PWR_EnterSleepMode(PWR_SLEEPEntry_WFI);	   //进入休眠//
						
			    RCC_HSICmd(ENABLE);			
				while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) !=SET);
				/* Enable Prefetch Buffer and set Flash Latency */
		        FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;
		        /* HCLK = SYSCLK */
		        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
		        /* PCLK = HCLK */
		        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;
		        // PLL configuration = (HSI/2) * 12 = 48 MHz
		        RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12); // 8M/2 * 12 = 48M
		        /* Enable PLL */
		        RCC->CR |= RCC_CR_PLLON;
		        /* Wait till PLL is ready */
		        while ((RCC->CR & RCC_CR_PLLRDY) == 0)
		        {
		        }
		        /* Select PLL as system clock source */
		        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // PLL 做系统时钟
		        /* Wait till PLL is used as system clock source */
		        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
		        {
		        }	

	            TIM_Cmd(TIM6, ENABLE);	
				
		        }	  
 			} 	
            else 
		    {   
			    return;
			}		
	
           }   
		   
		}
}
void Get_Battery(void)
{
          VBat_Start = ADC_Get_VBat();    //获取电池电压   

		  if(VBat_Start<2000)   
         { 
	        Led_Out_Close();
	        for(;1;) 
			{ 
		     // 喂狗
            // IWDG_Feed();

		     Power_Led_Close;
			 Delay_ms(150);  
           	 Power_Led_Open;
             Delay_ms(150);	 
				
			 VBat_Start = ADC_Get_VBat();
				
             if(VBat_Start>=2000)            //电池电压正常返回,6.5V以上
			 {
			   break;
			 }
      		}
	      }                            //是1 电池电压低于7V报警处理 
          else 
          {
		    return;
		  }			               //是0 电池电压正常	
}
void IWDG_Config(uint8_t prv ,uint16_t rlv)
{	
	// 使能 预分频寄存器PR和重装载寄存器RLR可写
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	// 设置预分频器值
	IWDG_SetPrescaler(prv);
	// 设置重装载寄存器值
	IWDG_SetReload(rlv);
	// 把重装载寄存器的值放到计数器中
	IWDG_ReloadCounter();
	// 使能 IWDG
	IWDG_Enable();	
}
// 喂狗
void IWDG_Feed(void)
{
	// 把重装载寄存器的值放到计数器中，喂狗，防止IWDG复位
	// 当计数器的值减到0的时候会产生系统复位
	IWDG_ReloadCounter();
}
void Get_Phase(void)
{
      	    IWDG_Feed();
  		   	if(Phase_Start == 0x01)           /* 正序 */		
			{			
					BuzzerOpen_Speen(0x02);   /* 开蜂鸣器 */		
			}
			else if(Phase_Start == 0x02)      /* 负序 */		
			{
					BuzzerOpen_Speen(0x01);   /* 开蜂鸣器 */
			}
			else 
			{
				    BuzzerClose_Speen();      /* 关闭蜂鸣器 */
			}		
}



