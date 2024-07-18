#ifndef LOADING_UNLOADING
#define LOADING_UNLOADING

#include "database.hpp"
#include "student_batch.hpp"
#include <unordered_map>

bool load_students_from_file(const std::string file_name, Database &DB);
bool load_batches_from_file(const std::string file_name, Database &DB);

std::vector<StudentBatchMap>
allocation(std::vector<Student> &student_list,
           std::unordered_map<int, Batch> &batch_list);

bool write_to_file(const std::string file_name,
                   std::vector<StudentBatchMap> &list);

#endif // LOADING_UNLOADING
