#include <glib.h>
#include "database.h"

typedef struct Database {
	GHashTable * invoices;
} Database;

const Database*
database_get()
{
	if (!db) {
		g_error("database is NOT initialised.\n");
		g_abort();
	}
	return db;
}

void
database_init()
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
database_invoice_save(Invoice *inv)
{
	return g_hash_table_insert(database_get()->invoices, inv->doc_no, inv);
}

Invoice*
database_invoice_get(gchar *doc_no)
{
	return g_hash_table_lookup(database_get()->invoices, doc_no);
}

void
invoices_foreach(gpointer doc_no, gpointer invp, gpointer thunkp)
{
	Invoice *inv = (Invoice *)invp;
	void (*thunk)(Invoice *) = thunkp;
	thunk(inv);
}

void
database_invoices_foreach(void (*thunk)(Invoice *))
{
	g_hash_table_foreach(db->invoices, invoices_foreach, thunk);
}

void
database_invoices_clear(void (*invoice_destroy_thunk)(Invoice *))
{
	g_autoptr(GList) keys = g_hash_table_get_keys(db->invoices);
	for (GList *key = keys; key; key = key->next) {
		Invoice *inv = g_hash_table_lookup(db->invoices, key);
		if (invoice_destroy_thunk)
			invoice_destroy_thunk(inv);
		g_hash_table_remove(db->invoices, key->data);
	}
}
