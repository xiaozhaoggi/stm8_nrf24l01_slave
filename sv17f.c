#include "stm8s.h"
#include "sv17f.h"
#include "stm8s_gpio.h"
#include "string.h"

uint8_t UART_RX_BUFF[20];
uint8_t rx_offset = 0;
uint8_t CHECK_STA[4] = {0xAA,0x01,0x00,0xAB};//检查播放
uint8_t PLAY[4] = {0xAA,0x02,0x00,0xAC};//播放
uint8_t PAUSE[4] = {0xAA,0x03,0x00,0xAD};//暂停
uint8_t STOP[4] = {0xAA,0x04,0x00,0xAE};//停止
uint8_t LAST[4] = {0xAA,0x05,0x00,0xAF};//上一曲
uint8_t NEXT[4] = {0xAA,0x06,0x00,0xB0};//下一曲
//TF-card mp3
uint8_t TF_PLAY[8] = {0x7E,0xFF,0x06,0x0D,0x00,0x00,0x00,0xEF};
uint8_t TF_STOP[8] = {0x7E,0xFF,0x06,0x16,0x00,0x00,0x00,0xEF};
uint8_t TF_01_CYCLE_PLAY[8] = {0x7E,0xFF,0x06,0x17,0x00,0x01,0x00,0xEF};
uint8_t TF_SET_VOLUME[8] = {0x7E,0xFF,0x06,0x06,0x00,0x00,0x1E,0xEF};
uint8_t loop_mode = 1;
uint16_t loop_times = 0x00FF;
void sv17f_init(void)
{
 // GPIO_Init(CON3_PORT, (GPIO_Pin_TypeDef)CON3_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);//CON3 上电后初始化为高电平
  GPIO_Init(UART_PORT, (GPIO_Pin_TypeDef)UART_TX_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);//UART TX GPIO初始化
  GPIO_Init(UART_PORT, (GPIO_Pin_TypeDef)UART_RX_PIN, GPIO_MODE_IN_PU_NO_IT);//UART RX GPIO初始化
  
  UART1_DeInit();
  UART1_Init((uint32_t)9600, UART1_WORDLENGTH_8D, 
                UART1_STOPBITS_1, UART1_PARITY_NO, 
                UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);//异步 波特率9600
  
  UART1_ClearITPendingBit(UART1_IT_RXNE);//清中断
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);//开中断
  
  UART1_Cmd(ENABLE);//串口使能
  
 // Delay_ms(500);
//  uart_send(TF_SET_VOLUME,8);
//  sv17f_ctrl(18);
//  Delay_ms(500);
  //sv17f_ctrl(19);
}
/*order : 
1：检查状态
2：播放
3：暂停
4：停止
5：上一曲
6：下一曲
18：设定循环模式
*/
void sv17f_ctrl(uint8_t order)
{
  uint8_t temp = order;
  
  switch(temp)
  {  
    case 1:
      uart_send(CHECK_STA,4);
      break;
      
    case 2:
      uart_send(PLAY,4);
      break;
      
    case 3:
      uart_send(PAUSE,4);
      break;
      
    case 4:
      uart_send(STOP,4);
      break;
      
    case 5:
      uart_send(LAST,4);
      break;
    
    case 6:
      uart_send(NEXT,4);
      break;
    
    case 18:
      uart_send(set_loop_mode(loop_mode),5);
      break;
      
    case 19:
      uart_send(set_loop_times(loop_times),6);
      break;  
      
    default:
      break;
  }
}

//设定循环模式
//mode : 0 全盘循环   1：单曲循环  2:单曲停止  7：顺序播放
uint8_t *set_loop_mode(uint8_t mode)
{
  static uint8_t buf[5] = {0};
  buf[0] = 0xAA;
  buf[1] = 0x18;
  buf[2] = 0x01;
  buf[3] = mode;
  buf[4] = buf[0]+buf[1]+buf[2]+buf[3];
  
  return buf;
}
//设定循环次数
uint8_t *set_loop_times(uint16_t times)
{
  static uint8_t buf[6] = {0};
  buf[0] = 0xAA;
  buf[1] = 0x19;
  buf[2] = 0x02;
  buf[3] = times >> 8;
  buf[4] = times & 0xff;
  buf[5] = buf[0]+buf[1]+buf[2]+buf[3]+buf[4];
  return buf;
}

void send_tf_play(void)
{

  uart_send(TF_PLAY,8);
}
void send_tf_stop(void)
{
  uart_send(TF_STOP,8);
}
void uart_send(uint8_t *data, uint8_t len)
{
    for(int i = 0; i < len; i++)
    {
      while(UART1_GetFlagStatus(UART1_FLAG_TC) == RESET);
         UART1_SendData8(*data++);
    }
    
}

void uart_irq(void)
{
  u8 res;
  res = UART1_ReceiveData8();
  if(rx_offset == 20)
  {
    rx_offset = 0;
    memset(UART_RX_BUFF, 0, 20);
  }
  UART_RX_BUFF[rx_offset++] = res;
}

/**
  * @brief Delay
  * @param nCount
  * @retval None
  */
void Delay_ms(uint16_t nCount)
{
  /* Decrement nCount value */
  for(int i = 0; i < 2000; i++)
  {
    for(int j = nCount; j > 0; j--);
  }
  
}

/*INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  if(SET == UART1_GetITStatus(UART1_IT_RXNE))
  {
    UART1_ClearITPendingBit(UART1_IT_RXNE);//清中断
    uart_irq();
  }
          
}*/