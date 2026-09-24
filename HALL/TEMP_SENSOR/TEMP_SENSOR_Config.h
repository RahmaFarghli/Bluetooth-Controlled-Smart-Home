#ifndef TEMP_SENSOR_CONFIG_H_
#define TEMP_SENSOR_CONFIG_H_

/* BUG FIX: this comment was stale - PORTA is not consumed by the keypad
 * in this project (see Home_Config.h, which separately confirms
 * HOME_LM35_ADC_CHANNEL = ADC_CHANNEL_0 / PA0 is used directly). Channel 0
 * matches that. (Note: HOME_u8GetTemperature() in Home_Program.c reads the
 * ADC directly and doesn't call this driver - this file isn't currently
 * used by the app, but is fixed here for correctness regardless.) */
#define TEMP_SENSOR_ADC_CHANNEL     ADC_CHANNEL_0


#endif /* TEMP_SENSOR_CONFIG_H_ */

