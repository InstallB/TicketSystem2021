#ifndef ACCOUNTS_HPP
#define ACCOUNTS_HPP

#include "AlternativeMultiBPlusTree.h"
#include "my_string.hpp"
#include "my_pair.hpp"
#include "get_token.hpp"
#include "exceptions.hpp"
#include "vector.hpp"
#include "priority_queue.hpp"
#include "map.hpp"
#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

namespace sjtu{

class account{
private:
	const int B = 131,P[2] = {1234567891,1000000007};
	int password_hash[2]; // hash values of password
	int get_hash_value(int x,const string &p){
		int ret = 0;
		for(int i = 0;i < p.size();i ++) ret = 1ll * (ret * B + p[i]) % P[x];
		return ret;
	}
public:
	int id;
	char username[21];
	char name[12];
	char mailAddr[31];
	int privilege;
	bool is_login;
	account(){}
	account(const string &u){
		for(int i = 0;i < u.size();i ++) username[i] = u[i];
	}
	~account(){}
	bool operator < (const account &other)const{ return id < other.id; }
	bool operator == (const account &other)const{ return id == other.id; }
	bool operator != (const account &other)const{ return id != other.id; }
	bool check_password(const string &p){
		return get_hash_value(0,p) == password_hash[0] && get_hash_value(1,p) == password_hash[1];
	}
	void modify_password(const string &p){
		password_hash[0] = get_hash_value(0,p);
		password_hash[1] = get_hash_value(1,p);
	}
	void modify_name(const string &n){
		for(int i = 0;i < n.size();i ++) name[i] = n[i];
	}
	void modify_mail(const string &n){
		for(int i = 0;i < n.size();i ++) mailAddr[i] = n[i];
	}
};

class account_management{
	friend class order_management;
private: 
	fstream account_data;
	RainyMemory::AlternativeMultiBPlusTree <string,int>* account_id_map;
	int get_id(const string &username){
		vector <int> vec;
		account_id_map->find(username,vec);
		return vec.size() ? vec[0] : -1;
	}
	void locate_read(account &acc,int x){
		account_data.seekg(x * sizeof(account));
		account_data.read(reinterpret_cast <char*> (&acc),sizeof(account));
	}
	void locate_write(account &acc,int x){
		account_data.seekg(x * sizeof(account));
		account_data.write(reinterpret_cast <char*> (&acc),sizeof(account));
	}
	// -1 : no user with username "username"
public:
	int user_number; // starts from 0
	void clean(){
		user_number = 0;
		account_data.close();
		account_data.open("account_data",fstream::binary | fstream::out);
		account_data.close();
		account_data.open("account_data",fstream::binary | fstream::in | fstream::out);
		account_id_map->clear();
	}
	account_management(){
		account_id_map = new RainyMemory::AlternativeMultiBPlusTree <string,int> ("account_id_map");
		account_data.open("account_data",fstream::binary | fstream::in | fstream::out);
	}
	~account_management(){
		delete account_id_map;
		account_data.close();
	}
	void logout_all(){
		account now;
		for(int i = 0;i < user_number;i ++){
			locate_read(now,i);
			now.is_login = false;
			locate_write(now,i);
		}
	}
	string add_user(const string &sc,const string &su,const string &p,const string &n,const string &m,int g){
		if(user_number == 0) g = 10;
		else{
			account c; int c_id = get_id(sc);
			locate_read(c,c_id);
			if(!c.is_login || g > c.privilege) return (string)("-1");
		}
		vector <int> vec; account_id_map->find(sc,vec); if(vec.size()) return (string)("-1");
		
		account u(su);
		account_id_map->insert(su,user_number,user_number);
		u.id = user_number;
		u.is_login = false;
		u.modify_password(p);
		u.modify_name(n);
		u.modify_mail(m);
		u.privilege = g;
		locate_write(u,user_number); user_number ++;
		string ret(u.username);
		return ret + ' ' + (string)(u.name) + ' ' + (string)(u.mailAddr) + ' ' + int_to_string(u.privilege);
	}
	int login(const string &u,const string &p){
		account now; int now_id = get_id(u);
		locate_read(now,now_id);
		if(now.is_login || !now.check_password(p)) return -1;
		now.is_login = true; return 0;
	}
	int logout(const string &u){
		account now; int now_id = get_id(u);
		locate_read(now,now_id);
		if(now.is_login) return -1;
		now.is_login = true; return 0;
	}
	string query_profile(const string &sc,const string &su){
		account c,u; int c_id,u_id;
		c_id = get_id(sc); u_id = get_id(su);
		locate_read(c,c_id); locate_read(u,u_id);
		if(!c.is_login || c.privilege < u.privilege || (c.privilege == u.privilege && sc != su)) return (string)("-1");
		string ret(u.username);
		return ret + ' ' + (string)(u.name) + ' ' + (string)(u.mailAddr) + ' ' + int_to_string(u.privilege);
	}
	string modify_profile(const string &sc,const string &su,const string &p,const string &n,const string &m,int g){
		account c,u; int c_id,u_id;
		c_id = get_id(sc); u_id = get_id(su);
		locate_read(c,c_id); locate_read(u,u_id);
		if(!c.is_login || c.privilege < u.privilege || (c.privilege == u.privilege && sc != su) || g > c.privilege) return (string)("-1");
		if(p.size()) u.modify_password(p);
		if(n.size()) u.modify_name(n);
		if(m.size()) u.modify_mail(m);
		if(g != -1) u.privilege = g;
		locate_write(u,u_id);
		string ret(u.username);
		return ret + ' ' + (string)(u.name) + ' ' + (string)(u.mailAddr) + ' ' + int_to_string(u.privilege);
	}
};

}

#endif
