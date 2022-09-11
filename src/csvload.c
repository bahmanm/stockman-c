#include <glib.h>
#include "csvload.h"
#include "database.h"
#include "models.h"

Invoice*
invoice_from_csv(gchar **fields);

InvoiceLine*
invoice_line_from_csv(gchar **fields);

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
