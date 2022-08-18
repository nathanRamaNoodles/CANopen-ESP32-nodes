#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdlib.h>
#include <stdint.h>
#include "driver/can.h"
#include "esp_timer.h"
#include "soc/soc.h"
#include "CANopen.h"
#include "CO_OD.h"
#include "CO_config.h"
#include "modul_config.h"
#include "dunker.h"
#include "Gyro.h"

/*=============================================CANBUS COnfigs==================================================================*/
static can_timing_config_t timingConfig = CAN_TIMING_CONFIG_1MBITS();     //Set Baudrate to 1Mbit
                                                                          //CAN Filter configuration
static can_filter_config_t filterConfig = CAN_FILTER_CONFIG_ACCEPT_ALL(); //Disable Message Filter
                                                                          //CAN General configuration
static can_general_config_t generalConfig = {.mode = TWAI_MODE_NORMAL,
                                             .tx_io = 21,                  /*TX IO Pin (CO_config.h)*/
                                             .rx_io = 22,                  /*RX IO Pin (CO_config.h)*/
                                             .clkout_io = CAN_IO_UNUSED,          /*No clockout pin*/
                                             .bus_off_io = CAN_IO_UNUSED,         /*No busoff pin*/
                                             .tx_queue_len = CAN_TX_QUEUE_LENGTH, /*ESP TX Buffer Size (CO_config.h)*/
                                             .rx_queue_len = CAN_RX_QUEUE_LENGTH, /*ESP RX Buffer Size (CO_config.h)*/
                                             .alerts_enabled = CAN_ALERT_NONE,    /*Disable CAN Alarms TODO: Enable for CO_CANverifyErrors*/
                                             .clkout_divider = 0};  
/*=====================================================================================================================================*/

//Timer Handle
esp_timer_handle_t CO_CANinterruptPeriodicTimer;

uint8_t counter = 0;

volatile uint32_t coInterruptCounter = 0U; /* variable increments each millisecond */

//Timer Interrupt Configuration
static void coMainTask(void *arg);

esp_timer_create_args_t coMainTaskArgs;
//Timer Handle
esp_timer_handle_t periodicTimer;

