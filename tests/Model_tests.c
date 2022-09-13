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
#include "models.h"

void
test_Invoice_addLine()
{
	// GIVEN
	Models_Invoice inv = {.lines = NULL};
	Models_InvoiceLine iline1 = {.line_no = 10, .product = "P1"};
	Models_InvoiceLine iline2 = {.line_no = 20, .product = "P2"};

	// EXPECT
	g_assert_null(inv.lines);

	// WHEN
	Models_Invoice_addLine(&inv, &iline1);

	// THEN
	g_assert_nonnull(inv.lines);
	g_assert_cmpint(1, ==, g_list_length(inv.lines));
	g_assert_nonnull(g_list_find(inv.lines, &iline1));

	// WHEN
	Models_Invoice_addLine(&inv, &iline2);

	// THEN
	g_assert_cmpint(2, ==, g_list_length(inv.lines));
	g_assert_nonnull(g_list_find(inv.lines, &iline1));
	g_assert_nonnull(g_list_find(inv.lines, &iline2));
}

void
test_InvoiceLine_compareByLineNo()
{
	// GIVEN
	Models_InvoiceLine iline1 = {.line_no = 10};
	Models_InvoiceLine iline2 = {.line_no = 20};

	// EXPECT
	g_assert_cmpint(0, ==, Models_InvoiceLine_compareByLineNo(&iline1, &iline1));
	g_assert_cmpint(0, >, Models_InvoiceLine_compareByLineNo(&iline1, &iline2));
	g_assert_cmpint(0, <, Models_InvoiceLine_compareByLineNo(&iline2, &iline1));
}

int
main(int argc, char **argv)
{
	g_test_init(&argc, &argv, NULL);
	g_test_add_func("/models/Invoice/addLine",
	                test_Invoice_addLine);
	g_test_add_func("/models/InvoiceLine/compareByLineNo",
	                test_InvoiceLine_compareByLineNo);
	return g_test_run();
}
