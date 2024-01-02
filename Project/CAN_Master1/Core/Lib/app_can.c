/***********************************************************************************************************
 ********************************************* Included files **********************************************
 ***********************************************************************************************************/

#include "app_can.h"
#include <stdio.h>
#include <stdbool.h>
#include "can.h"

/***********************************************************************************************************
 ************************************************* Defines *************************************************
 ***********************************************************************************************************/

#define APP_CAN_MAX_DLC     (8U)

/***********************************************************************************************************
 *********************************************** Data types ************************************************
 ***********************************************************************************************************/

/*
 * Configuration structure
 */
typedef struct
{
    AppCan_Frame_t frame_name;
    uint8_t data[APP_CAN_MAX_DLC];
	uint8_t dlc;
	uint16_t identifier;
    uint32_t period;
    uint32_t counter;
}AppCan_Config_t;

/***********************************************************************************************************
 **************************************** Local function prototypes ****************************************
 ***********************************************************************************************************/

static void AppCan_TxFrame(AppCan_Frame_t frame_id);
static bool AppCan_ProcessFrame(AppCan_Frame_t frame_id);

/***********************************************************************************************************
 ******************************************** Exported objects *********************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ********************************************* Local objects ***********************************************
 ***********************************************************************************************************/

AppCan_Config_t AppCan_UserConfig[AppCanFrameMax] =
{
    #define APP_CAN_CFG_FRAME(name, id, time, offset, length) { \
        .frame_name=name, \
		.data={0}, \
		.dlc=length, \
		.identifier=id, \
        .period=time, \
        .counter=offset},
		APP_CAN_CFG_TABLE
    #undef APP_CAN_CFG_FRAME
};

/***********************************************************************************************************
 ******************************************* Exported functions ********************************************
 ***********************************************************************************************************/

void AppCan_Init(void)
{
	HAL_CAN_Start(&hcan1);
}

void AppCan_Task(void)
{
	bool send_frame;

    for(uint8_t i = 0; i < (uint8_t)AppCanFrameMax; i++)
    {
        send_frame = AppCan_ProcessFrame((AppCan_Frame_t)i);

        if(false != send_frame)
        {
        	AppCan_TxFrame((AppCan_Frame_t)i);
        }
    }
}

/*
 * Fill CAN frame with data
 */
void AppCan_FillFrame(AppCan_Frame_t frame_id, uint8_t* data)
{
    for(uint8_t i = 0U; i < AppCan_UserConfig[frame_id].dlc; i++)
    {
    	AppCan_UserConfig[frame_id].data[i] = data[i];
    }
}

/***********************************************************************************************************
 ******************************************** Local functions **********************************************
 ***********************************************************************************************************/

static void AppCan_TxFrame(AppCan_Frame_t frame_id)
{
	CAN_TxHeaderTypeDef tx_header;
	uint32_t tx_mailbox;

	tx_header.DLC = AppCan_UserConfig[frame_id].dlc;
	tx_header.StdId = AppCan_UserConfig[frame_id].identifier;
	tx_header.IDE = CAN_ID_STD;
	tx_header.RTR = CAN_RTR_DATA;

	HAL_CAN_AddTxMessage(&hcan1, &tx_header, AppCan_UserConfig[frame_id].data, &tx_mailbox);
	while(HAL_CAN_IsTxMessagePending(&hcan1, tx_mailbox));
}

/*
 * Process CAN frame.
 * Ret: 0 = don't send frame, 1 = send frame (checks period counter)
 */
static bool AppCan_ProcessFrame(AppCan_Frame_t frame_id)
{
    uint32_t period = AppCan_UserConfig[frame_id].period;
    uint32_t counter = ++AppCan_UserConfig[frame_id].counter;
    bool ret;

    if(counter >= period)
    {
    	AppCan_UserConfig[frame_id].counter = 0U;
        ret = true;
    }
    else
    {
        ret = false;
    }

    return ret;
}


