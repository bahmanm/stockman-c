#include <glib.h>

void
first_test()
{
	g_assert_true(1 == 2);
}

int
main(int argc, char **argv)
{
	g_test_init(&argc, &argv, NULL);
	g_test_add_func("/model/invoice/first", first_test);
	return g_test_run();
}
