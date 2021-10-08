#ifndef __LED_H
#define __LED_H

#define LED_GPIO_PORT  (GPIOB)
#define LED_GPIO_PINS  (GPIO_PIN_4 | GPIO_PIN_5)

#define LED_SW_PORT             GPIOD
#define LED_SW_PIN              GPIO_PIN_2

#define KEY_PORT                GPIOD
#define KEY_PIN                 GPIO_PIN_4

#define KEY     (GPIO_ReadInputPin(GPIOD, GPIO_PIN_4))

void led_init(void);

#endif

