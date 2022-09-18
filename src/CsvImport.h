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
#include "model/model.h"

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
CsvImport_processline(gchar *line);
