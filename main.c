/**
  ******************************************************************************
  * @file    GPIO_Toggle\main.c
  * @author  MCD Application Team
  * @version V2.0.4
  * @date    26-April-2018
  * @brief   This file contains the main function for GPIO Toggle example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "nrf.h"
#include "stm8s_clk.h"
#include "sv17f.h"
#include "led.h"
/**
  * @addtogroup GPIO_Toggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Evalboard I/Os configuration */



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay_ms (uint16_t nCount);

/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

  uint8_t voice_sta1 = 0;
  uint8_t voice_sta2 = 0;
void main(void)
{ 
  uint8_t rec = 0;

  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
  /* Initialize I/Os in Output Mode */
  sv17f_init();
  nrf_config();
  led_init();
  enableInterrupts(); 
  
  while (1)
  { 
    rec = 0;
    Delay_ms(5000);//��һ�ο���������ʱһ��ʱ�����ܿ���SV17 
    /* Toggles LEDs */
    GPIO_WriteReverse(LED_GPIO_PORT, (GPIO_Pin_TypeDef)GPIO_PIN_5);
    //read_bytes(RX_ADDR_P0, 5);//��ȡ���յ�ַ
    rec = get_payload();
    if(rec)
    {
       GPIO_WriteLow(LED_GPIO_PORT, (GPIO_Pin_TypeDef)GPIO_PIN_4);//���ź� LED1��
       voice_sta1 = 0;
    }
    else
    {
       GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)GPIO_PIN_4);//���ź� LED1��
       voice_sta1 = 1;
    }
    
    if(voice_sta1 == 0)//������յ��ź� ������ ֹͣ����
    {
     if(voice_sta2 == 1)//�����ǰ�ڲ���
      {
        sv17f_ctrl(4);
      }
      GPIO_WriteLow(LED_SW_PORT, (GPIO_Pin_TypeDef)LED_SW_PIN);
      voice_sta2 = 0;
    }
     if(voice_sta1 == 1)//���û�н��յ��ź� ���� ��ʼ����
    {
      if(voice_sta2 == 0) //�����ǰδ����
      {  
        sv17f_ctrl(18);
        Delay_ms(500);
        sv17f_ctrl(19);
        Delay_ms(500);
        sv17f_ctrl(2);
        GPIO_WriteHigh(LED_SW_PORT, (GPIO_Pin_TypeDef)LED_SW_PIN);
      }
      voice_sta2 = 1;
    }
    
    
  }

}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
