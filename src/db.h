#pragma once

#include <glib.h>
#include "models.h"

typedef struct Database Database;

static Database* db;

/**
 * Initialise the database.
 * Attempt to use an uninitialised database causes the program to abort.
 */
void
database_init();

/**
 * Returns the initialised database instance.
 */
const Database*
database_get();

/**
 * Find an invoice.
 *
 * @param doc_no Invoice number
 * @return Invoice if exists, `NULL` otherwise.
 */
Invoice*
database_invoice_get(gchar *doc_no);

/**
 * Persist an invoice.
 *
 * Creates a new record if an invoice with this `doc_no` does not exist,
 * overwrites the existing record otherwise.
 *
 * @param inv Invoice to persist
 * @return `TRUE` if successful, `FALSE` otherwise.
 */
gboolean
database_invoice_save(Invoice *inv);

/**
 * Iterate over all invoices and execute a given thunk for each.
 *
 * @param thunk Function to execute for each invoice.
 */
void
database_invoices_foreach(void (*thunk)(Invoice*));
