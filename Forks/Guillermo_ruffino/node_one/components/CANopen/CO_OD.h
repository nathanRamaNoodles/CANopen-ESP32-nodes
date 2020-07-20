// clang-format off
/*******************************************************************************

   File - CO_OD.c/CO_OD.h
   CANopen Object Dictionary.

   This file was automatically generated with libedssharp Object
   Dictionary Editor v0.8-0-gb60f4eb   DON'T EDIT THIS FILE MANUALLY !!!!
*******************************************************************************/


#ifndef CO_OD_H_
#define CO_OD_H_

/*******************************************************************************
   CANopen DATA TYPES
*******************************************************************************/
   typedef bool_t       BOOLEAN;
   typedef uint8_t      UNSIGNED8;
   typedef uint16_t     UNSIGNED16;
   typedef uint32_t     UNSIGNED32;
   typedef uint64_t     UNSIGNED64;
   typedef int8_t       INTEGER8;
   typedef int16_t      INTEGER16;
   typedef int32_t      INTEGER32;
   typedef int64_t      INTEGER64;
   typedef float32_t    REAL32;
   typedef float64_t    REAL64;
   typedef char_t       VISIBLE_STRING;
   typedef oChar_t      OCTET_STRING;

   #ifdef DOMAIN
   #undef DOMAIN
   #endif

   typedef domain_t     DOMAIN;

#ifndef timeOfDay_t
    typedef union {
        unsigned long long ullValue;
        struct {
            unsigned long ms:28;
            unsigned reserved:4;
            unsigned days:16;
            unsigned reserved2:16;
        };
    }timeOfDay_t;
#endif

    typedef timeOfDay_t TIME_OF_DAY;
    typedef timeOfDay_t TIME_DIFFERENCE;


/*******************************************************************************
   FILE INFO:
      FileName:     Desaster4.eds
      FileVersion:  1
      CreationTime: 12:05PM
      CreationDate: 03-30-2020
      CreatedBy:    Alexander Miller, Mathias Parys
******************************************************************************/


/*******************************************************************************
   DEVICE INFO:
      VendorName:     IDiAL IMSL - FH Dortmund
      VendorNumber    1
      ProductName:    IMSL-ESP-Desaster4-Node
      ProductNumber:  1
******************************************************************************/


/*******************************************************************************
   FEATURES
*******************************************************************************/
  #define CO_NO_SYNC                     1   //Associated objects: 1005-1007
  #define CO_NO_EMERGENCY                1   //Associated objects: 1014, 1015
  #define CO_NO_TIME                     0   //Associated objects: 1012, 1013
  #define CO_NO_SDO_SERVER               1   //Associated objects: 1200-127F
  #define CO_NO_SDO_CLIENT               1   //Associated objects: 1280-12FF
  #define CO_NO_LSS_SERVER               0   //LSS Slave
  #define CO_NO_LSS_CLIENT               0   //LSS Master
  #define CO_NO_RPDO                     5   //Associated objects: 14xx, 16xx
  #define CO_NO_TPDO                     4   //Associated objects: 18xx, 1Axx
  #define CO_NO_NMT_MASTER               1


/*******************************************************************************
   OBJECT DICTIONARY
*******************************************************************************/
   #define CO_OD_NoOfElements             57


/*******************************************************************************
   TYPE DEFINITIONS FOR RECORDS
*******************************************************************************/
/*1018    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     vendorID;
               UNSIGNED32     productCode;
               UNSIGNED32     revisionNumber;
               UNSIGNED32     serialNumber;
               }              OD_identity_t;
/*1200    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     COB_IDClientToServer;
               UNSIGNED32     COB_IDServerToClient;
               }              OD_SDOServerParameter_t;
/*1280    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     COB_IDClientToServer;
               UNSIGNED32     COB_IDServerToClient;
               UNSIGNED8      nodeIDOfTheSDOServer;
               }              OD_SDOClientParameter_t;
/*1400    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     COB_IDUsedByRPDO;
               UNSIGNED8      transmissionType;
               }              OD_RPDOCommunicationParameter_t;
/*1600    */ typedef struct {
               UNSIGNED8      numberOfMappedObjects;
               UNSIGNED32     mappedObject1;
               UNSIGNED32     mappedObject2;
               UNSIGNED32     mappedObject3;
               UNSIGNED32     mappedObject4;
               UNSIGNED32     mappedObject5;
               UNSIGNED32     mappedObject6;
               UNSIGNED32     mappedObject7;
               UNSIGNED32     mappedObject8;
               }              OD_RPDOMappingParameter_t;
