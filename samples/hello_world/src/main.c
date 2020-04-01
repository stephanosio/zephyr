/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>

#include <stdio.h>
#include <math.h>

#include <arm_math.h>

#include "dwt.h"

#define ITERATIONS	(1000)

static ALWAYS_INLINE void test_routine(void)
{
	volatile float input = -200.0f * PI;
	volatile float epsilon = 4.0f * PI / (float)ITERATIONS;
	volatile float result;

	for (int i = 0; i < ITERATIONS; i++)
	{
#if 0
		result = cosf(input);
#else
		result = arm_cos_f32(input);
#endif
		input += epsilon;
	}
}

#define REPEAT		(20)

void main(void)
{
	uint32_t key;
	uint64_t accum = 0;

	volatile uint32_t cycles;

	key = irq_lock();

	for (int i = 1; i <= REPEAT; i++)
	{
		dwt_start();
		test_routine();
		cycles = dwt_stop();

		printf("%d: %u cycles\n", i, cycles);

		accum += cycles;
	}

	printf("\navg = %llu cycles\n", accum / REPEAT);

	irq_unlock(key);
}
