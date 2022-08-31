#include "hatox.h"
#include "esp_log.h"

int8_t hatox_init(CO_t *CO)
{
		int8_t ret = 0;
		_CO = CO;
		return ret;
}

uint8_t hatox_getLeftStickX(void) {
		return OD_hatox_status_register[ODA_hatox_status_register_analog_data_2];
}
uint8_t hatox_getLeftStickY(void) {
		return OD_hatox_status_register[ODA_hatox_status_register_analog_data_1];
}
uint8_t hatox_getRightStickX(void) {
		return OD_hatox_status_register[ODA_hatox_status_register_analog_data_4];
}
uint8_t hatox_getRightStickY(void) {
		return OD_hatox_status_register[ODA_hatox_status_register_analog_data_3];
}
uint8_t hatox_getStickDir(void) {
		return OD_hatox_status_register[ODA_hatox_status_register_analog_data_0];
}
uint16_t hatox_getButtonStatus(void) {
		return (OD_hatox_status_register[ODA_hatox_status_register_digital_data_1] << 8) + OD_hatox_status_register[ODA_hatox_status_register_digital_data_0];
}

// uint8_t hatox_setText(uint8_t line, uint8_t column, char* text)
// {
// 		while (CO->TPDO[0]->sendRequest == 1)
// 		{
// 		}
// 		if (strlen(text) < 7 && strlen(text) > 0 && line > 0 && line < 4 && column > 0 && column < 17)
// 		{
// 				OD_motor_current_readings[ODA_motor_current_readings_line] = line;
// 				OD_motor_current_readings[ODA_motor_current_readings_column] = column;
// 				for (uint8_t i = 0; i < strlen(text); i++)
// 				{
// 						OD_motor_current_readings[ODA_motor_current_readings_char_0 + i] = text[i];
// 				}
// 				CO->TPDO[0]->sendRequest = 1;
// 				return 1;
// 		}
// 		return 0;
// }
