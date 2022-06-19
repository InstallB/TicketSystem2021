#ifndef GET_TOKEN_HPP
#define GET_TOKEN_HPP

#include "my_string.hpp"
using namespace std;

namespace sjtu{

const int DAY = 24 * 60;

string get_token(){
	string ret;
	char ch; ch = getchar();
	while(ch <= 32) ch = getchar();
	while(ch > 32){ ret += ch; ch = getchar(); }
	return ret;
}

int string_to_int(const string &s){
	int ret = 0;
	for(int i = 0;i < s.size();i ++) ret = ret * 10 + s[i] - '0';
	return ret;
}

string int_to_string(int x){
	string s,t;
	do{ s += (char)('0' + x % 10); x /= 10; }while(x);
	for(int i = s.size() - 1;i >= 0;i --) t += s[i];
	return t;
}

int date_to_int(const string &d){ // xx-xx
	int num = (d[3] - '0') * 10 + d[4] - '0';
	if(d[1] == '7') num += 30;
	if(d[1] == '8') num += 30 + 31;
	return num * DAY;
}

int time_to_int(const string &d){ // 
	return ((d[0] - '0') * 10 + d[1] - '0') * 60 + (d[3] - '0') * 10 + (d[4] - '0');
}

string int_to_date(int x){
	string ret;
	ret += '0';
	if(x / DAY >= 61) ret += '8',x -= DAY * 61;
	else if(x / DAY >= 30) ret += '7',x -= DAY * 30;
	else ret += '6';
	ret += '-';
	ret += (char)('0' + (x / DAY) / 10);
	ret += (char)('0' + (x / DAY) % 10);
	return ret;
}

string int_to_time(int x){
	string ret;
	ret += '0';
	if(x / DAY >= 61) ret += '8',x -= DAY * 61;
	else if(x / DAY >= 30) ret += '7',x -= DAY * 30;
	else ret += '6';
	ret += '-';
	ret += (char)('0' + (x / DAY) / 10);
	ret += (char)('0' + (x / DAY) % 10);
	ret += ' ';
	x %= DAY;
	ret += (char)('0' + (x / 60) / 10);
	ret += (char)('0' + (x / 60) % 10);
	x %= 60;
	ret += (char)('0' + x / 10);
	ret += (char)('0' + x % 10);
	return ret;
}

template <typename T>
T max(T x,T y){ return x < y ? y : x; }

template <typename T>
T min(T x,T y){ return x < y ? x : y; }

}

#endif
