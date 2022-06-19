#ifndef ORDERS_HPP
#define ORDERS_HPP

#include "accounts.hpp"
#include "trains.hpp"
#include "my_string.hpp"
#include "my_pair.hpp"
#include "get_token.hpp"
#include "exceptions.hpp"
#include "AlternativeMultiBPlusTree.h"
#include "vector.hpp"
#include "priority_queue.hpp"
#include "map.hpp"
#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

namespace sjtu{

class order{
public:
	int id;
	int user_id;
	int train_id,train_date; // date from start
	int start,end; // stations
	int number;
	int status; // 0 is success, 1 is pending, 2 is refunded
	int price;
	order(){}
	~order(){}
	bool operator < (const order &other)const{ return id < other.id; }
	bool operator == (const order &other)const{ return id == other.id; }
	bool operator != (const order &other)const{ return id != other.id; }
};

class order_management{
	fstream order_data;
	RainyMemory::AlternativeMultiBPlusTree <int,int> ticket_user_map;
	RainyMemory::AlternativeMultiBPlusTree <pair <int,int>,int> ticket_train_map; // "status = pending" orders
	
	void locate_read(order &acc,int x){
		order_data.seekg(x * sizeof(order));
		order_data.read(reinterpret_cast <char*> (&acc),sizeof(order));
	}
	void locate_write(order &acc,int x){
		order_data.seekg(x * sizeof(order));
		order_data.write(reinterpret_cast <char*> (&acc),sizeof(order));
	}
	void judge(train &t,int oid){
		order o; int flg = 1;
		locate_read(o,oid);
		for(int i = o.start;i < o.end;i ++) if(t.tickets[o.train_date][i] < o.number) flg = 0;
		if(flg){
			for(int i = o.start;i < o.end;i ++) t.tickets[o.train_date][i] -= o.number;
			ticket_train_map.erase(pair <int,int> (o.train_id,o.train_date),id);
			o.status = 0;
			locate_write(o,oid);
		}
	}
public:
	int order_number;
	account_management* account_;
	train_management* train_;
	void clean(){
		order_number = 0;
		order_data.close();
		order_data.open("order_data",fstream::binary | fstream::out);
		order_data.close();
		order_data.open("order_data",fstream::binary);
		ticket_user_map.clear();
		ticket_train_map.clear();
	}
	order_management(account_management* account__,train_management* train__){
		account_ = account__; train_ = train__;
		order_data.open("order_data",fstream::binary);
	}
	~order_management(){
		order_data.close();
	}
	void buy_ticket(const string &u,const string &i,int d,int n,const string &fr,const string &to,bool q = false){
		int aid = account_->get_id(u);
		account a; account_->locate_read(a,aid);
		int tid = train_->get_id(i);
		train t; train_->locate_read(t,tid);
		int oid = order_number ++; order o;
		o.id = oid; o.user_id = aid; o.train_id = tid; o.number = n;
		for(int j = 0;j < t.stationNum;j ++){
			if(fr == t.stations[j]) o.start = j;
			if(to == t.stations[j]) o.end = j;
		}
		o.train_date = d / DAY - (t.startTime + t.pre[o.start])) / DAY;
		o.price = 0; for(int j = o.start;j < o.end;j ++) o.price += t.prices[j];
		
		int min_seat = 0x3f3f3f3f; for(int j = o.start;j < o.end;j ++) min_seat = min(min_seat,t.tickets[o.train_date][j]);
		if(min_seat < n && !q){ order_number --; cout << -1 << endl; return; }
		locate_write(o,oid);
		ticket_user_map.insert(aid,oid,oid);
		if(min_seat < n && q){
			cout << "queue" << endl;
			ticket_train_map.insert(pair <int,int> (o.train_id,o.train_date),oid,oid);
			return;
		}
		for(int j = o.start;j < o.end;j ++) t.tickets[o.train_date][j] -= n;
		train_->locate_write(t,tid);
		cout << 0 << endl;
	}
	void query_order(const string &u){
		int aid = account_->get_id(u);
		vector <int> vec; ticket_user_map.find(aid,vec);
		order o; train t;
		for(int i = 0;i < vec.size();i ++){
			locate_read(o,vec[i]);
			cout << '[' << (o.status == 0 ? "success" : (o.status == 1 ? "pending" : "refunded")) << ']' << ' ';
			train_->locate_read(t,o.train_id);
			cout << t.stations[o.start] << ' ';
			cout << int_to_time(t.pre[o.start]) << " -> ";
			cout << t.stations[o.end] << ' ';
			cout << int_to_time(t.pre[o.end]) << ' ';
			cout << o.number << ' ' << o.price << endl;
		}
	}
	int refund_ticket(const string &u,int n = 1){
		n --;
		int aid = account_->get_id(u);
		account now; account_->locate_read(now,u);
		if(!now.is_login) return -1;
		
		vector <int> vec;
		ticket_user_map.find(aid,vec);
		if(n >= vec.size()) return -1; // index_out_of_bound
		int id = vec[n];
		order o; locate_read(o,id);
		if(o.status == 2) return -1;
		if(o.status == 0){
			int tid = o.train_id;
			train t;
			train_->locate_read(t,tid);
			for(int i = o.start;i < o.end;i ++) t.tickets[i] += o.number;
			ticket_train_map.find(pair <int,int> (o.train_id,o.train_date),vec);
			for(int i = (int)(vec.size()) - 1;i >= 0;i --) judge(t,vec[i]);
			train_->locate_write(t,tid);
		} // success
		if(o.status == 1) ticket_train_map.erase(pair <int,int> (o.train_id,o.train_date),id);
		o.status = 2;
		locate_write(o,id);
		return 0;
	}
};

}

#endif
