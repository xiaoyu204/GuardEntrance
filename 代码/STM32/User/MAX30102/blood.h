/**
 * ************************************************************************
 *
 * @file blood.h
 * @author zxr
 * @brief
 *
 * ************************************************************************
 * @copyright Copyright (c) 2024 zxr
 * ************************************************************************
 */
#ifndef _BLOOD_H
#define _BLOOD_H

#include "main.h"
#include "bsp_max30102.h"
#include "algorithm.h"
#include "math.h"

void blood_data_translate(void);
void blood_data_update(void);
void blood_Loop(int *dis_hr_valid, int *dis_spo2_valid);

#endif
