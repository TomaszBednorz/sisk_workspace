/***********************************************************************************************************
 ********************************************* Included files **********************************************
 ***********************************************************************************************************/

#include <app_uart.h>
#include <app_can.h>
#include <stdio.h>
#include <string.h>
#include "usart.h"

/***********************************************************************************************************
 ************************************************* Defines *************************************************
 ***********************************************************************************************************/

#define APP_UART_CONVERT_TO_DEC(in)  (uint32_t)((uint8_t)in - 48U)

/***********************************************************************************************************
 *********************************************** Data types ************************************************
 ***********************************************************************************************************/

typedef enum
{
	APP_STATE_1_MODE = 0U,
	APP_STATE_2_ID,
	APP_STATE_3_DLC,
	APP_STATE_4_DATA_1,
	APP_STATE_5_DATA_2,
	APP_STATE_6_DATA_3,
	APP_STATE_7_DATA_4,
	APP_STATE_8_DATA_5,
	APP_STATE_9_DATA_6,
	APP_STATE_10_DATA_7,
	APP_STATE_11_DATA_8,
	APP_STATE_12_PERIOD,
	APP_STATE_13_SUMMARY,
	APP_STATE_14_ERROR,
	APP_STATE_MAX
}AppUart_StateMachine_t;

/***********************************************************************************************************
 **************************************** Local function prototypes ****************************************
 ***********************************************************************************************************/

void AppUart_SendInfo(AppUart_StateMachine_t state);
uint16_t AppUart_DecodeInputHex(uint8_t len);
static uint8_t AppUart_ConvertToHex(uint8_t in);
uint32_t AppUart_DecodeInputDec(uint8_t len);
void AppUart_ProcessMode(void);
void AppUart_ProcessId(void);
void AppUart_ProcessDlc(void);
void AppUart_ProcessData(uint8_t num);
void AppUart_ProcessPeriod(void);
void AppUart_ProcessSummary(void);
void AppUart_ProcessError(void);

/***********************************************************************************************************
 ******************************************** Exported objects *********************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ********************************************* Local objects ***********************************************
 ***********************************************************************************************************/

static AppUart_StateMachine_t AppUart_StateMachine;
static char AppUart_Msg[250];
static uint8_t AppUart_UartRxData[4];

/* User CAN request */
static uint8_t AppUart_RequestMode;
static uint16_t AppUart_RequestId;
static uint8_t AppUart_RequestDlc;
static uint8_t AppUart_RequestData[8];
static uint16_t AppUart_RequestPeriod;

static char* AppCan_MsgTable[APP_STATE_MAX] = \
		{"(1) Select mode (dec):\r\n" \
		 "1 - Send single CAN frame\r\n" \
		 "2 - Add CAN frame to scheduler ", \
		 "\r\n(3) Frame ID (hex): ", \
		 "\r\n(1) Data length (dec): ", \
		 "\r\n(2) First byte (hex) ", \
		 "\r\n(2) Second byte (hex) ", \
		 "\r\n(2) Third byte (hex) ", \
		 "\r\n(2) Fourth byte (hex) ", \
		 "\r\n(2) Fifth byte (hex) ", \
		 "\r\n(2) Sixth byte (hex) ", \
		 "\r\n(2) Seventh byte (hex) ", \
		 "\r\n(2) Eighth byte (hex) ", \
		 "\r\n(4) Period [100ms base](dec): ", \
		 "\r\n(1) Frame ready to transmit/schedule. Send 1 character to continue. ", \
		 "\r\n(1) Incorrect input! Try again. Send 1 character to continue. "};

/***********************************************************************************************************
 ******************************************* Exported functions ********************************************
 ***********************************************************************************************************/

void AppUart_Init(void)
{
	sprintf(AppUart_Msg, "Use the serial port to transmit a CAN frame or\r\n" \
						 "add a CAN frame to the scheduler.\r\n" \
						 "The required number of characters is present in parentheses.\r\n%s", \
						 AppCan_MsgTable[APP_STATE_1_MODE]);
	HAL_UART_Transmit_IT(&huart3, (uint8_t*)AppUart_Msg, sizeof(AppUart_Msg));

	HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 1);
}

