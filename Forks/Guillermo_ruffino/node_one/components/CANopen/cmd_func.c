/******************************************************************************
* Includes
******************************************************************************/
#include "cmd_func.h"
#include "esp_log.h"
#include "C:/Users/Dominykas/Desktop/CANOpen/CANopen-ESP32-nodes/Forks/Guillermo_ruffino/node_one/components/configurations/dunker.h"
//#include "CANopen.h"
/******************************************************************************
* Private definitions and macros
******************************************************************************/
#define GIMLI_CENTRAL_SUPPORT_SATE_OD_INDEX     0x6305
#define GIMLI_CENTRAL_SUPPORT_SATE_OD_SUBINDEX  0x00

#define LED_ENABLE_OD_INDEX                     0x6304
#define LED_ENABLE_OD_SUBINDEX                  0x00

#define GIMLI_SATE_OD_INDEX                     0x6303
#define GIMLI_STATE_OD_SUBINDEX                 0x00

#define CENTRAL_SUPPORT_SATE_OD_INDEX           0x6302
#define CENTRAL_SUPPORT_STATE_OD_SUBINDEX       0x00

#define AUTO_MODE_CONTROL_OD_INDEX              0x6300
#define AUTO_MODE_CONTROL_OD_SUBINDEX           0x00

#define CURRENT_MONITOR_ID              0x6101
#define CURRENT_MONITOR_DRIVER_1_SUBID  0X01
#define CURRENT_MONITOR_DRIVER_2_SUBID  0X02
#define CURRENT_MONITOR_DRIVER_3_SUBID  0X03
#define CURRENT_MONITOR_DRIVER_4_SUBID  0X04
#define CURRENT_MONITOR_DRIVER_5_SUBID  0X05

#define NO_OF_MOTORS                    5
/******************************************************************************
* Private types
******************************************************************************/

/******************************************************************************
* Private constants
******************************************************************************/

/******************************************************************************
* Private variables
******************************************************************************/
uint8_t current_measurement_motor_subid[NO_OF_MOTORS] = {0X01, 0x02, 0x03, 0x04, 0x05};
/******************************************************************************
* Exported variables and references
******************************************************************************/

/******************************************************************************
* Prototypes of private functions
******************************************************************************/

/******************************************************************************
* Definitions of private functions
******************************************************************************/

/******************************************************************************
* Definitions of exported functions
******************************************************************************/
void CMD_Send_Byte_GIMLI_Control (bool state) {
    uint8_t sdo_tx_data_GIMLI_open = 1;
    uint8_t sdo_tx_data_GIMLI_close = 0;

    if (state == 1) {
        CO_SDOclientDownloadInitiate(CO->SDOclient[0], GIMLI_SATE_OD_INDEX,  GIMLI_STATE_OD_SUBINDEX, &sdo_tx_data_GIMLI_open, 1, 0);
	    int err =  dunker_coProcessDownloadSDO();
        if  (err < 0 ) {
            ESP_LOGE("GIMLI_CONTROL", "failed to send SDO:\n err code: %d", err);
        } 
    } else if (state == 0) {
        CO_SDOclientDownloadInitiate(CO->SDOclient[0], GIMLI_SATE_OD_INDEX,  GIMLI_STATE_OD_SUBINDEX, &sdo_tx_data_GIMLI_close, 1, 0);
	    int err =  dunker_coProcessDownloadSDO();
        if  (err < 0 ) {
            ESP_LOGE("GIMLI_CONTROL", "failed to send SDO:\n err code: %d", err);
        } 
    }
}

void CMD_Send_Byte_Central_Control (bool state) {
    uint8_t sdo_tx_data_central_support_open = 1;
    uint8_t sdo_tx_data_central_support_close = 0;

    if (state == 1) {
        CO_SDOclientDownloadInitiate(CO->SDOclient[0], CENTRAL_SUPPORT_SATE_OD_INDEX,  CENTRAL_SUPPORT_STATE_OD_SUBINDEX, &sdo_tx_data_central_support_open, 1, 0);
	    int err =  dunker_coProcessDownloadSDO();
        if  (err < 0 ) {
            ESP_LOGE("CENTRAL_SUPPORT_CONTROL", "failed to send SDO\nError code: %d", err);
        } 
    } else if (state == 0) {
        CO_SDOclientDownloadInitiate(CO->SDOclient[0], CENTRAL_SUPPORT_SATE_OD_INDEX,  CENTRAL_SUPPORT_STATE_OD_SUBINDEX, &sdo_tx_data_central_support_close, 1, 0);
	    int err =  dunker_coProcessDownloadSDO();
        if  (err != 0 ) {
            ESP_LOGE("CENTRAL_SUPPORT_CONTROL", "failed to send SDO\nError code: %d", err);
        } 
    }
}

uint8_t CMD_Request_Upload_Status  (void) {
        uint8_t received_state = 0;
        uint16_t OD_entry_len = 0;

        OD_entry_len =  CO_OD_Entry_Length(CO->SDO[0], GIMLI_CENTRAL_SUPPORT_SATE_OD_INDEX, GIMLI_CENTRAL_SUPPORT_SATE_OD_SUBINDEX);
        OD_entry_len += 3;  //OD nuskaitymui is slave irenginio reikia  bent 4 baitu, nors OD dydis ir 1baito dydzio
 		uint8_t sdo_rx_data_buffer[OD_entry_len];
		memset(sdo_rx_data_buffer, 0, sizeof(sdo_rx_data_buffer));

        CO_SDOclientUploadInitiate(CO->SDOclient[0], GIMLI_CENTRAL_SUPPORT_SATE_OD_INDEX, GIMLI_CENTRAL_SUPPORT_SATE_OD_SUBINDEX, sdo_rx_data_buffer, OD_entry_len, 0);
		ESP_LOGE("Request_Status", "upload initiated, waitting for prodess");
        int err = dunker_coProcessUploadSDO();
        if (err != 0) {
            ESP_LOGE("Request_Status", "failed to send SDO\n Error code: %d", err);
            return received_state = 4;
        }
        received_state = sdo_rx_data_buffer[0];
        
        return  received_state;
}

