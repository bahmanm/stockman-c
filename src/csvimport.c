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
#include "csvimport.h"
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
csvimport_line_process(gchar *line)
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
