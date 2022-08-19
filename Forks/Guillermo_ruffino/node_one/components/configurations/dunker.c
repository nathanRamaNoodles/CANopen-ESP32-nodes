#include "dunker.h"
#include "modul_config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "CANopen.h"
#include "CO_OD.h"
#include "esp_log.h"

// static motorRegister motor[MAX_MOTORS];

static CO_t *_CO = NULL;
static uint8_t _nodeId = 0x00;
static uint8_t _tpdoNum = 0x00;

/**
 * @brief Array for all Motors and the corresponding object-dictionary entries
 *
 */
static motorRegister motor[] =
{{&OD_motor_0_device_command, &OD_motor_0_error_register, &OD_motor_0_status_register, &OD_motor_0_mode_of_operation, &OD_motor_0_power_enable, &OD_motor_0_velocity_target_value},
 {&OD_motor_1_device_command, &OD_motor_1_error_register, &OD_motor_1_status_register, &OD_motor_1_mode_of_operation, &OD_motor_1_power_enable, &OD_motor_1_velocity_target_value}};

/**
 * @brief Number of created objects
 *
 */
static uint8_t numMotors = 0;

/**
 * @brief Motornumber of this object
 *
 */
uint8_t motorNumber = 0;

void dunker(void)
{
		motorNumber = numMotors;
		numMotors++;
}

int8_t dunker_init(CO_t *CO, uint8_t nodeId, uint8_t tpdoNum)
{
		int8_t ret = 0;
		_CO = CO;
		_nodeId = nodeId;
		_tpdoNum = tpdoNum;
		/*Configure PDO Mapping on Device 0x1A*/
		// uint32_t mappedRxObjects[] = {0x40000108, 0x40030108, 0x40040108, 0x43000120};
		// ret = dunker_mapRPDO(0, _nodeId, mappedRxObjects, 4);
		// uint32_t mappedTxObjects[] = {0x40020120, 0x40010110};
		// ret += dunker_mapTPDO(0, _nodeId, mappedTxObjects, 2, 0x100, 0x100);
		return ret;
}

int8_t dunker_clearError(void)
{
		*motor[motorNumber].command = CMD_ClearError;
		*motor[motorNumber].velocity = 0;
		CO->TPDO[_tpdoNum]->sendRequest = 1;
		return 0;
}

int8_t dunker_quickStop(void)
{
		*motor[motorNumber].command = CMD_QuickStop;
		*motor[motorNumber].velocity = 0;
		CO->TPDO[_tpdoNum]->sendRequest = 1;
		return 0;
}

int8_t dunker_halt(void)
{
		*motor[motorNumber].command = CMD_Halt;
		*motor[motorNumber].velocity = 0;
		CO->TPDO[_tpdoNum]->sendRequest = 1;
		return 0;
}

int8_t dunker_continueMovement(void)
{
		*motor[motorNumber].command = CMD_Continue;
		*motor[motorNumber].velocity = 0;
		CO->TPDO[_tpdoNum]->sendRequest = 1;
		return 0;
}

int8_t dunker_setEnable(uint8_t value)
{

		int8_t ret = 0;

		*motor[motorNumber].mode = OPERATION_MODE;
		*motor[motorNumber].power = 1;
		CO->TPDO[_tpdoNum]->sendRequest = 1;

		/*Check for fault condition*/
		if (!(*motor[motorNumber].status & STAT_Error))
		{
				if (value)
				{
						while (!(*motor[motorNumber].status & STAT_Enabled))
						{
								vTaskDelay(1 / portTICK_PERIOD_MS);
								*motor[motorNumber].mode = OPERATION_MODE;
								*motor[motorNumber].power = value;
								CO->TPDO[_tpdoNum]->sendRequest = 1;
						}
				}
				else
				{
						*motor[motorNumber].mode = OPERATION_MODE;
						*motor[motorNumber].power = value;
						CO->TPDO[_tpdoNum]->sendRequest = 1;
				}
		}
		else
		{
				ret = *motor[motorNumber].error; //Can't dis-/enable motor while in fault condition
				ESP_LOGE("Dunker.setEnable", "Can't dis-/enable motor while in fault condition!");
		}
		return ret;
}

int8_t dunker_setSpeed(int32_t speed)
{
		int8_t ret = 0;

		/*Check for fault condition*/
		if (!(*motor[motorNumber].status & STAT_Error))
		{
				*motor[motorNumber].velocity = speed;
				CO->TPDO[_tpdoNum]->sendRequest = 1;
		}
		else
		{
				ret = *motor[motorNumber].error; //Can't set speed while in fault condition
		}
		return ret;
}

int8_t dunker_coProcessUploadSDO(void)
{
		uint32_t SdoAbortCode = CO_SDO_AB_NONE;
		int8_t ret = 0;
		uint32_t dataSize = 0;
		uint16_t delay = 1;
		
		do
		{
				ret = CO_SDOclientUpload(_CO->SDOclient[0], 1, 5000, &dataSize, &SdoAbortCode);
				vTaskDelay((delay / portTICK_PERIOD_MS) < 1? 1:(10 / portTICK_PERIOD_MS)); //tikus verciam i mili sekundes ir darom  delay 1 milisekundes.

		} while (ret > 0);
		return ret;
}