/*1800    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     COB_IDUsedByTPDO;
               UNSIGNED8      transmissionType;
               UNSIGNED16     inhibitTime;
               UNSIGNED8      compatibilityEntry;
               UNSIGNED16     eventTimer;
               UNSIGNED8      SYNCStartValue;
               }              OD_TPDOCommunicationParameter_t;
/*1a00    */ typedef struct {
               UNSIGNED8      numberOfMappedObjects;
               UNSIGNED32     mappedObject1;
               UNSIGNED32     mappedObject2;
               UNSIGNED32     mappedObject3;
               UNSIGNED32     mappedObject4;
               UNSIGNED32     mappedObject5;
               UNSIGNED32     mappedObject6;
               UNSIGNED32     mappedObject7;
               UNSIGNED32     mappedObject8;
               }              OD_TPDOMappingParameter_t;

/*******************************************************************************
   TYPE DEFINITIONS FOR OBJECT DICTIONARY INDEXES

   some of those are redundant with CO_SDO.h CO_ObjDicId_t <Common CiA301 object 
   dictionary entries>
*******************************************************************************/
/*1000 */
        #define OD_1000_deviceType                                  0x1000

/*1001 */
        #define OD_1001_errorRegister                               0x1001

/*1003 */
        #define OD_1003_preDefinedErrorField                        0x1003

        #define OD_1003_0_preDefinedErrorField_maxSubIndex          0
        #define OD_1003_1_preDefinedErrorField_standardErrorField   1
        #define OD_1003_2_preDefinedErrorField_standardErrorField   2
        #define OD_1003_3_preDefinedErrorField_standardErrorField   3
        #define OD_1003_4_preDefinedErrorField_standardErrorField   4
        #define OD_1003_5_preDefinedErrorField_standardErrorField   5
        #define OD_1003_6_preDefinedErrorField_standardErrorField   6
        #define OD_1003_7_preDefinedErrorField_standardErrorField   7
        #define OD_1003_8_preDefinedErrorField_standardErrorField   8

/*1005 */
        #define OD_1005_COB_ID_SYNCMessage                          0x1005

/*1006 */
        #define OD_1006_communicationCyclePeriod                    0x1006

/*1007 */
        #define OD_1007_synchronousWindowLength                     0x1007

/*1008 */
        #define OD_1008_manufacturerDeviceName                      0x1008

/*1009 */
        #define OD_1009_hardwareVersion                             0x1009

/*100a */
        #define OD_100a_softwareVersion                             0x100a

/*1014 */
        #define OD_1014_COB_ID_EMCY                                 0x1014

/*1015 */
        #define OD_1015_inhibitTimeEMCY                             0x1015

/*1016 */
        #define OD_1016_consumerHeartbeatTime                       0x1016

        #define OD_1016_0_consumerHeartbeatTime_maxSubIndex         0
        #define OD_1016_1_consumerHeartbeatTime_consumerHeartbeatTime 1
        #define OD_1016_2_consumerHeartbeatTime_consumerHeartbeatTime 2
        #define OD_1016_3_consumerHeartbeatTime_consumerHeartbeatTime 3
        #define OD_1016_4_consumerHeartbeatTime_consumerHeartbeatTime 4

/*1017 */
        #define OD_1017_producerHeartbeatTime                       0x1017

/*1018 */
        #define OD_1018_identity                                    0x1018

        #define OD_1018_0_identity_maxSubIndex                      0
        #define OD_1018_1_identity_vendorID                         1
        #define OD_1018_2_identity_productCode                      2
        #define OD_1018_3_identity_revisionNumber                   3
        #define OD_1018_4_identity_serialNumber                     4

