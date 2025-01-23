#pragma once
#include <iostream>
#include <fstream>
using namespace std;

/*write to file*/
void record(ofstream& file, uint64_t type);
void record(ofstream& file, string& type);
void record(ofstream& file, char* type);

/*reading from file*/
void reading(ifstream& file, uint64_t* type);
void reading(ifstream& file, string* type);
void reading(ifstream& file, char* type);