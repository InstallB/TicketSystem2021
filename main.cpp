#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <fstream>

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

int main(){
	fstream file("numbers");
	account_management account_ = new account_management;
	train_management train_ = new train_management;
	order_management order_ = new ;
	if(file){ file >> account_.user_number >> train_.train_number >> order_.order_number; }
	else{ file.open("numbers",fstream::out); file.close(); }
	string now;
	now = get_token();
	cout << now << ' ';
	while(1){
		now = get_token();
		if(now == "add_user"){
			string c,u,p,n,m; int g;
			while(1){
				now = get_token();
				if(now[0] != '-') break;
				if(now[1] == 'c') c = get_token();
				if(now[1] == 'u') u = get_token();
				if(now[1] == 'p') p = get_token();
				if(now[1] == 'n') n = get_token();
				if(now[1] == 'm') m = get_token();
				if(now[1] == 'g') g = string_to_int(get_token());
			}
			cout << account_.add_user(c,u,p,n,m,g) << endl;
		}
		if(now == "login"){
			string u,p;
			while(1){
				now = get_token();
				if(now[0] != '-') break;
				if(now[1] == 'u') u = get_token();
				if(now[1] == 'p') p = get_token();
			}
			cout << account_.login(u,p) << endl;
		}
		if(now == "logout"){
			string u;
			while(1){
				now = get_token();
				if(now[0] != '-') break;
				if(now[1] == 'u') u = get_token();
			}
			cout << account_.logout(u) << endl;
		}
		if(now == "query_profile"){
			string c,u;
			while(1){
				now = get_token();
				if(now[0] != '-') break;
				if(now[1] == 'c') c = get_token();
				if(now[1] == 'u') u = get_token();
			}
			cout << account_.query_profile(u,p) << endl;
		}
		if(now == "modify_profile"){
			string c,u,p,n,m; int g;
			while(1){
				now = get_token();
				if(now[0] != '-') break;
				if(now[1] == 'c') c = get_token();
				if(now[1] == 'u') u = get_token();
				if(now[1] == 'p') p = get_token();
				if(now[1] == 'n') n = get_token();
				if(now[1] == 'm') m = get_token();
				if(now[1] == 'g') g = string_to_int(get_token());
			}
			cout << account_.modify_profile(c,u,p,n,m,g) << endl;
		}
		if(now == "add_train"){
			string i,s,p,x,t,d; char y = 'A'; int n = 0,m = 0,o = 0;
			while(1){
				now = get_token();
				if(now[0] != '-') break;
				if(now[1] == 'i') i = get_token();
				if(now[1] == 's') s = get_token();
				if(now[1] == 'p') p = get_token();
				if(now[1] == 'x') x = get_token();
				if(now[1] == 't') t = get_token();
				if(now[1] == 'd') d = get_token();
				if(now[1] == 'y') y = (get_token())[0];
				if(now[1] == 'n') n = string_to_int(get_token());
				if(now[1] == 'm') m = string_to_int(get_token());
				if(now[1] == 'o') o = string_to_int(get_token());
			}
			cout << add_train(i,n,m,s,p,x,t,o,d,y) << endl;
		}
		if(now == "delete_train"){
			string i;
			while(1){
				now = get_token();
				if(now[0] != '-') break;
				if(now[1] == 'i') i = get_token();
			}
			cout << delete_train(i) << endl;
		}
		if(now == "release_train"){
			string i;
			while(1){
				now = get_token();
				if(now[0] != '-') break;
				if(now[1] == 'i') i = get_token();
			}
			cout << release_train(i) << endl;
		}
		if(now == "query_train"){
			string i,d;
			while(1){
				now = get_token();
				if(now[0] != '-') break;
				if(now[1] == 'i') i = get_token();
				if(now[1] == 'd') d = get_token();
			}
			query_train(i,date_to_int(d));
		}
		if(now == "query_ticket"){
			string s,t,d,p = "time";
			while(1){
				now = get_token();
				if(now[0] != '-') break;
				if(now[1] == 's') s = get_token();
				if(now[1] == 't') t = get_token();
				if(now[1] == 'd') d = get_token();
				if(now[1] == 'p') p = get_token();
			}
			query_ticket(s,t,d,p);
		}
		if(now == "query_transfer"){
			string s,t,d,p = "time";
			while(1){
				now = get_token();
				if(now[0] != '-') break;
				if(now[1] == 's') s = get_token();
				if(now[1] == 't') t = get_token();
				if(now[1] == 'd') d = get_token();
				if(now[1] == 'p') p = get_token();
			}
			query_transfer(s,t,d,p);
		}
		if(now == "buy_ticket"){
			string u,i,f,t,q; int d,n;
			while(1){
				now = get_token();
				if(now[0] != '-') break;
				if(now[1] == 'u') u = get_token();
				if(now[1] == 'i') i = get_token();
				if(now[1] == 'f') f = get_token();
				if(now[1] == 't') t = get_token();
				if(now[1] == 'q') q = get_token();
				if(now[1] == 'd') d = string_to_int(get_token());
				if(now[1] == 'n') n = string_to_int(get_token());
			}
			buy_ticket(u,i,d,n,f,t,q == "true");
		}
		if(now == "refund_ticket"){
			string u; int n = 1;
			while(1){
				now = get_token();
				if(now[0] != '-') break;
				if(now[1] == 'u') u = get_token();
				if(now[1] == 'n') n = string_to_int(get_token());
			}
			cout << refund_ticket(u,n) << endl;
		}
		if(now == "clean"){ clean(account_,train_,order_); now = get_token(); }
		if(now == "exit"){ exit(account_,train_,order_); break; }
		cout << now << ' ';
	}
	delete account_;
	delete train_;
	delete order_;
	return 0;
}
