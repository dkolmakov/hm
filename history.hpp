#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#include <string>
#include <iostream>

class History {
   sqlite3 *db;

   void exec_sql(std::string sql) {
	   char *zErrMsg = 0;
	   int rc;

	   rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);

	   if( rc != SQLITE_OK ){
	      fprintf(stderr, "SQL error: %s\n", zErrMsg);
	      sqlite3_free(zErrMsg);
	      throw -3;
	   }
   }

	public:

		History(std::string path) {
		   char *zErrMsg = 0;
		   int rc;
		   const char *sql;

		   rc = sqlite3_open(path.c_str(), &db);
   
		   if ( rc ) {
		      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		      throw -1;
		   }

		   //sql = "CREATE VIRTUAL TABLE IF NOT EXISTS history USING fts5(date,pwd,cmd);";
		   sql = "CREATE TABLE IF NOT EXISTS commands (\
				sess_id BIGINT, \
		   		date TEXT, \
				pwd TEXT, \
				cmd TEXT);";

		   rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
		   
		   if( rc != SQLITE_OK){
		      fprintf(stderr, "SQL error: %s\n", zErrMsg);
		      sqlite3_free(zErrMsg);
		      throw -2;
		   }

		   sql = "CREATE TABLE IF NOT EXISTS sessions (\
				id INTEGER PRIMARY KEY, \
		   		date TEXT, \
				name TEXT);";

		   rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
		   
		   if( rc != SQLITE_OK){
		      fprintf(stderr, "SQL error: %s\n", zErrMsg);
		      sqlite3_free(zErrMsg);
		      throw -2;
		   }
		}

		~History() {
   		   sqlite3_close(db);
		}

		void insert_cmd(int64_t session, std::string pwd, std::string cmd) {
                   std::string sql = "INSERT INTO commands (sess_id,date,pwd,cmd) VALUES (";
		   sql += std::to_string(session) + ",";
		   sql += "DATETIME(),";
		   sql += "\"" + pwd + "\",";
		   sql += "\"" + cmd + "\");";
		   exec_sql(sql);
		}
		
		int64_t insert_sess(std::string name) {
                   std::string sql = "INSERT INTO sessions (date,name) VALUES (";
		   sql += "DATETIME(),";
		   sql += "\"" + name + "\");";
		   exec_sql(sql);

		   return sqlite3_last_insert_rowid(db);
		}
};

