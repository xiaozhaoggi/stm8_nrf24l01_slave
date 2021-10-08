#include "nrf.h"
#include "stm8s_spi.h"



uint8_t trans_addr[5] = {0x01,0x02,0x03,0x04,0x05};
uint8_t trans_data = 0x01;
uint8_t RX_BUFF[5] = {0};
void nrf_config(void)
{
  GPIO_Init(NRF_CE_PORT, (GPIO_Pin_TypeDef)NRF_CE_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(NRF_CE_PORT, (GPIO_Pin_TypeDef)NRF_IRQ_PIN,GPIO_MODE_OUT_PP_LOW_FAST);//NRF IRQ引脚初始化
  GPIO_Init(SPI_CS_PORT, (GPIO_Pin_TypeDef)SPI_CS_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  /* SPI configuration */
  SPI_DeInit();
  
  //SPI相关GPIO初始化
  GPIO_Init(SPI_GPIO_PORT, (GPIO_Pin_TypeDef)SPI_SCLK_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(SPI_GPIO_PORT, (GPIO_Pin_TypeDef)SPI_MOSI_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  //此设置很关键，作为主设备一定要将其设置为输入
  GPIO_Init(SPI_GPIO_PORT, (GPIO_Pin_TypeDef)SPI_MISO_PIN,  GPIO_MODE_IN_PU_NO_IT);
  
  /* Initialize SPI in Slave mode  */
  SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_2, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW,
           SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT,(uint8_t)0x07);

  /* Enable the SPI*/
  SPI_Cmd(ENABLE);
  
  send_bytes(trans_addr, 5, WRITE_REG + RX_ADDR_P0);// 接收地址
  send_onebyte(0x01,WRITE_REG + EN_AA);// 使能接收通道0自动应答
  send_onebyte(0x01, WRITE_REG + EN_RXADDR);// 使能接收通道 0
  send_onebyte(40, WRITE_REG + RF_CH);// 选择射频信道
  send_onebyte(W_TX_PAYLOAD_WIDTH, WRITE_REG + RX_PW_P0);//设置负载长度，使用 PIPE0 接收
 // send_onebyte(0x03, WRITE_REG + SETUP_AW);// 设置地址宽度为 5bytes
  send_onebyte(0x04, WRITE_REG +  FEATURE);//使能动态负载
  send_onebyte(0x01, WRITE_REG +  DYNPD);//开启 DPL_P0
  send_onebyte(0x0f, WRITE_REG + RF_SETUP); // 数据传输率 2Mbps 及功率
  send_onebyte(0x0f, WRITE_REG + CONFIG);// 配置为接收方、CRC 为 2Bytes
  //send_onebyte(0xff, WRITE_REG + STATUS);
  
  GPIO_WriteHigh(NRF_CE_PORT, NRF_CE_PIN);//CE置1 激活模块
}


uint8_t state = 0;
uint8_t get_payload(void)
{
  
  uint8_t payload2 = 0;
  
  state = read_onebyte(STATUS);
  
  if(state & RX_DR)
  {
    Delay_ms(50);
    send_onebyte(state | 0x40, WRITE_REG + STATUS);
    payload2 = read_onebyte(R_RX_PAYLOAD);
    send_onebyte(0xff, FLUSH_RX);
  }

  return payload2;
}

void send_onebyte(uint8_t data, uint8_t addr)
{
  GPIO_WriteLow(SPI_CS_PORT, SPI_CS_PIN);//拉低CS 选中模块
  
  while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET);
  SPI_SendData(addr);
  
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
  SPI_ReceiveData();
  
  while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET);
  SPI_SendData(data);
  
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
  SPI_ReceiveData();
  
  GPIO_WriteHigh(SPI_CS_PORT, SPI_CS_PIN);//拉高CS
}

void send_bytes(uint8_t *data, uint8_t num, uint8_t addr)
{
  GPIO_WriteLow(SPI_CS_PORT, SPI_CS_PIN);//拉低CS 选中模块
  while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET);
  SPI_SendData(addr);
  for(int i = num; i > 0; i--)
  {
    while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET);
    SPI_SendData(*data);
    
    while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
    SPI_ReceiveData();
    
    data++;
  }
  GPIO_WriteHigh(SPI_CS_PORT, SPI_CS_PIN);//拉高CS
}



uint8_t temp;
uint8_t read_onebyte(uint8_t addr)
{
  
  GPIO_WriteLow(SPI_CS_PORT, SPI_CS_PIN);//拉低CS 选中模块
  
  while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET);
  SPI_SendData(addr);
  
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
  temp = SPI_ReceiveData();//清标志位
  
  while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET);
  SPI_SendData(0);//发生波形
  
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
  temp = SPI_ReceiveData();//读数据
  
  GPIO_WriteHigh(SPI_CS_PORT, SPI_CS_PIN);//拉高CS
  
  return temp;
}
void read_bytes(uint8_t addr, uint8_t num)
{
  
  GPIO_WriteLow(SPI_CS_PORT, SPI_CS_PIN);//拉低CS 选中模块
  
  while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET);
  SPI_SendData(addr);
  
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
  temp = SPI_ReceiveData();//清标志位
  
  for(int i = 0; i < num; i++)
  {
    while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET);
    SPI_SendData(0);//发生波形
    
    while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
    RX_BUFF[i] = SPI_ReceiveData();//读数据
  }
  
  GPIO_WriteHigh(SPI_CS_PORT, SPI_CS_PIN);//拉高CS
}
void Delay_us(uint16_t us)
{
   uint16_t i;
  i = us/2;
  i--;
  asm("nop"); asm("nop");
   for(;i!=0;i--)      
    {
         asm("nop");    asm("nop");    asm("nop");
         asm("nop");    asm("nop");    asm("nop");
         asm("nop");    asm("nop");    asm("nop");
    }
   
}