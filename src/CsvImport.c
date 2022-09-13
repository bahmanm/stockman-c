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

Models_Invoice*
invoice_from_csv(gchar **fields);

Models_InvoiceLine*
invoice_line_from_csv(gchar **fields);

Models_Invoice*
invoice_from_csv(gchar **fields)
{
	Models_Invoice *inv = g_malloc(sizeof(Models_Invoice));
	inv->doc_no = fields[0];
	inv->customer = fields[1];
	inv->date = fields[2];
	inv->total = g_ascii_strtod(fields[3], NULL);
	inv->discount = g_ascii_strtod(fields[4], NULL);
	inv->lines = NULL;
	return inv;
}

Models_InvoiceLine*
invoice_line_from_csv(gchar **fields)
{
	Models_InvoiceLine *iline = g_malloc(sizeof(Models_InvoiceLine));
	iline->line_no = g_ascii_strtoull(fields[5], NULL, 10);
	iline->product = fields[6];
	iline->qty = g_ascii_strtoull(fields[7], NULL, 10);
	iline->price = g_ascii_strtod(fields[8], NULL);
	iline->line_amt = g_ascii_strtod(fields[9], NULL);
	return iline;
}

void
CsvImport_processLine(gchar *line)
{
	gchar **fields = g_strsplit(line, ",", -1);
	gchar *doc_no = fields[0];
	Models_Invoice *inv = Database_Invoice_get(doc_no);
	if (!inv)
		inv = invoice_from_csv(fields);
	Models_InvoiceLine *iline = invoice_line_from_csv(fields);
	Models_Invoice_addLine(inv, iline);
	Database_Invoice_save(inv);
}