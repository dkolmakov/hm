#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#include <string>
#include <iostream>

class History {

   sqlite3 *db;

   sqlite3_stmt *insert_cmd_stmt;
   sqlite3_stmt *insert_sess_stmt;
   sqlite3_stmt *insert_last_cmd_stmt;
   sqlite3_stmt *select_last_cmd_stmt;

   int exec_sql(std::string sql) {
	   char *zErrMsg = 0;
	   int rc;

	   rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);

	   if( rc != SQLITE_OK ){
	      fprintf(stderr, "SQL error: %s\n", zErrMsg);
	      sqlite3_free(zErrMsg);
	      throw -3;
	   }

	   return rc;
   }


   int prepare_sql(std::string sql, sqlite3_stmt **stmt) {
	   int rc;

	   rc = sqlite3_prepare_v2(db, sql.c_str(), -1, stmt, NULL);

	   if( rc != SQLITE_OK ){
	      fprintf(stderr, "SQL preparation error\n");
	      throw -3;
	   }

	   return rc;
   } 

   int bind_value(sqlite3_stmt *stmt, const char* name, std::string value) {
	   int rc;

	   int index = sqlite3_bind_parameter_index(stmt, name);

	   if( index == 0 ){
	      fprintf(stderr, "SQL bind parameter index error: no \"%s\" parameter\n", name);
	      throw -3;
	   }

	   rc = sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);
	   
	   if( rc != SQLITE_OK ){
	      fprintf(stderr, "SQL bind error\n");
	      throw -3;
	   }
	
	   return rc;
   }

	public:

		History(std::string path) {
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
				cmd TEXT); \
			 CREATE TABLE IF NOT EXISTS sessions (\
				id INTEGER PRIMARY KEY, \
		   		date TEXT, \
				name TEXT); \
			 CREATE TABLE IF NOT EXISTS last_commands (\
				sess_id INTEGER PRIMARY KEY, \
		   		pwd TEXT, \
				cmd TEXT);";

		   exec_sql(sql);

		   sql = "INSERT INTO commands (sess_id,date,pwd,cmd) "
		         " VALUES ( :sess , DATETIME() , :pwd , :cmd );";
		   prepare_sql(sql, &insert_cmd_stmt);
			
		   sql = "INSERT OR REPLACE INTO last_commands (sess_id, pwd, cmd) "
			 "VALUES ( :sess, :pwd, :cmd );";
		   prepare_sql(sql, &insert_last_cmd_stmt);
			
		   sql = "SELECT * FROM last_commands WHERE sess_id = :sess AND pwd = :pwd;";
		   prepare_sql(sql, &select_last_cmd_stmt);
		}

		~History() {
   		   sqlite3_close(db);
		}

		std::string get_last_cmd(int sess_id, std::string pwd) {
			std::string cmd = "";

			bind_value(select_last_cmd_stmt, ":sess", std::to_string(sess_id));
			bind_value(select_last_cmd_stmt, ":pwd", pwd);

			while (sqlite3_step(select_last_cmd_stmt) == SQLITE_ROW) {
				cmd = std::string(reinterpret_cast< char const* >(sqlite3_column_text(select_last_cmd_stmt, 2)));
			};

			return cmd;
		}

		void set_last_cmd(int sess_id, std::string pwd, std::string cmd) {
			bind_value(insert_last_cmd_stmt, ":sess", std::to_string(sess_id));
			bind_value(insert_last_cmd_stmt, ":pwd", pwd);
			bind_value(insert_last_cmd_stmt, ":cmd", cmd);

			while (sqlite3_step(insert_last_cmd_stmt) != SQLITE_DONE) {};

		}

		void insert_cmd(int64_t session, std::string pwd, std::string cmd) {
			std::string last_cmd = get_last_cmd(session, pwd);

			if (cmd != last_cmd) {

			   bind_value(insert_cmd_stmt, ":sess", std::to_string(session));
			   bind_value(insert_cmd_stmt, ":pwd", pwd);
			   bind_value(insert_cmd_stmt, ":cmd", cmd);

			   while (sqlite3_step(insert_cmd_stmt) != SQLITE_DONE) {};

		   	   set_last_cmd(session, pwd, cmd);
			}
		}
		
		int64_t insert_sess(std::string name) {
                   std::string sql = "INSERT INTO sessions (date,name) VALUES (";
		   sql += "DATETIME(),";
		   sql += "\"" + name + "\");";
		   exec_sql(sql);

		   return sqlite3_last_insert_rowid(db);
		}
};

