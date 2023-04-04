#include "interrupt.h"
#include "usart.h"
char RxBuffer[30];
unsigned char BufIndex = 0;
unsigned char Rxdat = 0;

extern bool view;
extern bool b1begin;
extern bool b2begin;
extern bool b3begin;
extern bool clean;
extern unsigned char b1code;
extern unsigned char b2code;
extern unsigned char b3code;
extern unsigned char errorflag;
unsigned int pwm200time=0;
unsigned int ld2time1 = 0;
unsigned int ld2time2 = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		RxBuffer[BufIndex++]=Rxdat;
		HAL_UART_Receive_IT(&huart1,&Rxdat,1);
	}
}

struct keys key[4]={0,0,0,0};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3)
	{
		key[0].key_sta = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
		key[1].key_sta = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
		key[2].key_sta = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
		key[3].key_sta = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
		for(int i =0;i<4;i++)
		{
			switch(key[i].judge_sta)
			{
				case 0://判断
				{
					if(key[i].key_sta==0)
					{
						key[i].judge_sta = 1;
						key[i].key_time = 0;
					}
				}
				break;
				case 1://消抖
				{
					if(key[i].key_sta==0)
					{
						key[i].judge_sta=2;
					}
					else
					{
						key[i].judge_sta=0;
					}
				}
				break;
				case 2://确认按键按下
				{
					if(key[i].key_sta==1)
					{
						key[i].judge_sta = 0;
						if(key[i].key_time<=70)
						{
							key[i].single_flag=1;
						}
					}
					else
					{
						key[i].key_time++;
						if(key[i].key_time>70)
						{
							key[i].long_flag = 1;
						}
					}
				}
				break;
			}
		}
	}
	if(htim->Instance==TIM4)
	{
		pwm200time++;
		if(pwm200time==500)
		{
			pwm200time=0;
			pwm_pro(800,100,50);			
			clean=1;
			view=0;
			b1code=0;
			b2code=0;
			b3code=0;
			b1begin=0;
			b2begin=0;
			b3begin=0;
			led_disp(0x00);
			HAL_TIM_Base_Stop_IT(htim);
		}
	}
	if(htim->Instance==TIM6)
	{
		if(errorflag)
		{
			ld2time1++;
			ld2time2++;
			if(ld2time1==2)
			{
				ld2time1=0;
				led_disp(0x00);
			}
			else
				led_disp(0x02);
			if(ld2time2==50)
			{
				ld2time2=0;
				HAL_TIM_Base_Stop_IT(htim);
			}
		}
	}
}
