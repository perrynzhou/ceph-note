/*
 *<tested_file_path>src/mngt/ocf_mngt_cache.c</tested_file_path>
 *	<tested_function>ocf_mngt_cache_set_fallback_pt_error_threshold</tested_function>
 *	<functions_to_leave>
 *	INSERT HERE LIST OF FUNCTIONS YOU WANT TO LEAVE
 *	ONE FUNCTION PER LINE
 *</functions_to_leave>
 */

#undef static

#undef inline


#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "print_desc.h"

#include "ocf/ocf.h"
#include "ocf_mngt_common.h"
#include "../ocf_core_priv.h"
#include "../ocf_queue_priv.h"
#include "../metadata/metadata.h"
#include "../engine/cache_engine.h"
#include "../utils/utils_part.h"
#include "../utils/utils_cache_line.h"
#include "../utils/utils_device.h"
#include "../utils/utils_io.h"
#include "../utils/utils_cache_line.h"
#include "../utils/utils_pipeline.h"
#include "../ocf_utils.h"
#include "../concurrency/ocf_concurrency.h"
#include "../eviction/ops.h"
#include "../ocf_ctx_priv.h"
#include "../cleaning/cleaning.h"

int __wrap_ocf_log_raw(ocf_logger_t logger, ocf_logger_lvl_t lvl,
		const char *fmt, ...)
{
	function_called();
}

ocf_ctx_t __wrap_ocf_cache_get_ctx(ocf_cache_t cache)
{
	function_called();
}

int __wrap_ocf_mng_cache_set_fallback_pt(ocf_cache_t cache)
{
	function_called();
}

bool __wrap_ocf_fallback_pt_is_on(ocf_cache_t cache)
{
}

char *__wrap_ocf_cache_get_name(ocf_cache_t cache)
{
}

void __wrap__ocf_mngt_test_volume_initial_write(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap_ocf_mngt_test_volume_first_read(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_test_volume_discard(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_test_volume_second_read(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_attach_cache_device(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_attach_check_ram(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_attach_load_properties(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_attach_prepare_metadata(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_test_volume(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_attach_load_superblock(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_attach_init_instance(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_attach_clean_pol(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_attach_flush_metadata(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_attach_discard(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_attach_flush(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_attach_shutdown_status(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_attach_post_init(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap_ocf_mngt_cache_stop_wait_io(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap_ocf_mngt_cache_stop_remove_cores(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap_ocf_mngt_cache_stop_unplug(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap_ocf_mngt_cache_stop_put_io_queues(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap_ocf_mngt_cache_detach_flush(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap_ocf_mngt_cache_detach_wait_pending(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap_ocf_mngt_cache_detach_update_metadata(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap_ocf_mngt_cache_detach_unplug(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_test_volume_first_read(
		  ocf_pipeline_t pipeline, void *priv, ocf_pipeline_arg_t arg)
{
}

void __wrap__ocf_mngt_test_volume_finish(
		  ocf_pipeline_t pipeline, void *priv, int error)
{
}

void __wrap__ocf_mngt_cache_attach_finish(
		  ocf_pipeline_t pipeline, void *priv, int error)
{
}

void __wrap_ocf_mngt_cache_stop_finish(
		  ocf_pipeline_t pipeline, void *priv, int error)
{
}

void __wrap_ocf_mngt_cache_detach_finish(
		  ocf_pipeline_t pipeline, void *priv, int error)
{
}

void __wrap_ocf_mngt_cache_save_finish(
		  ocf_pipeline_t pipeline, void *priv, int error)
{
}

static void ocf_mngt_cache_set_fallback_pt_error_threshold_test01(void **state)
{
	struct ocf_cache cache;
	int new_threshold;
	int result;

	print_test_description("Appropriate error code on invalid threshold value");

	new_threshold = -1;

	result = ocf_mngt_cache_set_fallback_pt_error_threshold(&cache, new_threshold);

	assert_int_equal(result, -OCF_ERR_INVAL);


	new_threshold = 10000001;

	result = ocf_mngt_cache_set_fallback_pt_error_threshold(&cache, new_threshold);

	assert_int_equal(result, -OCF_ERR_INVAL);
}

static void ocf_mngt_cache_set_fallback_pt_error_threshold_test02(void **state)
{
	struct ocf_cache cache;
	int new_threshold;
	int old_threshold;

	print_test_description("Invalid new threshold value doesn't change current threshold");

	new_threshold = -1;
	old_threshold = cache.fallback_pt_error_threshold = 1000;

	ocf_mngt_cache_set_fallback_pt_error_threshold(&cache, new_threshold);

	assert_int_equal(cache.fallback_pt_error_threshold, old_threshold);


	new_threshold = 10000001;
	old_threshold = cache.fallback_pt_error_threshold = 1000;

	ocf_mngt_cache_set_fallback_pt_error_threshold(&cache, new_threshold);

	assert_int_equal(cache.fallback_pt_error_threshold, old_threshold);
}

static void ocf_mngt_cache_set_fallback_pt_error_threshold_test03(void **state)
{
	struct ocf_cache cache;
	int new_threshold, old_threshold;

	print_test_description("Setting new threshold value");

	new_threshold = 5000;
	old_threshold = cache.fallback_pt_error_threshold = 1000;

	ocf_mngt_cache_set_fallback_pt_error_threshold(&cache, new_threshold);

	assert_int_equal(cache.fallback_pt_error_threshold, new_threshold);


	new_threshold = 1000000;
	old_threshold = cache.fallback_pt_error_threshold = 1000;

	ocf_mngt_cache_set_fallback_pt_error_threshold(&cache, new_threshold);

	assert_int_equal(cache.fallback_pt_error_threshold, new_threshold);


	new_threshold = 0;
	old_threshold = cache.fallback_pt_error_threshold = 1000;

	ocf_mngt_cache_set_fallback_pt_error_threshold(&cache, new_threshold);

	assert_int_equal(cache.fallback_pt_error_threshold, new_threshold);
}

static void ocf_mngt_cache_set_fallback_pt_error_threshold_test04(void **state)
{
	struct ocf_cache cache;
	int new_threshold;
	int result;

	print_test_description("Return appropriate value on success");

	new_threshold = 5000;

	result = ocf_mngt_cache_set_fallback_pt_error_threshold(&cache, new_threshold);

	assert_int_equal(result, 0);


	new_threshold = 1000000;

	result = ocf_mngt_cache_set_fallback_pt_error_threshold(&cache, new_threshold);

	assert_int_equal(cache.fallback_pt_error_threshold, new_threshold);


	new_threshold = 0;

	result = ocf_mngt_cache_set_fallback_pt_error_threshold(&cache, new_threshold);

	assert_int_equal(result, 0);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(ocf_mngt_cache_set_fallback_pt_error_threshold_test01),
		cmocka_unit_test(ocf_mngt_cache_set_fallback_pt_error_threshold_test02),
		cmocka_unit_test(ocf_mngt_cache_set_fallback_pt_error_threshold_test03),
		cmocka_unit_test(ocf_mngt_cache_set_fallback_pt_error_threshold_test04)
	};

	print_message("Unit test of src/mngt/ocf_mngt_cache.c");

	return cmocka_run_group_tests(tests, NULL, NULL);
}
