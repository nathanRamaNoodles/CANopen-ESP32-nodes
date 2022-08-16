#ifndef CONFIG_H
#define CONFIG_H

//####  CAN CONFIG  ####

#define CAN_TX_IO (21)  /** CAN TX pin */
#define CAN_RX_IO (22) /** CAN RX pin */
#define CAN_TX_QUEUE_LENGTH (5) /** CAN TX buffer size */
#define CAN_RX_QUEUE_LENGTH (5) /** CAN RX buffer size */

#define CO_CAN_PSEUDO_INTERRUPT_INTERVAL (1000) /** CAN "Interrupt" Interval in μs*/
#define CO_MAIN_TASK_INTERVAL (1000)   /* Interval of tmrTask thread in microseconds */

#define CAN_TICKS_TO_WAIT (10000) /*CAN TX/RX Timeout value*/

//----------------------------------

#endif /* CONFIG_H */
