#ifndef __SV17F_H
#define __SV17F_H

#include "stm8s_gpio.h"
#include "stm8s.h"

#define CON3_PORT       GPIOD
#define CON3_PIN        GPIO_PIN_3

#define UART_PORT       GPIOD
#define UART_TX_PIN     GPIO_PIN_5
#define UART_RX_PIN     GPIO_PIN_6

void sv17f_init(void);
void sv17f_ctrl(uint8_t order);
uint8_t *set_loop_mode(uint8_t mode);
uint8_t *set_loop_times(uint16_t times);
void send_tf_play(void);
void send_tf_stop(void);
void uart_send(uint8_t *data, uint8_t len);
void uart_irq(void);
void Delay_ms(uint16_t nCount);
#endif
