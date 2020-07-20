#ifndef HATOX_CONFIG_H
#define HATOX_CONFIG_H

//####  MAIN CONFIG  ####
#define BOOT_WAIT 2000
#define MAIN_WAIT 100 /** Time in ms between every main loop cycle */

//----------------------------------

//#### CANOPEN CONFIG ####
#define CAN_BITRATE 1000    /** 1Mbit*/
#define NODE_ID_SELF 0x1A   /** ESP32 Modul ID*/
#define NODE_ID_MASTER 0x42 /** Dunker Motor ID*/
#define NODE_ID_MOTOR1 0x1B /** Dunker Motor ID*/
#define NODE_ID_GYRO 0x04   /** Dunker Motor ID*/
#define NODE_ID_HATOX 0x03  /** Dunker Motor ID*/
//----------------------------------

#endif /* HATOX_CONFIG_H */
