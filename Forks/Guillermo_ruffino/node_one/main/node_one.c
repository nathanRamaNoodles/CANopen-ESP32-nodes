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

#include "esp_system.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"
#include "cmd_decl.h"
#include "esp_vfs_fat.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "freertos/queue.h"
#include "sdkconfig.h"
#include "esp_intr_alloc.h"


#if CONFIG_IDF_TARGET_ESP32
    #include "esp32/rom/uart.h"
#elif CONFIG_IDF_TARGET_ESP32S2
    #include "esp32s2/rom/uart.h"
#endif

#define EX_UART_NUM UART_NUM_0
#define PATTERN_CHR_NUM    (3)         /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/

#define BUF_SIZE (1024)
#define RD_BUF_SIZE (BUF_SIZE)
static QueueHandle_t uart0_queue;

static intr_handle_t handle_console;

uint8_t rxbuf[256];
// Register to collect data length
uint16_t urxlen;

#define PROMPT_STR CONFIG_IDF_TARGET


static const char* TAG = "example";

/* Console command history can be stored to and loaded from a file.
 * The easiest way to do this is to use FATFS filesystem on top of
 * wear_levelling library.
 */


/*=================================================================================*/
char* line ;


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

uint8_t rxbuf[256];
bool uart_rx_irq_flag = 0;
// Register to collect data length
uint16_t urxlen;

/*=================UART INTERUPT==========================================*/
static void IRAM_ATTR uart_intr_handle(void *arg)
{
  uint16_t rx_fifo_len, status;
  uint16_t i = 0;
    status = UART0.int_st.val; // read UART interrupt Status
    rx_fifo_len = UART0.status.rxfifo_cnt; // read number of bytes in UART buffer
  
  while(rx_fifo_len){
   rxbuf[i++] = UART0.fifo.rw_byte; // read all bytes
   rx_fifo_len--;
 }
  uart_rx_irq_flag = 1;
 // after reading bytes from buffer clear UART interrupt status
 uart_clear_intr_status(EX_UART_NUM, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);

// a test code or debug code to indicate UART receives successfully,
// you can redirect received byte as echo also
// uart_write_bytes(EX_UART_NUM, (const char*) "RX Done", 7)
}

/*===============================CLI includes===================================*/

static void initialize_nvs(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}



static void initialize_console(void)
{
    /* Drain stdout before reconfiguring it */
    fflush(stdout);
    fsync(fileno(stdout));

    /* Disable buffering on stdin */
    setvbuf(stdin, NULL, _IONBF, 0);

    /* Minicom, screen, idf_monitor send CR when ENTER key is pressed */
    esp_vfs_dev_uart_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
    /* Move the caret to the beginning of the next line on '\n' */
    esp_vfs_dev_uart_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);

    /* Configure UART. Note that REF_TICK is used so that the baud rate remains
     * correct while APB frequency is changing in light sleep mode.
     */
    uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	};

	ESP_ERROR_CHECK(uart_param_config(EX_UART_NUM, &uart_config));

	//Set UART log level
	esp_log_level_set(TAG, ESP_LOG_INFO);

	//Set UART pins (using UART0 default pins ie no changes.)
	ESP_ERROR_CHECK(uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

	//Install UART driver, and get the queue.
	ESP_ERROR_CHECK(uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0));

	// release the pre registered UART handler/subroutine
	ESP_ERROR_CHECK(uart_isr_free(EX_UART_NUM));

	// register new UART subroutine
	ESP_ERROR_CHECK(uart_isr_register(EX_UART_NUM,uart_intr_handle, NULL, ESP_INTR_FLAG_IRAM, &handle_console));

	// enable RX interrupt
	ESP_ERROR_CHECK(uart_enable_rx_intr(EX_UART_NUM));

    /* Initialize the console */
    esp_console_config_t console_config = {
            .max_cmdline_args = 8,
            .max_cmdline_length = 256,
#if CONFIG_LOG_COLORS
            .hint_color = atoi(LOG_COLOR_CYAN)
#endif
    };
    ESP_ERROR_CHECK( esp_console_init(&console_config) );

    /* Configure linenoise line completion library */
    /* Enable multiline editing. If not set, long commands will scroll within
     * single line.
     */
    linenoiseSetMultiLine(1);

    /* Tell linenoise where to get command completions and hints */
    linenoiseSetCompletionCallback(&esp_console_get_completion);
    linenoiseSetHintsCallback((linenoiseHintsCallback*) &esp_console_get_hint);

    /* Set command history size */
    linenoiseHistorySetMaxLen(100);

    /* Don't return empty lines */
    linenoiseAllowEmpty(true);

