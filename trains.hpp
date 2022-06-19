#ifndef TRAINS_HPP
#define TRAINS_HPP

#include "accounts.hpp"
#include "orders.hpp"
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

class train{
public:
	int id;
	char trainID[21];
	int stationNum;
	char stations[100][12];
	int seatNum;
	int prices[99];
	int startTime;
	int travelTimes[99];
	int stopoverTimes[98];
	int pre[100]; // startTime + travelTimes[0-(i-1)] + stopoverTimes[0-(i-2)]
	int saleDate[2];
	char type;
	int tickets[92][99];
	bool is_released;
	train(){}
	~train(){}
	bool operator < (const train &other)const{ return id < other.id; }
	class compare_id{
		bool operator ()(const train &x,const train &y){
			return strcmp(x.trainID,y.trainID) < 0;
		}
	};
	bool operator == (const train &other)const{ return id == other.id; }
	bool operator != (const train &other)const{ return id != other.id; }
};

class train_management{
	friend class order_management;
private:
	fstream train_data;
	RainyMemory::AlternativeMultiBPlusTree <string,int> train_id_map;
	RainyMemory::AlternativeMultiBPlusTree <string,int> train_pos_map;
	int get_id(const string &u){
		vector <int> vec;
		train_id_map.find(u,vec);
		return vec.size() ? vec[0] : -1;
	}
	void locate_read(train &t,int x){
		train_data.seekg(x * sizeof(train));
		train_data.read(reinterpret_cast <char*> (&t),sizeof(train));
	}
	void locate_write(train &t,int x){
		train_data.seekg(x * sizeof(train));
		train_data.write(reinterpret_cast <char*> (&t),sizeof(train));
	}
public:
	int train_number;
	void clean(){
		train_number = 0;
		train_data.close();
		train_data.open("train_data",fstream::binary | fstream::out);
		train_data.close();
		train_data.open("train_data",fstream::binary);
		train_id_map.clear();
		train_pos_map.clear();
	}
	train_management(){
		train_data.open("train_data",fstream::binary);
	}
	~train_management(){
		train_data.close();
	}
	int add_train(const string &i,int n,int m,const string &s,const string &p,const string &x,const string &t,const string &o,const string &d,char y){
		vector <int> vec; train_id_map.find(i,vec); if(vec.size()) return -1;
		
		string tmp,start_station;
		train now; int id = train_number ++;
		// id
		now.id = id;
		// trainID[21];
		for(int j = 0;j < i.size();j ++) now.trainID[j] = i[j];
		// stationNum;
		now.stationNum = n;
		// stations[100][12];
		for(int k = 0,pos = 0,j = 0;j < s.size();j ++){
			if(s[i] == '|') k ++,pos = 0;
			else now.stations[k][pos ++] = s[j];
			if(k == 0) start_station += s[j];
		}
		// seatNum;
		now.seatNum = m;
		// prices[99];
		tmp = "";
		for(int k = 0,j = 0;j < p.size();j ++){
			if(s[i] == '|'){
				now.prices[k ++] = string_to_int(tmp);
				tmp = "";
			}
			else tmp += p[j];
		}
		// startTime;
		now.startTime = time_to_int(x);
		// travelTimes[99];
		tmp = "";
		for(int k = 0,j = 0;j < t.size();j ++){
			if(s[i] == '|'){
				now.travelTimes[k ++] = string_to_int(tmp);
				tmp = "";
			}
			else tmp += t[j];
		}
		// stopoverTimes[98];
		if(n > 2){
		tmp = "";
			for(int k = 0,j = 0;j < o.size();j ++){
				if(s[i] == '|'){
					now.stopoverTimes[k ++] = string_to_int(tmp);
					tmp = "";
				}
				else tmp += o[j];
			}
		}
		// saleDate[2];
		tmp = ""; for(int j = 0;j < 5;j ++) tmp += d[j]; now.saleDate[0] = date_to_int(tmp);
		tmp = ""; for(int j = 6;j < 11;j ++) tmp += d[j]; now.saleDate[1] = date_to_int(tmp);
		// type;
		now.type = y;
		// tickets[92][99];
		for(int j = 0;j <= (now.saleDate[1] - now.saleDate[0]) / DAY;j ++){
			for(int k = 0;k < n - 1;k ++){
				now.tickets[j][k] = m;
			}
		}
		// is_released;
		now.is_released = false;
		// pre[100]
		for(int sum_time = now.startTime,j = 0;j < n;j ++){
			now.pre[j] = sum_time;
			if(j == n - 1) break;
			sum_time += now.travelTimes[j];
			if(j) sum_time += now.stopoverTimes[j - 1];
		}
		
		locate_write(now,id);
		train_id_map.insert(i,id,id);
		for(int j = 0;j < now.stationNum;j ++){
			train_pos_map.insert(now.stations[j],id,id);
		}
		return 0;
	}
	int delete_train(const string &i){
		int id = get_id(i); train now;
		if(id == -1) return -1; // train not found
		locate_read(now,id);
		if(now.is_released) return -1;
		train_id_map.erase(i,id);
		train_pos_map.erase(i,id);
		return 0;
	}
	int release_train(const string &i){
		int id = get_id(i); train now;
		if(id == -1) return -1; // train not found
		locate_read(now,id);
		now.is_released = true;
		locate_write(now,id);
		return 0;
	}
	void query_train(const string &i,int d){
		int id = get_id(i); train now;
		if(id == -1) return -1; // train not found
		locate_read(now,id);
		int price_sum = 0,now_time = d * DAY + now.startTime;
		cout << now.trainID << ' ' << now.type << '\n';
		for(int i = 0;i < now.stationNum;i ++){
			cout << now.stations[i] << ' ';
			if(i == 0) cout << "xx-xx xx:xx";
			else{ cout << int_to_time(now_time); now_time += now.stopoverTimes[i - 1]; }
			cout << " -> ";
			if(i == now.stationNum - 1) cout << "xx-xx xx:xx";
			else{ cout << int_to_time(now_time); now_time += now.travelTimes[i]; }
			cout << ' ' << price_sum << ' ' << now.tickets[(d - now.startTime) / DAY][i] << endl;
			if(i != now.stationNum - 1) price_sum += now.prices[i];
		}
	}
	void query_ticket(const string &s,const string &t,const string &sd,const string &P = "time"){
		vector <int> vec; int d = date_to_int(sd);
		sjtu::priority_queue <pair <int,train>,train::compare_ID> pq;
		train_pos_map.find(s,vec);
		for(int p : vec){
			int id = p,l,r = -1,sum_time = 0,sum_price = 0; train now;
			locate_read(now,id);
			for(int i = 0;i < now.stationNum;i ++){
				if(s == now.stations[i]) l = i;
				if(t == now.stations[i]){ r = i; break; }
			}
			if(r == -1) continue;
			int l_d = d - now.pre[l] / DAY * DAY;
			if(!(now.saleDate[0] <= l_d && l_d <= now.saleDate[1])) continue;
			for(int i = l;i < r;i ++){
				sum_price += now.prices[i];
				sum_time += now.travelTimes[i];
				if(i != l) sum_time += now.stopoverTimes[i - 1];
			}
			if(P[0] == 't') pq.push(pair <int,train> (sum_time,now);
			if(P[0] == 'c') pq.push(pair <int,train> (sum_price,now);
		}
		cout << pq.size() << '\n';
		while(!pq.empty()){
			train now = q.top().second;
			int l,r,min_ticket = 0x3f3f3f3f,sum_price = 0; train now;
			for(int i = 0;i < now.stationNum;i ++){
				if(s == now.stations[i]) l = i;
				if(t == now.stations[i]){ r = i; break; }
			}
			if(r == -1) continue;
			int l_d = d - now.pre[l] / DAY * DAY,t_d = (l_d - now.saleDate[0]) / DAY;
			for(int i = l;i < r;i ++){
				sum_price += now.prices[i];
				if(i != l) min_ticket = min(min_ticket,now.tickets[t_d][i]);
			}
			cout << now.trainID << ' ';
			cout << now.stations[l] << ' ' << int_to_date(l_d + now.pre[l]) << " -> ";
			cout << now.stations[r] << ' ' << int_to_date(l_d + now.pre[r]) << ' ';
			cout << sum_price << ' ' << min_ticket << endl;
		}
	}
	void query_transfer(const string &s,const string &t,const string &sd,const string &P = "time"){
		vector <int> vecs_id,vect_id; int d = date_to_int(sd);
		train_pos_map.find(s,vecs_id);
		train_pos_map.find(t,vect_id);
		vector <train> vecs,vect;
		for(int p : vecs_id){ train now; locate_read(now,p); vecs.push_back(now); }
		for(int p : vect_id){ train now; locate_read(now,p); vect.push_back(now); }
		pair <pair <int,int>,pair <string,string> > ans,tmp; train ap,aq; bool has_ans = false; int ap,aq,ap_l,ap_r,aq_l,aq_r,ap_l_d,aq_l_d,ap_sum_price,ap_min_ticket,aq_sum_price,aq_min_ticket;
		
		for(train p : vecs){
			int p_l; for(int i = 0;i < p.stationNum;i ++) if(s == p.stations[i]) p_l = i;
			int p_l_d = d - p.pre[l] / DAY * DAY,p_t_d = (p_l_d - p.saleDate[0]) / DAY;
			if(!(p.saleDate[0] <= p_l_d && p_l_d <= p.saleDate[1])) continue;
			
			map <string,pair <int,int> > mp; // pair <time,p_r>
			for(int i = p_l + 1;i < p.stationNum;i ++) mp[p.station[i]] = p_l_d + p.pre[i];
			
			for(train q : vect){
				int q_r; for(int i = 0;i < q.stationNum;i ++) if(t == q.stations[i]) q_r = t;
				for(int i = 0;i < q_r;i ++){
					if(mp.find(q.stations[i]) == mp.end()) continue;
					int q_l_d = mp[q.stations[i]].first / DAY * DAY + DAY * (q.pre[i] % DAY <= mp[q.stations[i]].first % DAY) - q.pre[i] / DAY * DAY,q_t_d = (q_l_d - q.SaleDate[0]) / DAY;
					if(!(q.saleDate[0] <= q_l_d && q_l_d <= q.saleDate[1])) continue;
					
					p_r = mp[q.stations[i]].second; q_l = i;
					int sum_time = q_l_d + q.pre[q_r] - (p_l_d + p.pre[q_l]),sum_price_p = 0,sum_price_q = 0,min_seat_p = 0x3f3f3f3f,min_seat_q = 0x3f3f3f3f;
					for(int i = p_l;i < p_r;i ++){ sum_price_p += p.prices[i]; min_seat_p = min(min_seat,p.tickets[p_t_d][i]); }
					for(int i = q_l;i < q_r;i ++){ sum_price_q += q.prices[i]; min_seat_q = min(min_seat,q.tickets[q_t_d][i]); }
					
					if(P[0] == 'c'){
						tmp = pair <pair <int,int>,pair <string,string> > (pair <int,int> (sum_price,sum_cost),pair <string,string> (p.trainID,q.trainID));
						if(tmp < ans || !has_ans){ ans = tmp; has_ans = true; ap = p; aq = q; ap_l = p_l; ap_r = p_r; aq_l = q_l; aq_r = q_r; ap_l_d = p_l_d; aq_l_d = q_l_d; ap_sum_price = sum_price_p; ap_min_ticket = min_seat_p; aq_sum_price = sum_price_q; aq_min_ticket = min_seat_q; }
					}
					if(P[0] == 't'){
						tmp = pair <pair <int,int>,pair <string,string> > (pair <int,int> (sum_cost,sum_price),pair <string,string> (p.trainID,q.trainID));
						if(tmp < ans || !has_ans){ ans = tmp; has_ans = true; ap = p; aq = q; ap_l = p_l; ap_r = p_r; aq_l = q_l; aq_r = q_r; ap_l_d = p_l_d; aq_l_d = q_l_d; ap_sum_price = sum_price_p; ap_min_ticket = min_seat_p; aq_sum_price = sum_price_q; aq_min_ticket = min_seat_q; }
					}
				}
			}
		}
		if(!has_ans) cout << 0 << endl;
		else{
			cout << ap.trainID << ' ';
			cout << ap.stations[ap_l] << ' ' << int_to_date(ap_l_d + ap.pre[ap_l]) << " -> ";
			cout << ap.stations[ap_r] << ' ' << int_to_date(ap_l_d + ap.pre[ap_r]) << ' ';
			cout << ap_sum_price << ' ' << ap_min_ticket << endl;
			cout << aq.trainID << ' ';
			cout << aq.stations[aq_l] << ' ' << int_to_date(aq_l_d + aq.pre[aq_l]) << " -> ";
			cout << aq.stations[aq_r] << ' ' << int_to_date(aq_l_d + aq.pre[aq_r]) << ' ';
			cout << aq_sum_price << ' ' << aq_min_ticket << endl;
		}
	}
};

}

#endif
