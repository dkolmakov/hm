#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <assert.h> 

#include <string>
#include <iostream>

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}

  void replace_all(std::string& str, const std::string& from, const std::string& to) {
	size_t pos = 0;

	while ((pos = str.find(from, pos)) != std::string::npos) {
		str.replace(pos, from.length(), to);
		pos += to.length();
	}
   }

   int prepare_sql(sqlite3 *db, std::string sql, sqlite3_stmt **stmt) {
	   int rc;

	   rc = sqlite3_prepare_v2(db, sql.c_str(), -1, stmt, NULL);

	   if( rc != SQLITE_OK ){
	      fprintf(stderr, "SQL preparation error: %d \n", rc);
	      throw -3;
	   }

	   return rc;
   } 

   int bind_value(sqlite3 *db, sqlite3_stmt *stmt, const char* name, std::string value) {
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



int main(int argc, char* argv[]) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   const char *sql;
   const char* data = "Callback function called";

   assert(argc == 3);

   std::string db_path = argv[1];
   std::string dir = argv[2];

   /* Open database */
   rc = sqlite3_open(db_path.c_str(), &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

   std::cout << std::endl << "Sessions" << std::endl;
   
   sql = "CREATE VIRTUAL TABLE IF NOT EXISTS history_fts USING "
	 "fts5(content=commands,date,pwd);";

   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Table created successfully\n");
   }

   sqlite3_stmt *select_dir_stmt;

   sql = "SELECT * FROM history_fts WHERE pwd MATCH :dir;";
   prepare_sql(db, sql, &select_dir_stmt);

   replace_all(dir, "/", " ");

   std::cout << dir << std::endl;

   bind_value(db, select_dir_stmt, ":dir", dir);

   while (sqlite3_step(select_dir_stmt) == SQLITE_ROW) {
	   std::cout << sqlite3_column_text(select_dir_stmt, 3) << std::endl;
   };

   sqlite3_close(db);

   return 0;
}