/*1019 */
        #define OD_1019_synchronousCounterOverflowValue             0x1019

/*1029 */
        #define OD_1029_errorBehavior                               0x1029

        #define OD_1029_0_errorBehavior_maxSubIndex                 0
        #define OD_1029_1_errorBehavior_communication               1
        #define OD_1029_2_errorBehavior_communicationOther          2
        #define OD_1029_3_errorBehavior_communicationPassive        3
        #define OD_1029_4_errorBehavior_generic                     4
        #define OD_1029_5_errorBehavior_deviceProfile               5
        #define OD_1029_6_errorBehavior_manufacturerSpecific        6

/*1200 */
        #define OD_1200_SDOServerParameter                          0x1200

        #define OD_1200_0_SDOServerParameter_maxSubIndex            0
        #define OD_1200_1_SDOServerParameter_COB_IDClientToServer   1
        #define OD_1200_2_SDOServerParameter_COB_IDServerToClient   2

/*1280 */
        #define OD_1280_SDOClientParameter                          0x1280

        #define OD_1280_0_SDOClientParameter_maxSubIndex            0
        #define OD_1280_1_SDOClientParameter_COB_IDClientToServer   1
        #define OD_1280_2_SDOClientParameter_COB_IDServerToClient   2
        #define OD_1280_3_SDOClientParameter_nodeIDOfTheSDOServer   3

/*1400 */
        #define OD_1400_RPDOCommunicationParameter                  0x1400

        #define OD_1400_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_1400_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_1400_2_RPDOCommunicationParameter_transmissionType 2

/*1401 */
        #define OD_1401_RPDOCommunicationParameter                  0x1401

        #define OD_1401_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_1401_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_1401_2_RPDOCommunicationParameter_transmissionType 2

/*1402 */
        #define OD_1402_RPDOCommunicationParameter                  0x1402

        #define OD_1402_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_1402_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_1402_2_RPDOCommunicationParameter_transmissionType 2

/*1403 */
        #define OD_1403_RPDOCommunicationParameter                  0x1403

        #define OD_1403_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_1403_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_1403_2_RPDOCommunicationParameter_transmissionType 2

/*1404 */
        #define OD_1404_RPDOCommunicationParameter                  0x1404

        #define OD_1404_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_1404_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_1404_2_RPDOCommunicationParameter_transmissionType 2

/*1600 */
        #define OD_1600_RPDOMappingParameter                        0x1600

        #define OD_1600_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_1600_1_RPDOMappingParameter_mappedObject1        1
        #define OD_1600_2_RPDOMappingParameter_mappedObject2        2
        #define OD_1600_3_RPDOMappingParameter_mappedObject3        3
        #define OD_1600_4_RPDOMappingParameter_mappedObject4        4
        #define OD_1600_5_RPDOMappingParameter_mappedObject5        5
        #define OD_1600_6_RPDOMappingParameter_mappedObject6        6
        #define OD_1600_7_RPDOMappingParameter_mappedObject7        7
        #define OD_1600_8_RPDOMappingParameter_mappedObject8        8

/*1601 */
        #define OD_1601_RPDOMappingParameter                        0x1601

        #define OD_1601_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_1601_1_RPDOMappingParameter_mappedObject1        1
        #define OD_1601_2_RPDOMappingParameter_mappedObject2        2
        #define OD_1601_3_RPDOMappingParameter_mappedObject3        3
        #define OD_1601_4_RPDOMappingParameter_mappedObject4        4
        #define OD_1601_5_RPDOMappingParameter_mappedObject5        5
        #define OD_1601_6_RPDOMappingParameter_mappedObject6        6
        #define OD_1601_7_RPDOMappingParameter_mappedObject7        7
        #define OD_1601_8_RPDOMappingParameter_mappedObject8        8

