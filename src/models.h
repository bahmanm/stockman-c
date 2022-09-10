#pragma once

#include <glib.h>

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

/**
 * Add a line to the invoice.
 *
 * @param inv Invoice
 * @param iline Invoice line
 */
void
invoice_add_line(Invoice *inv, InvoiceLine *iline);

/**
 * Compare a given invoice line with another line by `line_no`.
 *
 * @param iline Invoice line
 * @param other The other invoice line
 * @return -1 if the given invoice line has a smaller `line_no`, 0 if they are
 *         equal and 1 if the given invoice line has a larger `line_no`.
 */
gint
invoice_line_compare_by_line_no(gconstpointer iline, gconstpointer other);