int8_t dunker_coProcessDownloadSDO(void)
{
		uint32_t SdoAbortCode = CO_SDO_AB_NONE;
		int8_t ret = 0;
		do
		{
				ret = CO_SDOclientDownload(_CO->SDOclient[0], 1, 5000, &SdoAbortCode);
		} while (ret > 0);
		return ret;
}

int8_t dunker_mapRPDO(uint8_t pdoNumber, uint8_t nodeId, uint32_t *mappedObjects, uint8_t numMappedObjects)
{
		int8_t ret = 0;
		uint32_t v32 = 0; //Temporary Storage
		uint8_t v8 = 0; //Temporary Storage

		//RPDO Disable
		v32 = ((0x200 + nodeId + pdoNumber) | 0x80000000);
		ESP_LOGE("mainTask", "RPDO disable");
		CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x1400 + pdoNumber, 1, (uint8_t *)&v32, sizeof(v32), 0);
		ret = dunker_coProcessDownloadSDO();

		//RPDO Disable Mapping
		v8 = 0;
		ESP_LOGE("mainTask", "RPDO disable mapping");
		CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x1600 + pdoNumber, 0, (uint8_t *)&v8, sizeof(v8), 0);
		ret = dunker_coProcessDownloadSDO();

		//RPDO Mapping
		for (uint8_t i = 0; i < numMappedObjects; i++)
		{
				v32 = mappedObjects[i];
				ESP_LOGE("mainTask", "RPDO mapping");
				CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x1600 + pdoNumber, 1 + i, (uint8_t *)&v32, sizeof(v32), 0);
				ret = dunker_coProcessDownloadSDO();
		}

		//RPDO Enable Mapping
		v8 = numMappedObjects;
		ESP_LOGE("mainTask", "RPDO enable mapping");
		CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x1600 + pdoNumber, 0, (uint8_t *)&v8, sizeof(v8), 0);
		ret = dunker_coProcessDownloadSDO();

		//RPDO Enable
		v32 = (0x200 + nodeId + pdoNumber);
		ESP_LOGE("mainTask", "RPDO enable");
		CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x1400 + pdoNumber, 1, (uint8_t *)&v32, sizeof(v32), 0);
		ret = dunker_coProcessDownloadSDO();

		return ret;
}

int8_t dunker_mapTPDO(uint8_t pdoNumber, uint8_t nodeId, uint32_t *mappedObjects, uint8_t numMappedObjects, uint16_t eventTime, uint16_t inhibitTime)
{
		int8_t ret = 0;
		uint32_t v32 = 0;
		uint16_t v16 = 0;
		uint8_t v8 = 0;

		//TPDO Disable
		ESP_LOGE("mainTask", "TPDO disable");
		v32 = ((0x180 + nodeId + pdoNumber) | 0x80000000);
		CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x1800 + pdoNumber, 1, (uint8_t *)&v32, sizeof(v32), 0);
		ret = dunker_coProcessDownloadSDO();

		//TPDO Disable Mapping
		ESP_LOGE("mainTask", "TPDO disable mapping");
		v8 = 0;
		CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x1a00 + pdoNumber, 0, (uint8_t *)&v8, sizeof(v8), 0);
		ret = dunker_coProcessDownloadSDO();

		//TPDO Set Eventtime
		ESP_LOGE("mainTask", "TPDO set event time");
		v16 = eventTime;
		CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x1800 + pdoNumber, 5, (uint8_t *)&v16, sizeof(v16), 0);
		ret = dunker_coProcessDownloadSDO();

		//TPDO Set Inhibittime
		ESP_LOGE("mainTask", "TPDO inhibit");
		v16 = inhibitTime;
		CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x1800 + pdoNumber, 3, (uint8_t *)&v16, sizeof(v16), 0);
		ret = dunker_coProcessDownloadSDO();

		//TPDO Mapping
		ESP_LOGE("mainTask", "TPDO mapping");
		for (uint8_t i = 0; i < numMappedObjects; i++)
		{
				v32 = mappedObjects[i];
				CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x1a00 + pdoNumber, 1 + i, (uint8_t *)&v32, sizeof(v32), 0);
				ret = dunker_coProcessDownloadSDO();
		}

		//TPDO Enable Mapping
		ESP_LOGE("mainTask", "TPDO enable mapping");
		v8 = numMappedObjects;
		CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x1a00 + pdoNumber, 0, (uint8_t *)&v8, sizeof(v8), 0);
		ret = dunker_coProcessDownloadSDO();

		//TPDO Enable
		ESP_LOGE("mainTask", "TPDO enable");
		v32 = (0x180 + nodeId + pdoNumber);
		CO_SDOclientDownloadInitiate(CO->SDOclient[0], 0x1800 + pdoNumber, 1, (uint8_t *)&v32, sizeof(v32), 0);
		ret = dunker_coProcessDownloadSDO();

		return ret;
}
