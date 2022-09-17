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
#include <stdio.h>
#include <glib.h>
#include <string.h>
#include "model/invoice.h"
#include "model/invoiceline.h"
#include "model/model.h"
#include "Database.h"
#include "CsvImport.h"

gchar**
file_get_lines(char *filepath, GError **error);

void
invoice_pretty_print(Stk_Model_Invoice *inv);

gchar**
file_get_lines(char *filepath, GError **error)
{
	gchar *txt;
	gsize txtlen;
	if (!g_file_get_contents(filepath, &txt, &txtlen, error))
		return FALSE;
	gchar **lines = g_strsplit(txt, "\n", -1);
	return lines;
}

void
invoice_pretty_print(Stk_Model_Invoice *inv)
{
	stk_model_invoice_set_lines(inv,
	                            g_list_sort(stk_model_invoice_get_lines(inv),
	                                        stk_model_invoiceline_compare_by_line_no));
	g_print("\n"
	        "+------------------------------------------------------------------------------+\n"
	        "| INVOICE#: %-35s  DATE:       %-10s        |\n"
	        "| CUSTOMER: %-35s  DISCOUNT%%: %6.2f             |\n"
	        "+------------------------------------------------------------------------------+\n"
	        "| #   | PRODUCT                     | QTY      | PRICE      | AMOUNT           |\n"
	        "+------------------------------------------------------------------------------+\n",
	        stk_model_invoice_get_doc_no(inv)->str,
	        stk_model_invoice_get_date(inv)->str,
	        stk_model_invoice_get_customer(inv)->str,
	        stk_model_invoice_get_discount(inv));
	for (GList *iline = stk_model_invoice_get_lines(inv); iline; iline = iline->next) {
		Stk_Model_InvoiceLine *data = iline->data;
		g_print("| %3d   %-26s     %8d   %10.2f   %15.2f |\n",
		        stk_model_invoiceline_get_line_no(data),
		        stk_model_invoiceline_get_product(data)->str,
		        stk_model_invoiceline_get_qty(data),
		        stk_model_invoiceline_get_price(data),
		        stk_model_invoiceline_get_line_amt(data));
	}
	g_print("+------------------------------------------------------------------------------+\n"
	        "|                                                     TOTAL: %17.2f |\n"
	        "+------------------------------------------------------------------------------+\n",
	        stk_model_invoice_get_total(inv));
}

int
main(int argc, char **argv)
{
	if (argc != 2) {
		g_print("USAGE: stockman-c PATH_TO_CSV_FILE\n");
		return 0;
	}
	g_log_set_writer_func(g_log_writer_standard_streams, NULL, NULL);
	Database_init();

	g_autoptr(GError) error = NULL;
	gchar **lines;
	if (!(lines = file_get_lines(argv[1], &error)))
		g_error("ERROR: %s\n", error->message);
	for (int line=0; lines[line]; line++)
		CsvImport_processLine(lines[line]);
	Database_Invoice_foreach(invoice_pretty_print);
	return 0;
}