void AppUart_ProcessInput(void)
{
	switch (AppUart_StateMachine) {
		case APP_STATE_1_MODE:
			AppUart_ProcessMode();
			break;
		case APP_STATE_2_ID:
			AppUart_ProcessId();
			break;
		case APP_STATE_3_DLC:
			AppUart_ProcessDlc();
			break;
		case APP_STATE_4_DATA_1:
			AppUart_ProcessData(1U);
			break;
		case APP_STATE_5_DATA_2:
			AppUart_ProcessData(2U);
			break;
		case APP_STATE_6_DATA_3:
			AppUart_ProcessData(3U);
			break;
		case APP_STATE_7_DATA_4:
			AppUart_ProcessData(4U);
			break;
		case APP_STATE_8_DATA_5:
			AppUart_ProcessData(5U);
			break;
		case APP_STATE_9_DATA_6:
			AppUart_ProcessData(6U);
			break;
		case APP_STATE_10_DATA_7:
			AppUart_ProcessData(7U);
			break;
		case APP_STATE_11_DATA_8:
			AppUart_ProcessData(8U);
			break;
		case APP_STATE_12_PERIOD:
			AppUart_ProcessPeriod();
			break;
		case APP_STATE_13_SUMMARY:
			AppUart_ProcessSummary();
			break;
		case APP_STATE_14_ERROR:
			AppUart_ProcessError();
			break;
		default:
			break;
	}

	AppUart_SendInfo(AppUart_StateMachine);
}



/***********************************************************************************************************
 ******************************************** Local functions **********************************************
 ***********************************************************************************************************/

void AppUart_SendInfo(AppUart_StateMachine_t state)
{
	memset(AppUart_Msg, '\0', sizeof(AppUart_Msg));

	switch (AppUart_StateMachine) {
		case APP_STATE_1_MODE:
			sprintf(AppUart_Msg, AppCan_MsgTable[APP_STATE_1_MODE]);
			break;
		case APP_STATE_2_ID:
			sprintf(AppUart_Msg, AppCan_MsgTable[APP_STATE_2_ID]);
			break;
		case APP_STATE_3_DLC:
			sprintf(AppUart_Msg, AppCan_MsgTable[APP_STATE_3_DLC]);
			break;
		case APP_STATE_4_DATA_1:
			sprintf(AppUart_Msg, AppCan_MsgTable[APP_STATE_4_DATA_1]);
			break;
		case APP_STATE_5_DATA_2:
			sprintf(AppUart_Msg, AppCan_MsgTable[APP_STATE_5_DATA_2]);
			break;
		case APP_STATE_6_DATA_3:
			sprintf(AppUart_Msg, AppCan_MsgTable[APP_STATE_6_DATA_3]);
			break;
		case APP_STATE_7_DATA_4:
			sprintf(AppUart_Msg, AppCan_MsgTable[APP_STATE_7_DATA_4]);
			break;
		case APP_STATE_8_DATA_5:
			sprintf(AppUart_Msg, AppCan_MsgTable[APP_STATE_8_DATA_5]);
			break;
		case APP_STATE_9_DATA_6:
			sprintf(AppUart_Msg, AppCan_MsgTable[APP_STATE_9_DATA_6]);
			break;
		case APP_STATE_10_DATA_7:
			sprintf(AppUart_Msg, AppCan_MsgTable[APP_STATE_10_DATA_7]);
			break;
		case APP_STATE_11_DATA_8:
			sprintf(AppUart_Msg, AppCan_MsgTable[APP_STATE_11_DATA_8]);
			break;
		case APP_STATE_12_PERIOD:
			sprintf(AppUart_Msg, AppCan_MsgTable[APP_STATE_12_PERIOD]);
			break;
		case APP_STATE_13_SUMMARY:
			sprintf(AppUart_Msg, AppCan_MsgTable[APP_STATE_13_SUMMARY]);
			break;
		case APP_STATE_14_ERROR:
			sprintf(AppUart_Msg, AppCan_MsgTable[APP_STATE_14_ERROR]);
			break;
		default:
			break;
	}

	HAL_UART_Transmit_IT(&huart3, (uint8_t*)AppUart_Msg, sizeof(AppUart_Msg));
}


