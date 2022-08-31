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
        CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x6303,  0x00, &sdo_tx_data_GIMLI_open, 1, 0);
	    int err =  dunker_coProcessDownloadSDO();
        if  (err < 0 ) {
            ESP_LOGE("GIMLI_CONTROL", "failed to send SDO:\n err code: %d", err);
        } 
    } else if (state == 0) {
        CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x6303,  0x00, &sdo_tx_data_GIMLI_close, 1, 0);
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
        CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x6302,  0x00, &sdo_tx_data_central_support_open, 1, 0);
	    int err =  dunker_coProcessDownloadSDO();
        if  (err < 0 ) {
            ESP_LOGE("CENTRAL_SUPPORT_CONTROL", "failed to send SDO:\n err code: %d", err);
        } 
    } else if (state == 0) {
        CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x6302,  0x00, &sdo_tx_data_central_support_close, 1, 0);
	    int err =  dunker_coProcessDownloadSDO();
        if  (err < 0 ) {
            ESP_LOGE("CENTRAL_SUPPORT_CONTROL", "failed to send SDO:\n err code: %d", err);
        } 
    }
}