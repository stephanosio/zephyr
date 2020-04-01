#pragma once

#include <zephyr.h>

static ALWAYS_INLINE void dwt_start(void)
{
	DWT->CYCCNT = 0;

	__DSB();
	__ISB();

	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static ALWAYS_INLINE uint32_t dwt_stop(void)
{
	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
	return DWT->CYCCNT;
}
