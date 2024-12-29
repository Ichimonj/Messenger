#include "Rand.hpp"
#include"openssl/rand.h"
#include"sstream"
using namespace std;

string SSLrand(size_t num_bytes)
{
	unsigned char* buffer = new unsigned char[num_bytes];
	
	stringstream ss;
	RAND_bytes(buffer, num_bytes);
	for (int i = 0; i < num_bytes; i++) {
		ss << hex << static_cast<int>(buffer[i]);
	}
	delete[] buffer;
	string ret = ss.str();
	ret.resize(num_bytes);
	return ret;
}