/*1602 */
        #define OD_1602_RPDOMappingParameter                        0x1602

        #define OD_1602_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_1602_1_RPDOMappingParameter_mappedObject1        1
        #define OD_1602_2_RPDOMappingParameter_mappedObject2        2
        #define OD_1602_3_RPDOMappingParameter_mappedObject3        3
        #define OD_1602_4_RPDOMappingParameter_mappedObject4        4
        #define OD_1602_5_RPDOMappingParameter_mappedObject5        5
        #define OD_1602_6_RPDOMappingParameter_mappedObject6        6
        #define OD_1602_7_RPDOMappingParameter_mappedObject7        7
        #define OD_1602_8_RPDOMappingParameter_mappedObject8        8

/*1603 */
        #define OD_1603_RPDOMappingParameter                        0x1603

        #define OD_1603_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_1603_1_RPDOMappingParameter_mappedObject1        1
        #define OD_1603_2_RPDOMappingParameter_mappedObject2        2
        #define OD_1603_3_RPDOMappingParameter_mappedObject3        3
        #define OD_1603_4_RPDOMappingParameter_mappedObject4        4
        #define OD_1603_5_RPDOMappingParameter_mappedObject5        5
        #define OD_1603_6_RPDOMappingParameter_mappedObject6        6
        #define OD_1603_7_RPDOMappingParameter_mappedObject7        7
        #define OD_1603_8_RPDOMappingParameter_mappedObject8        8

/*1604 */
        #define OD_1604_RPDOMappingParameter                        0x1604

        #define OD_1604_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_1604_1_RPDOMappingParameter_mappedObject1        1
        #define OD_1604_2_RPDOMappingParameter_mappedObject2        2
        #define OD_1604_3_RPDOMappingParameter_mappedObject3        3
        #define OD_1604_4_RPDOMappingParameter_mappedObject4        4
        #define OD_1604_5_RPDOMappingParameter_mappedObject5        5
        #define OD_1604_6_RPDOMappingParameter_mappedObject6        6
        #define OD_1604_7_RPDOMappingParameter_mappedObject7        7
        #define OD_1604_8_RPDOMappingParameter_mappedObject8        8

/*1800 */
        #define OD_1800_TPDOCommunicationParameter                  0x1800

        #define OD_1800_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_1800_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_1800_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_1800_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_1800_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_1800_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_1800_6_TPDOCommunicationParameter_SYNCStartValue 6

/*1801 */
        #define OD_1801_TPDOCommunicationParameter                  0x1801

        #define OD_1801_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_1801_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_1801_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_1801_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_1801_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_1801_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_1801_6_TPDOCommunicationParameter_SYNCStartValue 6

/*1802 */
        #define OD_1802_TPDOCommunicationParameter                  0x1802

        #define OD_1802_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_1802_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_1802_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_1802_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_1802_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_1802_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_1802_6_TPDOCommunicationParameter_SYNCStartValue 6

/*1803 */
        #define OD_1803_TPDOCommunicationParameter                  0x1803

        #define OD_1803_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_1803_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_1803_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_1803_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_1803_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_1803_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_1803_6_TPDOCommunicationParameter_SYNCStartValue 6

/*1a00 */
        #define OD_1a00_TPDOMappingParameter                        0x1a00

        #define OD_1a00_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a00_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a00_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a00_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a00_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a00_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a00_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a00_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a00_8_TPDOMappingParameter_mappedObject8        8

/*1a01 */
        #define OD_1a01_TPDOMappingParameter                        0x1a01

        #define OD_1a01_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a01_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a01_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a01_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a01_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a01_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a01_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a01_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a01_8_TPDOMappingParameter_mappedObject8        8

/*1a02 */
        #define OD_1a02_TPDOMappingParameter                        0x1a02

        #define OD_1a02_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a02_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a02_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a02_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a02_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a02_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a02_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a02_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a02_8_TPDOMappingParameter_mappedObject8        8

/*1a03 */
        #define OD_1a03_TPDOMappingParameter                        0x1a03

        #define OD_1a03_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a03_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a03_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a03_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a03_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a03_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a03_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a03_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a03_8_TPDOMappingParameter_mappedObject8        8

/*1f80 */
        #define OD_1f80_NMTStartup                                  0x1f80

/*2100 */
        #define OD_2100_errorStatusBits                             0x2100

/*6000 */
        #define OD_6000_gyro_status_register                        0x6000

        #define OD_6000_0_gyro_status_register_maxSubIndex          0
        #define OD_6000_1_gyro_status_register_status               1

