#include <glib.h>
#include "models.h"

void
invoice_add_line(Invoice *inv, InvoiceLine *iline)
{
	inv->lines = g_list_prepend(inv->lines, iline);
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
