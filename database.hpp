#ifndef DATABASE_H
#define DATABASE_H

#include "student_batch.hpp"
#include <sqlite3.h>
#include <string>
#include <unordered_map>
#include <vector>

class Database {
private:
  sqlite3 *db;

public:
  Database(const std::string db_name); // Constructor
  ~Database();                         // Destructer

  void createStudentTable();
  void insertStudent(Student student);
  std::vector<Student> getStudents();

  void createBatchTable();
  void insertBatch(Batch batch);
  std::unordered_map<int, Batch> getBatches();
};

#endif // !DATABASE_H
