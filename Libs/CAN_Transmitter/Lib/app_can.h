#ifndef _APP_CAN_H_
#define _APP_CAN_H_

/***********************************************************************************************************
 ********************************************* Included files **********************************************
 ***********************************************************************************************************/

#include <stdint.h>

/***********************************************************************************************************
 ************************************************* Defines *************************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 *********************************************** Data types ************************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ********************************************* Exported objects ********************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ************************************** Exported function prototypes ***************************************
 ***********************************************************************************************************/

void AppCan_Init(void);
void AppCan_Task(void);
void AppCan_TxFrame(uint8_t* buffer, uint8_t len, uint16_t id);
void AppCan_AddToScheaduler(uint8_t* buffer, uint8_t len, uint16_t id, uint32_t period);

#endif  /* _APP_CAN_H_ */