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

uint8_t counter = 0;

volatile uint32_t coInterruptCounter = 0U; /* variable increments each millisecond */

//Timer Interrupt Configuration
static void coMainTask(void *arg);

esp_timer_create_args_t coMainTaskArgs;
//Timer Handle
esp_timer_handle_t periodicTimer;

void mainTask(void *pvParameter)
{
		coMainTaskArgs.callback = &coMainTask;
		coMainTaskArgs.name = "coMainTask";
		CO_NMT_reset_cmd_t reset = CO_RESET_NOT;
		// vTaskDelay(BOOT_WAIT / portTICK_PERIOD_MS);
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

				reset = CO_RESET_NOT;
				coInterruptCounterPrevious = coInterruptCounter;

				/*Set Operating Mode of Slaves to Operational*/
				// CO_sendNMTcommand(CO, 0x01, NODE_ID_MASTER);
				//CO_sendNMTcommand(CO, 0x01, NODE_ID_MOTOR1);
				//CO_sendNMTcommand(CO, 0x01, NODE_ID_GYRO);
				//CO_sendNMTcommand(CO, 0x01, NODE_ID_HATOX);

				/* Initialise system components */
				// dunker_init(CO, NODE_ID_MOTOR0, 2);
				// gyro_init(CO);

				/* application init code goes here. */
				//rosserialSetup();

				while (reset == CO_RESET_NOT)
				{
						/* loop for normal program execution ******************************************/
						uint32_t coInterruptCounterCopy;
						uint32_t coInterruptCounterDiff;
						coInterruptCounterCopy = coInterruptCounter;
						coInterruptCounterDiff = coInterruptCounterCopy - coInterruptCounterPrevious;
						coInterruptCounterPrevious = coInterruptCounterCopy;

						/* CANopen process */
						reset = CO_process(CO, coInterruptCounterDiff, NULL);

						/* Nonblocking application code may go here. */
						if (counter == 0)
						{
								// dunker_setEnable(1);
								// dunker_setSpeed(1000);
								counter++;
						}
						if (coInterruptCounter > 4000 && counter == 1)
						{
								// dunker_setSpeed(3000);
								counter++;
						}
						if (coInterruptCounter > 8000 && counter == 2)
						{
								// dunker_quickStop();
								counter++;
						}
						if (coInterruptCounter > 12000 && counter == 3)
						{
								// dunker_continueMovement();
								// dunker_setSpeed(1000);
								counter++;
						}
						if (coInterruptCounter > 16000 && counter == 4)
						{
								// dunker_halt();
								// dunker_setEnable(0);
								counter++;
						}

						/* Wait */
						vTaskDelay(MAIN_WAIT / portTICK_PERIOD_MS);
				}
		}
		/* program exit
		 * ***************************************************************/
		/* reset */
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
				CO_process_RPDO(CO, syncWas);

				/* Write outputs */
				CO_process_TPDO(CO, syncWas, CO_MAIN_TASK_INTERVAL);
		}
}

void app_main()
{
		xTaskCreate(&mainTask, "mainTask", 4096, NULL, 5, NULL);
}
