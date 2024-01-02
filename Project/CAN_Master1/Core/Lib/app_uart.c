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

#define APP_UART_CONVERT_TO_DEC(in)   (in - 48U)

#define APP_UART_MAX_PWM_VALUE		  (100U)

/***********************************************************************************************************
 *********************************************** Data types ************************************************
 ***********************************************************************************************************/

typedef enum
{
	APP_STATE_1_R = 0U,
	APP_STATE_2_G,
	APP_STATE_3_B,
	APP_STATE_4_SUMMARY,
	APP_STATE_5_ERROR,
	APP_STATE_MAX
}AppUart_StateMachine_t;

/***********************************************************************************************************
 **************************************** Local function prototypes ****************************************
 ***********************************************************************************************************/

void AppUart_SendInfo(AppUart_StateMachine_t state);
uint16_t AppUart_DecodeInputDec(uint8_t len);
void AppUart_ProcessData(uint8_t num);
void AppUart_ProcessSummary(void);
void AppUart_ProcessError(void);

/***********************************************************************************************************
 ******************************************** Exported objects *********************************************
 ***********************************************************************************************************/

/***********************************************************************************************************
 ********************************************* Local objects ***********************************************
 ***********************************************************************************************************/

static AppUart_StateMachine_t AppUart_StateMachine;
static char AppUart_Msg[150];
static uint8_t AppUart_UartRxData[3];

static uint8_t AppUartRGB[3];

static char* AppUart_MsgTable[APP_STATE_MAX] = \
		{"\r\n(3) Red led PWM in %% (dec):", \
		 "\r\n(3) Green led PWM in %% (dec):", \
		 "\r\n(3) Blue led PWM in %% (dec):", \
		 "\r\n(1) Color saved. Send 1 character to continue. ", \
		 "\r\n(1) Incorrect input! Try again. Send 1 character to continue. "};

/***********************************************************************************************************
 ******************************************* Exported functions ********************************************
 ***********************************************************************************************************/

void AppUart_Init(void)
{
	sprintf(AppUart_Msg, "\r\nUse the serial port to choose RGB LED color." \
						 "\r\nThe required number of characters is present in parentheses.%s", \
						 AppUart_MsgTable[APP_STATE_1_R]);
	HAL_UART_Transmit_IT(&huart3, (uint8_t*)AppUart_Msg, sizeof(AppUart_Msg));

	HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 3);
}

void AppUart_ProcessInput(void)
{
	switch (AppUart_StateMachine) {
		case APP_STATE_1_R:
			AppUart_ProcessData(0U);
			break;
		case APP_STATE_2_G:
			AppUart_ProcessData(1U);
			break;
		case APP_STATE_3_B:
			AppUart_ProcessData(2U);
			break;
		case APP_STATE_4_SUMMARY:
			AppUart_ProcessSummary();
			break;
		case APP_STATE_5_ERROR:
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
		case APP_STATE_1_R:
			sprintf(AppUart_Msg, AppUart_MsgTable[APP_STATE_1_R]);
			break;
		case APP_STATE_2_G:
			sprintf(AppUart_Msg, AppUart_MsgTable[APP_STATE_2_G]);
			break;
		case APP_STATE_3_B:
			sprintf(AppUart_Msg, AppUart_MsgTable[APP_STATE_3_B]);
			break;
		case APP_STATE_4_SUMMARY:
			sprintf(AppUart_Msg, AppUart_MsgTable[APP_STATE_4_SUMMARY]);
			break;
		case APP_STATE_5_ERROR:
			sprintf(AppUart_Msg, AppUart_MsgTable[APP_STATE_5_ERROR]);
			break;
		default:
			break;
	}

	HAL_UART_Transmit_IT(&huart3, (uint8_t*)AppUart_Msg, sizeof(AppUart_Msg));
}

uint16_t AppUart_DecodeInputDec(uint8_t len)
{
	static uint8_t mul[3] = {100U, 10U, 1U};
	uint16_t ret_val = 0U;

	for(uint8_t i = 0; i < len; i++)
	{
		ret_val += (uint16_t)APP_UART_CONVERT_TO_DEC(AppUart_UartRxData[i]) * mul[3-len+i];
	}

	return ret_val;
}

/* 0 - R, 1 - G, 2 - B */
void AppUart_ProcessData(uint8_t num)
{
	uint8_t input;

	input = (uint8_t)AppUart_DecodeInputDec(3U);

	if(input <= APP_UART_MAX_PWM_VALUE)
	{
		AppUartRGB[num] = input;

		if(AppUart_StateMachine < APP_STATE_3_B)
		{
			HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 3);
		}
		else
		{
			HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 1);
		}

		AppUart_StateMachine++;
	}
	else
	{
		AppUart_StateMachine = APP_STATE_5_ERROR;
		HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 1);
	}
	


}

void AppUart_ProcessSummary(void)
{
	AppCan_FillFrame(AppCanFrame5_Rgb, AppUartRGB);

	AppUart_StateMachine = APP_STATE_1_R;
	HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 3);
}

void AppUart_ProcessError(void)
{
	AppUart_StateMachine = APP_STATE_1_R;
	HAL_UART_Receive_IT(&huart3, AppUart_UartRxData, 3);
}