/*6001 */
        #define OD_6001_gyro_command_register                       0x6001

        #define OD_6001_0_gyro_command_register_maxSubIndex         0
        #define OD_6001_1_gyro_command_register_command             1

/*6002 */
        #define OD_6002_gyro_temperature_register                   0x6002

        #define OD_6002_0_gyro_temperature_register_maxSubIndex     0
        #define OD_6002_1_gyro_temperature_register_temperature     1

/*6003 */
        #define OD_6003_gyro_angle_register                         0x6003

        #define OD_6003_0_gyro_angle_register_maxSubIndex           0
        #define OD_6003_1_gyro_angle_register_angle                 1

/*6004 */
        #define OD_6004_gyro_lifecounter_register                   0x6004

        #define OD_6004_0_gyro_lifecounter_register_maxSubIndex     0
        #define OD_6004_1_gyro_lifecounter_register_lifecounter     1

/*6100 */
        #define OD_6100_hatox_status_register                       0x6100

        #define OD_6100_0_hatox_status_register_maxSubIndex         0
        #define OD_6100_1_hatox_status_register_analog_data_0       1
        #define OD_6100_2_hatox_status_register_analog_data_1       2
        #define OD_6100_3_hatox_status_register_analog_data_2       3
        #define OD_6100_4_hatox_status_register_analog_data_3       4
        #define OD_6100_5_hatox_status_register_analog_data_4       5
        #define OD_6100_6_hatox_status_register_digital_data_0      6
        #define OD_6100_7_hatox_status_register_digital_data_1      7

/*6101 */
        #define OD_6101_hatox_command_register                      0x6101

        #define OD_6101_0_hatox_command_register_maxSubIndex        0
        #define OD_6101_1_hatox_command_register_line               1
        #define OD_6101_2_hatox_command_register_column             2
        #define OD_6101_3_hatox_command_register_char_0             3
        #define OD_6101_4_hatox_command_register_char_1             4
        #define OD_6101_5_hatox_command_register_char_2             5
        #define OD_6101_6_hatox_command_register_char_3             6
        #define OD_6101_7_hatox_command_register_char_4             7
        #define OD_6101_8_hatox_command_register_char_5             8

/*6200 */
        #define OD_6200_motor_0_device_command                      0x6200

/*6201 */
        #define OD_6201_motor_0_error_register                      0x6201

/*6202 */
        #define OD_6202_motor_0_status_register                     0x6202

/*6203 */
        #define OD_6203_motor_0_mode_of_operation                   0x6203

/*6204 */
        #define OD_6204_motor_0_power_enable                        0x6204

/*6205 */
        #define OD_6205_motor_0_velocity_target_value               0x6205

/*6300 */
        #define OD_6300_motor_1_device_command                      0x6300

/*6301 */
        #define OD_6301_motor_1_error_register                      0x6301

/*6302 */
        #define OD_6302_motor_1_status_register                     0x6302

/*6303 */
        #define OD_6303_motor_1_mode_of_operation                   0x6303

/*6304 */
        #define OD_6304_motor_1_power_enable                        0x6304

/*6305 */
        #define OD_6305_motor_1_velocity_target_value               0x6305

/*******************************************************************************
   STRUCTURES FOR VARIABLES IN DIFFERENT MEMORY LOCATIONS
*******************************************************************************/
#define  CO_OD_FIRST_LAST_WORD     0x55 //Any value from 0x01 to 0xFE. If changed, EEPROM will be reinitialized.

/***** Structure for ROM variables ********************************************/
struct sCO_OD_ROM{
               UNSIGNED32     FirstWord;

/*1400      */ OD_RPDOCommunicationParameter_t RPDOCommunicationParameter[5];
/*1600      */ OD_RPDOMappingParameter_t RPDOMappingParameter[5];
/*1800      */ OD_TPDOCommunicationParameter_t TPDOCommunicationParameter[4];
/*1a00      */ OD_TPDOMappingParameter_t TPDOMappingParameter[4];

               UNSIGNED32     LastWord;
};

