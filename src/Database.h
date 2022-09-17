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

#include "model/invoice.h"
#include "model/model.h"

typedef struct Database Database;

static Database* db;

/**
 * Initialise the database.
 */
void
Database_init();

/**
 * Returns the initialised database instance.
 * Attempt to `get` an uninitialised database causes the program to abort.
 */
const Database*
Database_get();

/**
 * Find an invoice.
 *
 * @param doc_no Invoice number
 * @return Invoice if exists, `NULL` otherwise.
 */
Stk_Model_Invoice*
Database_Invoice_get(gchar *doc_no);

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
Database_Invoice_save(Stk_Model_Invoice *inv);

/**
 * Iterate over all invoices and execute a given thunk for each.
 *
 * @param thunk Function to execute for each invoice.
 */
void
Database_Invoice_foreach(void (*thunk)(Stk_Model_Invoice *));

/**
 * Remove all invoices from the database.
 *
 * @param invoice_destroy_thunk `NULL` or Function to run for each invoice;
 *        mainly to free up the memory.
 */
void
Database_Invoice_clear(void (*invoice_destroy_thunk)(Stk_Model_Invoice *));
