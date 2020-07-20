#ifndef DUNKER_H_
#define DUNKER_H_

#include <stdint.h>
#include "CANopen.h"
#include "CO_OD.h"

/*Constants*/
static const uint32_t STAT_Enabled = (1 << 0);
static const uint32_t STAT_Error = (1 << 1);
static const uint32_t STAT_Warning = (1 << 2);
static const uint32_t STAT_Moving = (1 << 3);
static const uint32_t STAT_Reached = (1 << 4);
static const uint32_t STAT_Limit = (1 << 5);
static const uint32_t STAT_FollowingError = (1 << 6);
static const uint32_t STAT_HomingDone = (1 << 7);
static const uint32_t STAT_Toggle = (1 << 8);
static const uint32_t STAT_CommandToggle = (1 << 9);
static const uint32_t STAT_CommandError = (1 << 10);
static const uint32_t STAT_StopOrHalt = (1 << 11);
static const uint32_t STAT_LimitCurrent = (1 << 12);
static const uint32_t STAT_LimitVel = (1 << 13);
static const uint32_t STAT_LimitPos = (1 << 14);
static const uint32_t STAT_LimitPWM = (1 << 15);
static const uint32_t STAT_LimitSetpointVq = (1 << 16);
static const uint32_t STAT_LimitSetpointVd = (1 << 17);
static const uint32_t STAT_ComOperational = (1 << 18);
static const uint32_t STAT_ComStarted = (1 << 19);
static const uint32_t STAT_OverTemperature = (1 << 20);
static const uint32_t STAT_MotorOvervoltage = (1 << 21);
static const uint32_t STAT_MotorUndervoltage = (1 << 22);
static const uint32_t STAT_Blockage = (1 << 23);
static const uint32_t STAT_ParamCmdExec = (1 << 24);
static const uint32_t STAT_BallastCircuit = (1 << 25);
static const uint32_t STAT_Direction = (1 << 26);
static const uint32_t STAT_Overload = (1 << 27);

static const uint8_t CMD_NOP = 0x0;
static const uint8_t CMD_ClearError = 0x1;
static const uint8_t CMD_QuickStop = 0x2;
static const uint8_t CMD_Halt = 0x3;
static const uint8_t CMD_Continue = 0x4;
static const uint8_t CMD_Update = 0x5;

static const int16_t OPERATION_MODE = 0x03; //Operation Mode 2 "Special Profile Velocity"

#define MAX_MOTORS 2 //Maximum number of motors

/**
 * @brief Struct for all nessesary registers in the object-dictionary
 *
 */
typedef struct motorRegister_s
{
		uint8_t *command;
		int16_t *error;
		uint32_t *status;
		uint8_t *mode;
		uint8_t *power;
		int32_t *velocity;
} motorRegister;

/**
 * @brief Construct a new Dunker object
 *
 */
void dunker(void);

/**
 * @brief Initialize motor by setting the RPDO- and TPDO-Mapping
 *
 * @param CO Pointer to CANopen object
 * @param nodeId CANopen node ID
 * @param tpdoNum Relevant TPDO-Number (Count from 0)
 * @return int8_t 0 = No Error, -n = Errorcode of mapRPDO + mapTPDO
 */
int8_t dunker_init(CO_t *CO, uint8_t nodeId, uint8_t tpdoNum);

/**
 * @brief Clear potential errors and reenable drive
 *
 * @return int8_t 0 = No Error, -n = Errorcode
 */
int8_t dunker_clearError(void);

/**
 * @brief Execute quick-stop (Quick-Stop-Deceleration). See also: "continueMovement(void)"
 *
 * @return int8_t 0 = No Error, -n = Errorcode
 */
int8_t dunker_quickStop(void);

/**
 * @brief Execute halt (General deceleration). See also: "continueMovement(void)"
 *
 * @return int8_t 0 = No Error, -n = Errorcode
 */
int8_t dunker_halt(void);

/**
 * @brief Continues movement after QuickStop or Halt.
 *
 * @return int8_t 0 = No Error, -n = Errorcode
 */
int8_t dunker_continueMovement(void);

/**
 * @brief Change motor status to "Operation enabled" or "Switch on disabled"
 *
 * @param value 1 = "Operation enabled" ; 0 = "Switch on disabled"
 * @return int8_t 0 = No Error, -n = Value of Motor Error-Register (see Motor-Documentation)
 */
int8_t dunker_setEnable(uint8_t value);

/**
 * @brief Set the motor velocity
 *
 * @param speed Motor velocity
 * @return int8_t 0 = No Error, -n = Value of Motor Error-Register (see Motor-Documentation)
 */
int8_t dunker_setSpeed(int32_t speed);

/**
 * @brief Process pending SDO-Download
 *
 * @return int8_t 0 = No Error, -n = Errorcode of "CO_SDOclientDownload(void)"
 */
int8_t dunker_coProcessDownloadSDO(void);

/**
 * @brief Process pending SDO-Upload
 *
 * @return int8_t 0 = No Error, -n = Errorcode of "CO_SDOclientUpload(void)"
 */
int8_t dunker_coProcessUploadSDO(void);

/**
 * @brief Changes the PDO-Mapping of given RPDO
 *
 * @param pdoNumber PDO to change (0-n)
 * @param nodeId Node ID
 * @param mappedObjects Array with objects to be mapped
 * @param numMappedObjects Nummber of mapped objects
 * @return int8_t 0 = No Error, -n = Errorcode of "coProcessDownloadSDO(void)"
 */
int8_t dunker_mapRPDO(uint8_t pdoNumber, uint8_t nodeId, uint32_t *mappedObjects, uint8_t numMappedObjects);

/**
 * @brief Changes the PDO-Mapping of given TPDO
 *
 * @param pdoNumber PDO to change (0-n)
 * @param nodeId Node ID
 * @param mappedObjects Array with objects to be mapped
 * @param numMappedObjects Nummber of mapped objects
 * @param eventTime TPDO event time in 100us steps
 * @param inhibitTime TPDO inhibit time in 100us steps
 * @return int8_t 0 = No Error, -n = Errorcode of "coProcessDownloadSDO(void)"
 */
int8_t dunker_mapTPDO(uint8_t pdoNumber, uint8_t nodeId, uint32_t *mappedObjects, uint8_t numMappedObjects, uint16_t eventTime, uint16_t inhibitTime);


#endif /* DUNKER_H_ */