uint16_t AppUart_DecodeInputHex(uint8_t len)
{
	static uint16_t mul[3] = {256U, 16U, 1U};
	uint16_t ret_val = 0U;

	for(uint8_t i = 0; i < len; i++)
	{
		ret_val += (uint16_t)AppUart_ConvertToHex(AppUart_UartRxData[i]) * mul[3-len+i];
	}

	return ret_val;
}

static uint8_t AppUart_ConvertToHex(uint8_t in)
{
	uint8_t ret_val = 0U;

	if((in >= 48U) && (in <= 57U))
	{
		ret_val = in - 48U;
	}
	else if((in >= 65U) && (in <= 70U))
	{
		ret_val = in - 55U;
	}
	else if((in >= 97U) && (in <= 102U))
	{
		ret_val = in - 87U;
	}

	return ret_val;
}

uint32_t AppUart_DecodeInputDec(uint8_t len)
{
	static uint32_t mul[4] = {1000U, 100U, 10U, 1U};
	uint32_t ret_val = 0U;

	for(uint8_t i = 0; i < len; i++)
	{
		ret_val += (uint32_t)APP_UART_CONVERT_TO_DEC(AppUart_UartRxData[i]) * mul[4-len+i];
	}

	return ret_val;
}

void AppUart_ProcessMode(void)
{
	uint8_t input;

	input = (uint8_t)AppUart_DecodeInputDec(1U);

	if((input == 1U) || (input == 2U))
	{
		AppUart_RequestMode = input;
		AppUart_StateMachine++;
		HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 3);
	}
	else
	{
		AppUart_StateMachine = APP_STATE_14_ERROR;
		HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 1);
	}
}

void AppUart_ProcessId(void)
{
	uint16_t input;

	input = (uint16_t)AppUart_DecodeInputHex(3U);

	if(input <= 2048U)
	{
		AppUart_RequestId = input;
		AppUart_StateMachine++;
	}
	else
	{
		AppUart_StateMachine = APP_STATE_14_ERROR;
	}

	HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 1);
}

void AppUart_ProcessDlc(void)
{
	uint8_t input;

	input = (uint8_t)AppUart_DecodeInputDec(1U);

	if((input <= 8U) && (input >= 1U))
	{
		for(uint8_t i = 0; i < 8U; i++)
		{
			AppUart_RequestData[i] = 0U;
		}

		AppUart_RequestDlc = input;
		AppUart_StateMachine++;
		HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 2);
	}
	else
	{
		AppUart_StateMachine = APP_STATE_14_ERROR;
		HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 1);
	}
}

void AppUart_ProcessData(uint8_t num)
{
	uint8_t input;

	input = (uint8_t)AppUart_DecodeInputHex(2U);

	AppUart_RequestData[num-1U] = input;

	if(num < AppUart_RequestDlc)
	{
		AppUart_StateMachine++;
		HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 2);
	}
	else
	{
		if(AppUart_RequestMode == 1U)
		{
			AppUart_StateMachine = APP_STATE_13_SUMMARY;
			HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 1);
		}
		else
		{
			AppUart_StateMachine = APP_STATE_12_PERIOD;
			HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 4);
		}
	}
}

void AppUart_ProcessPeriod(void)
{
	uint32_t input;

	input = (uint32_t)AppUart_DecodeInputDec(4U);

	if(input > 0U)
	{
		AppUart_RequestPeriod = input;
		AppUart_StateMachine++;
	}
	else
	{
		AppUart_StateMachine = APP_STATE_14_ERROR;
	}

	HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 1);
}

void AppUart_ProcessSummary(void)
{
	if(AppUart_RequestMode == 1U)
	{
		AppCan_TxFrame(AppUart_RequestData, AppUart_RequestDlc, AppUart_RequestId);
	}
	else
	{
		AppCan_AddToScheaduler(AppUart_RequestData, AppUart_RequestDlc, AppUart_RequestId, AppUart_RequestPeriod);
	}

	AppUart_StateMachine = APP_STATE_1_MODE;
	HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 1);
}

void AppUart_ProcessError(void)
{
	AppUart_StateMachine = APP_STATE_1_MODE;
	HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 1);
}


