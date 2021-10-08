#ifndef __NRF_H
#define __NRF_H

#include "stm8s.h"

//REG
#define CONFIG          0x00
#define EN_AA           0x01
#define EN_RXADDR       0x02
#define SETUP_AW        0x03
#define SETUP_RETR      0x04
#define RF_CH           0x05
#define RF_SETUP        0x06
#define STATUS          0x07
#define OBSERVE_TX      0x08
#define RSSI            0x09
#define RX_ADDR_P0      0x0a
#define TX_ADDR         0x10    
#define RX_PW_P0        0x11
#define RX_PW_P1        0x12
#define RX_PW_P2        0x13
#define RX_PW_P3        0x14
#define RX_PW_P4        0x15
#define RX_PW_P5        0x16
#define FIFO_STATUS     0x17    
#define DYNPD           0x1c
#define FEATURE         0x1d

//CMD
#define READ_REG                0x00
#define WRITE_REG               0x20
#define W_TX_PAYLOAD_WIDTH      32
#define W_TX_PAYLOAD_NOACK      0xb0
#define R_RX_PAYLOAD            0x61
#define FLUSH_RX                0xE2
#define WR_TX_PLOAD             0xA0    

// STATUS Register 
#define RX_DR			0x40 
#define TX_DS			0x20
#define MAX_RT			0x10

#define NRF_CE_PORT     GPIOC
#define NRF_CE_PIN      GPIO_PIN_4  
#define NRF_IRQ_PIN     GPIO_PIN_3 

#define SPI_CS_PORT     GPIOA
#define SPI_CS_PIN      GPIO_PIN_3         
  

#define SPI_GPIO_PORT           GPIOC   
#define SPI_SCLK_PIN            GPIO_PIN_5
#define SPI_MOSI_PIN            GPIO_PIN_6
#define SPI_MISO_PIN            GPIO_PIN_7

void nrf_config(void);
uint8_t get_payload(void);
void send_onebyte(uint8_t data, uint8_t addr);
void send_bytes(uint8_t *data, uint8_t num, uint8_t addr);
uint8_t read_onebyte(uint8_t addr);
void read_bytes(uint8_t addr, uint8_t num);
void Delay_us(uint16_t nCount);
#endif