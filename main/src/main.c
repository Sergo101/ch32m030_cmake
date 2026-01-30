/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.1
 * Date               : 2024/12/24
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *complementary output and deadband insertion mode routines:
 *TIM1_CH1(PB9),TIM1_CH1N(PB8)
 *TIM2_CH1(PA5),TIM2_CH1N(PA7),TIM2_CH2(PA6),TIM2_CH2N(PA4),
 *TIM3_CH1(PC0),TIM3_CH1N(PB2),TIM3_CH2(PC1),TIM3_CH2N(PB3),
 *This example demonstrates complementary output mode with dead zone of TIM1/TIM2/TIM3
 *Note:The dead time base of TIM2/TIM3 is the TIM2/TIM3 module clock
 */

#include "debug.h"

uint16_t sinarr[] = {1500, 1695, 1888, 2074, 2250, 2413, 2560, 2690, 2799, 
                    2885, 2948, 2987, 3000, 2987, 2948, 2885, 2799, 2690, 
                    2560, 2413, 2250, 2074, 1888, 1695, 1500, 1304, 1111, 
                    925, 750, 586, 439, 309, 200, 114, 51, 12, 0, 12, 51, 
                    114, 200, 309, 439, 586, 749, 925, 1111, 1304 };

#define TIM_INCREMENT       1
#define TIM_COUNT           3000

/* PWM Output Mode Definition */
#define PWM_TIM1   0
#define PWM_TIM2   1
#define PWM_TIM3   2

/* PWM Output Mode Selection */
#define PWM_MODE PWM_TIM1
//#define PWM_MODE PWM_TIM2
// #define PWM_MODE PWM_TIM3

/*********************************************************************
 * @fn      TIM1_Dead_Time_Init
 *
 * @brief   Initializes TIM1 complementary output and dead time.
 *
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *          ccp - the pulse value.
 *
 * @return  none
 */
void TIM1_Dead_Time_Init(u16 arr, u16 psc, u16 ccp)
{
    GPIO_InitTypeDef        GPIO_InitStructure = {0};
    TIM_OCInitTypeDef       TIM_OCInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
    TIM_BDTRInitTypeDef     TIM_BDTRInitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOB | RCC_PB2Periph_TIM1, ENABLE);

    /* TIM1_CH1 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* TIM1_CH1N */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* TIM1_CH2 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* TIM1_CH2N */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* TIM1_CH3 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* TIM1_CH3N */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);

    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
    TIM_BDTRInitStructure.TIM_DeadTime = 0x20;
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
}

void TIM1_SetChannelPulse(uint16_t ccp1, uint16_t ccp2, uint16_t ccp3)
{
    TIM1->CH1CVR = ccp1;
    TIM1->CH2CVR = ccp2;
    TIM1->CH3CVR = ccp3;
}

void init_buttons(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOA, ENABLE);

    /* K1 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* K2 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    uint16_t p1 = 0;
    uint16_t p2 = 0;
    uint16_t p3 = 48 / 4;

    uint32_t delay = 1000;

    SystemCoreClockUpdate();
    Delay_Init();
    init_buttons();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    SystemCoreClockUpdate();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", (int)SystemCoreClock);
    printf( "ChipID:%08x\r\n", (int)DBGMCU_GetCHIPID() );
    


    TIM1_Dead_Time_Init(TIM_COUNT-1, 1 - 1, 50);
#if (PWM_MODE == PWM_TIM1)
    
    
#elif (PWM_MODE == PWM_TIM2)
    
    TIM2_Dead_Time_Init(100-1, 7200 - 1, 50);

#elif (PWM_MODE == PWM_TIM3)

    TIM3_Dead_Time_Init(400-1, 1 - 1, 200);

#endif

    while(1)
    {
        Delay_Us(delay);
        // Delay_Ms(1);
        TIM1_SetChannelPulse(1500, ((sinarr[p2] - 1500)) + 1500, ((sinarr[p3] - 1500)) + 1500);
        // TIM1_SetChannelPulse(1500, sinarr[p2], sinarr[p3]);
        // TIM1_SetChannelPulse(1500, 1500, 0);
        p1 += TIM_INCREMENT;
        p2 += TIM_INCREMENT;
        p3 += TIM_INCREMENT;

        if(p1 > 47) p1 = 0;
        if(p2 > 47) p2 = 0;
        if(p3 > 47) p3 = 0;

        if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7))
        {
            delay++;
            if(delay > 100000) delay = 100000;
        }
        if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2))
        {
            delay--;
            if(delay < 1) delay = 1;
        }
    }
}