/***** Structure for RAM variables ********************************************/
struct sCO_OD_RAM{
               UNSIGNED32     FirstWord;

/*1000      */ UNSIGNED32      deviceType;
/*1001      */ UNSIGNED8       errorRegister;
/*1003      */ UNSIGNED32      preDefinedErrorField[8];
/*1005      */ UNSIGNED32      COB_ID_SYNCMessage;
/*1006      */ UNSIGNED32      communicationCyclePeriod;
/*1007      */ UNSIGNED32      synchronousWindowLength;
/*1008      */ VISIBLE_STRING  manufacturerDeviceName[13];
/*1009      */ VISIBLE_STRING  hardwareVersion[4];
/*100a      */ VISIBLE_STRING  softwareVersion[4];
/*1014      */ UNSIGNED32      COB_ID_EMCY;
/*1015      */ UNSIGNED16      inhibitTimeEMCY;
/*1016      */ UNSIGNED32      consumerHeartbeatTime[4];
/*1017      */ UNSIGNED16      producerHeartbeatTime;
/*1018      */ OD_identity_t   identity;
/*1019      */ UNSIGNED8       synchronousCounterOverflowValue;
/*1029      */ UNSIGNED8       errorBehavior[6];
/*1200      */ OD_SDOServerParameter_t SDOServerParameter[1];
/*1280      */ OD_SDOClientParameter_t SDOClientParameter[1];
/*1f80      */ UNSIGNED32      NMTStartup;
/*2100      */ OCTET_STRING    errorStatusBits[10];
/*6000      */ UNSIGNED8       gyro_status_register[1];
/*6001      */ UNSIGNED8       gyro_command_register[1];
/*6002      */ INTEGER16       gyro_temperature_register[1];
/*6003      */ REAL32          gyro_angle_register[1];
/*6004      */ UNSIGNED8       gyro_lifecounter_register[1];
/*6100      */ UNSIGNED8       hatox_status_register[7];
/*6101      */ UNSIGNED8       hatox_command_register[8];
/*6200      */ UNSIGNED8       motor_0_device_command;
/*6201      */ INTEGER16       motor_0_error_register;
/*6202      */ UNSIGNED32      motor_0_status_register;
/*6203      */ UNSIGNED8       motor_0_mode_of_operation;
/*6204      */ UNSIGNED8       motor_0_power_enable;
/*6205      */ INTEGER32       motor_0_velocity_target_value;
/*6300      */ UNSIGNED8       motor_1_device_command;
/*6301      */ INTEGER16       motor_1_error_register;
/*6302      */ UNSIGNED32      motor_1_status_register;
/*6303      */ UNSIGNED8       motor_1_mode_of_operation;
/*6304      */ UNSIGNED8       motor_1_power_enable;
/*6305      */ INTEGER32       motor_1_velocity_target_value;

               UNSIGNED32     LastWord;
};

/***** Structure for EEPROM variables ********************************************/
struct sCO_OD_EEPROM{
               UNSIGNED32     FirstWord;


               UNSIGNED32     LastWord;
};

/***** Declaration of Object Dictionary variables *****************************/
extern struct sCO_OD_ROM CO_OD_ROM;

extern struct sCO_OD_RAM CO_OD_RAM;

extern struct sCO_OD_EEPROM CO_OD_EEPROM;

/*******************************************************************************
   ALIASES FOR OBJECT DICTIONARY VARIABLES
*******************************************************************************/
/*1000, Data Type: UNSIGNED32 */
        #define OD_deviceType                                       CO_OD_RAM.deviceType

/*1001, Data Type: UNSIGNED8 */
        #define OD_errorRegister                                    CO_OD_RAM.errorRegister

/*1003, Data Type: UNSIGNED32, Array[8] */
        #define OD_preDefinedErrorField                             CO_OD_RAM.preDefinedErrorField
        #define ODL_preDefinedErrorField_arrayLength                8
        #define ODA_preDefinedErrorField_standardErrorField         0

/*1005, Data Type: UNSIGNED32 */
        #define OD_COB_ID_SYNCMessage                               CO_OD_RAM.COB_ID_SYNCMessage

