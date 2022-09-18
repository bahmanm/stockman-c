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


Stk_Model_Invoice*
invoice_from_csv(gchar **fields);

Stk_Model_InvoiceLine*
invoice_line_from_csv(gchar **fields);

Stk_Model_Invoice*
invoice_from_csv(gchar **fields)
{
	Stk_Model_Invoice *inv = stk_model_invoice_new();
	stk_model_invoice_set_doc_no(inv, fields[0]);
	stk_model_invoice_set_customer(inv, fields[1]);
	stk_model_invoice_set_date(inv, fields[2]);
	stk_model_invoice_set_total(inv, g_ascii_strtod(fields[3], NULL));
	stk_model_invoice_set_total(inv, g_ascii_strtod(fields[3], NULL));
	stk_model_invoice_set_discount(inv, g_ascii_strtod(fields[4], NULL));
	g_object_ref(inv);
	return inv;
}

Stk_Model_InvoiceLine*
invoice_line_from_csv(gchar **fields)
{
	Stk_Model_InvoiceLine *iline = stk_model_invoiceline_new();
	stk_model_invoiceline_set_line_no(iline, g_ascii_strtoull(fields[5], NULL, 10));
	stk_model_invoiceline_set_product(iline, fields[6]);
	stk_model_invoiceline_set_qty(iline, g_ascii_strtoull(fields[7], NULL, 10));
	stk_model_invoiceline_set_price(iline, g_ascii_strtod(fields[8], NULL));
	stk_model_invoiceline_set_line_amt(iline, g_ascii_strtod(fields[9], NULL));
	g_object_ref(iline);
	return iline;
}

void
CsvImport_processline(gchar *line)
{
	gchar **fields = g_strsplit(line, ",", -1);
	gchar *doc_no = fields[0];
	g_autoptr(Stk_Model_Invoice) inv = Database_Invoice_get(doc_no);
	if (!inv)
		inv = invoice_from_csv(fields);
	g_autoptr(Stk_Model_InvoiceLine) iline = invoice_line_from_csv(fields);
	stk_model_invoice_add_line(inv, iline);
	Database_Invoice_save(inv);
}
