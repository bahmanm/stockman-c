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
#include "database.h"
#include "models.h"
#include "csvimport.h"

void
database_setup()
{
	Database_init();
}

void
database_teardown()
{
	Database_Invoice_clear(NULL);
}

gboolean
invoice_line_has_line_no(const void *iline_vptr, const void *line_no_vptr)
{
	Models_InvoiceLine *iline = (Models_InvoiceLine *)iline_vptr;
	guint line_no = *(guint *)line_no_vptr;
	if (line_no == iline->line_no)
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
	CsvImport_processLine(csvline1);
	CsvImport_processLine(csvline2);

	/* THEN */
	Models_Invoice *inv = Database_Invoice_get("SI-862");
	g_assert_nonnull(inv);
	g_assert_cmpstr("SI-862", ==, inv->doc_no);
	g_assert_cmpstr("C-114", ==, inv->customer);
	g_assert_cmpstr("2016/10/16", ==, inv->date);
	g_assert_cmpfloat_with_epsilon(8707.20f, inv->total, 0.009f);
	g_assert_cmpfloat_with_epsilon(29.00, inv->discount, 0.009f);

	g_assert_nonnull(inv->lines);
	g_assert_cmpint(2, ==, g_list_length(inv->lines));
	guint line_no = 2;
	GList *node = g_list_find_custom(inv->lines, &line_no, invoice_line_has_line_no);
	g_assert_nonnull(node);
	Models_InvoiceLine *iline = node->data;
	g_assert_cmpint(1, ==, iline->line_no);
	g_assert_cmpstr("P-7964", ==, iline->product);
	g_assert_cmpint(192, ==, iline->qty);
	g_assert_cmpfloat_with_epsilon(4.33, iline->price, 0.009f);
	g_assert_cmpfloat_with_epsilon(831.36, iline->line_amt, 0.009f);
}

void
test_CsvImport_processLine__new_invoice()
{
	/* GIVEN */
	gchar *csvline = "SI-862,C-114,2016/10/16,8707.20,29,1,P-7964,192,4.33,831.36";
	// database is empty

	/* WHEN */
	CsvImport_processLine(csvline);

	/* THEN */
	Models_Invoice *inv = Database_Invoice_get("SI-862");
	g_assert_nonnull(inv);
	g_assert_cmpstr("SI-862", ==, inv->doc_no);
	g_assert_cmpstr("C-114", ==, inv->customer);
	g_assert_cmpstr("2016/10/16", ==, inv->date);
	g_assert_cmpfloat_with_epsilon(8707.20f, inv->total, 0.009f);
	g_assert_cmpfloat_with_epsilon(29.00, inv->discount, 0.009f);

	g_assert_nonnull(inv->lines);
	g_assert_cmpint(1, ==, g_list_length(inv->lines));
	Models_InvoiceLine *iline = inv->lines->data;
	g_assert_cmpint(1, ==, iline->line_no);
	g_assert_cmpstr("P-7964", ==, iline->product);
	g_assert_cmpint(192, ==, iline->qty);
	g_assert_cmpfloat_with_epsilon(4.33, iline->price, 0.009f);
	g_assert_cmpfloat_with_epsilon(831.36, iline->line_amt, 0.009f);
}

int
main(int argc, char **argv)
{
	g_test_init(&argc, &argv, NULL);
	g_test_add("/CsvImport/processLine/new_invoice",
	           gpointer, NULL,
	           database_setup, test_CsvImport_processLine__new_invoice, database_teardown);
	g_test_add("/CsvImport/processLine/existing_invoice",
	           gpointer, NULL,
	           database_setup, test_CsvImport_processLine__existing_invoice, database_teardown);
	return g_test_run();
}
