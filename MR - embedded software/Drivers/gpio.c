/*
 * gpio.c
 *
 *  Created on: May 22, 2020
 *      Author: mati97
 */


#include "gpio.h"

static GPIO_InitTypeDef DATA_InitStruct = {
		.Pin = DQ0_Pin|DQ1_Pin|DQ2_Pin|DQ3_Pin|DQ4_Pin|DQ5_Pin|DQ6_Pin|DQ7_Pin,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_LOW
};

void write_address(uint32_t address){

	GPIOA->ODR = address&0xffff;
	HAL_GPIO_WritePin(A16_GPIO_Port, A16_Pin, (GPIO_PinState)((address>>16)&0x0001));
}

void write_data(uint8_t data){


	if( DATA_InitStruct.Mode != GPIO_MODE_OUTPUT_PP){
		DATA_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		HAL_GPIO_Init(GPIOB, &DATA_InitStruct);
	}

	GPIOB->ODR = (GPIOB->ODR&0xff00)|data;
}

uint8_t read_data(void){


	if( DATA_InitStruct.Mode != GPIO_MODE_INPUT){
		DATA_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(GPIOB, &DATA_InitStruct);
	}

	return (uint8_t)(GPIOB->IDR&0xff);
}

void write(uint32_t address, uint8_t data){
	write_address(address);
	HAL_GPIO_WritePin(CE__GPIO_Port,CE__Pin, 0);
	HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin, 1);
	HAL_GPIO_WritePin(WE__GPIO_Port,WE__Pin, 0);
	write_data(data);
	HAL_GPIO_WritePin(WE__GPIO_Port,WE__Pin, 1);
	HAL_GPIO_WritePin(CE__GPIO_Port,CE__Pin, 1);
	HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin, 0);
}

uint8_t read(uint32_t address){
	write_address(address);
	HAL_GPIO_WritePin(CE__GPIO_Port,CE__Pin, 0);
	HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin, 1);
	HAL_GPIO_WritePin(OE__GPIO_Port,OE__Pin, 0);
	uint8_t read = read_data();
	HAL_GPIO_WritePin(CE__GPIO_Port,CE__Pin, 1);
	HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin, 0);
	HAL_GPIO_WritePin(OE__GPIO_Port,OE__Pin, 1);

	return read;
}

void bulk_write(uint32_t address, uint8_t* data, uint8_t len){
	//write_address(address);
	HAL_GPIO_WritePin(CE__GPIO_Port,CE__Pin, 0);
	HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin, 1);
	for(uint8_t i = 0; i<len; i++){
		HAL_GPIO_WritePin(WE__GPIO_Port,WE__Pin, 0);
		write_address(address+i);
		write_data(*(data+i));
		HAL_GPIO_WritePin(WE__GPIO_Port,WE__Pin, 0);
	}
	HAL_GPIO_WritePin(WE__GPIO_Port,WE__Pin, 1);
	HAL_GPIO_WritePin(CE__GPIO_Port,CE__Pin, 1);
	HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin, 0);
}

uint8_t* bulk_read(uint32_t address, uint8_t len){

	static uint8_t ret[256];

	HAL_GPIO_WritePin(CE__GPIO_Port,CE__Pin, 0);
	HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin, 1);
	HAL_GPIO_WritePin(OE__GPIO_Port,OE__Pin, 0);
	for(uint8_t i=0; i<len; i++){
		write_address(address+i);
		*(ret+i) = read_data();
	}
	HAL_GPIO_WritePin(CE__GPIO_Port,CE__Pin, 1);
	HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin, 0);
	HAL_GPIO_WritePin(OE__GPIO_Port,OE__Pin, 1);

	return ret;
}
