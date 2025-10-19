// server/storage/db_test.cpp (tạm)
#include "../../Sqlite/sqlite3.h"
#include <iostream>
int main(){
  sqlite3* db=nullptr;
  if (sqlite3_open("test.db", &db)!=SQLITE_OK) { std::cerr<<"open fail\n"; return 1; }
  char* err=nullptr;
  sqlite3_exec(db, "PRAGMA user_version;", nullptr, nullptr, &err);
  if (err) { std::cerr<<err<<"\n"; sqlite3_free(err); }
  sqlite3_close(db);
  std::cout<<"OK\n";
  return 0;
}
