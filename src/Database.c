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
#include "Database.h"
#include "Model/Invoice.h"

#define STRCPY(S, V)                                                           \
  gchar *__tmp_gchar_ptr = S;                                                  \
  guint __tmp_gchar_ptr_size = sizeof(__tmp_gchar_ptr);                        \
  V = (gchar *)g_malloc(__tmp_gchar_ptr_size);                                 \
  g_strlcpy(V, __tmp_gchar_ptr, __tmp_gchar_ptr_size)

struct _Stk_Database {
	GObject parent;

	GHashTable *invoices;
};

G_DEFINE_TYPE(Stk_Database, Stk_Database, G_TYPE_OBJECT)

static GObject *instance = NULL;

static GObject *
Stk_Database_constructor(GType type,
                         guint n_construct_properties,
                         GObjectConstructParam *construct_properties)
{
	if (!instance)
		instance = G_OBJECT_CLASS(Stk_Database_parent_class)->constructor(type,
		           n_construct_properties,
		           construct_properties);
	return g_object_ref(G_OBJECT(instance));
}

static void
Stk_Database_init(Stk_Database *self)
{
	self->invoices = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_object_unref);
}

static void
Stk_Database_finalise(GObject *object)
{
	Stk_Database *self = STK_DATABASE(object);
	if (self->invoices)
		g_hash_table_unref(self->invoices);
}

static void
Stk_Database_class_init(Stk_DatabaseClass *klass)
{
	G_OBJECT_CLASS(klass)->constructor = Stk_Database_constructor;
	G_OBJECT_CLASS(klass)->finalize = Stk_Database_finalise;
}

Stk_Database *
Stk_Database_get()
{
	return g_object_new(STK_TYPE_DATABASE, NULL);
}

gboolean
Stk_Database_Invoice_save(Stk_Model_Invoice *inv)
{
	if (!inv)
		return FALSE;
	g_autoptr(Stk_Database) db = Stk_Database_get();
	g_object_ref(inv);
	gchar *doc_no = NULL;
	STRCPY(Stk_Model_Invoice_get_doc_no(inv)->str, doc_no);
	if (g_hash_table_contains(db->invoices, doc_no)) {
		g_object_unref(STK_MODEL_INVOICE(g_hash_table_lookup(db->invoices, doc_no)));
		return g_hash_table_replace(db->invoices, doc_no, inv);
	} else {
		return g_hash_table_insert(db->invoices, doc_no, inv);
	}
}

Stk_Model_Invoice *
Stk_Database_Invoice_get(gchar *doc_no)
{
	g_autoptr(Stk_Database) db = Stk_Database_get();
	return g_hash_table_lookup(db->invoices, doc_no);
}

void
invoices_foreach(gpointer doc_no, gpointer invp, gpointer funcp)
{
	Stk_Model_Invoice *inv = invp;
	void (*func)(Stk_Model_Invoice *) = funcp;
	func(inv);
}

void
Stk_Database_Invoice_foreach(void (*func)(Stk_Model_Invoice *))
{
	g_autoptr(Stk_Database) db = Stk_Database_get();
	g_hash_table_foreach(db->invoices, invoices_foreach, func);
}

void
Stk_Database_Invoice_clear(void (*invoice_destroy_func)(Stk_Model_Invoice *))
{
	g_autoptr(Stk_Database) db = Stk_Database_get();
	g_hash_table_remove_all(db->invoices);
}
