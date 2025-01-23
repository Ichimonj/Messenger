#pragma once
#pragma warning(disable : 4996)
#include "string"
#include "iomanip"
#include "sstream"
#include "openssl/sha.h"
using namespace std;

string Hash(string& str);