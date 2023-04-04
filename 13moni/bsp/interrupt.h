#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_
#include "main.h"
#include "stdbool.h"
#include "led.h"
struct keys
{
	unsigned char judge_sta;
	bool key_sta;
	bool single_flag;
	unsigned int key_time;
	bool long_flag;
};

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#endif
