#ifndef STUDENT_BATCH_H
#define STUDENT_BATCH_H

#include <iostream>
#include <string>

class Student {
public:
  int rollnum; // Unique
  int rank;
  std::string name;
  int preference1;
  int preference2;
  int preference3;

  Student()
      : rollnum(-1), rank(-1), name(""), preference1(-1), preference2(-1),
        preference3(-1){};

  // Student(int roll_num, std::string name, int rank, int preference1,
  //         int preference2, int preference3)
  //     : rollnum(roll_num), rank(rank), name(name), preference1(preference1),
  //       preference2(preference2), preference3(preference3) {}


  void printSelf() {
    std::cout << "Roll no. : " << rollnum;
    std::cout << "Name     : " << name;
    std::cout << "rank     : " << rank;
  }
};

class Batch {
public:
  int size;
  int batchcode;
  std::string name;

  Batch() : name(""), size(-1), batchcode(-1){};

  // Batch(int batchcode, std::string name, int size)
  // : batchcode(batchcode), name(name), size(size){
  //
  void printSelf() {
    std::cout << "Batch Code     : " << batchcode;
    std::cout << "Name           : " << name;
    std::cout << "Space left     : " << size;
  }
};

class StudentBatchMap {
public:
  Student student;
  Batch batch;

  StudentBatchMap(Student student, Batch batch) :
    student(student), batch(batch) {}
};

#endif // !STUDENT_BATCH_H
