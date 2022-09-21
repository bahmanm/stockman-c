/*
 * Copyright 2022 Bahman Movaqar
 *
 * This file is part of Stockman-C.
 *
 * Stockman-C is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Stockman-C is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Stockman-C. If not, see <https://www.gnu.org/licenses/>.
 */
#include <glib.h>
#include "Model/Model.h"

void
test_Stk_Model_Invoice_add_line()
{
	/* GIVEN */
	g_autoptr(Stk_Model_Invoice) inv = Stk_Model_Invoice_new();

	/* EXPECT */
	g_assert_null(Stk_Model_Invoice_get_lines(inv));

	/* WHEN */
	g_autoptr(Stk_Model_InvoiceLine) iline1 = Stk_Model_InvoiceLine_new();
	Stk_Model_InvoiceLine_set_line_no(iline1, 10);
	Stk_Model_InvoiceLine_set_product(iline1, "P1");
	Stk_Model_Invoice_add_line(inv, iline1);

	/* THEN */
	GList *lines1 = Stk_Model_Invoice_get_lines(inv);
	g_assert_nonnull(lines1);
	g_assert_cmpint(1, ==, g_list_length(lines1));
	g_assert_nonnull(g_list_find(lines1, iline1));

	/* WHEN */
	g_autoptr(Stk_Model_InvoiceLine)  iline2 = Stk_Model_InvoiceLine_new();
	Stk_Model_InvoiceLine_set_line_no(iline2, 20);
	Stk_Model_InvoiceLine_set_product(iline2, "P2");
	Stk_Model_Invoice_add_line(inv, iline2);

	/* THEN */
	GList *lines2 = Stk_Model_Invoice_get_lines(inv);
	g_assert_cmpint(2, ==, g_list_length(lines2));
	g_assert_nonnull(g_list_find(lines2, iline1));
	g_assert_nonnull(g_list_find(lines2, iline2));
}

void
test_Stk_Model_InvoiceLine_compare_by_line_no()
{
	/* GIVEN */
	g_autoptr(Stk_Model_InvoiceLine) iline1 = Stk_Model_InvoiceLine_new();
	Stk_Model_InvoiceLine_set_line_no(iline1, 10);
	g_autoptr(Stk_Model_InvoiceLine) iline2 = Stk_Model_InvoiceLine_new();
	Stk_Model_InvoiceLine_set_line_no(iline2, 20);

	/* EXPECT */
	g_assert_cmpint(0, ==, Stk_Model_InvoiceLine_compare_by_line_no(iline1, iline1));
	g_assert_cmpint(0, >, Stk_Model_InvoiceLine_compare_by_line_no(iline1, iline2));
	g_assert_cmpint(0, <, Stk_Model_InvoiceLine_compare_by_line_no(iline2, iline1));
}

int
main(int argc, char **argv)
{
	g_test_init(&argc, &argv, NULL);
	g_test_add_func("/Model/Invoice/add_line",
	                test_Stk_Model_Invoice_add_line);
	g_test_add_func("/Model/InvoiceLine/compare_by_line_no",
	                test_Stk_Model_InvoiceLine_compare_by_line_no);
	return g_test_run();
}
