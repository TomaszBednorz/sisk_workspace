#ifndef _APP_CAN_H_
#define _APP_CAN_H_

/***********************************************************************************************************
 ********************************************* Included files **********************************************
 ***********************************************************************************************************/

#include <stdint.h>

/***********************************************************************************************************
 ************************************************* Defines *************************************************
 ***********************************************************************************************************/

/* CAN frames configuration: name, ID, period (100ms base), offset (100ms base), dlc */
#define APP_CAN_CFG_TABLE  \
    APP_CAN_CFG_FRAME(AppCanFrame1_Gyro, 0x10, 20U, 0U, 6U)   \
    APP_CAN_CFG_FRAME(AppCanFrame2_Acc, 0x15, 20U, 5U, 6U)    \
    APP_CAN_CFG_FRAME(AppCanFrame3_Temp, 0x20, 20U, 10U, 2U)    \
    APP_CAN_CFG_FRAME(AppCanFrame4_Light, 0x40, 20U, 15U, 2U)  \
    APP_CAN_CFG_FRAME(AppCanFrame5_Rgb, 0x80, 50U, 1U, 3U)

    
/***********************************************************************************************************
 *********************************************** Data types ************************************************
 ***********************************************************************************************************/

typedef enum
{
    #define APP_CAN_CFG_FRAME(name, id, period, offset, dlc)   name,
        APP_CAN_CFG_TABLE
    #undef APP_CAN_CFG_FRAME
	AppCanFrameMax
}AppCan_Frame_t;

/***********************************************************************************************************
 ********************************************* Exported objects ********************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ************************************** Exported function prototypes ***************************************
 ***********************************************************************************************************/

void AppCan_Init(void);
void AppCan_Task(void);
void AppCan_FillFrame(AppCan_Frame_t frame_id, uint8_t* data);

// Update data in frame



#endif  /* _APP_CAN_H_ */
