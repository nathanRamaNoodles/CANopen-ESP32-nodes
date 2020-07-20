#include "Gyro.h"

static CO_t *CO_local;

void gyro_init(CO_t *CO)
{
		CO_local = CO;
}

/* return current angle */
float gyro_getAngle(bool rad)
{
		if (rad)
		{
				return OD_gyro_angle_register[ODA_gyro_angle_register_angle];
		}
		return OD_gyro_angle_register[ODA_gyro_angle_register_angle] * CALCULATE_RAD_TO_DEGREE;
}

/* return gyroscope temperature */
float gyro_getTemperature(void)
{
		return ((float)(OD_gyro_temperature_register[ODA_gyro_temperature_register_temperature]) / 8);
}

/* return gyroscope life counter */
uint8_t gyro_getLifeCounter(void)
{
		return OD_gyro_lifecounter_register[ODA_gyro_lifecounter_register_lifecounter];
}

/* status flag: Value>0 if drift compensation active */
uint8_t gyro_isDriftCompenstaionActive(void)
{
		return OD_gyro_status_register[ODA_gyro_status_register_status] & STATUS_IS_DIRFT_COMPENSATION_ENABLE;
}

/* status flag: Value>0 if confirm angle to zero message */
uint8_t gyro_confirmAngleSetToZero(void)
{
		return OD_gyro_status_register[ODA_gyro_status_register_status] & STATUS_CONFIRM_ANGLE_TO_ZERO;
}

/* status flag: Value>0 if drift compensation is complete */
uint8_t gyro_isDriftCompenstaionComplete(void)
{
		return OD_gyro_status_register[ODA_gyro_status_register_status] & STATUS_IS_DRIFT_COMPENSATION_COMPLETE;
}

/* status flag: returns parameter error status. See datasheet for more information */
uint8_t gyro_GyroParameterError(void)
{
		return OD_gyro_status_register[ODA_gyro_status_register_status] & STATUS_PARAMETER_ERROR;
}

/* Set angle to zero flag. must be checked (confirmAngleSetToZero) and reset (disableAngleToZero) */
void gyro_enableAngleToZero(void)
{
		OD_gyro_command_register[ODA_gyro_command_register_command] |= CMD_ANGLE_TO_ZERO;
		CO_local->TPDO[0]->sendRequest = 1;
}

/* Reset angle to zero flag. must be checked (confirmAngleSetToZero) */
void gyro_disableAngleToZero(void)
{
		OD_gyro_command_register[ODA_gyro_command_register_command] &= ~CMD_ANGLE_TO_ZERO;
		CO_local->TPDO[0]->sendRequest = 1;
}

/* Set drift compensation flag. (must be checked (complete) & disabled again)*/
void gyro_enableDriftCompensation(void)
{
		OD_gyro_command_register[ODA_gyro_command_register_command] |= CMD_DRIFT_COMPENSATION;
		CO_local->TPDO[0]->sendRequest = 1;
}

/* Reset drift compensation flag */
void gyro_disableDriftCompensation(void)
{
		OD_gyro_command_register[ODA_gyro_command_register_command] &= ~CMD_DRIFT_COMPENSATION;
		CO_local->TPDO[0]->sendRequest = 1;
}
