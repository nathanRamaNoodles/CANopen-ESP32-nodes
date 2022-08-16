#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_log.h"
#include <stdlib.h>
#include <stdint.h>
#include "driver/can.h"
#include <driver/adc.h>
#include <driver/gpio.h>
#include "esp_timer.h"
#include "soc/soc.h"
#include "CANopen.h"
#include "CO_OD.h"
#include "CO_config.h"
#include "modul_config.h"

#include <sys/param.h>
#include "esp_system.h"
#include "nvs_flash.h"

uint8_t counter = 0;
uint8_t LED_red, LED_green;
volatile uint16_t CO_timer1ms = 0U; /* variable increments each millisecond */
volatile static bool_t CANopenConfiguredOK = false;
volatile uint32_t coInterruptCounter = 0U; /* variable increments each millisecond */

//Timer Interrupt Configuration
static void coMainTask(void *arg);

esp_timer_create_args_t coMainTaskArgs;
//Timer Handle
esp_timer_handle_t periodicTimer;

void mainTask(void *pvParameter)
{
		CO_ReturnError_t err;
		CO_NMT_reset_cmd_t reset;
		uint32_t heapMemoryUsed;

		void *CANmoduleAddress = NULL; /* CAN module address */
		uint8_t pendingNodeId = 10; /* read from dip switches or nonvolatile memory,
	                                   configurable by LSS slave */
		uint8_t activeNodeId =
				10; /* Copied from CO_pendingNodeId in the communication reset section */
		uint16_t pendingBitRate = 125; /* read from dip switches or nonvolatile
	                                      memory, configurable by LSS slave */

		// Configure potentiometer reader
		adc1_config_width(ADC_WIDTH_BIT_12);
		adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);

		// Configure push buttons
		const gpio_config_t io_conf = {.intr_type = GPIO_PIN_INTR_DISABLE,
				                       .mode = GPIO_MODE_INPUT,
				                       .pin_bit_mask = (1ULL << GPIO_NUM_25) |
				                                       (1ULL << GPIO_NUM_32) |
				                                       (1ULL << GPIO_NUM_33),
				                       .pull_up_en = 1};

		gpio_config(&io_conf);

		/* Allocate memory */
		err = CO_new(&heapMemoryUsed);
		if (err != CO_ERROR_NO) {
				printf("Error: Can't allocate memory\n");
		} else {
				printf("Allocated %d bytes for CANopen objects\n", heapMemoryUsed);
		}

		coMainTaskArgs.callback = &coMainTask;
		coMainTaskArgs.name = "coMainTask";
		reset = CO_RESET_NOT;
		vTaskDelay(BOOT_WAIT / portTICK_PERIOD_MS);

		OD_powerOnCounter++;

		printf("CANopenNode - Reset application, count = %d\n",
		           OD_powerOnCounter);

		while (reset != CO_RESET_APP)
		{
				/* CANopen communication reset - initialize CANopen objects
				 * *******************/
				uint16_t timer1msPrevious = 0;

				printf("CANopenNode - Reset communication...\n");

				/* disable CAN and CAN interrupts */
				CANopenConfiguredOK = false;

				/* initialize CANopen */
				err = CO_CANinit(CANmoduleAddress, pendingBitRate);
				if (err != CO_ERROR_NO) {
						printf("Error: CAN initialization failed: %d\n", err);
				}
				err = CO_LSSinit(&pendingNodeId, &pendingBitRate);
				if (err != CO_ERROR_NO) {
						printf("Error: LSS slave initialization failed: %d\n", err);
				}
				activeNodeId = pendingNodeId;
				err = CO_CANopenInit(activeNodeId);
				if (err == CO_ERROR_NO) {
						CANopenConfiguredOK = true;
				} else if (err != CO_ERROR_NODE_ID_UNCONFIGURED_LSS) {
						printf("Error: CANopen initialization failed: %d\n", err);
				}

				/* CANopen communication reset - initialize CANopen objects *******************/
				CO_ReturnError_t err;
				uint32_t coInterruptCounterPrevious;

				/* initialize CANopen */
				// err = CO_init(NULL, NODE_ID_SELF /* NodeID */, CAN_BITRATE /* bit rate */);
				// if (err != CO_ERROR_NO)
				// {
				// 		ESP_LOGE("mainTask", "CO_init failed. Errorcode: %d", err);
				// 		CO_errorReport(CO->em, CO_EM_MEMORY_ALLOCATION_ERROR, CO_EMC_SOFTWARE_INTERNAL, err);
				// 		esp_restart();
				// }
				/* Configure Timer interrupt function for execution every CO_MAIN_TASK_INTERVAL */
				ESP_ERROR_CHECK(esp_timer_create(&coMainTaskArgs, &periodicTimer));
				ESP_ERROR_CHECK(esp_timer_start_periodic(periodicTimer, CO_MAIN_TASK_INTERVAL));

				/* start CAN */
				CO_CANsetNormalMode(CO->CANmodule[0]);

				reset = CO_RESET_NOT;
				coInterruptCounterPrevious = coInterruptCounter;

				while (reset == CO_RESET_NOT)
				{
						/* loop for normal program execution
						 * ******************************************/
						uint16_t timer1msCopy, timer1msDiff;

						timer1msCopy = CO_timer1ms;
						timer1msDiff = timer1msCopy - timer1msPrevious;
						timer1msPrevious = timer1msCopy;

						/* CANopen process */
						reset = CO_process(CO, (uint32_t)timer1msDiff * 1000, NULL);
						LED_red = CO_LED_RED(CO->LEDs, CO_LED_CANopen);
						LED_green = CO_LED_GREEN(CO->LEDs, CO_LED_CANopen);

						/* Nonblocking application code may go here. */
						OD_readInput8Bit[0]++;

						OD_readInput8Bit[1] = 0;
						if (gpio_get_level(GPIO_NUM_25))
								OD_readInput8Bit[1] |= 1;
						if (gpio_get_level(GPIO_NUM_32))
								OD_readInput8Bit[1] |= 2;
						if (gpio_get_level(GPIO_NUM_33))
								OD_readInput8Bit[1] |= 4;

						 uint32_t val = adc1_get_raw(ADC1_CHANNEL_0); // GPIO36
						OD_readInput8Bit[4] = val & 0x000000ff;
						OD_readInput8Bit[5] = (val & 0x0000ff00) >> 8;
						OD_readInput8Bit[6] = (val & 0x00ff0000) >> 16;
						OD_readInput8Bit[7] = (val & 0xff000000) >> 24;

						// if (timer1msCopy % 1000 == 0) {
						// 		ESP_LOGI("ADC", "val: %d", val);
						// }

						/* Process EEPROM */

						/* optional sleep for short time */
						vTaskDelay(pdMS_TO_TICKS(1000));
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
				syncWas = CO_process_SYNC(CO, CO_MAIN_TASK_INTERVAL, NULL);

				/* Read inputs */
				CO_process_RPDO(CO, syncWas);

				/* Write outputs */
				CO_process_TPDO(CO, syncWas, CO_MAIN_TASK_INTERVAL, NULL);
		}
}

void app_main()
{
		xTaskCreate(&mainTask, "mainTask", 4096, NULL, 5, NULL);
}