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
#include "Invoice.h"

struct _Stk_Model_Invoice {
	GObject parent;

	GString *doc_no;
	GString *customer;
	GString *date;
	gdouble total;
	gdouble discount;
	GList *lines;
};

G_DEFINE_TYPE(Stk_Model_Invoice, stk_model_invoice, G_TYPE_OBJECT)

static void stk_model_invoice_init(Stk_Model_Invoice *self)
{
	self->doc_no = NULL;
	self->customer = NULL;
	self->date = NULL;
	self->lines = NULL;
}

static void
Stk_Model_Invoice_finalise(GObject *object)
{
	Stk_Model_Invoice *self = STK_MODEL_INVOICE(object);
	if (self->customer)
		g_string_free(g_steal_pointer(&self->customer), TRUE);
	if (self->date)
		g_string_free(g_steal_pointer(&self->date), TRUE);
	if (self->doc_no)
		g_string_free(g_steal_pointer(&self->doc_no), TRUE);
	if (self->lines)
		g_list_free_full(g_steal_pointer(&self->lines), g_object_unref);
	G_OBJECT_CLASS(stk_model_invoice_parent_class)->finalize(object);
}


static void
stk_model_invoice_class_init(Stk_Model_InvoiceClass *klass)
{
	G_OBJECT_CLASS(klass)->finalize = Stk_Model_Invoice_finalise;
}

Stk_Model_Invoice *
stk_model_invoice_new()
{
	return g_object_new(STK_MODEL_TYPE_INVOICE, NULL);
}

GString *
stk_model_invoice_get_doc_no(Stk_Model_Invoice *self)
{
	return self->doc_no;
}

void
stk_model_invoice_set_doc_no(Stk_Model_Invoice *self, gchar *doc_no)
{
	if (self->doc_no)
		g_string_free(g_steal_pointer(&self->doc_no), TRUE);
	self->doc_no = g_string_new(doc_no);
}

GString *
stk_model_invoice_get_customer(Stk_Model_Invoice *self)
{
	return self->customer;
}

void
stk_model_invoice_set_customer(Stk_Model_Invoice *self, gchar *customer)
{
	if (self->customer)
		g_string_free(g_steal_pointer(&self->customer), TRUE);
	self->customer = g_string_new(customer);
}

GString *
stk_model_invoice_get_date(Stk_Model_Invoice *self)
{
	return self->date;
}

void
stk_model_invoice_set_date(Stk_Model_Invoice *self, gchar *date)
{
	if (self->date)
		g_string_free(g_steal_pointer(&self->date), TRUE);
	self->date = g_string_new(date);
}

gdouble
stk_model_invoice_get_total(Stk_Model_Invoice *self)
{
	return self->total;
}

void
stk_model_invoice_set_total(Stk_Model_Invoice *self, gdouble total)
{
	self->total = total;
}

gdouble
stk_model_invoice_get_discount(Stk_Model_Invoice *self)
{
	return self->discount;
}

void
stk_model_invoice_set_discount(Stk_Model_Invoice *self, gdouble discount)
{
	self->discount = discount;
}

GList *
stk_model_invoice_get_lines(Stk_Model_Invoice *self)
{
	return self->lines;
}

void
stk_model_invoice_set_lines(Stk_Model_Invoice *self, GList *lines)
{
	if (self->lines)
		g_list_free_full(g_steal_pointer(&self->lines), g_object_unref);
	self->lines = lines;
}

void
stk_model_invoice_add_line(Stk_Model_Invoice *self, Stk_Model_InvoiceLine *line)
{
	if (line) {
		g_object_ref(line);
		self->lines = g_list_prepend(self->lines, line);
	}
}
