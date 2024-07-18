#include "database.hpp"
#include "loading_unloading.hpp"
#include "student_batch.hpp"
#include <unordered_map>

int main() {
  Database DB("database.db");
  DB.createStudentTable();
  DB.createBatchTable();

  if (!load_students_from_file("students_sheet.txt", DB)) {
    return 1;
  }
  if (!load_batches_from_file("batches_sheet.txt", DB)) {
    return 1;
  }

  std::vector<Student> students = DB.getStudents();
  std::unordered_map<int, Batch> batches = DB.getBatches();

  std::vector<StudentBatchMap> result_of_allocation = allocation(students, batches);

  students.clear();
  batches.clear();
  std::vector<Student>().swap(students);
  std::unordered_map<int, Batch>().swap(batches);

  if (!write_to_file("result.txt", result_of_allocation)) {
    return 1;
  }

  std::cout << "execution complete\n";

  return 0;
}