void CMD_Send_Byte_Auto_Mode_Toggle (bool state) {
    uint8_t sdo_tx_auto_mode_enable = 1;
    uint8_t sdo_tx_auto_mode_disable = 2;

    if (state == 1) {
        CO_SDOclientDownloadInitiate(CO->SDOclient[0], AUTO_MODE_CONTROL_OD_INDEX,  AUTO_MODE_CONTROL_OD_SUBINDEX, &sdo_tx_auto_mode_enable, 1, 0);
	    int err =  dunker_coProcessDownloadSDO();
        if  (err < 0 ) {
            ESP_LOGE("CENTRAL_SUPPORT_CONTROL", "failed to send SDO\nError code: %d", err);
        } 
    } else if (state == 0) {
        CO_SDOclientDownloadInitiate(CO->SDOclient[0], AUTO_MODE_CONTROL_OD_INDEX,  AUTO_MODE_CONTROL_OD_SUBINDEX, &sdo_tx_auto_mode_disable, 1, 0);
	    int err =  dunker_coProcessDownloadSDO();
        if  (err != 0 ) {
            ESP_LOGE("CENTRAL_SUPPORT_CONTROL", "failed to send SDO\nError code: %d", err);
        } 
    }
}

float *CMD_Request_Upload_Current_of_selected_motor (uint8_t motor_no) {
        float  received_current = 0.00;
        float  received_current_of_all_motors[NO_OF_MOTORS];
        uint16_t OD_entry_len = 0;
        if (motor_no == 0) {
            OD_entry_len =  CO_OD_Entry_Length(CO->SDO[0], CURRENT_MONITOR_ID, CURRENT_MONITOR_DRIVER_1_SUBID);
            OD_entry_len += 3;  //OD nuskaitymui is slave irenginio reikia  bent 4 baitu, nors OD dydis ir 1baito dydzio
 		    uint8_t sdo_rx_data_buffer[OD_entry_len];
		    memset(sdo_rx_data_buffer, 0, sizeof(sdo_rx_data_buffer));
            
            for (int i = 0; i < NO_OF_MOTORS; i++) {
                CO_SDOclientUploadInitiate(CO->SDOclient[0], CURRENT_MONITOR_ID, current_measurement_motor_subid[i], sdo_rx_data_buffer, OD_entry_len, 0);
		        //ESP_LOGE("Request_Status", "upload initiated, waitting for prodess");
                int err = dunker_coProcessUploadSDO();
                // ESP_LOGE("Request_Status", "Error code: %d", err);
                if (err != 0) {
                    ESP_LOGE("Request_Status", "failed to send SDO\n Error code: %x", err);
                }

                //printf("current: %d %d %d %d in hex: %x %x %x %x/n", sdo_rx_data_buffer[0],sdo_rx_data_buffer[1],sdo_rx_data_buffer[2],sdo_rx_data_buffer[3], sdo_rx_data_buffer[0],sdo_rx_data_buffer[1],sdo_rx_data_buffer[2],sdo_rx_data_buffer[3]);
                memcpy(received_current_of_all_motors, sdo_rx_data_buffer, sizeof(received_current_of_all_motors[0]));
                printf("received current of %d motor: %f\n",i + 1 ,received_current_of_all_motors[i]);
            }
            return received_current_of_all_motors;
        } else{
            OD_entry_len =  CO_OD_Entry_Length(CO->SDO[0], CURRENT_MONITOR_ID, CURRENT_MONITOR_DRIVER_1_SUBID);
            OD_entry_len += 3;  //OD nuskaitymui is slave irenginio reikia  bent 4 baitu, nors OD dydis ir 1baito dydzio
 		    uint8_t sdo_rx_data_buffer[OD_entry_len];
		    memset(sdo_rx_data_buffer, 0, sizeof(sdo_rx_data_buffer));

            CO_SDOclientUploadInitiate(CO->SDOclient[0], CURRENT_MONITOR_ID, current_measurement_motor_subid[motor_no - 1], sdo_rx_data_buffer, OD_entry_len, 0);
		    //ESP_LOGE("Request_Status", "upload initiated, waitting for prodess");
            int err = dunker_coProcessUploadSDO();
            // ESP_LOGE("Request_Status", "Error code: %d", err);
            if (err != 0) {
                ESP_LOGE("Request_Status", "failed to send SDO\n Error code: %x", err);
            }

            //printf("current: %d %d %d %d in hex: %x %x %x %x/n", sdo_rx_data_buffer[0],sdo_rx_data_buffer[1],sdo_rx_data_buffer[2],sdo_rx_data_buffer[3], sdo_rx_data_buffer[0],sdo_rx_data_buffer[1],sdo_rx_data_buffer[2],sdo_rx_data_buffer[3]);
            memcpy(&received_current, sdo_rx_data_buffer, sizeof(received_current));
            printf("received current: %f\n", received_current);

            return  &received_current;
            }
}