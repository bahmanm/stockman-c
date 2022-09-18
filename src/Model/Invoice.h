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

#include <glib-object.h>
#include "InvoiceLine.h"

#define STK_MODEL_TYPE_INVOICE (stk_model_invoice_get_type())
G_DECLARE_FINAL_TYPE(Stk_Model_Invoice, stk_model_invoice, STK_MODEL, INVOICE,
                     GObject)

Stk_Model_Invoice *stk_model_invoice_new();

GString *
stk_model_invoice_get_doc_no(Stk_Model_Invoice *self);
void
stk_model_invoice_set_doc_no(Stk_Model_Invoice *self, gchar *doc_no);

GString *
stk_model_invoice_get_customer(Stk_Model_Invoice *self);
void
stk_model_invoice_set_customer(Stk_Model_Invoice *self, gchar *customer);

GString *
stk_model_invoice_get_date(Stk_Model_Invoice *self);
void
stk_model_invoice_set_date(Stk_Model_Invoice *self, gchar *date);

gdouble
stk_model_invoice_get_total(Stk_Model_Invoice *self);
void
stk_model_invoice_set_total(Stk_Model_Invoice *self, gdouble total);

gdouble
stk_model_invoice_get_discount(Stk_Model_Invoice *self);
void
stk_model_invoice_set_discount(Stk_Model_Invoice *self, gdouble discount);

GList *
stk_model_invoice_get_lines(Stk_Model_Invoice *self);
void
stk_model_invoice_set_lines(Stk_Model_Invoice *self, GList *lines);
void
stk_model_invoice_add_line(Stk_Model_Invoice *self, Stk_Model_InvoiceLine *line);
