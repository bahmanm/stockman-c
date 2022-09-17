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

#define STK_MODEL_TYPE_INVOICELINE (stk_model_invoiceline_get_type())

G_DECLARE_FINAL_TYPE(Stk_Model_InvoiceLine, std_model_invoiceline, STK_MODEL,
                     INVOICELINE, GObject)

Stk_Model_InvoiceLine *
stk_model_invoiceline_new(void);

guint
stk_model_invoiceline_get_line_no(Stk_Model_InvoiceLine *self);
void
stk_model_invoiceline_set_line_no(Stk_Model_InvoiceLine *self,
                                  guint line_no);

GString *
stk_model_invoiceline_get_product(Stk_Model_InvoiceLine *self);
void
stk_model_invoiceline_set_product(Stk_Model_InvoiceLine *self,
                                  gchar *product);

guint
stk_model_invoiceline_get_qty(Stk_Model_InvoiceLine *self);
void
stk_model_invoiceline_set_qty(Stk_Model_InvoiceLine *self, guint qty);

gdouble
stk_model_invoiceline_get_price(Stk_Model_InvoiceLine *self);
void
stk_model_invoiceline_set_price(Stk_Model_InvoiceLine *self,
                                gdouble price);

gdouble
stk_model_invoiceline_get_line_amt(Stk_Model_InvoiceLine *self);
void
stk_model_invoiceline_set_line_amt(Stk_Model_InvoiceLine *self, gdouble line_amt);

gint
stk_model_invoiceline_compare_by_line_no(gconstpointer iline_vptr, gconstpointer other_vptr);
