#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#include <string>
#include <iostream>
#include <fstream>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int main(int argc, char* argv[]) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   const char *sql;

   /* Open database */
   rc = sqlite3_open("test.db", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stdout, "Opened database successfully\n");
   }

   sql = "CREATE VIRTUAL TABLE history USING fts5(date,time,pwd,cmd);";
   //sql = "CREATE TABLE history (date TEXT, time TEXT, pwd TEXT, cmd TEXT);";

   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Table created successfully\n");
   }

   std::ifstream file;
   std::string s;
   std::string insert = "INSERT INTO history (date,time,pwd,cmd) VALUES ";

   file.open("/home/dmitry/.bash-history/2019-09-03");
   bool first = true;
 
   while (std::getline(file, s)) {
	if (first) {
        	insert += "(";
		first = false;
	} 
	else {
        	insert += ", (";
	}

	size_t i = 0;
	size_t i_prev = 0;
	size_t count = 0;

	std::cout << s << std::endl;

	while (i != std::string::npos && count < 3) {
		i_prev = i;
		i = s.find(' ', i + 1);
		insert += "\"" + s.substr(i_prev, i) + "\", ";
		count++;

		std::cout << s.substr(i_prev, i) << std::endl;
	}

	insert += "\"" + s.substr(i_prev) + "\"";
 
        insert += ")";
   }

   insert += ";";

   std::cout << insert << std::endl;

   rc = sqlite3_exec(db, insert.c_str(), callback, 0, &zErrMsg);

   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Data inserted successfully\n");
   }

   sqlite3_close(db);
   return 0;
}
