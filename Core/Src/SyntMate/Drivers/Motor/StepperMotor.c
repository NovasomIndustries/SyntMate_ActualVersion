/*
 * StepperMotor.c
 *
 *  Created on: Mar 12, 2021
 *      Author: fil
 */

#include "main.h"

uint16_t	step_acceleration_tab[OC_TABLE_SIZE];
uint16_t	step_steady_tab[OC_TABLE_SIZE];
uint16_t	step_deceleration_tab[OC_TABLE_SIZE];
uint16_t	step_homing_tab[OC_HOMING_TABLE_SIZE];
uint32_t	step_phase_position = 0;
uint32_t	step_rotations = 0;
uint32_t	step_phase_state = STEP_IDLE;

void StepperInit(void)
{
uint32_t	i;

	HAL_GPIO_WritePin(DIRSTEP_GPIO_Port, DIRSTEP_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DIRSTEP_GPIO_Port, DIRSTEP_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ENASTEP_GPIO_Port, ENASTEP_Pin, GPIO_PIN_SET);
	for(i=0;i<OC_TABLE_SIZE;i++)
	{
		if ( (OC_START_VALUE-(i*OC_ACCELERATION_FACTOR)) < OC_END_VALUE)
			 step_acceleration_tab[i] = step_deceleration_tab[OC_TABLE_SIZE-i-1] = OC_END_VALUE;
		else
		{
			step_acceleration_tab[i] = OC_START_VALUE - (i*OC_ACCELERATION_FACTOR);
			step_deceleration_tab[i] = OC_END_VALUE+(i*OC_ACCELERATION_FACTOR);
		}
		step_steady_tab[i] = OC_END_VALUE;
	}
	for(i=0;i<OC_HOMING_TABLE_SIZE;i++)
		step_homing_tab[i] = OC_START_VALUE;

}

static void step_start_t(uint32_t tab,uint32_t state)
{
	HAL_TIM_OC_Stop(&htim16, TIM_CHANNEL_1);
	step_phase_state = state;
	HAL_TIM_OC_Start(&htim16, TIM_CHANNEL_1);
	HAL_DMA_Start_IT(htim16.hdma[TIM_DMA_ID_CC1], tab, (uint32_t)&htim16.Instance->ARR, OC_TABLE_SIZE);
	__HAL_TIM_ENABLE_DMA(&htim16, TIM_DMA_CC1);
}

static void step_home_t(void)
{
	HAL_TIM_OC_Stop(&htim16, TIM_CHANNEL_1);
	step_phase_state = STEP_HOMING;
	HAL_TIM_OC_Start(&htim16, TIM_CHANNEL_1);
	HAL_DMA_Start_IT(htim16.hdma[TIM_DMA_ID_CC1], (uint32_t )&step_homing_tab, (uint32_t)&htim16.Instance->ARR, OC_HOMING_TABLE_SIZE);
	__HAL_TIM_ENABLE_DMA(&htim16, TIM_DMA_CC1);
}

void SyntMate_do_step_table(void)
{
	switch(step_phase_state)
	{
	case	STEP_ACCELERATION :
		step_start_t((uint32_t)&step_steady_tab,STEP_STEADY);
		step_rotations--;
		break;

	case	STEP_STEADY :
		if ( step_rotations == 1 )
			step_start_t((uint32_t)&step_deceleration_tab,STEP_DECELERATION);
		else
			step_start_t((uint32_t)&step_steady_tab,STEP_STEADY);
		step_rotations--;
		break;
	case	STEP_DECELERATION :
		HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
		step_phase_state = STEP_IDLE;
		HAL_GPIO_WritePin(ENASTEP_GPIO_Port, ENASTEP_Pin, GPIO_PIN_SET);
		break;
	case	STEP_HOMING :
		HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
		step_phase_state = STEP_IDLE;
		HAL_GPIO_WritePin(ENASTEP_GPIO_Port, ENASTEP_Pin, GPIO_PIN_SET);
		break;
	default					  : break;
	}
}

void SyntMate_do_rotations(uint32_t rotation_number)
{
	if ( step_phase_state == STEP_IDLE)
	{
		step_rotations = (rotation_number*4)-1;
		step_start_t((uint32_t)&step_acceleration_tab,STEP_ACCELERATION);
		HAL_GPIO_WritePin(ENASTEP_GPIO_Port, ENASTEP_Pin, GPIO_PIN_RESET);
	}
}

uint32_t	distance=0;
void SyntMate_do_homing_steps(void)
{
	while ( 1 )
	{
		SyntMate_VL53L0X_StartMeasure();
		while(measure_in_progress == 1)
			HAL_Delay(1);
		if ( range < CAVITY_HOLE_SIZE )
		{
			distance = range;
			return;
		}
		if ( step_phase_state == STEP_IDLE)
		{
			step_rotations = OC_HOMING_STEPS;
			step_home_t();
			HAL_GPIO_WritePin(ENASTEP_GPIO_Port, ENASTEP_Pin, GPIO_PIN_RESET);
			SyntMate_VL53L0X_StartMeasure();
			while(measure_in_progress == 1)
				HAL_Delay(1);
			if ( range < CAVITY_HOLE_SIZE )
			{
				distance = range;
				return;
			}
		}
	}
}

void SyntMate_stop_rotations(void)
{
	HAL_GPIO_WritePin(ENASTEP_GPIO_Port, ENASTEP_Pin, GPIO_PIN_SET);
	HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
	step_phase_state = STEP_IDLE;
}
