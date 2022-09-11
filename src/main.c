#include <stdio.h>
#include <glib.h>
#include <string.h>
#include "models.h"
#include "database.h"
#include "csvload.h"

gchar**
file_get_lines(char *filepath, GError **error);

void
invoice_pretty_print(Invoice *inv);

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
invoice_pretty_print(Invoice *inv)
{
	inv->lines = g_list_sort(inv->lines, invoice_line_compare_by_line_no);
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
		InvoiceLine *data = iline->data;
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
	g_log_set_writer_func(g_log_writer_standard_streams, NULL, NULL);
	database_init();

	g_autoptr(GError) error = NULL;
	gchar **lines;
	if (!(lines = file_get_lines("/home/bahman/tmp/sales-invoices-tiny.csv", &error)))
		g_error("ERROR: %s\n", error->message);
	for (int line=0; lines[line]; line++)
		csv_line_process(lines[line]);
	database_invoices_foreach(invoice_pretty_print);
	return 0;
}
