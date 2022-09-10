#include <stdio.h>
#include <glib.h>
#include <string.h>
#include "models.h"
#include "database.h"

gchar**
file_get_lines(char *filepath, GError **error);

Invoice*
invoice_from_csv(gchar **fields);

void
invoice_pretty_print(Invoice *inv);

InvoiceLine*
invoice_line_from_csv(gchar **fields);

GString*
invoice_line_to_str(InvoiceLine *iline);

void
csv_line_process(gchar *line);


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

Invoice*
invoice_from_csv(gchar **fields)
{
	Invoice *inv = g_malloc(sizeof(Invoice));
	inv->doc_no = fields[0];
	inv->customer = fields[1];
	inv->date = fields[2];
	inv->total = g_ascii_strtod(fields[3], NULL);
	inv->discount = g_ascii_strtod(fields[4], NULL);
	inv->lines = NULL;
	return inv;
}

InvoiceLine*
invoice_line_from_csv(gchar **fields)
{
	InvoiceLine *iline = g_malloc(sizeof(InvoiceLine));
	iline->line_no = g_ascii_strtoull(fields[5], NULL, 10);
	iline->product = fields[6];
	iline->qty = g_ascii_strtoull(fields[7], NULL, 10);
	iline->price = g_ascii_strtod(fields[8], NULL);
	iline->line_amt = g_ascii_strtod(fields[9], NULL);
	return iline;
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

void
csv_line_process(gchar *line)
{
	gchar **fields = g_strsplit(line, ",", -1);
	gchar *doc_no = fields[0];
	Invoice *inv = database_invoice_get(doc_no);
	if (!inv)
		inv = invoice_from_csv(fields);
	InvoiceLine *iline = invoice_line_from_csv(fields);
	invoice_add_line(inv, iline);
	database_invoice_save(inv);
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
