/* MDB Tools - A library for reading MS Access database file
 * Copyright (C) 2000 Brian Bruns
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _mdbsql_h_
#define _mdbsql_h_

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <mdbtools.h>

typedef struct {
	MdbHandle *mdb;
	int all_columns;
	unsigned int num_columns;
	GPtrArray *columns;
	unsigned int num_tables;
	GPtrArray *tables;
	MdbTableDef *cur_table;
	MdbSargNode *sarg_tree;
	GList *sarg_stack;
	/* FIX ME */
	void *bound_values[256];
	unsigned char *kludge_ttable_pg;
	long max_rows;
	char error_msg[1024];
	int limit;
	long row_count;
} MdbSQL;

typedef struct {
	char *name;
	int  disp_size;
	void *bind_addr;   /* if !NULL then cp parameter to here */
	int  bind_type;
	int  *bind_len;
	int  bind_max;
} MdbSQLColumn;

typedef struct {
	char *name;
	char *alias;
} MdbSQLTable;

typedef struct {
	char *col_name;
	MdbSarg *sarg;
} MdbSQLSarg;

extern char *g_input_ptr;

#undef YY_INPUT
#define YY_INPUT(b, r, ms) (r = mdb_sql_yyinput(b, ms));

#define mdb_sql_has_error(sql) ((sql)->error_msg[0] ? 1 : 0)
#define mdb_sql_last_error(sql) ((sql)->error_msg)

void mdb_sql_error(MdbSQL* sql, char *fmt, ...);
extern MdbSQL *_mdb_sql(MdbSQL *sql);
extern MdbSQL *mdb_sql_init();
extern MdbSQLSarg *mdb_sql_alloc_sarg();
extern MdbHandle *mdb_sql_open(MdbSQL *sql, char *db_name);
extern int mdb_sql_add_sarg(MdbSQL *sql, char *col_name, int op, char *constant);
extern void mdb_sql_all_columns(MdbSQL *sql);
extern int mdb_sql_add_column(MdbSQL *sql, char *column_name);
extern int mdb_sql_add_table(MdbSQL *sql, char *table_name);
extern void mdb_sql_dump(MdbSQL *sql);
extern void mdb_sql_exit(MdbSQL *sql);
extern void mdb_sql_reset(MdbSQL *sql);
extern void mdb_sql_listtables(MdbSQL *sql);
extern void mdb_sql_select(MdbSQL *sql);
extern void mdb_sql_dump_node(MdbSargNode *node, int level);
extern void mdb_sql_close(MdbSQL *sql);
extern void mdb_sql_add_or(MdbSQL *sql);
extern void mdb_sql_add_and(MdbSQL *sql);
extern void mdb_sql_add_not(MdbSQL *sql);
extern void mdb_sql_describe_table(MdbSQL *sql);
extern MdbSQL* mdb_sql_run_query (MdbSQL*, const gchar*);
extern void mdb_sql_set_maxrow(MdbSQL *sql, int maxrow);
extern int mdb_sql_eval_expr(MdbSQL *sql, char *const1, int op, char *const2);
extern void mdb_sql_bind_all(MdbSQL *sql);
extern int mdb_sql_fetch_row(MdbSQL *sql, MdbTableDef *table);
extern int mdb_sql_add_temp_col(MdbSQL *sql, MdbTableDef *ttable, int col_num, char *name, int col_type, int col_size, int is_fixed);
extern void mdb_sql_bind_column(MdbSQL *sql, int colnum, void *varaddr, int *len_ptr);
extern int mdb_sql_add_limit(MdbSQL *sql, char *limit);

#ifdef __cplusplus
  }
#endif

#endif
