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
#include "models.h"
#include "database.h"
#include "csvimport.h"

gchar**
file_get_lines(char *filepath, GError **error);

void
invoice_pretty_print(Models_Invoice *inv);

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
invoice_pretty_print(Models_Invoice *inv)
{
	inv->lines = g_list_sort(inv->lines, Models_InvoiceLine_compareByLineNo);
	g_print("\n"
	        "+------------------------------------------------------------------------------+\n"
	        "| INVOICE#: %-35s  DATE:       %-10s        |\n"
	        "| CUSTOMER: %-35s  DISCOUNT%%: %6.2f             |\n"
	        "+------------------------------------------------------------------------------+\n"
	        "| #   | PRODUCT                     | QTY      | PRICE      | AMOUNT           |\n"
	        "+------------------------------------------------------------------------------+\n",
	        inv->doc_no, inv->date,
	        inv->customer, inv->discount);
	for (GList *iline = inv->lines; iline; iline = iline->next) {
		Models_InvoiceLine *data = iline->data;
		g_print("| %3d   %-26s     %8d   %10.2f   %15.2f |\n",
		        data->line_no, data->product,
		        data->qty, data->price, data->line_amt);
	}
	g_print("+------------------------------------------------------------------------------+\n"
	        "|                                                     TOTAL: %17.2f |\n"
	        "+------------------------------------------------------------------------------+\n",
	        inv->total);
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
