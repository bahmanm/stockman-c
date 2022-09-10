#include <glib.h>
#include "db.h"

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

Database*
database_init()
{
	g_debug("Initialising the database...");
	Database* db = g_malloc(sizeof(Database));
	db->invoices = g_hash_table_new(g_str_hash, g_str_equal);
	return db;
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
