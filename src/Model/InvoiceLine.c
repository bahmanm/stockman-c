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

G_DEFINE_FINAL_TYPE(Stk_Model_InvoiceLine, Stk_Model_InvoiceLine, G_TYPE_OBJECT)

static void Stk_Model_InvoiceLine_init(Stk_Model_InvoiceLine *self) {}

static void
Stk_Model_InvoiceLine_finalise(GObject *object)
{
	Stk_Model_InvoiceLine *self = STK_MODEL_INVOICELINE(object);
	if (self->product)
		g_string_free(self->product, TRUE);
	G_OBJECT_CLASS(Stk_Model_InvoiceLine_parent_class)->finalize(object);
}

static void
Stk_Model_InvoiceLine_class_init(Stk_Model_InvoiceLineClass *klass)
{
	G_OBJECT_CLASS(klass)->finalize = Stk_Model_InvoiceLine_finalise;
}

Stk_Model_InvoiceLine *
Stk_Model_InvoiceLine_new(void)
{
	return g_object_new(STK_MODEL_TYPE_INVOICELINE, NULL);
}

guint
Stk_Model_InvoiceLine_get_line_no(Stk_Model_InvoiceLine *self)
{
	return self->line_no;
}

void
Stk_Model_InvoiceLine_set_line_no(Stk_Model_InvoiceLine *self,
                                  guint line_no)
{
	self->line_no = line_no;
}

GString *
Stk_Model_InvoiceLine_get_product(Stk_Model_InvoiceLine *self)
{
	return self->product;
}

void
Stk_Model_InvoiceLine_set_product(Stk_Model_InvoiceLine *self,
                                  gchar *product)
{
	if (self->product)
		g_string_free(self->product, TRUE);
	self->product = g_string_new(product);
}

guint
Stk_Model_InvoiceLine_get_qty(Stk_Model_InvoiceLine *self)
{
	return self->qty;
}

void
Stk_Model_InvoiceLine_set_qty(Stk_Model_InvoiceLine *self, guint qty)
{
	self->qty = qty;
}

gdouble
Stk_Model_InvoiceLine_get_price(Stk_Model_InvoiceLine *self)
{
	return self->price;
}

void
Stk_Model_InvoiceLine_set_price(Stk_Model_InvoiceLine *self,
                                gdouble price)
{
	self->price = price;
}

gdouble
Stk_Model_InvoiceLine_get_line_amt(Stk_Model_InvoiceLine *self)
{
	return self->line_amt;
}

void
Stk_Model_InvoiceLine_set_line_amt(Stk_Model_InvoiceLine *self, gdouble line_amt)
{
	self->line_amt = line_amt;
}

gint
Stk_Model_InvoiceLine_compare_by_line_no(gconstpointer iline_vptr, gconstpointer other_vptr)
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
