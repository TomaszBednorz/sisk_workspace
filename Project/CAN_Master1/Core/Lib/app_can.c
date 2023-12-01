/***********************************************************************************************************
 ********************************************* Included files **********************************************
 ***********************************************************************************************************/

#include <app_can.h>
#include <stdio.h>
#include "can.h"

/***********************************************************************************************************
 ************************************************* Defines *************************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 *********************************************** Data types ************************************************
 ***********************************************************************************************************/

typedef struct
{
	uint8_t data[8];
	uint8_t dlc;
	uint16_t id;
	uint32_t period;
}AppCan_CanFrame_t;

/***********************************************************************************************************
 **************************************** Local function prototypes ****************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ******************************************** Exported objects *********************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ********************************************* Local objects ***********************************************
 ***********************************************************************************************************/

static AppCan_CanFrame_t AppCan_CanFrames[10];
static uint8_t AppCan_HowManyScheaduledFrames = 0U;
static uint8_t AppCan_CurrentFrame = 0U;

/***********************************************************************************************************
 ******************************************* Exported functions ********************************************
 ***********************************************************************************************************/

void AppCan_Init(void)
{
	HAL_CAN_Start(&hcan1);
}

void AppCan_Task(void)
{
	static uint32_t my_tim = 0U;
	static uint8_t enable_tx = 0U;
	static uint32_t latch_period = 0U;

	if((my_tim % 10U) == 0U)
	{
		enable_tx = 1U;
		latch_period = my_tim / 10U;
		AppCan_CurrentFrame = 0U;
	}

	if(enable_tx == 1U)
	{
		if(AppCan_CurrentFrame < AppCan_HowManyScheaduledFrames)
		{
			if((latch_period % AppCan_CanFrames[AppCan_CurrentFrame].period) == 0U)
			{
				AppCan_TxFrame(AppCan_CanFrames[AppCan_CurrentFrame].data, \
							   AppCan_CanFrames[AppCan_CurrentFrame].dlc, \
							   AppCan_CanFrames[AppCan_CurrentFrame].id);
			}
			AppCan_CurrentFrame++;

			if(AppCan_CurrentFrame >= AppCan_HowManyScheaduledFrames)
			{
				enable_tx = 0U;
			}
		}
	}

	my_tim++;
}

void AppCan_TxFrame(uint8_t* buffer, uint8_t len, uint16_t id)
{
	CAN_TxHeaderTypeDef tx_header;
	uint32_t tx_mailbox;

	tx_header.DLC = len;
	tx_header.StdId = id;
	tx_header.IDE = CAN_ID_STD;
	tx_header.RTR = CAN_RTR_DATA;

	HAL_CAN_AddTxMessage(&hcan1, &tx_header, buffer, &tx_mailbox);
	while(HAL_CAN_IsTxMessagePending(&hcan1, tx_mailbox));
}

void AppCan_AddToScheaduler(uint8_t* buffer, uint8_t len, uint16_t id, uint32_t period)
{
	for(uint8_t i = 0; i  < len; i++)
	{
		AppCan_CanFrames[AppCan_HowManyScheaduledFrames].data[i] = buffer[i];
	}

	AppCan_CanFrames[AppCan_HowManyScheaduledFrames].dlc = len;
	AppCan_CanFrames[AppCan_HowManyScheaduledFrames].id = id;
	AppCan_CanFrames[AppCan_HowManyScheaduledFrames].period = period;

	AppCan_HowManyScheaduledFrames++;
}

/***********************************************************************************************************
 ******************************************** Local functions **********************************************
 ***********************************************************************************************************/










