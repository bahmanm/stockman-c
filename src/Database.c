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

typedef struct Database {
	GHashTable * invoices;
} Database;

const Database*
Database_get()
{
	if (!db) {
		g_error("database is NOT initialised.\n");
		g_abort();
	}
	return db;
}

void
Database_init()
{
	if (db) {
		g_debug("Database already initialised.");
		return;
	}
	g_debug("Initialising the database...");
	db = g_malloc(sizeof(Database));
	db->invoices = g_hash_table_new(g_str_hash, g_str_equal);
}

gboolean
Database_Invoice_save(Models_Invoice *inv)
{
	return g_hash_table_insert(Database_get()->invoices, inv->doc_no, inv);
}

Models_Invoice*
Database_Invoice_get(gchar *doc_no)
{
	return g_hash_table_lookup(Database_get()->invoices, doc_no);
}

void
invoices_foreach(gpointer doc_no, gpointer invp, gpointer thunkp)
{
	Models_Invoice *inv = (Models_Invoice *)invp;
	void (*thunk)(Models_Invoice *) = thunkp;
	thunk(inv);
}

void
Database_Invoice_foreach(void (*thunk)(Models_Invoice *))
{
	g_hash_table_foreach(db->invoices, invoices_foreach, thunk);
}

void
Database_Invoice_clear(void (*invoice_destroy_thunk)(Models_Invoice *))
{
	g_autoptr(GList) keys = g_hash_table_get_keys(db->invoices);
	for (GList *key = keys; key; key = key->next) {
		Models_Invoice *inv = g_hash_table_lookup(db->invoices, key);
		if (invoice_destroy_thunk)
			invoice_destroy_thunk(inv);
		g_hash_table_remove(db->invoices, key->data);
	}
}