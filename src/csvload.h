#pragma once
#include <glib.h>
#include "models.h"

/**
 * Load a line from the invoices CSV file.
 * The line is converted to an invoice line and saved in the database along
 * with the corresponding invoice.
 *
 * The CSV file is assumed to have the following columns in order:
 * - docNo
 * - customer
 * - date
 * - total
 * - discount
 * - lineNo
 * - product
 * - qty
 * - price
 * - lineAmt
 *
 * @param line The line from CSV file
 */
void
csv_line_process(gchar *line);
