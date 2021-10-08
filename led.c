#include "led.h"
#include "stm8s_tim2.h"
#include "stm8s_gpio.h"
void led_init(void)
{
  
  GPIO_Init(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(LED_SW_PORT, (GPIO_Pin_TypeDef)LED_SW_PIN, GPIO_MODE_OUT_PP_LOW_FAST);//LED_SW  GPIO��ʼ��
  GPIO_Init(KEY_PORT, (GPIO_Pin_TypeDef)KEY_PIN, GPIO_MODE_IN_PU_NO_IT);//KEY  GPIO��ʼ��
  
  
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_64, 19999);   // 64��Ƶ ������ֵ20000��Ƶ��=250K/20000=12.5HZ
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
  TIM2_ARRPreloadConfig(ENABLE);
  TIM2_SetAutoreload(10000);
  TIM2_Cmd(ENABLE);
  
  
}