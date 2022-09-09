#include <stdio.h>
#include <glib.h>
#include <string.h>

typedef struct InvoiceLine {
	guint line_no;
	gchar *product;
	guint qty;
	gdouble price;
	gdouble line_amt;
} InvoiceLine;

typedef struct Invoice {
	gchar *doc_no;
	gchar *customer;
	gchar *date;
	gdouble total;
	gdouble discount;
	GList *lines;
} Invoice;

typedef struct Database {
	GHashTable * invoices;
} Database;

gchar**
file_get_lines(char *filepath, GError **error);

Invoice*
invoice_from_csv(gchar **fields);

void
invoice_pretty_print(gpointer doc_no, gpointer inv, gpointer user_data);

void
invoice_add_line(Invoice *inv, InvoiceLine *iline);

InvoiceLine*
invoice_line_from_csv(gchar **fields);

GString*
invoice_line_to_str(InvoiceLine *iline);

gint
invoice_line_compare_by_line_no(gconstpointer iline, gconstpointer other);

const Database*
database_get();

Database*
database_init();

Invoice*
database_invoice_get(gchar *doc_no);

gboolean
database_invoice_save(Invoice *inv);

gboolean
database_invoice_update(Invoice *inv);

gboolean
database_invoice_delete(gchar *doc_no);

void
csv_line_process(gchar *line);

Database* db;

gboolean
database_invoice_save(Invoice *inv)
{
	if (g_hash_table_lookup(database_get()->invoices, inv->doc_no))
		return FALSE;
	g_hash_table_insert(database_get()->invoices, inv->doc_no, inv);
	return TRUE;
}

Invoice*
database_invoice_get(gchar *doc_no)
{
	return g_hash_table_lookup(database_get()->invoices, doc_no);
}

gboolean
database_invoice_update(Invoice *inv)
{
	return g_hash_table_insert(database_get()->invoices, inv->doc_no, inv);
}

gboolean
database_invoice_delete(gchar *doc_no)
{
	return g_hash_table_remove(database_get()->invoices, doc_no);
}


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

gint
invoice_line_compare_by_line_no(gconstpointer ilineptr, gconstpointer otherptr)
{
	InvoiceLine *iline = (InvoiceLine *)ilineptr;
	InvoiceLine *other = (InvoiceLine *)otherptr;
	if (iline->line_no < other->line_no)
		return -1;
	else if (iline->line_no == other->line_no)
		return 0;
	else
		return 1;
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
invoice_pretty_print(gpointer doc_no, gpointer invp, gpointer user_data)
{
	Invoice *inv = (Invoice*) invp;
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
invoice_add_line(Invoice *inv, InvoiceLine *iline)
{
	inv->lines = g_list_prepend(inv->lines, iline);
}

const Database*
database_get()
{
	if (!db) {
		g_error("database is NOT initialised.\n");
		g_abort();
	}
	return db;
}

Database*
database_init()
{
	g_debug("Initialising the database...");
	Database* db = g_malloc(sizeof(Database));
	db->invoices = g_hash_table_new(g_str_hash, g_str_equal);
	return db;
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
	database_invoice_update(inv);
}

int
main(int argc, char **argv)
{
	g_log_set_writer_func(g_log_writer_standard_streams, NULL, NULL);
	db = database_init();

	g_autoptr(GError) error = NULL;
	gchar **lines;
	if (!(lines = file_get_lines("/home/bahman/tmp/sales-invoices-tiny.csv", &error)))
		g_error("ERROR: %s\n", error->message);
	for (int line=0; lines[line]; line++)
		csv_line_process(lines[line]);
	g_hash_table_foreach(db->invoices, invoice_pretty_print, NULL);
	return 0;
}
