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
#include "Database.h"

void
database_setup()
{
	//Stk_Database_init();
}

void
database_teardown()
{
	Stk_Database_Invoice_clear(NULL);
}

void
test_Stk_Database_Invoice_get_and_save()
{
	{
		/* GIVEN */
		g_autoptr(Stk_Model_Invoice) inv = stk_model_invoice_new();
		stk_model_invoice_set_doc_no(inv, "I1");

		/* WHEN */
		Stk_Database_Invoice_save(inv);
	}

	/* THEN */
	Stk_Model_Invoice* actual = Stk_Database_Invoice_get("I1");
	g_assert_nonnull(actual);
	g_assert_cmpstr("I1", ==, (stk_model_invoice_get_doc_no(actual))->str);
}

guint invoices_foreach_func_counter  = 0;
GString *invoices_foreach_func_doc_nos[2] = {NULL, NULL};

void
invoices_foreach_func(Stk_Model_Invoice *inv)
{
	invoices_foreach_func_doc_nos[invoices_foreach_func_counter] = stk_model_invoice_get_doc_no(inv);
	invoices_foreach_func_counter += 1;
}

void
test_Stk_Database_Invoice_foreach()
{
	/* GIVEN */
	gchar *inv1_doc_no = "I1";
	gchar *inv2_doc_no = "I2";
	{
		g_autoptr(Stk_Model_Invoice) inv1 = stk_model_invoice_new();
		stk_model_invoice_set_doc_no(inv1, "I1");
		Stk_Database_Invoice_save(inv1);
		g_autoptr(Stk_Model_Invoice) inv2 = stk_model_invoice_new();
		stk_model_invoice_set_doc_no(inv2, "I2");
		Stk_Database_Invoice_save(inv2);
	}

	/* WHEN */
	Stk_Database_Invoice_foreach(invoices_foreach_func);

	/* THEN */
	g_assert_cmpint(2, ==, invoices_foreach_func_counter);
	// foreach doesn't guarantee order
	g_assert_true((g_strcmp0(invoices_foreach_func_doc_nos[0]->str, inv1_doc_no) == 0
	               && g_strcmp0(invoices_foreach_func_doc_nos[1]->str, inv2_doc_no) == 0)
	              ||
	              (g_strcmp0(invoices_foreach_func_doc_nos[0]->str, inv2_doc_no) == 0
	               && g_strcmp0(invoices_foreach_func_doc_nos[1]->str, inv1_doc_no) == 0));
}

void
test_Stk_Database_Invoice_clear()
{
	{
		/* GIVEN */
		g_autoptr(Stk_Model_Invoice) inv1 = stk_model_invoice_new();
		stk_model_invoice_set_doc_no(inv1, "I1");
		Stk_Database_Invoice_save(inv1);
	}

	/* WHEN */
	Stk_Database_Invoice_clear(NULL);

	/* THEN */
	g_assert_null(Stk_Database_Invoice_get("I1"));
}

int
main(int argc, char **argv)
{
	g_test_init(&argc, &argv, NULL);
	g_test_add("/Database/Invoice/get_and_save",
	           gpointer, NULL,
	           database_setup, test_Stk_Database_Invoice_get_and_save, database_teardown);
	g_test_add("/Database/Invoice/foreach",
	           gpointer, NULL,
	           database_setup, test_Stk_Database_Invoice_foreach, database_teardown);
	g_test_add("/Database/Invoice/clear",
	           gpointer, NULL,
	           database_setup, test_Stk_Database_Invoice_clear, database_teardown);
	return g_test_run();
}
