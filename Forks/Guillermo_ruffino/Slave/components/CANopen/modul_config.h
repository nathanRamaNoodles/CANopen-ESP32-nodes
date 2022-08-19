/**
 * @file hatox_config.h
 * @author Alexander Miller (alexander.miller@fh-dortmund.de)
 * @brief Configuration File
 * @version 0.1
 * @date 2019-09-13
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef HATOX_CONFIG_H
#define HATOX_CONFIG_H

//####  MAIN CONFIG  ####
#define BOOT_WAIT 2000
#define MAIN_WAIT 100 /** Time in ms between every main loop cycle */

//----------------------------------

//#### CANOPEN CONFIG ####
#define CAN_BITRATE 125    /** 1Mbit*/
#define NODE_ID_SELF 0x4A   /** ESP32 Modul ID*/
#define NODE_ID_MOTOR0 0x1A /** Dunker Motor ID*/
#define NODE_ID_MOTOR1 0x1B /** Dunker Motor ID*/
#define NODE_ID_GYRO 0x04   /** Dunker Motor ID*/
#define NODE_ID_HATOX 0x03  /** Dunker Motor ID*/
//----------------------------------

//####  ROSSERIAL CONFIG  ####
#define ROS_SERVER_AP "ESPNet"
#define ROS_SERVER_PASS "fb2fe3eda4"

//#define CONFIG_ROSSERIAL_OVER_SERIAL /** Use Serial */
#define CONFIG_ROSSERIAL_OVER_ETH /** Use Ethernet instead of Serial */
//#define CONFIG_ROSSERIAL_OVER_WIFI    /** Use WIFI instead of Serial */

//#define ROS_SERVER_IP "192.168.10.1" /** Rosserial server ip */
#define ROS_SERVER_IP "192.168.0.27" /** Rosserial server ip (DEV!) */
#define ROS_SERVER_PORT 11411        /** Rosserial server port */

#define UART_PORT UART_NUM_0   /** Serial interface for Rosserial */
#define UART_TX_PIN GPIO_NUM_1 /** Serial TX Pinnummber */
#define UART_RX_PIN GPIO_NUM_3 /** Serial RX Pinnummber */

#define DEFAULT_ETHERNET_PHY_CONFIG phy_lan8720_default_ethernet_config /** Default Ethernet configuration */

#define PIN_SMI_MDC 23  /** PHY ETH MDC Pinnumber */
#define PIN_SMI_MDIO 18 /** PYH ETH MDIO Pinnumber */

#endif /* HATOX_CONFIG_H */