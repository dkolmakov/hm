#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#include <string>
#include <iostream>

class History {
   sqlite3 *db;

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
		   sql = "CREATE TABLE IF NOT EXISTS history (date TEXT, pwd TEXT, cmd TEXT);";

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

		void insert(std::string pwd, std::string cmd) {
		   char *zErrMsg = 0;
		   int rc;

                   std::string sql = "INSERT INTO history (date,pwd,cmd) VALUES (";
		   sql += "DATETIME(),";
		   sql += "\"" + pwd + "\",";
		   sql += "\"" + cmd + "\");";
		   
		   rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);

		   if( rc != SQLITE_OK ){
		      fprintf(stderr, "SQL error: %s\n", zErrMsg);
		      sqlite3_free(zErrMsg);
		      throw -3;
		   }
		}
};

