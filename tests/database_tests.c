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
#include "database.h"
#include "models.h"

void
database_setup()
{
	database_init();
}

void
database_teardown()
{
	database_invoices_clear(NULL);
}

void
test_database_invoice_get_and_save()
{
	/* GIVEN */
	Invoice inv = {.doc_no = "I1", .lines = NULL};

	/* WHEN */
	database_invoice_save(&inv);

	/* THEN */
	Invoice *actual = database_invoice_get("I1");
	g_assert_cmpstr("I1", ==, actual->doc_no);
}

guint invoices_foreach_thunk_counter  = 0;
gchar *invoices_foreach_thunk_doc_nos[2] = {NULL, NULL};

void
invoices_foreach_thunk(Invoice * inv)
{
	invoices_foreach_thunk_doc_nos[invoices_foreach_thunk_counter] = inv->doc_no;
	invoices_foreach_thunk_counter += 1;
}

void
test_database_invoices_foreach()
{
	/* GIVEN */
	Invoice inv1 = {.doc_no = "I1", .lines = NULL};
	database_invoice_save(&inv1);
	Invoice inv2 = {.doc_no = "I2", .lines = NULL};
	database_invoice_save(&inv2);

	/* WHEN */
	database_invoices_foreach(invoices_foreach_thunk);

	/* THEN */
	g_assert_cmpint(2, ==, invoices_foreach_thunk_counter);
	// foreach doesn't guarantee order
	g_assert_true((invoices_foreach_thunk_doc_nos[0] == inv1.doc_no && invoices_foreach_thunk_doc_nos[1] == inv2.doc_no)
	              ||
	              (invoices_foreach_thunk_doc_nos[0] == inv2.doc_no && invoices_foreach_thunk_doc_nos[1] == inv1.doc_no));
}

void
test_database_invoices_clear()
{
	/* GIVEN */
	Invoice inv1 = {.doc_no = "I1", .lines = NULL};
	database_invoice_save(&inv1);

	/* WHEN */
	database_invoices_clear(NULL);

	/* THEN */
	g_assert_null(database_invoice_get("I1"));
}

int
main(int argc, char **argv)
{
	g_test_init(&argc, &argv, NULL);
	g_test_add("/database/invoices/get_and_save",
	           gpointer, NULL,
	           database_setup, test_database_invoice_get_and_save, database_teardown);
	g_test_add("/database/invoices/foreach",
	           gpointer, NULL,
	           database_setup, test_database_invoices_foreach, database_teardown);
	g_test_add("/database/invoices/clear",
	           gpointer, NULL,
	           database_setup, test_database_invoices_clear, database_teardown);
	return g_test_run();
}
