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
#include "CsvImport.h"
#include "Database.h"

#define FREE_PTRARR(PA)                                                        \
  for (int __ptrarr_element = 0; PA[__ptrarr_element]; __ptrarr_element++)     \
          g_free(PA[__ptrarr_element]);                                        \
  g_free(PA);


Stk_Model_Invoice*
invoice_from_csv(gchar **fields);

Stk_Model_InvoiceLine*
invoice_line_from_csv(gchar **fields);

Stk_Model_Invoice*
invoice_from_csv(gchar **fields)
{
	Stk_Model_Invoice *inv = Stk_Model_Invoice_new();
	Stk_Model_Invoice_set_doc_no(inv, fields[0]);
	Stk_Model_Invoice_set_customer(inv, fields[1]);
	Stk_Model_Invoice_set_date(inv, fields[2]);
	Stk_Model_Invoice_set_total(inv, g_ascii_strtod(fields[3], NULL));
	Stk_Model_Invoice_set_discount(inv, g_ascii_strtod(fields[4], NULL));
	return inv;
}

Stk_Model_InvoiceLine*
invoice_line_from_csv(gchar **fields)
{
	Stk_Model_InvoiceLine *iline = Stk_Model_InvoiceLine_new();
	Stk_Model_InvoiceLine_set_line_no(iline, g_ascii_strtoull(fields[5], NULL, 10));
	Stk_Model_InvoiceLine_set_product(iline, fields[6]);
	Stk_Model_InvoiceLine_set_qty(iline, g_ascii_strtoull(fields[7], NULL, 10));
	Stk_Model_InvoiceLine_set_price(iline, g_ascii_strtod(fields[8], NULL));
	Stk_Model_InvoiceLine_set_line_amt(iline, g_ascii_strtod(fields[9], NULL));
	return iline;
}

void
CsvImport_processline(gchar *line)
{
	gchar **fields = g_strsplit(line, ",", -1);
	gchar *doc_no = fields[0];
	Stk_Model_Invoice *inv = Stk_Database_Invoice_get(doc_no);
	if (!inv)
		inv = invoice_from_csv(fields);
	g_object_ref(inv);
	g_autoptr(Stk_Model_InvoiceLine) iline = invoice_line_from_csv(fields);
	Stk_Model_Invoice_add_line(inv, iline);
	Stk_Database_Invoice_save(inv);
	g_object_unref(inv);
	FREE_PTRARR(fields);
}
