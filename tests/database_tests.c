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
	Models_Invoice inv = {.doc_no = "I1", .lines = NULL};

	/* WHEN */
	Database_Invoice_save(&inv);

	/* THEN */
	Models_Invoice *actual = Database_Invoice_get("I1");
	g_assert_cmpstr("I1", ==, actual->doc_no);
}

guint invoices_foreach_thunk_counter  = 0;
gchar *invoices_foreach_thunk_doc_nos[2] = {NULL, NULL};

void
invoices_foreach_thunk(Models_Invoice * inv)
{
	invoices_foreach_thunk_doc_nos[invoices_foreach_thunk_counter] = inv->doc_no;
	invoices_foreach_thunk_counter += 1;
}

void
test_Database_Invoice_foreach()
{
	/* GIVEN */
	Models_Invoice inv1 = {.doc_no = "I1", .lines = NULL};
	Database_Invoice_save(&inv1);
	Models_Invoice inv2 = {.doc_no = "I2", .lines = NULL};
	Database_Invoice_save(&inv2);

	/* WHEN */
	Database_Invoice_foreach(invoices_foreach_thunk);

	/* THEN */
	g_assert_cmpint(2, ==, invoices_foreach_thunk_counter);
	// foreach doesn't guarantee order
	g_assert_true((invoices_foreach_thunk_doc_nos[0] == inv1.doc_no && invoices_foreach_thunk_doc_nos[1] == inv2.doc_no)
	              ||
	              (invoices_foreach_thunk_doc_nos[0] == inv2.doc_no && invoices_foreach_thunk_doc_nos[1] == inv1.doc_no));
}

void
test_Database_Invoice_clear()
{
	/* GIVEN */
	Models_Invoice inv1 = {.doc_no = "I1", .lines = NULL};
	Database_Invoice_save(&inv1);

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
