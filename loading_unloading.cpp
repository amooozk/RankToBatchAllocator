#include "loading_unloading.hpp"
#include "student_batch.hpp"
#include <algorithm>
#include <fstream>
#include <ostream>

bool load_students_from_file(const std::string file_name, Database &DB) {
  std::ifstream student_list_file(file_name);

  if (!student_list_file) {
    std::cerr << "error occured while opening student file\n";
    return false;
  }

  std::string line;

  Student student;
  while (student_list_file >> student.rollnum >> student.name >> student.rank) {
    if (student.rollnum != -1 && student.rank != -1 && student.name != "") {
      DB.insertStudent(student);
    } else {
      std::cerr << "error occured while reading student from file\n";
      return false;
    }
    student.name = "";
    student.rollnum = 0;
    student.rank = -1;
  }

  return true;
}

bool load_batches_from_file(const std::string file_name, Database &DB) {
  std::ifstream batch_list_file(file_name);

  if (!batch_list_file) {
    std::cerr << "error occured while opening batch file\n";
    return false;
  }

  std::string line;

  Batch batch;
  while (batch_list_file >> batch.batchcode >> batch.name >> batch.size) {
    if (batch.batchcode != -1 && batch.name != "" && batch.size != -1) {
      DB.insertBatch(batch);
    } else {
      std::cerr << "error encountered while reading batches from file \n";
      return false;
    }
    batch.name = "";
    batch.batchcode = -1;
    batch.size = -1;
  }

  return true;
}

Batch *allocate_check(const int &pref,
                      std::unordered_map<int, Batch> &batch_list) {
  std::unordered_map<int, Batch>::iterator it = batch_list.find(pref);
  if (it != batch_list.end() && it->second.size != 0) {
    return &(it->second);
  }
  return nullptr;
}

std::vector<StudentBatchMap>
allocation(std::vector<Student> &student_list,
           std::unordered_map<int, Batch> &batch_list) {
  std::vector<StudentBatchMap> student_batch_map;

  for (Student student : student_list) {
    Batch *batch_found = allocate_check(student.preference1, batch_list);
    if (batch_found != nullptr) {
      batch_found->size--;
      student_batch_map.emplace_back(StudentBatchMap(student, *batch_found));
      continue;
    }

    batch_found = allocate_check(student.preference2, batch_list);
    if (batch_found != nullptr) {
      batch_found->size--;
      student_batch_map.emplace_back(StudentBatchMap(student, *batch_found));
      continue;
    }

    batch_found = allocate_check(student.preference3, batch_list);
    if (batch_found != nullptr) {
      batch_found->size--;
      student_batch_map.emplace_back(StudentBatchMap(student, *batch_found));
      continue;
    }

    std::unordered_map<int, Batch>::iterator it = std::find_if(
        batch_list.begin(), batch_list.end(),
        [](const std::pair<int, Batch> &pair) { return pair.second.size > 0; });
    if (it == batch_list.end()) {
      std::cerr << "error encountered while finding batch for student : " << student.rollnum << "\n";
      continue;
    }
    it->second.size--;
    student_batch_map.emplace_back(StudentBatchMap(student, it->second));
  }

  return student_batch_map;
}

bool write_to_file(const std::string file_name, std::vector<StudentBatchMap> &list) {
  std::ofstream final_file(file_name);

  if (!final_file) {
    std::cerr << "error opening final output file\n";
    return false;
  }

  std::unordered_map<Batch*, std::vector<Student*>> list_students_of_batch;

  std::cout << "List student -> batch\n";
  for (StudentBatchMap& student_batch_pair : list) {
    std::cout << "-----\n";
    student_batch_pair.student.printSelf();
    student_batch_pair.batch.printSelf();
    list_students_of_batch[&student_batch_pair.batch].push_back(&student_batch_pair.student);
    std::cout << "-----\n\n";
  }

  std::cout << "\n\n-------------\n\n";
  
  std::cout << "List batch -> students\n";
  for (auto &entry: list_students_of_batch) {
    std::cout << "-----\n";
    entry.first->printSelf();
    std::cout << "\nStudents:\n";
    
    for (Student* student: entry.second) {
      std::cout << "\n";
      student->printSelf();
      std::cout << "\n";
    }

    std::cout << "-----\n\n";
  }

  return true;
}
