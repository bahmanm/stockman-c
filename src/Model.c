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
#include "Model.h"

void
Model_Invoice_addLine(Model_Invoice *inv, Model_InvoiceLine *iline)
{
	inv->lines = g_list_prepend(inv->lines, iline);
}


gint
Model_InvoiceLine_compareByLineNo(gconstpointer iline_vptr, gconstpointer other_vptr)
{
	Model_InvoiceLine *iline = (Model_InvoiceLine *)iline_vptr;
	Model_InvoiceLine *other = (Model_InvoiceLine *)other_vptr;
	if (iline->line_no < other->line_no)
		return -1;
	else if (iline->line_no == other->line_no)
		return 0;
	else
		return 1;
}
