#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "util/set.h"

void test_set_new(void **state) {
	OSET set;
	set_new(&set);
	assert_int_equal(set.n, 0);
	assert_int_equal(set.free, 0);
	assert_null(set.p);
	assert_int_equal(set.sizeofp, 0);
}

int main(int argc, const char *argv[]) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_set_new),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}

