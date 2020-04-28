/*
 * Copyright (c) 2020 Stephanos Ioannidis <root@stephanos.io>
 * Copyright (C) 2010-2020 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zephyr.h>
#include <stdlib.h>
#include <arm_math.h>
#include "../../common/test_common.h"

#include "fir_q15.pat"

#define SNR_ERROR_THRESH	((float32_t)59)
#define ABS_ERROR_THRESH_Q15	((q15_t)2)

static void test_arm_fir_q15(void)
{
	size_t sample_index, block_index;
	size_t block_size, tap_count;
	size_t sample_count = ARRAY_SIZE(in_config) / 2;
	size_t length = ARRAY_SIZE(ref_val);
	const u16_t *config = in_config;
	const q15_t *input = (const q15_t *)in_val;
	const q15_t *coeff = (const q15_t *)in_coeff;
	const q15_t *ref = (const q15_t *)ref_val;
	q15_t *state, *output_buf, *output;
	arm_fir_instance_q15 inst;

	/* Allocate buffers */
	state = malloc(64 * sizeof(q15_t));
	zassert_not_null(state, ASSERT_MSG_BUFFER_ALLOC_FAILED);

	output_buf = malloc(length * sizeof(q15_t));
	zassert_not_null(output_buf, ASSERT_MSG_BUFFER_ALLOC_FAILED);

	output = output_buf;

	/* Enumerate samples */
	for (sample_index = 0; sample_index < sample_count; sample_index++) {
		/* Resolve sample configurations */
		block_size = config[0];
		tap_count = config[1];

#ifdef CONFIG_CMSIS_DSP_POWERQUAD
		/* Skip unsupported block sizes for NXP PowerQuad */
		if (block_size <= 2) {
			/* Copy reference value to the output buffer */
			memcpy(output, ref + (output - output_buf),
			       block_size * 2 * sizeof(q15_t));

			/* Increment pointers to the next sample */
			output += block_size * 2;
			coeff += tap_count;
			config += 2;
			continue;
		}
#endif

		/* Initialise instance */
		arm_fir_init_q15(&inst, tap_count, coeff, state, block_size);

		/* TODO: Add MEVI support */

		/* Reset input pointer */
		input = (const q15_t *)in_val;

		/* Enumerate blocks */
		for (block_index = 0; block_index < 2; block_index++) {
			/* Run test function */
			arm_fir_q15(&inst, input, output, block_size);

			/* Increment pointers */
			input += block_size;
			output += block_size;
		}

		/* Increment pointers */
		coeff += tap_count;
		config += 2;
	}

	/* Validate output */
	zassert_true(
		test_snr_error_q15(length, output_buf, ref, SNR_ERROR_THRESH),
		ASSERT_MSG_SNR_LIMIT_EXCEED);

	zassert_true(
		test_near_equal_q15(length, output_buf, ref,
			ABS_ERROR_THRESH_Q15),
		ASSERT_MSG_ABS_ERROR_LIMIT_EXCEED);

	/* Free buffers */
	free(state);
	free(output_buf);
}

void test_filtering_fir_q15(void)
{
	ztest_test_suite(filtering_fir_q15,
		ztest_unit_test(test_arm_fir_q15)
		);

	ztest_run_test_suite(filtering_fir_q15);
}