#if CONFIG_STORE_HISTORY
    /* Load command history from filesystem */
    linenoiseHistoryLoad(HISTORY_PATH);
#endif
}

void cmd_exe(void) {
	 	int ret;
        if (uart_rx_irq_flag == 1) {
        printf("%s\n", rxbuf);
        esp_err_t err = esp_console_run((char *)rxbuf, &ret);
        if (err == ESP_ERR_NOT_FOUND) {
            printf("Unrecognized command\n");

        } else if (err == ESP_ERR_INVALID_ARG) {
            // command was empty

        } else if (err == ESP_OK && ret != ESP_OK) {
            printf("Command returned non-zero error code: 0x%x (%s)\n", ret, esp_err_to_name(ret));

        } else if (err != ESP_OK) {
            printf("Internal error: %s\n", esp_err_to_name(err));
        
        }
        //printf("esp32>\n");
        uart_rx_irq_flag = 0;
        memset(rxbuf, 0, 256);
        }
}





void mainTask(void *pvParameter)
{
/*====================CMD init===============================================*/
    initialize_console();
    /* Register commands */
    esp_console_register_help_command();
    register_system();
   // register_AT_commands();
    /* Prompt to be printed before each line.
     * This can be customized, made dynamic, etc.
     */
    const char* prompt = LOG_COLOR_I PROMPT_STR "> " LOG_RESET_COLOR;
    printf("\n"
           "CAN Open Master control CLI\n"
           "Type 'help' to get the list of commands.\n"
           "Use UP/DOWN arrows to navigate through command history.\n"
           "Press TAB when typing command name to auto-complete.\n"
           "Press Enter or Ctrl+C will terminate the console environment.\n");
    /* Figure out if the terminal supports escape sequences */
    int probe_status = linenoiseProbe();
    if (probe_status) { /* zero indicates success */
        printf("\r\n"
               "Your terminal application does not support escape sequences.\n"
               "Line editing and history features are disabled.\n"
               "On Windows, try using Putty instead.\n");
        linenoiseSetDumbMode(1);
#if CONFIG_LOG_COLORS
        /* Since the terminal doesn't support escape sequences,
         * don't use color codes in the prompt.
         */
        prompt = PROMPT_STR "> ";
		
#endif //CONFIG_LOG_COLORS
    }
/*================================================================================*/
		ESP_LOGE("mainTask", "Starting Application");
		coMainTaskArgs.callback = &coMainTask;
		coMainTaskArgs.name = "coMainTask";
		CO_NMT_reset_cmd_t reset = CO_RESET_NOT;
		vTaskDelay(BOOT_WAIT / portTICK_PERIOD_MS);
		while (reset != CO_RESET_APP)
		{
				ESP_LOGE("mainTask", "First while loop");
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
					
						uint16_t len = 0;
						len =  CO_OD_Entry_Length(CO->SDO[0], 0x6304, 0x00);
						ESP_LOGE("maintask", "1008 OD Length: %d", len);
						uint8_t sdo_rx_data_buffer[len];
						memset(sdo_rx_data_buffer, 0, sizeof(sdo_rx_data_buffer));
						uint8_t sdo_tx_data_buffer[4] = {0xAA, 0xBB, 0xCC, 0xDD};
						uint8_t sdo_tx_data_byte = 1;
						uint8_t sdo_tx_data_byte_2 = 0;
					//	const twai_message_t msg_buffer = {.identifier = 0x61A, .data_length_code = 8, .data = {0x4C, 0x08,  0x10, 0x00, 0x00, 0x00, 0x00, 0x00} };
					ESP_LOGE("mainTask", "Slave device name: %c %c %c %c %c %c %c %c %c %c %c %c %c\n\r ", 
																													sdo_rx_data_buffer[0],
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
																													sdo_rx_data_buffer[12]
																													);
				int i = 0;
				int k = 0;
				printf("esp32>\n");
				while (reset == CO_RESET_NOT)
				{
/*============================CMD==============================*/
						cmd_exe();
/*==========================================================*/
						
						/*download*/
						// CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x1008, 0, sdo_rx_data_buffer, 13, 0);
						// dunker_coProcessDownloadSDO();
						/* upload*/	
						// request every 10 s
						if ((k % 100) == 0) {
						// twai_transmit(&msg_buffer, 1000);
							ESP_LOGE("maintask", "beggining of a While");
							CO_SDOclientUploadInitiate(CO->SDOclient[0], 0x6304, 0, sdo_rx_data_buffer, len, 0);
							int j = dunker_coProcessUploadSDO();

							ESP_LOGE("mainTask", "Slave device name: %c %c %c %c %c %c %c %c %c %c %c %c %c\n\r Error:  %d", 
																													sdo_rx_data_buffer[0],
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
																													sdo_rx_data_buffer[12],
																													j);
						k = 0;
						}
						// if ( i == 10) {
						// 	CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x6303,  0x00, &sdo_tx_data_byte, 1, 0);
						// 	dunker_coProcessDownloadSDO(); 
						// 	ESP_LOGE("mainTask", "data was downlaoded to server"); 
						// } else if (i == 20) {
						// 	CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x6303,  0x00, &sdo_tx_data_byte_2, 1, 0);
						// 	dunker_coProcessDownloadSDO(); 
						// 	ESP_LOGE("mainTask", "data was downlaoded to server");
						// 	i = 0; 
						// }																				
						// i++;
						k++;	

							// if ((i % 150) == 0) {
							// 	CO_SDOclientUploadInitiate(CO->SDOclient[0], 0x6305, 0x00, sdo_rx_data_buffer, 13, 0);
							// 	int j = dunker_coProcessUploadSDO();
							// 	ESP_LOGE("mainTask", "Slave device name: %x %x %x %x %x %x %x %x %x %x %x %x %x\n\r Error:  %d", 
							// 																						sdo_rx_data_buffer[0],
							// 																						sdo_rx_data_buffer[1],
							// 																						sdo_rx_data_buffer[2],
							// 																						sdo_rx_data_buffer[3],
							// 																						sdo_rx_data_buffer[4],
							// 																						sdo_rx_data_buffer[5],
							// 																						sdo_rx_data_buffer[6],
							// 																						sdo_rx_data_buffer[7],
							// 																						sdo_rx_data_buffer[8],
							// 																						sdo_rx_data_buffer[9],
							// 																						sdo_rx_data_buffer[10],
							// 																						sdo_rx_data_buffer[11],
							// 																						sdo_rx_data_buffer[12],
							// 																											j);
							// }
						// }	
																										
						/* loop for normal program execution ******************************************/
						reset = CO_process(CO, coInterruptCounterDiff, NULL);
						//ESP_LOGE("mainTask", "CO_Process init");
						// /* Nonblocking application code may go here. */
						// if (counter == 0)
						// {		
						// 		ESP_LOGE("mainTask", "GET salve dev name: ");																						
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

						vTaskDelay(MAIN_WAIT / portTICK_PERIOD_MS);
				}
		}
		/* program exit
		 * ***************************************************************/
/*=========================console terminate===================================*/
		ESP_LOGE(TAG, "Error or end-of-input, terminating console");
    	esp_console_deinit();
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



