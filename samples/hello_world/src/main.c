/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <sys/printk.h>
#include <malloc.h>

/* Run this on qemu_xtensa and see the first malloc return NULL. */

volatile char rubbish[552 + 24];

void main(void)
{
	void *ptr;

	for (int i = 0; i < 34; i++) {
		ptr = malloc(128);

		printk("%03d: %p\n", i, ptr);
	}

	(void)rubbish[0];
}
