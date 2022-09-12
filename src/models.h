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
Invoice_addLine(Invoice *inv, InvoiceLine *iline);

/**
 * Compare a given invoice line with another line by `line_no`.
 *
 * @param iline Invoice line
 * @param other The other invoice line
 * @return -1 if the given invoice line has a smaller `line_no`, 0 if they are
 *         equal and 1 if the given invoice line has a larger `line_no`.
 */
gint
InvoiceLine_compareByLineNo(gconstpointer iline, gconstpointer other);
