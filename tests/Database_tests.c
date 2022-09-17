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
#include "model/invoice.h"

void
database_setup()
{
	Database_init();
}

void
database_teardown()
{
	Database_Invoice_clear(NULL);
}

void
test_Database_Invoice_get_and_save()
{
	/* GIVEN */
	g_autoptr(Stk_Model_Invoice) inv = stk_model_invoice_new();
	stk_model_invoice_set_doc_no(inv, "I1");

	/* WHEN */
	Database_Invoice_save(inv);

	/* THEN */
	g_autoptr(Stk_Model_Invoice) actual = Database_Invoice_get("I1");
	g_assert_cmpstr("I1", ==, stk_model_invoice_get_doc_no(actual)->str);
}

guint invoices_foreach_thunk_counter  = 0;
GString *invoices_foreach_thunk_doc_nos[2] = {NULL, NULL};

void
invoices_foreach_thunk(Stk_Model_Invoice *inv)
{
	invoices_foreach_thunk_doc_nos[invoices_foreach_thunk_counter] = stk_model_invoice_get_doc_no(inv);
	invoices_foreach_thunk_counter += 1;
}

void
test_Database_Invoice_foreach()
{
	/* GIVEN */
	g_autoptr(Stk_Model_Invoice) inv1 = stk_model_invoice_new();
	stk_model_invoice_set_doc_no(inv1, "I1");
	Database_Invoice_save(inv1);
	g_autoptr(Stk_Model_Invoice) inv2 = stk_model_invoice_new();
	stk_model_invoice_set_doc_no(inv2, "I2");
	Database_Invoice_save(inv2);

	/* WHEN */
	Database_Invoice_foreach(invoices_foreach_thunk);

	/* THEN */
	g_assert_cmpint(2, ==, invoices_foreach_thunk_counter);
	// foreach doesn't guarantee order
	g_assert_true((invoices_foreach_thunk_doc_nos[0] == stk_model_invoice_get_doc_no(inv1)
	               && invoices_foreach_thunk_doc_nos[1] == stk_model_invoice_get_doc_no(inv2))
	              ||
	              (invoices_foreach_thunk_doc_nos[0] == stk_model_invoice_get_doc_no(inv2)
	               && invoices_foreach_thunk_doc_nos[1] == stk_model_invoice_get_doc_no(inv1)));
}

void
test_Database_Invoice_clear()
{
	/* GIVEN */
	g_autoptr(Stk_Model_Invoice) inv1 = stk_model_invoice_new();
	stk_model_invoice_set_doc_no(inv1, "I1");
	Database_Invoice_save(inv1);

	/* WHEN */
	Database_Invoice_clear(NULL);

	/* THEN */
	g_assert_null(Database_Invoice_get("I1"));
}

int
main(int argc, char **argv)
{
	g_test_init(&argc, &argv, NULL);
	g_test_add("/Database/Invoice/get_and_save",
	           gpointer, NULL,
	           database_setup, test_Database_Invoice_get_and_save, database_teardown);
	g_test_add("/Database/Invoice/foreach",
	           gpointer, NULL,
	           database_setup, test_Database_Invoice_foreach, database_teardown);
	g_test_add("/Database/Invoice/clear",
	           gpointer, NULL,
	           database_setup, test_Database_Invoice_clear, database_teardown);
	return g_test_run();
}
