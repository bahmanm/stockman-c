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
#include "InvoiceLine.h"

struct _Stk_Model_InvoiceLine {
	GObject parent;

	guint line_no;
	GString *product;
	guint qty;
	gdouble price;
	gdouble line_amt;
};

G_DEFINE_FINAL_TYPE(Stk_Model_InvoiceLine, stk_model_invoiceline, G_TYPE_OBJECT)

static void stk_model_invoiceline_init(Stk_Model_InvoiceLine *self) {}

static void
stk_model_invoiceline_class_init(Stk_Model_InvoiceLineClass *klass) {}

Stk_Model_InvoiceLine *
stk_model_invoiceline_new(void)
{
	return g_object_new(STK_MODEL_TYPE_INVOICELINE, NULL);
}

guint
stk_model_invoiceline_get_line_no(Stk_Model_InvoiceLine *self)
{
	return self->line_no;
}

void
stk_model_invoiceline_set_line_no(Stk_Model_InvoiceLine *self,
                                  guint line_no)
{
	self->line_no = line_no;
}

GString *
stk_model_invoiceline_get_product(Stk_Model_InvoiceLine *self)
{
	return self->product;
}

void
stk_model_invoiceline_set_product(Stk_Model_InvoiceLine *self,
                                  gchar *product)
{
	if (self->product)
		g_object_unref(self->product);
	self->product = g_string_new(product);
}

guint
stk_model_invoiceline_get_qty(Stk_Model_InvoiceLine *self)
{
	return self->qty;
}

void
stk_model_invoiceline_set_qty(Stk_Model_InvoiceLine *self, guint qty)
{
	self->qty = qty;
}

gdouble
stk_model_invoiceline_get_price(Stk_Model_InvoiceLine *self)
{
	return self->price;
}

void
stk_model_invoiceline_set_price(Stk_Model_InvoiceLine *self,
                                gdouble price)
{
	self->price = price;
}

gdouble
stk_model_invoiceline_get_line_amt(Stk_Model_InvoiceLine *self)
{
	return self->line_amt;
}

void
stk_model_invoiceline_set_line_amt(Stk_Model_InvoiceLine *self, gdouble line_amt)
{
	self->line_amt = line_amt;
}

gint
stk_model_invoiceline_compare_by_line_no(gconstpointer iline_vptr, gconstpointer other_vptr)
{
	Stk_Model_InvoiceLine *iline = (Stk_Model_InvoiceLine *)iline_vptr;
	Stk_Model_InvoiceLine *other = (Stk_Model_InvoiceLine *)other_vptr;
	if (iline->line_no < other->line_no)
		return -1;
	else if (iline->line_no == other->line_no)
		return 0;
	else
		return 1;
}