/*1006, Data Type: UNSIGNED32 */
        #define OD_communicationCyclePeriod                         CO_OD_RAM.communicationCyclePeriod

/*1007, Data Type: UNSIGNED32 */
        #define OD_synchronousWindowLength                          CO_OD_RAM.synchronousWindowLength

/*1008, Data Type: VISIBLE_STRING */
        #define OD_manufacturerDeviceName                           CO_OD_RAM.manufacturerDeviceName
        #define ODL_manufacturerDeviceName_stringLength             13

/*1009, Data Type: VISIBLE_STRING */
        #define OD_hardwareVersion                                  CO_OD_RAM.hardwareVersion
        #define ODL_hardwareVersion_stringLength                    4

/*100a, Data Type: VISIBLE_STRING */
        #define OD_softwareVersion                                  CO_OD_RAM.softwareVersion
        #define ODL_softwareVersion_stringLength                    4

/*1014, Data Type: UNSIGNED32 */
        #define OD_COB_ID_EMCY                                      CO_OD_RAM.COB_ID_EMCY

/*1015, Data Type: UNSIGNED16 */
        #define OD_inhibitTimeEMCY                                  CO_OD_RAM.inhibitTimeEMCY

/*1016, Data Type: UNSIGNED32, Array[4] */
        #define OD_consumerHeartbeatTime                            CO_OD_RAM.consumerHeartbeatTime
        #define ODL_consumerHeartbeatTime_arrayLength               4
        #define ODA_consumerHeartbeatTime_consumerHeartbeatTime     0

/*1017, Data Type: UNSIGNED16 */
        #define OD_producerHeartbeatTime                            CO_OD_RAM.producerHeartbeatTime

/*1018, Data Type: identity_t */
        #define OD_identity                                         CO_OD_RAM.identity

/*1019, Data Type: UNSIGNED8 */
        #define OD_synchronousCounterOverflowValue                  CO_OD_RAM.synchronousCounterOverflowValue

/*1029, Data Type: UNSIGNED8, Array[6] */
        #define OD_errorBehavior                                    CO_OD_RAM.errorBehavior
        #define ODL_errorBehavior_arrayLength                       6
        #define ODA_errorBehavior_communication                     0
        #define ODA_errorBehavior_communicationOther                1
        #define ODA_errorBehavior_communicationPassive              2
        #define ODA_errorBehavior_generic                           3
        #define ODA_errorBehavior_deviceProfile                     4
        #define ODA_errorBehavior_manufacturerSpecific              5

/*1200, Data Type: SDOServerParameter_t */
        #define OD_SDOServerParameter                               CO_OD_RAM.SDOServerParameter

/*1280, Data Type: SDOClientParameter_t */
        #define OD_SDOClientParameter                               CO_OD_RAM.SDOClientParameter

/*1400, Data Type: RPDOCommunicationParameter_t */
        #define OD_RPDOCommunicationParameter                       CO_OD_ROM.RPDOCommunicationParameter

/*1600, Data Type: RPDOMappingParameter_t */
        #define OD_RPDOMappingParameter                             CO_OD_ROM.RPDOMappingParameter

/*1800, Data Type: TPDOCommunicationParameter_t */
        #define OD_TPDOCommunicationParameter                       CO_OD_ROM.TPDOCommunicationParameter

/*1a00, Data Type: TPDOMappingParameter_t */
        #define OD_TPDOMappingParameter                             CO_OD_ROM.TPDOMappingParameter

/*1f80, Data Type: UNSIGNED32 */
        #define OD_NMTStartup                                       CO_OD_RAM.NMTStartup

/*2100, Data Type: OCTET_STRING */
        #define OD_errorStatusBits                                  CO_OD_RAM.errorStatusBits
        #define ODL_errorStatusBits_stringLength                    10

/*6000, Data Type: UNSIGNED8, Array[1] */
        #define OD_gyro_status_register                             CO_OD_RAM.gyro_status_register
        #define ODL_gyro_status_register_arrayLength                1
        #define ODA_gyro_status_register_status                     0

