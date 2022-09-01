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
/******************************************************************************
* Private types
******************************************************************************/

/******************************************************************************
* Private constants
******************************************************************************/

/******************************************************************************
* Private variables
******************************************************************************/

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
    uint8_t sdo_tx_auto_mode_disable = 0;

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