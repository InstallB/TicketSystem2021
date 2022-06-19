#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "accounts.hpp"
#include "trains.hpp"
#include "orders.hpp"
#include "my_string.hpp"
#include "my_pair.hpp"
#include "get_token.hpp"
#include "system.hpp"
#include "exceptions.hpp"
#include "AlternativeMultiBPlusTree.h"
#include "vector.hpp"
#include "priority_queue.hpp"
#include "map.hpp"
using namespace std;

namespace sjtu{

void rollback(){}

void clean(account_management* account_,train_management* train_,order_management* order_){
	account_->clean();
	train_->clean();
	order_->clean();
}

void exit(account_management* account_,train_management* train_,order_management* order_){
	account now;
	for(int i = 0;i < account_.user_number;i ++){
		account_->locate_read(now,i);
		now.is_login = false;
		account_->locate_write(now,i);
	}
	fstream file;
	file.open("numbers",fstream::out);
	file.close();
	file.open("numbers",fstream::in | fstream::out);
	file << account_->user_number << ' ' << train_->train_number << ' ' << order_->order_number << endl;
	cout << "bye" << endl;
}

}

#endif
