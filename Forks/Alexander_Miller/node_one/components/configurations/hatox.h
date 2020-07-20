#ifndef HATOX_H_
#define HATOX_H_

#include "CANopen.h"
#include <stdint.h>
#include <string.h>
#include "CO_OD.h"

static const int16_t HATOX_BTN_RUN = (1 << 0);         //Bitposition for RUN Button
static const int16_t HATOX_BTN_FAST = (1 << 1);         //Bitposition for Rabbit Button
static const int16_t HATOX_BTN_SLOW = (1 << 2);         //Bitposition for Turtle Button
static const int16_t HATOX_BTN_1 = (1 << 3);         //Bitposition for 1 Button
static const int16_t HATOX_BTN_2 = (1 << 4);         //Bitposition for 2 Button
static const int16_t HATOX_BTN_3 = (1 << 5);         //Bitposition for 3 Button
static const int16_t HATOX_BTN_4 = (1 << 6);         //Bitposition for 4 Button
static const int16_t HATOX_BTN_STOP = (1 << 7);         //Bitposition for STOP Button
static const int16_t HATOX_BTN_5 = (1 << 8);         //Bitposition for 5 Button
static const int16_t HATOX_BTN_6 = (1 << 9);         //Bitposition for 6 Button

CO_t *_CO;

int8_t hatox_init(CO_t *CO);
uint8_t hatox_getLeftStickX(void);
uint8_t hatox_getLeftStickY(void);
uint8_t hatox_getRightStickX(void);
uint8_t hatox_getRightStickY(void);
uint8_t hatox_getStickDir(void);
uint16_t hatox_getButtonStatus(void);
uint8_t hatox_setText(uint8_t line, uint8_t column, char* text);


#endif
