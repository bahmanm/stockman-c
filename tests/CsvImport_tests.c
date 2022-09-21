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
#include "Database.h"
#include "CsvImport.h"
#include "Model/Invoice.h"
#include "Model/InvoiceLine.h"

void
database_setup()
{
}

void
database_teardown()
{
	Stk_Database_Invoice_clear();
}

gboolean
invoice_line_has_line_no(const void *iline_vptr, const void *line_no_vptr)
{
	Stk_Model_InvoiceLine *iline = (Stk_Model_InvoiceLine *)iline_vptr;
	guint line_no = *(guint *)line_no_vptr;
	if (line_no == Stk_Model_InvoiceLine_get_line_no(iline))
		return TRUE;
	else
		return FALSE;
}

void
test_CsvImport_processLine__existing_invoice()
{
	/* GIVEN */
	gchar *csvline1 = "SI-862,C-114,2016/10/16,8707.20,29,1,P-7964,192,4.33,831.36";
	gchar *csvline2 = "SI-862,C-114,2016/10/16,8707.20,29,2,P-1461,121,75.83,9175.43";
	// database is empty

	/* WHEN */
	CsvImport_processline(csvline1);
	CsvImport_processline(csvline2);

	/* THEN */
	Stk_Model_Invoice *inv = Stk_Database_Invoice_get("SI-862");
	g_assert_nonnull(inv);
	g_assert_cmpstr("SI-862", ==, Stk_Model_Invoice_get_doc_no(inv)->str);
	g_assert_cmpstr("C-114", ==, Stk_Model_Invoice_get_customer(inv)->str);
	g_assert_cmpstr("2016/10/16", ==, Stk_Model_Invoice_get_date(inv)->str);
	g_assert_cmpfloat_with_epsilon(8707.20f, Stk_Model_Invoice_get_total(inv), 0.009f);
	g_assert_cmpfloat_with_epsilon(29.00, Stk_Model_Invoice_get_discount(inv), 0.009f);

	GList *lines = Stk_Model_Invoice_get_lines(inv);
	g_assert_nonnull(lines);
	g_assert_cmpint(2, ==, g_list_length(lines));
	guint line_no = 2;
	GList *node = g_list_find_custom(lines, &line_no, invoice_line_has_line_no);
	g_assert_nonnull(node);
	Stk_Model_InvoiceLine *iline = node->data;
	g_assert_cmpint(1, ==, Stk_Model_InvoiceLine_get_line_no(iline));
	g_assert_cmpstr("P-7964", ==, Stk_Model_InvoiceLine_get_product(iline)->str);
	g_assert_cmpint(192, ==, Stk_Model_InvoiceLine_get_qty(iline));
	g_assert_cmpfloat_with_epsilon(4.33, Stk_Model_InvoiceLine_get_price(iline), 0.009f);
	g_assert_cmpfloat_with_epsilon(831.36, Stk_Model_InvoiceLine_get_line_amt(iline), 0.009f);
}

void
test_CsvImport_processLine__new_invoice()
{
	/* GIVEN */
	gchar *csvline = "SI-862,C-114,2016/10/16,8707.20,29,1,P-7964,192,4.33,831.36";
	// database is empty

	/* WHEN */
	CsvImport_processline(csvline);

	/* THEN */
	g_autoptr(Stk_Model_Invoice) inv = Stk_Database_Invoice_get("SI-862");
	g_assert_nonnull(inv);
	g_assert_cmpstr("SI-862", ==, Stk_Model_Invoice_get_doc_no(inv)->str);
	g_assert_cmpstr("C-114", ==, Stk_Model_Invoice_get_customer(inv)->str);
	g_assert_cmpstr("2016/10/16", ==, Stk_Model_Invoice_get_date(inv)->str);
	g_assert_cmpfloat_with_epsilon(8707.20f, Stk_Model_Invoice_get_total(inv), 0.009f);
	g_assert_cmpfloat_with_epsilon(29.00, Stk_Model_Invoice_get_discount(inv), 0.009f);

	GList *lines = Stk_Model_Invoice_get_lines(inv);
	g_assert_nonnull(lines);
	g_assert_nonnull(lines->data);
	g_assert_cmpint(1, ==, g_list_length(lines));
	Stk_Model_InvoiceLine *iline = lines->data;
	g_assert_cmpint(1, ==, Stk_Model_InvoiceLine_get_line_no(iline));
	g_assert_cmpstr("P-7964", ==, Stk_Model_InvoiceLine_get_product(iline)->str);
	g_assert_cmpint(192, ==, Stk_Model_InvoiceLine_get_qty(iline));
	g_assert_cmpfloat_with_epsilon(4.33, Stk_Model_InvoiceLine_get_price(iline), 0.009f);
	g_assert_cmpfloat_with_epsilon(831.36, Stk_Model_InvoiceLine_get_line_amt(iline), 0.009f);
}

int
main(int argc, char **argv)
{
	g_test_init(&argc, &argv, NULL);
	g_test_add("/CsvImport/processline/new_invoice",
	           gpointer, NULL,
	           database_setup, test_CsvImport_processLine__new_invoice, database_teardown);
	g_test_add("/CsvImport/processline/existing_invoice",
	           gpointer, NULL,
	           database_setup, test_CsvImport_processLine__existing_invoice, database_teardown);
	return g_test_run();
}
