#ifndef GYRO_H_
#define GYRO_H_

#include "CANopen.h"
#include <stdint.h>
#include "CO_OD.h"

static const uint8_t STATUS_IS_DIRFT_COMPENSATION_ENABLE = 0b00000001;
static const uint8_t STATUS_CONFIRM_ANGLE_TO_ZERO = 0b00000010;
static const uint8_t STATUS_IS_DRIFT_COMPENSATION_COMPLETE = 0b00010000;
static const uint8_t STATUS_PARAMETER_ERROR = 0b10000000;
static const uint8_t CMD_ANGLE_TO_ZERO = 0b00000010;
static const uint8_t CMD_DRIFT_COMPENSATION = 0b00000001;
static const float CALCULATE_RAD_TO_DEGREE = 57.2958f;


void gyro_init(CO_t *CO);

float gyro_getAngle(bool rad);     /* return current angle in degrees */

float gyro_getTemperature(void);     /* return gyroscope temperature */

uint8_t gyro_getLifeCounter(void);     /* return gyroscope life counter */

uint8_t gyro_isDriftCompenstaionActive(void);     /* status flag: Value>0 if drift compensation active */

uint8_t gyro_confirmAngleSetToZero(void);     /* status flag: Value>0 if confirm angle to zero message */

uint8_t gyro_isDriftCompenstaionComplete(void);     /* status flag: Value>0 if drift compensation is complete */

uint8_t gyro_GyroParameterError(void);     /* status flag: returns parameter error status. See datasheet for more information */

void gyro_enableAngleToZero(void);     /* Set angle to zero flag. must be checked (confirmAngleSetToZero) and reset (disableAngleToZero) */

void gyro_disableAngleToZero(void);     /* Reset angle to zero flag. must be checked (confirmAngleSetToZero) */

void gyro_enableDriftCompensation(void);     /* Set drift compensation flag. (must be checked (complete) & disabled again)*/

void gyro_disableDriftCompensation(void);     /* Reset drift compensation flag */

#endif
