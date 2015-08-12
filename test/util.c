#include "gerais.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

/* A test case that does nothing and succeeds. */
static void test_monta_nome(void **state) {
	char fullname[PATH_MAX];
	monta_nome(fullname, "/home/", "teste", "txt");
	assert_string_equals("/home/teste.txt", fullname);
}

/*
Suite *util_suite(void)
{
    Suite *s = suite_create("util");
    TCase *tc_util = tcase_create("util");
    suite_add_tcase (s, tc_util);
//    tcase_add_checked_fixture(tc_util, ck_leak_check_setup, ck_leak_check_teardown);
    tcase_add_test(tc_util, test_monta_nome);
    tcase_set_timeout(tc_util, 60);

    return s;
}
*/

int main(int argc, const char *argv[]) {
	const UnitTest tests[] = {
		unit_test(test_monta_nome),
	};
	return run_tests(tests);
/*
	Suite *s = util_suite();
	SRunner *sr = srunner_create (s);
	tests_set_cwd();
	srunner_run_all(sr, CK_ENV);
	failure_count = srunner_ntests_failed (sr);
	srunner_free (sr);
	if (failure_count == 0) {
		return EXIT_SUCCESS;
	}
	return  EXIT_FAILURE;
*/
}

