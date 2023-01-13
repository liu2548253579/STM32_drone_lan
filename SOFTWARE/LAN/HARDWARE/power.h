#ifndef   _power_H
#define   _power_H


void BATT_Init(void);
float BATT_GetVoltage(void);
void LOW_VOL_Alarm (float vol);

#endif



//end