/*6001, Data Type: UNSIGNED8, Array[1] */
        #define OD_gyro_command_register                            CO_OD_RAM.gyro_command_register
        #define ODL_gyro_command_register_arrayLength               1
        #define ODA_gyro_command_register_command                   0

/*6002, Data Type: INTEGER16, Array[1] */
        #define OD_gyro_temperature_register                        CO_OD_RAM.gyro_temperature_register
        #define ODL_gyro_temperature_register_arrayLength           1
        #define ODA_gyro_temperature_register_temperature           0

/*6003, Data Type: REAL32, Array[1] */
        #define OD_gyro_angle_register                              CO_OD_RAM.gyro_angle_register
        #define ODL_gyro_angle_register_arrayLength                 1
        #define ODA_gyro_angle_register_angle                       0

/*6004, Data Type: UNSIGNED8, Array[1] */
        #define OD_gyro_lifecounter_register                        CO_OD_RAM.gyro_lifecounter_register
        #define ODL_gyro_lifecounter_register_arrayLength           1
        #define ODA_gyro_lifecounter_register_lifecounter           0

/*6100, Data Type: UNSIGNED8, Array[7] */
        #define OD_hatox_status_register                            CO_OD_RAM.hatox_status_register
        #define ODL_hatox_status_register_arrayLength               7
        #define ODA_hatox_status_register_analog_data_0             0
        #define ODA_hatox_status_register_analog_data_1             1
        #define ODA_hatox_status_register_analog_data_2             2
        #define ODA_hatox_status_register_analog_data_3             3
        #define ODA_hatox_status_register_analog_data_4             4
        #define ODA_hatox_status_register_digital_data_0            5
        #define ODA_hatox_status_register_digital_data_1            6

/*6101, Data Type: UNSIGNED8, Array[8] */
        #define OD_hatox_command_register                           CO_OD_RAM.hatox_command_register
        #define ODL_hatox_command_register_arrayLength              8
        #define ODA_hatox_command_register_line                     0
        #define ODA_hatox_command_register_column                   1
        #define ODA_hatox_command_register_char_0                   2
        #define ODA_hatox_command_register_char_1                   3
        #define ODA_hatox_command_register_char_2                   4
        #define ODA_hatox_command_register_char_3                   5
        #define ODA_hatox_command_register_char_4                   6
        #define ODA_hatox_command_register_char_5                   7

/*6200, Data Type: UNSIGNED8 */
        #define OD_motor_0_device_command                           CO_OD_RAM.motor_0_device_command

/*6201, Data Type: INTEGER16 */
        #define OD_motor_0_error_register                           CO_OD_RAM.motor_0_error_register

/*6202, Data Type: UNSIGNED32 */
        #define OD_motor_0_status_register                          CO_OD_RAM.motor_0_status_register

/*6203, Data Type: UNSIGNED8 */
        #define OD_motor_0_mode_of_operation                        CO_OD_RAM.motor_0_mode_of_operation

/*6204, Data Type: UNSIGNED8 */
        #define OD_motor_0_power_enable                             CO_OD_RAM.motor_0_power_enable

/*6205, Data Type: INTEGER32 */
        #define OD_motor_0_velocity_target_value                    CO_OD_RAM.motor_0_velocity_target_value

/*6300, Data Type: UNSIGNED8 */
        #define OD_motor_1_device_command                           CO_OD_RAM.motor_1_device_command

/*6301, Data Type: INTEGER16 */
        #define OD_motor_1_error_register                           CO_OD_RAM.motor_1_error_register

/*6302, Data Type: UNSIGNED32 */
        #define OD_motor_1_status_register                          CO_OD_RAM.motor_1_status_register

/*6303, Data Type: UNSIGNED8 */
        #define OD_motor_1_mode_of_operation                        CO_OD_RAM.motor_1_mode_of_operation

/*6304, Data Type: UNSIGNED8 */
        #define OD_motor_1_power_enable                             CO_OD_RAM.motor_1_power_enable

/*6305, Data Type: INTEGER32 */
        #define OD_motor_1_velocity_target_value                    CO_OD_RAM.motor_1_velocity_target_value

#endif
// clang-format on
