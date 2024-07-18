#include "database.hpp"
#include <iostream>
#include <unordered_map>

Database::Database(const std::string db_name) {
  int check = sqlite3_open(db_name.c_str(), &db);

  if (check) {
    std::cerr << "Cannot open " << db_name << " Database:\t" << sqlite3_errmsg(db) << "\n";
    exit(1);
  }
}

Database::~Database() { sqlite3_close(db); }

void Database::createStudentTable() {
  std::string sql = "CREATE TABLE IF NOT EXISTS students (rollnum INTEGER "
                    "PRIMARY KEY, name TEXT, rank INTEGER, preference1 INTEGER, "
                    "preference2 INTEGER, preference3 INTEGER)";
  char *error_message;

  int check = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &error_message);
  if (check != SQLITE_OK) {
    std::cerr << "SQL Error while creating table:\t" << error_message << "\n";
    sqlite3_free(error_message);
  }
}

void Database::insertStudent(Student student) {
  std::string sql =
      "INSERT OR IGNORE INTO students (rollnum, name, rank, preference1, "
      "preference2, preference3) VALUES (?, ?, ?, ?, ?, ?)";
  sqlite3_stmt *statement;
  char *error_message;

  int check = sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr);

  if (check != SQLITE_OK) {
    std::cerr << "SQL Error while inserting student with rollnum " << student.rollnum << ":\t" << error_message << "\n";
    return;
  }

  sqlite3_bind_int(statement, 1, student.rollnum);
  sqlite3_bind_text(statement, 2, student.name.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(statement, 3, student.rank);
  sqlite3_bind_int(statement, 4, student.preference1);
  sqlite3_bind_int(statement, 5, student.preference2);
  sqlite3_bind_int(statement, 6, student.preference3);

  check = sqlite3_step(statement);

  if (check != SQLITE_DONE) {
    std::cerr << "SQL Error: \t" << sqlite3_errmsg(db) << "\n";
  }

  sqlite3_finalize(statement); // deletes the statement
}

std::vector<Student> Database::getStudents() {
  std::vector<Student> students;
  char *error_message;

  std::string sql = "SELECT * FROM students ORDER BY rank ASC";
  sqlite3_stmt *statement;

  int check = sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr);
  if (check != SQLITE_OK) {
    std::cerr << "SQL error selecting students:\t" << error_message << "\n";
    return students;
  }

  while ((check = sqlite3_step(statement)) == SQLITE_ROW) {
    Student student;
    student.rollnum = sqlite3_column_int(statement, 0);
    student.name = (const char *)sqlite3_column_text(statement, 1);
    student.rank = sqlite3_column_int(statement, 2);
    student.preference1 = sqlite3_column_int(statement, 3);
    student.preference2 = sqlite3_column_int(statement, 4);
    student.preference3 = sqlite3_column_int(statement, 5);
    students.push_back(student);
  }

  sqlite3_finalize(statement);
  return students;
}

void Database::createBatchTable() {
  std::string sql = "CREATE TABLE IF NOT EXISTS batches (batchcode INTEGER "
                    "PRIMARY KEY, name TEXT, size INTEGER)";
  char *error_message;

  int check = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &error_message);
  if (check != SQLITE_OK) {
    std::cerr << "SQL Error while creating batch table:\t" << error_message << "\n";
    sqlite3_free(error_message);
  }
}

void Database::insertBatch(Batch batch) {
  std::string sql =
      "INSERT OR IGNORE INTO batches (batchcode, name, size) VALUES (?, ?, ?)";
  sqlite3_stmt *statement;
  char *error_message;

  int check = sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr);

  if (check != SQLITE_OK) {
    std::cerr << "SQL Error while inserting batch with batchcode " << batch.batchcode << ":\t" << sqlite3_errmsg(db) << "\n";
    return;
  }

  sqlite3_bind_int(statement, 1, batch.batchcode);
  sqlite3_bind_text(statement, 2, batch.name.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(statement, 3, batch.size);

  check = sqlite3_step(statement);

  if (check != SQLITE_DONE) {
    std::cerr << "SQL Error while writing in db: \t" << sqlite3_errmsg(db) << "\n";
  }

  sqlite3_finalize(statement); // deletes the statement
}

std::unordered_map<int, Batch> Database::getBatches() {
  std::unordered_map<int, Batch> batches;

  std::string sql = "SELECT * FROM batches";
  sqlite3_stmt *statement;
  char *error_message;

  int check = sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr);
  if (check != SQLITE_OK) {
    std::cerr << "SQL error while selecting batches:\t" << error_message << "\n";
    return batches;
  }

  while ((check = sqlite3_step(statement)) == SQLITE_ROW) {
    Batch batch;
    batch.batchcode = sqlite3_column_int(statement, 0);
    batch.name = (const char *)sqlite3_column_text(statement, 1);
    batch.size = sqlite3_column_int(statement, 2);
    batches.insert({batch.batchcode, batch});
  }

  sqlite3_finalize(statement);
  return batches;
}