void mainTask(void *pvParameter)
{
		ESP_LOGE("mainTask", "Starting Application");
		coMainTaskArgs.callback = &coMainTask;
		coMainTaskArgs.name = "coMainTask";
		CO_NMT_reset_cmd_t reset = CO_RESET_NOT;
		vTaskDelay(BOOT_WAIT / portTICK_PERIOD_MS);
		while (reset != CO_RESET_APP)
		{
				/* CANopen communication reset - initialize CANopen objects *******************/
				CO_ReturnError_t err;
				uint32_t coInterruptCounterPrevious;

				/* initialize CANopen */
				err = CO_init(NULL, NODE_ID_SELF /* NodeID */, CAN_BITRATE /* bit rate */);
				if (err != CO_ERROR_NO)
				{
						ESP_LOGE("mainTask", "CO_init failed. Errorcode: %d", err);
						CO_errorReport(CO->em, CO_EM_MEMORY_ALLOCATION_ERROR, CO_EMC_SOFTWARE_INTERNAL, err);
						esp_restart();
				}
				/* Configure Timer interrupt function for execution every CO_MAIN_TASK_INTERVAL */
				ESP_ERROR_CHECK(esp_timer_create(&coMainTaskArgs, &periodicTimer));
				ESP_ERROR_CHECK(esp_timer_start_periodic(periodicTimer, CO_MAIN_TASK_INTERVAL));

				/* start CAN */
				CO_CANsetNormalMode(CO->CANmodule[0]);
				ESP_LOGE("mainTask", "CAN bus started");
    /*WORKAROUND: INTERRUPT ALLOCATION FÜR CAN NICHT MÖGLICH DA BEREITS IM IDF TREIBER VERWENDET*/
    /* Configure Timer interrupt function for execution every CO_CAN_PSEUDO_INTERRUPT_INTERVAL */
    /*Set Canmodule to normal mode*/
    			CO->CANmodule[0]->CANnormal = true;
				reset = CO_RESET_NOT;
				coInterruptCounterPrevious = coInterruptCounter;

				/*Set Operating Mode of Slaves to Operational*/
				CO_sendNMTcommand(CO, 0x01, NODE_ID_MOTOR0);
				ESP_LOGE("mainTask", "NMT sent");
				// CO_sendNMTcommand(CO, 0x01, NODE_ID_MOTOR1);
				// CO_sendNMTcommand(CO, 0x01, NODE_ID_GYRO);
				//CO_sendNMTcommand(CO, 0x01, NODE_ID_HATOX);

				/* Initialise system components */
				dunker_init(CO, NODE_ID_MOTOR0, 2);
				ESP_LOGE("mainTask", "Dunker init");
				// gyro_init(CO);

				/* application init code goes here. */
				//rosserialSetup();

				uint32_t coInterruptCounterCopy;
						uint32_t coInterruptCounterDiff;
						coInterruptCounterCopy = coInterruptCounter;
						coInterruptCounterDiff = coInterruptCounterCopy - coInterruptCounterPrevious;
						coInterruptCounterPrevious = coInterruptCounterCopy;

						/* CANopen process */
						reset = CO_process(CO, coInterruptCounterDiff, NULL);
						ESP_LOGE("mainTask", "CO_Process init");

						uint8_t sdo_rx_data_buffer[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
						 const twai_message_t msg_buffer = {.identifier = 0x61A, .data_length_code = 8, .data = {0x4C, 0x09,  0x10, 0x00, 0x00, 0x00, 0x00, 0x00} };
					

				while (reset == CO_RESET_NOT)
				{
						twai_transmit(&msg_buffer, 1000);
						ESP_LOGE("maintask", "beggining of a While");

						/*download*/
						// CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x1008, 0, sdo_rx_data_buffer, 13, 0);
						// dunker_coProcessDownloadSDO();
						/* upload*/		
						CO_SDOclientUploadInitiate(CO->SDOclient[0], 0x1008, 0, sdo_rx_data_buffer, 13, 0);
						dunker_coProcessUploadSDO();

								ESP_LOGE("mainTask", "Slave device name: %d %d %d %d %d %d %d %d %d %d %d %d %d", sdo_rx_data_buffer[0],
																													sdo_rx_data_buffer[1],
																													sdo_rx_data_buffer[2],
																													sdo_rx_data_buffer[3],
																													sdo_rx_data_buffer[4],
																													sdo_rx_data_buffer[5],
																													sdo_rx_data_buffer[6],
																													sdo_rx_data_buffer[7],
																													sdo_rx_data_buffer[8],
																													sdo_rx_data_buffer[9],
																													sdo_rx_data_buffer[10],
																													sdo_rx_data_buffer[11],
																													sdo_rx_data_buffer[12]);
																													
						/* loop for normal program execution ******************************************/
						// /* Nonblocking application code may go here. */
						// if (counter == 0)
						// {		
						// 		ESP_LOGE("mainTask", "GET salve dev name: ");

						// 		// twai_transmit(&msg_buffer, 1000);


								// CO_SDOclientUploadInitiate(CO->SDOclient[0], 0x1008, 0, sdo_rx_data_buffer, 13, 0);
								// dunker_coProcessUploadSDO();
								// ESP_LOGE("mainTask", "Slave device name: %d %d %d %d %d %d %d %d %d %d %d %d %d", sdo_rx_data_buffer[0],
								// 																					sdo_rx_data_buffer[1],
								// 																					sdo_rx_data_buffer[2],
								// 																					sdo_rx_data_buffer[3],
								// 																					sdo_rx_data_buffer[4],
								// 																					sdo_rx_data_buffer[5],
								// 																					sdo_rx_data_buffer[6],
								// 																					sdo_rx_data_buffer[7],
								// 																					sdo_rx_data_buffer[8],
								// 																					sdo_rx_data_buffer[9],
								// 																					sdo_rx_data_buffer[10],
								// 																					sdo_rx_data_buffer[11],
								// 																					sdo_rx_data_buffer[12]);
																													
						// 		// ESP_LOGE("mainTask", "Dunker_setEnable begin");
						// 		// dunker_setEnable(1);
						// 		// ESP_LOGE("mainTask", "Dunker_setSpeed 1000");
						// 		// dunker_setSpeed(1000);
						// 		ESP_LOGE("mainTask", "Counter 0++");
						// 		counter++;
						// }
						// if (coInterruptCounter > 4000 && counter == 1)
						// {
						// 		// ESP_LOGE("mainTask", "Dunker_setSpeed 3000");
						// 		// dunker_setSpeed(3000);
						// 		ESP_LOGE("mainTask", "Counter = 1 ++");
						// 		counter++;
						// }
						// if (coInterruptCounter > 8000 && counter == 2)
						// {		
						// 		// ESP_LOGE("mainTask", "Dunker_quickstop");
						// 		// dunker_quickStop();
						// 		ESP_LOGE("mainTask", "Counter = 2 ++");
						// 		counter++;
						// }
						// if (coInterruptCounter > 12000 && counter == 3)
						// {		
						// 		// ESP_LOGE("mainTask", "Dunker_continueMovement");
						// 		// dunker_continueMovement();
						// 		// ESP_LOGE("mainTask", "Dunker_setSpeed 1000");
						// 		// dunker_setSpeed(1000);
						// 		ESP_LOGE("mainTask", "Counter = 3 ++");
						// 		counter = 0;
						// }
						// if (coInterruptCounter > 16000 && counter == 4)
						// {		
						// 		// ESP_LOGE("mainTask", "Dunker_halt");
						// 		// dunker_halt();
						// 		// ESP_LOGE("mainTask", "Dunker_setEnable");
						// 		// dunker_setEnable(0);
						// 		ESP_LOGE("mainTask", "Counter = 4 ++");
						// 		counter++;
						// }

						/* Wait */
						vTaskDelay(pdMS_TO_TICKS(10000));
				}
		}
		/* program exit
		 * ***************************************************************/
		/* reset */
		ESP_LOGE("mainTask", "MCU restart");
		esp_restart();
}

/* CanOpen-Task executes in constant intervals ********************************/
static void coMainTask(void *arg)
{
		coInterruptCounter++;

		if (CO->CANmodule[0]->CANnormal)
		{
				bool_t syncWas;

				/* Process Sync */
				syncWas = CO_process_SYNC(CO, CO_MAIN_TASK_INTERVAL);

				/* Read inputs */
				//CO_process_RPDO(CO, syncWas);

				/* Write outputs */
				//CO_process_TPDO(CO, syncWas, CO_MAIN_TASK_INTERVAL);
		}
}

void app_main()
{
		xTaskCreate(&mainTask, "mainTask", 4096, NULL, 5, NULL);
}
