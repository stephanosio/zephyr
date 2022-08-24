/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/zephyr.h>

extern __weak void test(int a, int b);

void main(void)
{
	printk("Hello World! %s\n", CONFIG_BOARD);

	if (test != NULL) {
		test(1, 2);
	}

	printk("Done\n");
}
