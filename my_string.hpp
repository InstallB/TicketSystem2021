#ifndef MY_STRING_HPP
#define MY_STRING_HPP

#include <iostream>
#include <cstring>
#include "exceptions.hpp"
using namespace std;

namespace sjtu{

class string{
	friend ostream & operator << (ostream & os,const string &obj);
private:
	int max_size;
	void double_space(){
		max_size <<= 1;
		char* t = new char[max_size];
		for(int i = 0;i < size_;i ++) t[i] = s[i];
		delete[] s;
	}
public:
	char* s;
	int size_;
	string(){ s = nullptr; size_ = 0; max_size = 8; }
	string(const string &other){
		size_ = other.size_; max_size = other.max_size;
		s = new char[size_];
		for(int i = 0;i < size_;i ++) s[i] = other.s[i];
	}
	string(char* other){
		size_ = strlen(other); max_size = size_;
		s = new char[size_];
		for(int i = 0;i < size_;i ++) s[i] = other[i];
	}
	~string(){ delete[] s; }
	int size() const { return size_; }
	void clear(){ if(s != nullptr) delete[] s; s = nullptr; size_ = 0; max_size = 8; }
	string operator = (const string &other){
		if(this == &other) return *this;
		size_ = other.size_; max_size = other.max_size;
		char* t = new char[size_];
		for(int i = 0;i < size_;i ++) t[i] = other.s[i];
		delete[] s; s = t;
		return *this;
	}
	string operator += (const char &ch){
		if(size_ == max_size) double_space();
		s[size_ ++] = ch;
		return *this;
	}
	string operator + (char* obj){
		int len = strlen(obj);
		for(int i = 0;i < len;i ++) (*this) += obj[i];
		return *this;
	}
	string operator + (const string &obj){
		for(int i = 0;i < obj.size_;i ++) (*this) += obj.s[i];
		return *this;
	}
	bool operator < (const string &other){
		for(int i = 0;i < size_;i ++){
			if(i >= other.size_) return false;
			if(s[i] < other.s[i]) return true;
			if(s[i] > other.s[i]) return false;
		}
		if(size_ < other.size_) return true;
		return false;
	}
	bool operator == (const string &other){
		if(size_ != other.size_) return false;
		for(int i = 0;i < size_;i ++) if(s[i] != other.s[i]) return false;
		return true;
	}
	bool operator != (const string &other){
		return !(*this == other); 
	}
	char & operator [] (int x) const {
		if(x < 0 || x >= size_) throw index_out_of_bound(); // out of bound
		return s[x];
	}
};

ostream & operator << (ostream &os,const string &obj){
	for(int i = 0;i < obj.size();i ++) os << obj[i];
	return os;
}

}

#endif
