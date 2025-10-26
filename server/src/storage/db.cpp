#include "db.h"

DB::DB() 
    : conn(
        // Logic chọn path file DB
        (env == "DEVELOPMENT") 
            ? "database/POP3V2.dev.db" 
            : "database/POP3V2.db"
      ),
      // KHỞI TẠO USER VÀ MAIL BẰNG OBJECT 'conn' đã được tạo
      user(conn), 
      mail(conn)
{
    // Code trong thân constructor:
    std::filesystem::create_directories("database");
    console.log("Database file: ", env);
    // Code C++ ở đây thường chỉ dành cho logic KHÔNG PHẢI KHỞI TẠO (như gọi hàm init, log)env
    
    // XÓA CÁC DÒNG GÁN CŨ KHỎI THÂN CONSTRUCTOR:
    // this->user = UserTable(conn);
    // this->mail = MailTable(conn);
}

void DB::dumpTables(){
  const char* q = "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;";
  sqlite3_stmt* st=nullptr;
  if (sqlite3_prepare_v2(this->conn.get(), q, -1, &st, nullptr)==SQLITE_OK){
    while(sqlite3_step(st)==SQLITE_ROW){
      console.debug("[TABLE] ", (const char*)sqlite3_column_text(st,0));
    }
  }
  sqlite3_finalize(st);
}
