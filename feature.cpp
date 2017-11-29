#include<bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define N 9999
#define DOUBLE_MAX 1.79e+308
#define DOUBLE_MIN -2.22e-308
#define EIGHT_MONTH (3600*24*30*8)
#define FIVE_MONTH (3600*24*30*5)
#define THREE_MONTH (3600*24*30*3)
#define ONE_MONTH (3600*24*30)
#define ONE_DAY (3600*24)
#define FIVE_DAY (3600*24*5)
#define Name(X) #X
#define Out(X) {string xname = Name(X); cout << xname.substr(5, xname.size()-1) << ": " << X << " ";}
using namespace std;


bool DEBUG_LOG = true;
bool DEBUG_TEST = true;
bool is_cache = false;
bool raw_data_cache = false;
int max_time_long = 0;
string test_user = "";
int change_label_cnt = 0;

typedef struct trade_data{
	long time_stamp;
	string user_id;
	int label;
	int row_key;

	trade_data& operator=(const trade_data data) {
		this->time_stamp = data.time_stamp;
		this->user_id = data.user_id;
		this->label = data.label;
		this->row_key = data.row_key;
	}
}trade_data;

bool operator<(const trade_data & x,const trade_data & y) {
	if(x.time_stamp == y.time_stamp)
		return x.row_key < y.row_key;
	return x.time_stamp < y.time_stamp;  
}

typedef struct login_data{
	int time_long;
	string user_id;
	string device;
	string log_from;
	string ip;
	string city;
	string result;
	long time_stamp;
	string type;
	int is_scan;
	int is_sec;
	vector<trade_data> trade_vec;
	string time_str;

	login_data& operator=(const login_data data) {
		this->time_long = data.time_long;
		this->user_id = data.user_id;
		this->device = data.device;
		this->log_from = data.log_from;
		this->ip = data.ip;
		this->city = data.city;
		this->result = data.result;
		this->time_stamp = data.time_stamp;
		this->type = data.type;
		this->is_scan = data.is_scan;
		this->is_sec = data.is_sec;
		this->time_str = data.time_str;

		(this->trade_vec).clear();
		for(int i = 0; i < data.trade_vec.size(); i++) {
			(this->trade_vec).push_back(data.trade_vec[i]);
		}
	}
}login_data;

void out_login(login_data data){
	if(!DEBUG_TEST)
		return;
	cout << "login info ";
	Out(data.time_long);
	Out(data.user_id);
	Out(data.device);
	Out(data.log_from);
	Out(data.ip);
	Out(data.city);
	Out(data.result);
	Out(data.time_stamp);
	Out(data.type);
	Out(data.is_scan);
	Out(data.is_sec);
	Out(data.time_str);
	cout << " " << data.trade_vec.size() << endl;
}

bool operator<(const login_data & x,const login_data & y) {
	if(x.time_stamp == y.time_stamp)
		return x.device < y.device;
	return x.time_stamp < y.time_stamp;
}

bool operator==(const login_data & x,const login_data & y) {
	return (x.device == y.device)
	     && (x.log_from == y.log_from)
	     && (x.ip == y.ip)
	     && (x.city == y.city)
	     && (fabs(x.time_stamp - y.time_stamp) < 10)
	     && (x.type == y.type)
	     && (x.is_scan == y.is_scan)
	     && (x.time_str == y.time_str);  
}

void out_trade(trade_data data) {
	if(!DEBUG_TEST)
		return;
	cout << "trade info ";
	Out(data.time_stamp);
	Out(data.user_id);
	Out(data.label);
	Out(data.row_key);
	cout << " " << endl;
}

set<string> black_row_key;

map<string, set<login_data> > login_info;
map<string, set<trade_data> > trade_info;
map<string, vector<login_data> > total_login_vec;
map<string, vector<trade_data> > total_trade_vec;
map<string, map<string, int> > discrete_map;

map<string, map<string, int> > device_user_map;
map<string, map<string, int> > ip_user_map;

set<login_data> total_login_map;
set<trade_data> total_trade_map;
map<string, map<string, int> > total_device_map;
map<string, map<string, int> > total_ip_map;
map<string, map<string, int> > trade_total_device_map;
map<string, map<string, int> > trade_total_ip_map;
map<string, map<string, int> > valid_total_device_map;
map<string, map<string, int> > valid_total_ip_map;
map<string, map<string, int> > black_total_device_map;
map<string, map<string, int> > black_total_ip_map;
map<string, map<string, int> > gray_total_device_map;
map<string, map<string, int> > gray_total_ip_map;

int train_pos = 0, train_neg = 0, test_pos = 0, test_neg = 0, valid_pos = 0, valid_neg = 0;
int case1_pos = 0, case1_neg = 0, case2_pos = 0, case2_neg = 0;

map<string, int> device_map;
map<string, int> ip_map;
map<string, int> city_map;
map<string, int> type_map;
map<string, int> log_from_map;

map<string, int> trade_device_map;
map<string, int> trade_ip_map;
map<string, int> trade_city_map;
map<string, int> trade_type_map;
map<string, int> trade_log_from_map;

map<string, int> valid_device_map;
map<string, int> valid_ip_map;
map<string, int> valid_city_map;
map<string, int> valid_type_map;
map<string, int> valid_log_from_map;

map<string, int> black_device_map;
map<string, int> black_ip_map;
map<string, int> black_city_map;
map<string, int> black_type_map;
map<string, int> black_log_from_map;

set<string> device_set;
set<string> ip_set;
set<string> city_set;
set<string> type_set;
set<string> log_from_set;


string train_file = "train_result";
ofstream train_out_file(train_file.c_str());
string test_file = "test_result";
ofstream test_out_file(test_file.c_str());
string valid_file = "valid_result";
ofstream valid_out_file(valid_file.c_str());
string predict_file = "predict_result";
ofstream predict_out_file(predict_file.c_str());

ofstream zero_raw_data_out("zero_raw_data");
ofstream sample_raw_data_out("sample_raw_data");

bool upload_device_map(string path) {
	ifstream file_in(path.c_str());
	if(!file_in) {
		is_cache = false;
		return false;
	}else {
		is_cache = true;
	}
	while(!file_in.eof()) {
		string key, value_key;
		int value_value;
		file_in >> key;
		map<string, int> tmp;
		while(!file_in.eof()) {
			file_in >> value_key >> value_value;
			if(value_key == "0" && value_value == 0)
				break;
			tmp.insert(make_pair(value_key, value_value));
		}
		device_user_map.insert(make_pair(key, tmp));
		
	}
	if(device_user_map.size() > 0) {
		cout << "device size " << device_user_map.size() << endl;
		is_cache = true;
	}else {
		is_cache = false;
	}
	return true;
}

bool write_device_map(string path) {
	ofstream file_out(path.c_str());
	std::stringstream ss;
	for(map<string, map<string, int> >::iterator it = device_user_map.begin(); it != device_user_map.end(); ++it) {
		file_out << it->first << " ";
		for(map<string, int>::iterator iter = (it->second).begin(); iter != (it->second).end(); ++iter) {
			file_out << iter->first << " " << iter->second << " ";
		}
		file_out << "0 0";
		file_out << " " << endl;
	}
	cout << "save file " << path << ", size " << device_user_map.size() << endl;
}

bool upload_ip_map(string path) {
	ifstream file_in(path.c_str());
	if(!file_in) {
		is_cache = false;
		return false;
	}else {
		is_cache = true;
	}
	while(!file_in.eof()) {
		string key, value_key;
		int value_value;
		file_in >> key;
		map<string, int> tmp;
		while(!file_in.eof()) {
			file_in >> value_key >> value_value;
			if(value_key == "0" && value_value == 0)
				break;
			tmp.insert(make_pair(value_key, value_value));
		}
		ip_user_map.insert(make_pair(key, tmp));
	}
	if(ip_user_map.size() > 0) {
		cout << "ip size " << ip_user_map.size() << endl;
		is_cache = true;
	}else {
		is_cache = false;
	}
	return true;
}

bool write_ip_map(string path) {
	ofstream file_out(path.c_str());
	std::stringstream ss;
	for(map<string, map<string, int> >::iterator it = ip_user_map.begin(); it != ip_user_map.end(); ++it) {
		file_out << it->first << " ";
		for(map<string, int>::iterator iter = (it->second).begin(); iter != (it->second).end(); ++iter) {
			file_out << iter->first << " " << iter->second << " ";
		}
		file_out << "0 0";
		file_out << " " << endl;
	}
	cout << "save file " << path << ", size " << ip_user_map.size() << endl;
}

bool init_discrete_map() {
	map<string, int> tmp;
	discrete_map.insert(make_pair("device", tmp));
	discrete_map.insert(make_pair("log_from", tmp));
	discrete_map.insert(make_pair("ip", tmp));
	discrete_map.insert(make_pair("city", tmp));
	discrete_map.insert(make_pair("result", tmp));
	discrete_map.insert(make_pair("type", tmp));
	upload_device_map("total_device_map");
	upload_ip_map("total_ip_map");
}

bool update_device_map(login_data data, string user_id) {
	map<string, map<string, int> >::iterator it = device_user_map.find(data.device);
	if(it == device_user_map.end()) {
		map<string, int> tmp;
		tmp.insert(make_pair(user_id, 1));
		tmp.insert(make_pair("sum", 1));
		device_user_map.insert(make_pair(data.device, tmp));
	}else {
		map<string, int> tmp = it->second;
		if(tmp.find(user_id) == tmp.end()) {
			tmp.insert(make_pair(user_id, 1));

			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}else {
			map<string, int>::iterator iter = tmp.find(user_id);
			int cnt = iter->second + 1;
			iter->second = cnt;

			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}
		it->second = tmp;
	}
}

bool update_total_device_map(login_data data, string user_id) {
	map<string, map<string, int> >::iterator it = total_device_map.find(data.device);
	if(it == total_device_map.end()) {
		map<string, int> tmp;
		tmp.insert(make_pair(user_id, 1));
		tmp.insert(make_pair("sum", 1));
		total_device_map.insert(make_pair(data.device, tmp));
	}else {
		map<string, int> tmp = it->second;
		if(tmp.find(user_id) == tmp.end()) {
			tmp.insert(make_pair(user_id, 1));

			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}else {
			map<string, int>::iterator iter = tmp.find(user_id);
			int cnt = iter->second + 1;
			iter->second = cnt;

			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}
		it->second = tmp;
	}
}

bool update_trade_total_device_map(login_data data, string user_id) {
	map<string, map<string, int> >::iterator it = trade_total_device_map.find(data.device);
	if(it == trade_total_device_map.end()) {
		map<string, int> tmp;
		tmp.insert(make_pair(user_id, 1));
		tmp.insert(make_pair("sum", 1));
		trade_total_device_map.insert(make_pair(data.device, tmp));
	}else {
		map<string, int> tmp = it->second;
		if(tmp.find(user_id) == tmp.end()) {
			tmp.insert(make_pair(user_id, 1));

			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}else {
			map<string, int>::iterator iter = tmp.find(user_id);
			int cnt = iter->second + 1;
			iter->second = cnt;

			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}
		it->second = tmp;
	}
}

bool update_valid_total_device_map(login_data data, string user_id) {
	map<string, map<string, int> >::iterator it = valid_total_device_map.find(data.device);
	if(it == valid_total_device_map.end()) {
		map<string, int> tmp;
		tmp.insert(make_pair(user_id, 1));
		tmp.insert(make_pair("sum", 1));
		valid_total_device_map.insert(make_pair(data.device, tmp));
	}else {
		map<string, int> tmp = it->second;
		if(tmp.find(user_id) == tmp.end()) {
			tmp.insert(make_pair(user_id, 1));

			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}else {
			map<string, int>::iterator iter = tmp.find(user_id);
			int cnt = iter->second + 1;
			iter->second = cnt;

			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}
		it->second = tmp;
	}
}

bool update_black_total_device_map(login_data data, string user_id) {
	map<string, map<string, int> >::iterator it = black_total_device_map.find(data.device);
	if(it == black_total_device_map.end()) {
		map<string, int> tmp;
		tmp.insert(make_pair(user_id, 1));
		tmp.insert(make_pair("sum", 1));
		black_total_device_map.insert(make_pair(data.device, tmp));
	}else {
		map<string, int> tmp = it->second;
		if(tmp.find(user_id) == tmp.end()) {
			tmp.insert(make_pair(user_id, 1));
			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}else {
			map<string, int>::iterator iter = tmp.find(user_id);
			int cnt = iter->second + 1;
			iter->second = cnt;

			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}
		it->second = tmp;
	}
}

bool update_gray_total_device_map(login_data data, string user_id) {
	map<string, map<string, int> >::iterator it = gray_total_device_map.find(data.device);
	if(it == gray_total_device_map.end()) {
		map<string, int> tmp;
		tmp.insert(make_pair(user_id, 1));
		tmp.insert(make_pair("sum", 1));
		gray_total_device_map.insert(make_pair(data.device, tmp));
	}else {
		map<string, int> tmp = it->second;
		if(tmp.find(user_id) == tmp.end()) {
			tmp.insert(make_pair(user_id, 1));
			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}else {
			map<string, int>::iterator iter = tmp.find(user_id);
			int cnt = iter->second + 1;
			iter->second = cnt;

			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}
		it->second = tmp;
	}
}

bool update_log_from_map(login_data data) {
	map<string, map<string, int> >::iterator it = discrete_map.find("log_from");
	map<string, int> tmp = it->second;
	if(tmp.find(data.log_from) == tmp.end()) {
		int size = tmp.size();
		tmp.insert(make_pair(data.log_from, size));
		it->second = tmp;
	}
}

bool update_ip_map(login_data data, string user_id) {
	map<string, map<string, int> >::iterator it = ip_user_map.find(data.ip);
	if(it == ip_user_map.end()) {
		map<string, int> tmp;
		tmp.insert(make_pair(user_id, 1));
		tmp.insert(make_pair("sum", 1));
		ip_user_map.insert(make_pair(data.ip, tmp));
	}else {
		map<string, int> tmp = it->second;
		if(tmp.find(user_id) == tmp.end()) {
			tmp.insert(make_pair(user_id, 1));
			
			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}else {
			map<string, int>::iterator iter = tmp.find(user_id);
			int cnt = iter->second + 1;
			iter->second = cnt;

			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}
		it->second = tmp;
	}
}

bool update_total_ip_map(login_data data, string user_id) {
	map<string, map<string, int> >::iterator it = total_ip_map.find(data.ip);
	if(it == total_ip_map.end()) {
		map<string, int> tmp;
		tmp.insert(make_pair(user_id, 1));
		tmp.insert(make_pair("sum", 1));
		total_ip_map.insert(make_pair(data.ip, tmp));
	}else {
		map<string, int> tmp = it->second;
		if(tmp.find(user_id) == tmp.end()) {
			tmp.insert(make_pair(user_id, 1));
			
			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}else {
			map<string, int>::iterator iter = tmp.find(user_id);
			int cnt = iter->second + 1;
			iter->second = cnt;

			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}
		it->second = tmp;
	}
}

bool update_trade_total_ip_map(login_data data, string user_id) {
	map<string, map<string, int> >::iterator it = trade_total_ip_map.find(data.ip);
	if(it == trade_total_ip_map.end()) {
		map<string, int> tmp;
		tmp.insert(make_pair(user_id, 1));
		tmp.insert(make_pair("sum", 1));
		trade_total_ip_map.insert(make_pair(data.ip, tmp));
	}else {
		map<string, int> tmp = it->second;
		if(tmp.find(user_id) == tmp.end()) {
			tmp.insert(make_pair(user_id, 1));
			
			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}else {
			map<string, int>::iterator iter = tmp.find(user_id);
			int cnt = iter->second + 1;
			iter->second = cnt;

			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}
		it->second = tmp;
	}
}

bool update_valid_total_ip_map(login_data data, string user_id) {
	map<string, map<string, int> >::iterator it = valid_total_ip_map.find(data.ip);
	if(it == valid_total_ip_map.end()) {
		map<string, int> tmp;
		tmp.insert(make_pair(user_id, 1));
		tmp.insert(make_pair("sum", 1));
		valid_total_ip_map.insert(make_pair(data.ip, tmp));
	}else {
		map<string, int> tmp = it->second;
		if(tmp.find(user_id) == tmp.end()) {
			tmp.insert(make_pair(user_id, 1));
			
			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}else {
			map<string, int>::iterator iter = tmp.find(user_id);
			int cnt = iter->second + 1;
			iter->second = cnt;

			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}
		it->second = tmp;
	}
}

bool update_black_total_ip_map(login_data data, string user_id) {
	map<string, map<string, int> >::iterator it = black_total_ip_map.find(data.ip);
	if(it == black_total_ip_map.end()) {
		map<string, int> tmp;
		tmp.insert(make_pair(user_id, 1));
		tmp.insert(make_pair("sum", 1));
		black_total_ip_map.insert(make_pair(data.ip, tmp));
	}else {
		map<string, int> tmp = it->second;
		if(tmp.find(user_id) == tmp.end()) {
			tmp.insert(make_pair(user_id, 1));
			
			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}else {
			map<string, int>::iterator iter = tmp.find(user_id);
			int cnt = iter->second + 1;
			iter->second = cnt;

			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}
		it->second = tmp;
	}
}

bool update_gray_total_ip_map(login_data data, string user_id) {
	map<string, map<string, int> >::iterator it = gray_total_ip_map.find(data.ip);
	if(it == gray_total_ip_map.end()) {
		map<string, int> tmp;
		tmp.insert(make_pair(user_id, 1));
		tmp.insert(make_pair("sum", 1));
		gray_total_ip_map.insert(make_pair(data.ip, tmp));
	}else {
		map<string, int> tmp = it->second;
		if(tmp.find(user_id) == tmp.end()) {
			tmp.insert(make_pair(user_id, 1));
			
			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}else {
			map<string, int>::iterator iter = tmp.find(user_id);
			int cnt = iter->second + 1;
			iter->second = cnt;

			map<string, int>::iterator it1 = tmp.find("sum");
			int size = it1->second + 1;
			it1->second = size;
		}
		it->second = tmp;
	}
}

bool update_city_map(login_data data) {
	map<string, map<string, int> >::iterator it = discrete_map.find("city");
	map<string, int> tmp = it->second;
	if(tmp.find(data.city) == tmp.end()) {
		int size = tmp.size();
		tmp.insert(make_pair(data.city, size));
		it->second = tmp;
	}
}

bool update_result_map(login_data data) {
	map<string, map<string, int> >::iterator it = discrete_map.find("result");
	map<string, int> tmp = it->second;
	if(tmp.find(data.result) == tmp.end()) {
		int size = tmp.size();
		tmp.insert(make_pair(data.result, size));
		it->second = tmp;
	}
}

bool update_type_map(login_data data) {
	map<string, map<string, int> >::iterator it = discrete_map.find("type");
	map<string, int> tmp = it->second;
	if(tmp.find(data.type) == tmp.end()) {
		int size = tmp.size();
		tmp.insert(make_pair(data.type, size));
		it->second = tmp;
	}
}

bool update_discrete_map(login_data data, string user_id) {
	if(!is_cache) {
		update_device_map(data, user_id);
		update_ip_map(data, user_id);
	}
	update_log_from_map(data);
	update_result_map(data);
	//update_city_map(data);
	update_type_map(data);
}

bool update_total_map(login_data data, string user_id) {
	if(!is_cache) {
		update_device_map(data, user_id);
		update_ip_map(data, user_id);
	}
}

bool output_discrete_map() {
	cout << "output discrete_map" << endl;
	for(map<string, map<string, int> >::iterator it = discrete_map.begin(); it != discrete_map.end(); ++it) {
		cout << "key " << it->first << ", value size " << (it->second).size() << endl;
		for(map<string, int>::iterator itt = (it->second).begin(); itt != (it->second).end(); ++itt) {
			cout << "-----key " << itt->first << ", value size " << itt->second << endl;
		}
	}
	if(!is_cache) {
		cout << "output total_device_map and total_ip_map" << endl;
		write_device_map("total_device_map");
		write_ip_map("total_ip_map");
	}
}

bool load_login_data(string login_path) {
	ifstream readdata(login_path.c_str());
	int line_num = 0;
	cout << "begin to load login data" << endl;
	while(!readdata.eof()) {
		line_num++;
		if(line_num%100000 == 0)
			cout << "parse login data, line " << line_num << endl;
		char buf[N];
		readdata.getline(buf, sizeof(buf));
		if(line_num == 1){
			continue;
		}
		const char * split = ",";
		char * p = NULL;
		p = strtok(buf, split);
		int i = 0;
		login_data login;
		vector<trade_data> trade_detail;
		login.trade_vec = trade_detail;
		string user_id, time;
		bool status = false;
		while(p != NULL)
		{
			if(i == 1)
				login.time_long = atoi(p);
			else if(i == 2)
				login.device = p;
			else if(i == 3)
				login.log_from = p;
			else if(i == 4)
				login.ip = p;
			else if(i == 5)
				login.city = p;
			else if(i == 6)
				login.result = p;
			else if(i == 7) {
				time = p;
				string sub_time = time.substr(0, time.length()-2);
				login.time_stamp = (atof(sub_time.c_str())*pow(10, 9));
			}
			else if(i == 8)
				login.type = p;
			else if(i == 9) {
				user_id = p;
				login.user_id = user_id;
			}
			else if(i == 10)
				login.is_scan = atoi(p);
			else if(i == 11)
				login.is_sec = atoi(p);
			else if(i == 12)
				login.time_str = p;
			p = strtok(NULL, split);
			i++;
		}
		if(i != 13)
			continue;
		if(max_time_long < login.time_long)
			max_time_long = login.time_long;
		tm tm_tmp;
		time_t t_tmp;
		strptime(login.time_str.c_str(), "%Y-%m-%d %H:%M:%S", &tm_tmp);
		tm_tmp.tm_isdst = -1;
		t_tmp = mktime(&tm_tmp);
		if(DEBUG_TEST) {
			if(user_id != test_user && test_user.length() > 0)
				continue;
			//cout << "log info: ";
			//out_login(login);
			//cout << " " << endl;
		}
		if(login_info.find(user_id) == login_info.end()) {
			set<login_data> log_list;
			log_list.insert(login);
			login_info.insert(make_pair(user_id, log_list));
		}else {
			map<string, set<login_data> >::iterator it = login_info.find(user_id);
			set<login_data> log_list = it->second;
			int size = log_list.size();
			log_list.insert(login);
			it->second = log_list;
			int size1 = (it->second).size();
		}
		//update_discrete_map(login, login.user_id);
	}
	if(DEBUG_LOG) {
		cout << "total login user " << login_info.size()  
			<< ", times " << line_num  << endl;
	}
	int login_total_cnt = 0;
	for(map<string, set<login_data> >::iterator it = login_info.begin(); it != login_info.end(); it++) {
		set<login_data> login_tmp = it->second;
		login_total_cnt += login_tmp.size();
		set<login_data> new_login;
		login_data last_data;
		last_data.time_stamp = 0;
		for(set<login_data>::iterator set_it = login_tmp.begin(); set_it != login_tmp.end(); set_it++) {
			login_data data = (*set_it);
			//if(!(last_data == data)) {
			update_discrete_map(data, data.user_id);
			//new_login.insert(data);
			//}
			//total_login_map.insert(data);
			last_data = data;
		}
	}
	if(DEBUG_TEST) {
		//cout << "total login data " << login_info.at(test_user).size() << endl;
	}
	cout << "total login size " << login_total_cnt << endl;
	max_time_long = max_time_long/1000;
	return true;
}

bool load_trade_data(string trade_path) {
	ifstream readdata(trade_path.c_str());
	int line_num = 0;
	cout << "begin to load trade data" << endl;
	int pos_num = 0, neg_num = 0;
	while(!readdata.eof()) {
		line_num++;
		if(line_num%100000 == 0)
			cout << "parse trade data, line " << line_num << endl;
		char buf[N];
		readdata.getline(buf, sizeof(buf));
		if(line_num == 1)
			continue;
		const char * split = ",";
		char * p = NULL;
		p = strtok(buf, split);
		int i = 0;
		trade_data trade;
		string user_id, time;
		bool status = false;
		while(p != NULL)
		{
			if(i == 0)
				trade.row_key = atoi(p);
			if(i == 1) {
				tm tm_tmp;
				time_t t_tmp;
				strptime(p, "%Y-%m-%d %H:%M:%S.0", &tm_tmp);
				tm_tmp.tm_isdst = -1;
				t_tmp = mktime(&tm_tmp);
				trade.time_stamp = t_tmp;
			}
			else if(i == 2){
				user_id = p;
				trade.user_id = user_id;
			}
			else if(i == 3)
				trade.label = atoi(p);
			p = strtok(NULL, split);
			i++;
		}
		if(i != 4)
			continue;
		if(DEBUG_TEST) {
			if(user_id != test_user && test_user.length() > 0)
				continue;
			//out_trade(trade);
		}
		if(trade.label)
			pos_num++;
		else
			neg_num++;
		if(trade_info.find(user_id) == trade_info.end()) {
			set<trade_data> trade_list;
			trade_list.insert(trade);
			trade_info.insert(make_pair(user_id, trade_list));
		}else {
			map<string, set<trade_data> >::iterator it = trade_info.find(user_id);
			set<trade_data> trade_list = it->second;
			trade_list.insert(trade);
			it->second = trade_list;
		}
		total_trade_map.insert(trade);
	}
	if(DEBUG_LOG){
		cout << "total trade user " << trade_info.size()  << ", trade times " << line_num << endl;
		cout << "pos label " << pos_num << ", neg label " << neg_num << endl;
	}
	return true;
}

bool clear_history_map() {
	device_map.clear();
	ip_map.clear();
	city_map.clear();
	type_map.clear();
	log_from_map.clear();

	device_map.insert(make_pair("sum", 0));
	ip_map.insert(make_pair("sum", 0));
	city_map.insert(make_pair("sum", 0));
	type_map.insert(make_pair("sum", 0));
	log_from_map.insert(make_pair("sum", 0));
	

	trade_device_map.clear();
	trade_ip_map.clear();
	trade_city_map.clear();
	trade_type_map.clear();
	trade_log_from_map.clear();

	trade_device_map.insert(make_pair("sum", 0));
	trade_ip_map.insert(make_pair("sum", 0));
	trade_city_map.insert(make_pair("sum", 0));
	trade_type_map.insert(make_pair("sum", 0));
	trade_log_from_map.insert(make_pair("sum", 0));
	

	valid_device_map.clear();
	valid_ip_map.clear();
	valid_city_map.clear();
	valid_type_map.clear();
	valid_log_from_map.clear();

	valid_device_map.insert(make_pair("sum", 0));
	valid_ip_map.insert(make_pair("sum", 0));
	valid_city_map.insert(make_pair("sum", 0));
	valid_type_map.insert(make_pair("sum", 0));
	valid_log_from_map.insert(make_pair("sum", 0));


	black_device_map.clear();
	black_ip_map.clear();
	black_city_map.clear();
	black_type_map.clear();
	black_log_from_map.clear();

	black_device_map.insert(make_pair("sum", 0));
	black_ip_map.insert(make_pair("sum", 0));
	black_city_map.insert(make_pair("sum", 0));
	black_type_map.insert(make_pair("sum", 0));
	black_log_from_map.insert(make_pair("sum", 0));

	device_set.clear();
	ip_set.clear();
	city_set.clear();
	type_set.clear();
	log_from_set.clear();
}

bool update_login_set(login_data data) {
	device_set.insert(data.device);
	ip_set.insert(data.ip);
	city_set.insert(data.city);
	type_set.insert(data.type);
	log_from_set.insert(data.log_from);
}

bool update_history_login(login_data data) {
	map<string, int>::iterator it = device_map.find(data.device);
	if(it == device_map.end()) {
		device_map.insert(make_pair(data.device, 1));
	}else {
		int size = it->second + 1;
		it->second = size;
	}
	it = device_map.find("sum");
	int size = it->second + 1;
	it->second = size;

	map<string, int>::iterator it1 = ip_map.find(data.ip);
	if(it1 == ip_map.end()) {
		ip_map.insert(make_pair(data.ip, 1));
	}else {
		int size = it1->second + 1;
		it1->second = size;
	}
	it1 = ip_map.find("sum");
	size = it1->second + 1;
	it1->second = size;

	map<string, int>::iterator it2 = city_map.find(data.city);
	if(it2 == city_map.end()) {
		city_map.insert(make_pair(data.city, 1));
	}else {
		int size = it2->second + 1;
		it2->second = size;
	}
	it2 = city_map.find("sum");
	size = it2->second + 1;
	it2->second = size;

	map<string, int>::iterator it3 = type_map.find(data.type);
	if(it3 == type_map.end()) {
		type_map.insert(make_pair(data.type, 1));
	}else {
		int size = it3->second + 1;
		it3->second = size;
	}
	it3 = type_map.find("sum");
	size = it3->second + 1;
	it3->second = size;

	map<string, int>::iterator it4 = log_from_map.find(data.log_from);
	if(it4 == log_from_map.end()) {
		log_from_map.insert(make_pair(data.log_from, 1));
	}else {
		int size = it4->second + 1;
		it4->second = size;
	}
	it4 = log_from_map.find("sum");
	size = it4->second + 1;
	it4->second = size;
}

bool update_history_trade(login_data data) {
	map<string, int>::iterator it = trade_device_map.find(data.device);
	if(it == trade_device_map.end()) {
		trade_device_map.insert(make_pair(data.device, 1));
	}else {
		int size = it->second + 1;
		it->second = size;
	}
	it = trade_device_map.find("sum");
	int size = it->second + 1;
	it->second = size;

	map<string, int>::iterator it1 = trade_ip_map.find(data.ip);
	if(it1 == trade_ip_map.end()) {
		trade_ip_map.insert(make_pair(data.ip, 1));
	}else {
		int size = it1->second + 1;
		it1->second = size;
	}
	it1 = trade_ip_map.find("sum");
	size = it1->second + 1;
	it1->second = size;

	map<string, int>::iterator it2 = trade_city_map.find(data.city);
	if(it2 == trade_city_map.end()) {
		trade_city_map.insert(make_pair(data.city, 1));
	}else {
		int size = it2->second + 1;
		it2->second = size;
	}
	it2 = trade_city_map.find("sum");
	size = it2->second + 1;
	it2->second = size;

	map<string, int>::iterator it3 = trade_type_map.find(data.type);
	if(it3 == trade_type_map.end()) {
		trade_type_map.insert(make_pair(data.type, 1));
	}else {
		int size = it3->second + 1;
		it3->second = size;
	}
	it3 = trade_type_map.find("sum");
	size = it3->second + 1;
	it3->second = size;

	map<string, int>::iterator it4 = trade_log_from_map.find(data.log_from);
	if(it4 == trade_log_from_map.end()) {
		trade_log_from_map.insert(make_pair(data.log_from, 1));
	}else {
		int size = it4->second + 1;
		it4->second = size;
	}
	it4 = trade_log_from_map.find("sum");
	size = it4->second + 1;
	it4->second = size;
}

bool update_history_valid(login_data data) {
	map<string, int>::iterator it = valid_device_map.find(data.device);
	if(it == valid_device_map.end()) {
		valid_device_map.insert(make_pair(data.device, 1));
	}else {
		int size = it->second + 1;
		it->second = size;
	}
	it = valid_device_map.find("sum");
	int size = it->second + 1;
	it->second = size;

	map<string, int>::iterator it1 = valid_ip_map.find(data.ip);
	if(it1 == valid_ip_map.end()) {
		valid_ip_map.insert(make_pair(data.ip, 1));
	}else {
		int size = it1->second + 1;
		it1->second = size;
	}
	it1 = valid_ip_map.find("sum");
	size = it1->second + 1;
	it1->second = size;

	map<string, int>::iterator it2 = valid_city_map.find(data.city);
	if(it2 == valid_city_map.end()) {
		valid_city_map.insert(make_pair(data.city, 1));
	}else {
		int size = it2->second + 1;
		it2->second = size;
	}
	it2 = valid_city_map.find("sum");
	size = it2->second + 1;
	it2->second = size;

	map<string, int>::iterator it3 = valid_type_map.find(data.type);
	if(it3 == valid_type_map.end()) {
		valid_type_map.insert(make_pair(data.type, 1));
	}else {
		int size = it3->second + 1;
		it3->second = size;
	}
	it3 = valid_type_map.find("sum");
	size = it3->second + 1;
	it3->second = size;

	map<string, int>::iterator it4 = valid_log_from_map.find(data.log_from);
	if(it4 == valid_log_from_map.end()) {
		valid_log_from_map.insert(make_pair(data.log_from, 1));
	}else {
		int size = it4->second + 1;
		it4->second = size;
	}
	it4 = valid_log_from_map.find("sum");
	size = it4->second + 1;
	it4->second = size;
}

bool update_history_black(login_data data) {
	map<string, int>::iterator it = black_device_map.find(data.device);
	if(it == black_device_map.end()) {
		black_device_map.insert(make_pair(data.device, 1));
	}else {
		int size = it->second + 1;
		it->second = size;
	}
	it = black_device_map.find("sum");
	int size = it->second + 1;
	it->second = size;

	map<string, int>::iterator it1 = black_ip_map.find(data.ip);
	if(it1 == black_ip_map.end()) {
		black_ip_map.insert(make_pair(data.ip, 1));
	}else {
		int size = it1->second + 1;
		it1->second = size;
	}
	it1 = black_ip_map.find("sum");
	size = it1->second + 1;
	it1->second = size;

	map<string, int>::iterator it2 = black_city_map.find(data.city);
	if(it2 == black_city_map.end()) {
		black_city_map.insert(make_pair(data.city, 1));
	}else {
		int size = it2->second + 1;
		it2->second = size;
	}
	it2 = black_city_map.find("sum");
	size = it2->second + 1;
	it2->second = size;

	map<string, int>::iterator it3 = black_type_map.find(data.type);
	if(it3 == black_type_map.end()) {
		black_type_map.insert(make_pair(data.type, 1));
	}else {
		int size = it3->second + 1;
		it3->second = size;
	}
	it3 = black_type_map.find("sum");
	size = it3->second + 1;
	it3->second = size;

	map<string, int>::iterator it4 = black_log_from_map.find(data.log_from);
	if(it4 == black_log_from_map.end()) {
		black_log_from_map.insert(make_pair(data.log_from, 1));
	}else {
		int size = it4->second + 1;
		it4->second = size;
	}
	it4 = black_log_from_map.find("sum");
	size = it4->second + 1;
	it4->second = size;
}

void get_max_min_avg(stringstream& ss, int feature_index, vector<double> data) {
	double max = 0;
	double min = 17280000.0;
	double avg = 0.0;
	int cnt = 0;

	for(int i = 0; i < data.size(); i++) {
		if(data[i] < 10)
			continue;
		if(max < data[i])
			max = data[i];
		if(min > data[i])
			min = data[i];
		avg += data[i];
		cnt++;
	}
	if(cnt)
		avg = avg/cnt;

	ss << ++feature_index << ":" << max << " ";
	ss << ++feature_index << ":" << min << " ";
	ss << ++feature_index << ":" << avg << " ";
}

string update_current_info(int feature_index, vector<login_data> login_list, int index, int i, int trade_size) {
	stringstream ss;
	login_data login = login_list[index];
	trade_data current = login.trade_vec[trade_size - 1];

	int label = current.label;
	long time_stamp = current.time_stamp;
	feature_index += 10;

	ss << ++feature_index << ":" << (time_stamp/3600 + 8)%24 << " ";
	ss << ++feature_index << ":" << ((time_stamp/3600 + 8)/24 + 4)%7 << " ";
	ss << ++feature_index << ":" << trade_size - 1 << " ";
	ss << ++feature_index << ":" << login.time_long << " ";
	ss << ++feature_index << ":" << time_stamp - login.time_stamp << " ";

	ss << ++feature_index << ":" << (3600 > (time_stamp - login.time_stamp)) << " ";
	//ss << ++feature_index << ":" << (login.time_long - (time_stamp - login.time_stamp)) << " ";
	ss << ++feature_index << ":" << i - index << " ";
	ss << ++feature_index << ":" << (login.time_stamp/3600 + 8)%24 << " ";
	ss << ++feature_index << ":" << ((login.time_stamp/3600 + 8)/24 + 4)%7 << " ";

	return ss.str();
}

string update_global_info(int feature_index, string user_id, vector<login_data> login_list, int index, int trade_size) {

	stringstream ss;
	login_data login = login_list[index];
	trade_data current = login.trade_vec[trade_size - 1];

	if(device_user_map.find(login.device) != device_user_map.end()) {
		map<string, int> tmp = device_user_map.at(login.device);
		if(tmp.find(user_id) != tmp.end() && tmp.find("sum") != tmp.end()) {
			ss << ++feature_index << ":" << tmp.at("sum") << " ";
			ss << ++feature_index << ":" << tmp.at(user_id) << " ";
			//ss << ++feature_index << ":" << tmp.size() << " ";
			ss << ++feature_index << ":" << tmp.at(user_id)*1.0/tmp.at("sum") << " ";
		}else {
			feature_index += 3;
		}
	}else{
		feature_index += 3;
	}
	if(ip_user_map.find(login.ip) != ip_user_map.end()) {
		map<string, int> tmp = ip_user_map.at(login.ip);
		if(tmp.find(user_id) != tmp.end() && tmp.find("sum") != tmp.end()) {
			ss << ++feature_index << ":" << tmp.at("sum") << " ";
			ss << ++feature_index << ":" << tmp.at(user_id) << " ";
			//ss << ++feature_index << ":" << tmp.size() << " ";
			ss << ++feature_index << ":" << tmp.at(user_id)*1.0/tmp.at("sum") << " ";
		}else {
			feature_index += 3;
		}
	}else{
		feature_index += 3;
	}

	/*if(test_user.length() > 0)
		cout << "debug " << black_total_device_map.size() << " " << total_device_map.size() << " " << trade_total_device_map.size() << endl;

	if(index > 0) {
		int device_status = 0;
		int ip_status = 0;
		for(int id = 0; id < index; id++) {
			if(current.time_stamp - login_list[id].time_stamp > FIVE_DAY) {
				continue;
			}
			if(black_total_device_map.find(login_list[id].device) != black_total_device_map.end()) {
				device_status = 1;
			}
			if(black_total_ip_map.find(login_list[id].ip) != black_total_ip_map.end()) {
				ip_status = 1;
			}
		}
		ss << ++feature_index << ":" << device_status << " ";
		ss << ++feature_index << ":" << ip_status << " ";
	} else
		feature_index += 2;*/

	/*int black_size = 0;
	if(black_total_device_map.find(login.device) != black_total_device_map.end()) {
		ss << ++feature_index << ":" << black_total_device_map.size() << " ";
		ss << ++feature_index << ":" << black_total_device_map.at(login.device).at("sum") << " ";
	}else {
		feature_index += 2;
	}

	if(black_total_ip_map.find(login.ip) != black_total_ip_map.end()) {
		ss << ++feature_index << ":" << black_total_ip_map.size() << " ";
		ss << ++feature_index << ":" << black_total_ip_map.at(login.ip).at("sum") << " ";
	}else {
		feature_index += 2;
	}

	if(gray_total_device_map.find(login.device) != gray_total_device_map.end()) {
		ss << ++feature_index << ":" << gray_total_device_map.size() << " ";
		ss << ++feature_index << ":" << gray_total_device_map.at(login.device).at("sum") << " ";
	}else {
		feature_index += 2;
	}

	if(gray_total_ip_map.find(login.ip) != gray_total_ip_map.end()) {
		ss << ++feature_index << ":" << gray_total_ip_map.size() << " ";
		ss << ++feature_index << ":" << gray_total_ip_map.at(login.ip).at("sum") << " ";
	}else {
		feature_index += 2;
	}

	if(valid_total_device_map.find(login.device) != valid_total_device_map.end()) {
		ss << ++feature_index << ":" << valid_total_device_map.size() << " ";
		ss << ++feature_index << ":" << valid_total_device_map.at(login.device).at("sum") << " ";
	}else {
		feature_index += 2;
	}

	if(valid_total_ip_map.find(login.ip) != valid_total_ip_map.end()) {
		ss << ++feature_index << ":" << valid_total_ip_map.size() << " ";
		ss << ++feature_index << ":" << valid_total_ip_map.at(login.ip).at("sum") << " ";
	}else {
		feature_index += 2;
	}

	if(total_device_map.find(login.device) != total_device_map.end()) {
		map<string, int> tmp = total_device_map.at(login.device);
		if(tmp.find(user_id) != tmp.end() && tmp.find("sum") != tmp.end()) {
			//ss << ++feature_index << ":" << (tmp.size() > 2) << " ";
			ss << ++feature_index << ":" << tmp.size() << " ";
			ss << ++feature_index << ":" << tmp.at(user_id)*1.0/tmp.at("sum") << " ";
		}
	}else{
		feature_index += 2;
	}
	if(total_ip_map.find(login.ip) != total_ip_map.end()) {
		map<string, int> tmp = total_ip_map.at(login.ip);
		if(tmp.find(user_id) != tmp.end() && tmp.find("sum") != tmp.end()) {
			//ss << ++feature_index << ":" << (tmp.size() > 2) << " ";
			ss << ++feature_index << ":" << tmp.at(user_id) << " ";
			//ss << ++feature_index << ":" << tmp.size() << " ";
			ss << ++feature_index << ":" << tmp.at(user_id)*1.0/tmp.at("sum") << " ";
		}
	}else{
		feature_index += 2;
	}

	if(trade_total_device_map.find(login.device) != trade_total_device_map.end()) {
		map<string, int> tmp = trade_total_device_map.at(login.device);
		if(tmp.find(user_id) != tmp.end() && tmp.find("sum") != tmp.end()) {
			ss << ++feature_index << ":" << tmp.at(user_id) << " ";
			ss << ++feature_index << ":" << tmp.at(user_id)*1.0/tmp.at("sum") << " ";
		}
	}else{
		feature_index += 2;
	}
	if(trade_total_ip_map.find(login.ip) != trade_total_ip_map.end()) {
		map<string, int> tmp = trade_total_ip_map.at(login.ip);
		if(tmp.find(user_id) != tmp.end() && tmp.find("sum") != tmp.end()) {
			ss << ++feature_index << ":" << tmp.at(user_id) << " ";
			ss << ++feature_index << ":" << tmp.at(user_id)*1.0/tmp.at("sum") << " ";
		}
	}else{
		feature_index += 2;
	}*/

	return ss.str();
}

void calc_user_history(vector<login_data> login_list, int index, long time_stamp, login_data login, trade_data current, int trade_size) {
	clear_history_map();
	int login_index = 0;
	for(; login_index < index; login_index++) {
		if(current.time_stamp - login_list[login_index].time_stamp > time_stamp) {
			continue;			
		}
		update_login_set(login_list[login_index]);
		if(login_list[login_index].result == "1") {
			update_history_login(login_list[login_index]);
			for(int j = 0; j < login_list[login_index].trade_vec.size(); j++) {
				if(login_list[login_index].trade_vec[j].label == 0 
					&& (current.time_stamp - login_list[login_index].trade_vec[j].time_stamp) > ONE_MONTH) {
					update_history_valid(login_list[login_index]);
				}else if(login_list[login_index].trade_vec[j].label == 1 
					&& (current.time_stamp - login_list[login_index].trade_vec[j].time_stamp) > ONE_MONTH) {
					update_history_black(login_list[login_index]);
				}
				update_history_trade(login_list[login_index]);
			}
		}
	}
	update_history_login(login_list[index]);
	for(int j = 0; j < trade_size; j++) {
		update_history_trade(login_list[login_index]);
	}
	//cout << " " << endl; 
}

string update_user_history_info(int feature_index, vector<login_data> login_list, int index, long time_threshold, int trade_size) {
	stringstream ss;
	login_data login = login_list[index];
	trade_data current = login.trade_vec[trade_size - 1];

	int label = current.label;
	long time_stamp = current.time_stamp;

	calc_user_history(login_list, index, time_threshold, login, current, trade_size);

	ss << ++feature_index << ":" << device_map.at("sum") << " ";
	if(device_map.find(login.device) != device_map.end()) {
		//ss << ++feature_index << ":" << device_map.at("sum") << " ";
		ss << ++feature_index << ":" << device_map.at(login.device) << " ";
		//ss << ++feature_index << ":" << device_map.size() << " ";
		ss << ++feature_index << ":" << device_map.at(login.device)*1.0/(device_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	if(ip_map.find(login.ip) != ip_map.end()) {
		//ss << ++feature_index << ":" << ip_map.at("sum") << " ";
		ss << ++feature_index << ":" << ip_map.at(login.ip) << " ";
		//ss << ++feature_index << ":" << ip_map.size() << " ";
		ss << ++feature_index << ":" << ip_map.at(login.ip)*1.0/(ip_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	if(city_map.find(login.city) != city_map.end()) {
		//ss << ++feature_index << ":" << city_map.at("sum") << " ";
		ss << ++feature_index << ":" << city_map.at(login.city) << " ";
		//ss << ++feature_index << ":" << city_map.size() << " ";
		ss << ++feature_index << ":" << city_map.at(login.city)*1.0/(city_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	if(type_map.find(login.type) != type_map.end()) {
		//ss << ++feature_index << ":" << type_map.at("sum") << " ";
		ss << ++feature_index << ":" << type_map.at(login.type) << " ";
		//ss << ++feature_index << ":" << type_map.size() << " ";
		ss << ++feature_index << ":" << type_map.at(login.type)*1.0/(type_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	if(log_from_map.find(login.log_from) != log_from_map.end()) {
		//ss << ++feature_index << ":" << log_from_map.at("sum") << " ";
		ss << ++feature_index << ":" << log_from_map.at(login.log_from) << " ";
		//ss << ++feature_index << ":" << log_from_map.size() << " ";
		ss << ++feature_index << ":" << log_from_map.at(login.log_from)*1.0/(log_from_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
	//10

	ss << ++feature_index << ":" << trade_device_map.at("sum") << " ";
	if(trade_device_map.find(login.device) != trade_device_map.end()) {
		//ss << ++feature_index << ":" << trade_device_map.at("sum") << " ";
		ss << ++feature_index << ":" << trade_device_map.at(login.device) << " ";
		//ss << ++feature_index << ":" << trade_device_map.size() << " ";
		ss << ++feature_index << ":" << trade_device_map.at(login.device)*1.0/(trade_device_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	if(trade_ip_map.find(login.ip) != trade_ip_map.end()) {
		//ss << ++feature_index << ":" << trade_ip_map.at("sum") << " ";
		ss << ++feature_index << ":" << trade_ip_map.at(login.ip) << " ";
		//ss << ++feature_index << ":" << trade_ip_map.size() << " ";
		ss << ++feature_index << ":" << trade_ip_map.at(login.ip)*1.0/(trade_ip_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	if(trade_city_map.find(login.city) != trade_city_map.end()) {
		//ss << ++feature_index << ":" << trade_city_map.at("sum") << " ";
		ss << ++feature_index << ":" << trade_city_map.at(login.city) << " ";
		//ss << ++feature_index << ":" << trade_city_map.size() << " ";
		ss << ++feature_index << ":" << trade_city_map.at(login.city)*1.0/(trade_city_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	if(trade_type_map.find(login.type) != trade_type_map.end()) {
		//ss << ++feature_index << ":" << trade_type_map.at("sum") << " ";
		ss << ++feature_index << ":" << trade_type_map.at(login.type) << " ";
		//ss << ++feature_index << ":" << trade_type_map.size() << " ";
		ss << ++feature_index << ":" << trade_type_map.at(login.type)*1.0/(trade_type_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	if(trade_log_from_map.find(login.log_from) != trade_log_from_map.end()) {
		//ss << ++feature_index << ":" << trade_log_from_map.at("sum") << " ";
		ss << ++feature_index << ":" << trade_log_from_map.at(login.log_from) << " ";
		//ss << ++feature_index << ":" << trade_log_from_map.size() << " ";
		ss << ++feature_index << ":" << trade_log_from_map.at(login.log_from)*1.0/(trade_log_from_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
	//10

	ss << ++feature_index << ":" << valid_device_map.at("sum") << " ";
	if(valid_device_map.find(login.device) != valid_device_map.end()) {
		//ss << ++feature_index << ":" << valid_device_map.at("sum") << " ";
		ss << ++feature_index << ":" << valid_device_map.at(login.device) << " ";
		//ss << ++feature_index << ":" << valid_device_map.size() << " ";
		ss << ++feature_index << ":" << valid_device_map.at(login.device)*1.0/(valid_device_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	if(valid_ip_map.find(login.ip) != valid_ip_map.end()) {
		//ss << ++feature_index << ":" << valid_ip_map.at("sum") << " ";
		ss << ++feature_index << ":" << valid_ip_map.at(login.ip) << " ";
		//ss << ++feature_index << ":" << valid_ip_map.size() << " ";
		ss << ++feature_index << ":" << valid_ip_map.at(login.ip)*1.0/(valid_ip_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	if(valid_city_map.find(login.city) != valid_city_map.end()) {
		//ss << ++feature_index << ":" << valid_city_map.at("sum") << " ";
		ss << ++feature_index << ":" << valid_city_map.at(login.city) << " ";
		//ss << ++feature_index << ":" << valid_city_map.size() << " ";
		ss << ++feature_index << ":" << valid_city_map.at(login.city)*1.0/(valid_city_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	if(valid_type_map.find(login.type) != valid_type_map.end()) {
		//ss << ++feature_index << ":" << valid_type_map.at("sum") << " ";
		ss << ++feature_index << ":" << valid_type_map.at(login.type) << " ";
		//ss << ++feature_index << ":" << valid_type_map.size() << " ";
		ss << ++feature_index << ":" << valid_type_map.at(login.type)*1.0/(valid_type_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	if(valid_log_from_map.find(login.log_from) != valid_log_from_map.end()) {
		//ss << ++feature_index << ":" << valid_log_from_map.at("sum") << " ";
		ss << ++feature_index << ":" << valid_log_from_map.at(login.log_from) << " ";
		//ss << ++feature_index << ":" << valid_log_from_map.size() << " ";
		ss << ++feature_index << ":" << valid_log_from_map.at(login.log_from)*1.0/(valid_log_from_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
	//10

	ss << ++feature_index << ":" << black_device_map.at("sum") << " ";
	if(black_device_map.find(login.device) != black_device_map.end()) {
		//ss << ++feature_index << ":" << black_device_map.at("sum") << " ";
		ss << ++feature_index << ":" << black_device_map.at(login.device) << " ";
		//ss << ++feature_index << ":" << black_device_map.size() << " ";
		ss << ++feature_index << ":" << black_device_map.at(login.device)*1.0/(black_device_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	if(black_ip_map.find(login.ip) != black_ip_map.end()) {
		//ss << ++feature_index << ":" << black_ip_map.at("sum") << " ";
		ss << ++feature_index << ":" << black_ip_map.at(login.ip) << " ";
		//ss << ++feature_index << ":" << black_ip_map.size() << " ";
		ss << ++feature_index << ":" << black_ip_map.at(login.ip)*1.0/(black_ip_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	if(black_city_map.find(login.city) != black_city_map.end()) {
		//ss << ++feature_index << ":" << black_city_map.at("sum") << " ";
		ss << ++feature_index << ":" << black_city_map.at(login.city) << " ";
		//ss << ++feature_index << ":" << black_city_map.size() << " ";
		ss << ++feature_index << ":" << black_city_map.at(login.city)*1.0/(black_city_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	if(black_type_map.find(login.type) != black_type_map.end()) {
		//ss << ++feature_index << ":" << black_type_map.at("sum") << " ";
		ss << ++feature_index << ":" << black_type_map.at(login.type) << " ";
		//ss << ++feature_index << ":" << black_type_map.size() << " ";
		ss << ++feature_index << ":" << black_type_map.at(login.type)*1.0/(black_type_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	if(black_log_from_map.find(login.log_from) != black_log_from_map.end()) {
		//ss << ++feature_index << ":" << black_log_from_map.at("sum") << " ";
		ss << ++feature_index << ":" << black_log_from_map.at(login.log_from) << " ";
		//ss << ++feature_index << ":" << black_log_from_map.size() << " ";
		ss << ++feature_index << ":" << black_log_from_map.at(login.log_from)*1.0/(black_log_from_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}

	ss << ++feature_index << ":" << device_set.count(login.device) << " ";
	ss << ++feature_index << ":" << ip_set.count(login.ip) << " ";
	ss << ++feature_index << ":" << city_set.count(login.city) << " ";
	ss << ++feature_index << ":" << type_set.count(login.type) << " ";
	ss << ++feature_index << ":" << log_from_set.count(login.log_from) << " ";

	ss << ++feature_index << ":" << device_set.size() << " ";
	ss << ++feature_index << ":" << ip_set.size() << " ";
	ss << ++feature_index << ":" << city_set.size() << " ";
	ss << ++feature_index << ":" << type_set.size() << " ";
	ss << ++feature_index << ":" << log_from_set.size() << " ";

	return ss.str();
}

string update_last_login_info(int feature_index, vector<login_data> login_list, int index, int trade_size) {
	stringstream ss;
	login_data login = login_list[index];
	trade_data current = login.trade_vec[trade_size - 1];

	int label = current.label;
	long time_stamp = current.time_stamp;

	int before_index = index -1;
	if(index>0) {
		before_index = index -1;
		if(login_list[before_index].device == login.device) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;
		if(login_list[before_index].log_from == login.log_from) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;
		if(login_list[before_index].ip == login.ip) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;
		if(login_list[before_index].city == login.city) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;

		if(login_list[before_index].result == login.result) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;
		if(login_list[before_index].type == login.type) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;
		ss << ++feature_index << ":" << log(time_stamp - login_list[before_index].time_stamp  + 1) << " ";
		ss << ++feature_index << ":" << log(login.time_stamp - login_list[before_index].time_stamp + 1)  << " ";
		ss << ++feature_index << ":" << login.time_long - login_list[before_index].time_long << " ";
		ss << ++feature_index << ":" << login_list[before_index].time_long << " ";
		ss << ++feature_index << ":" << (login_list[before_index].time_long - login.time_long) << " ";
		ss << ++feature_index << ":" << log(max_time_long + login.time_long - login_list[before_index].time_long + 1) << " ";

	}else {
		feature_index += 12;
	}

	return ss.str();
}

string update_last_last_login_info(int feature_index, vector<login_data> login_list, int index, int trade_size) {
	stringstream ss;
	login_data login = login_list[index];
	trade_data current = login.trade_vec[trade_size - 1];

	int label = current.label;
	long time_stamp = current.time_stamp;

	int before_index = index - 1;
	if(before_index > 1) {
		if(login_list[before_index - 1].device == login.device) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;

		if(login_list[before_index - 1].log_from == login.log_from) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;
		if(login_list[before_index - 1].ip == login.ip) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;
		if(login_list[before_index - 1].city == login.city) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;


		if(login_list[before_index - 1].result == login.result) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;

		if(login_list[before_index - 1].type == login.type) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;

		ss << ++feature_index << ":" << time_stamp - login_list[before_index - 1].time_stamp << " ";

		ss << ++feature_index << ":" << login.time_stamp - login_list[before_index - 1].time_stamp << " ";
		ss << ++feature_index << ":" << login_list[before_index - 1].time_long << " ";
		ss << ++feature_index << ":" << (login_list[before_index - 1].time_long - login.time_long) << " ";
		ss << ++feature_index << ":" << log(max_time_long + login.time_long - login_list[before_index - 1].time_long + 1) << " ";
		ss << ++feature_index << ":" << login.time_long - login_list[before_index - 1].time_long << " ";

		if(login_list[before_index - 1].device == login_list[before_index].device) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;

		if(login_list[before_index - 1].log_from == login_list[before_index].log_from) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;
		if(login_list[before_index - 1].ip == login_list[before_index].ip) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;
		if(login_list[before_index - 1].city == login_list[before_index].city) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;


		if(login_list[before_index - 1].result == login_list[before_index].result) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;

		if(login_list[before_index - 1].type == login_list[before_index].type) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;

		ss << ++feature_index << ":" << login_list[before_index].time_stamp - login_list[before_index - 1].time_stamp << " ";
		ss << ++feature_index << ":" << log(max_time_long + login_list[before_index].time_long - login_list[before_index - 1].time_long + 1) << " ";
		ss << ++feature_index << ":" << login_list[before_index].time_long - login_list[before_index - 1].time_long << " ";
		ss << ++feature_index << ":" << login_list[before_index - 1].time_long - login_list[before_index].time_long << " ";
	}else {
		feature_index += 22;
	}

	return ss.str();
}

string update_user_history_login_info1(int feature_index, vector<login_data> login_list, int index, long time_threshold, int trade_size) {
	stringstream ss;
	login_data login = login_list[index];
	trade_data current = login.trade_vec[trade_size - 1];

	int label = current.label;
	long time_stamp = current.time_stamp;

	if(index > 0) {
		map<string, int> log_device_info;
		map<string, int> log_ip_info;
		map<string, int> log_city_info;
		map<string, int> trade_device_info;
		map<string, int> trade_ip_info;
		map<string, int> trade_city_info;
		vector<double> time_diff;
		int login_cnt = 0, trade_cnt = 0;
		long device_time_diff = 17280000;
		long ip_time_diff = 17280000;
		long city_time_diff = 17280000;
		int sum_login = 0;
		int sum_trade = 0;
		for(int id = 0; id <= index; id++) {
			if(((login_list[index].time_stamp - login_list[id].time_stamp) < time_threshold 
				&& (login_list[index].time_stamp - login_list[id].time_stamp) > 10) || id == index) {
				sum_login++;
				if(log_device_info.find(login_list[id].device) == log_device_info.end()) {
					int size = 1;
					log_device_info.insert(make_pair(login_list[id].device, size));
				}else {
					map<string, int>::iterator map_it = log_device_info.find(login_list[id].device);
					int size = map_it->second;
					size = size + 1;
					map_it->second = size;
				}
				if(log_ip_info.find(login_list[id].ip) == log_ip_info.end()) {
					int size = 1;
					log_ip_info.insert(make_pair(login_list[id].ip, size));
				}else {
					map<string, int>::iterator map_it = log_ip_info.find(login_list[id].ip);
					int size = map_it->second;
					size = size + 1;
					map_it->second = size;
				}
				if(log_city_info.find(login_list[id].city) == log_city_info.end()) {
					int size = 1;
					log_city_info.insert(make_pair(login_list[id].city, size));
				}else {
					map<string, int>::iterator map_it = log_city_info.find(login_list[id].city);
					int size = map_it->second;
					size = size + 1;
					map_it->second = size;
				}
				if(login_list[id].result == "1")
					login_cnt++;
				if(login_list[id].trade_vec.size() > 0) {
					sum_trade += 1;
					trade_cnt += login_list[id].trade_vec.size();
					if(trade_device_info.find(login_list[id].device) == trade_device_info.end()) {
						int size = 1;
						trade_device_info.insert(make_pair(login_list[id].device, size));
					}else {
						map<string, int>::iterator map_it = trade_device_info.find(login_list[id].device);
						int size = map_it->second;
						size = size + 1;
						map_it->second = size;
					}
					if(trade_ip_info.find(login_list[id].ip) == trade_ip_info.end()) {
						int size = 1;
						trade_ip_info.insert(make_pair(login_list[id].ip, size));
					}else {
						map<string, int>::iterator map_it = trade_ip_info.find(login_list[id].ip);
						int size = map_it->second;
						size = size + 1;
						map_it->second = size;
					}
					if(trade_city_info.find(login_list[id].city) == trade_city_info.end()) {
						int size = 1;
						trade_city_info.insert(make_pair(login_list[id].city, size));
					}else {
						map<string, int>::iterator map_it = trade_city_info.find(login_list[id].city);
						int size = map_it->second;
						size = size + 1;
						map_it->second = size;
					}
				}

				if(id < index) 
					time_diff.push_back(login_list[id + 1].time_stamp - login_list[id].time_stamp);

				if((login_list[index].time_stamp - login_list[id].time_stamp) > 10 
					&& login_list[index].device == login_list[id].device) {
					device_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				}
				if((login_list[index].time_stamp - login_list[id].time_stamp) > 10 
					&& login_list[index].ip == login_list[id].ip) {
					ip_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				}
				if((login_list[index].time_stamp - login_list[id].time_stamp) > 10 
					&& login_list[index].city == login_list[id].city) {
					city_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				}
			}
		}
		ss << ++feature_index << ":" << log_device_info.size() << " ";
		if(log_device_info.size() > 0) {
			ss << ++feature_index << ":" << log_device_info.at(login_list[index].device)*1.0/sum_login << " ";
		}
		else
			++feature_index;
		ss << ++feature_index << ":" << log_ip_info.size() << " ";
		if(log_ip_info.size() > 0)
			ss << ++feature_index << ":" << log_ip_info.at(login_list[index].ip)*1.0/sum_login << " ";
		else
			++feature_index;
		ss << ++feature_index << ":" << log_city_info.size() << " ";
		if(log_city_info.size() > 0)
			ss << ++feature_index << ":" << log_city_info.at(login_list[index].city)*1.0/sum_login << " ";
		else
			++feature_index;

		ss << ++feature_index << ":" << sum_login << " ";
		ss << ++feature_index << ":" << sum_trade << " ";
		ss << ++feature_index << ":" << login_cnt << " ";
		ss << ++feature_index << ":" << trade_cnt << " ";

		ss << ++feature_index << ":" << log(device_time_diff + 1) << " ";
		ss << ++feature_index << ":" << log(ip_time_diff + 1) << " ";
		ss << ++feature_index << ":" << log(city_time_diff + 1) << " ";

		if(time_diff.size() > 0)
			get_max_min_avg(ss, feature_index, time_diff);
		else
			feature_index += 3;

		int before_index = index - 1;
		if(login_list[before_index].device != login.device 
			&& login_list[before_index].ip != login.ip
			&& login_list[before_index].city != login.city) {
			ss << ++feature_index << ":1 ";
		}

		/*int login_cnt_tmp = 0;
		for(int idx = index - 1; idx > 0; idx--) {
			if(((login_list[index].time_stamp - login_list[id].time_stamp) > 3600*24))
				continue;
			if(login_list[before_index].device == login.device 
				&& login_list[before_index].ip == login.ip
				&& login_list[before_index].city == login.city
				&& login_list[before_index].result == "1"
				&& login.result == "1")
				login_cnt_tmp++;
			else
				break;
		}
		ss << ++feature_index << ":" << login_cnt_tmp << " ";*/
	}

	return ss.str();
}

string update_user_history_login_info2(int feature_index, vector<login_data> login_list, int index, long time_threshold, int trade_size) {
	stringstream ss;
	login_data login = login_list[index];
	trade_data current = login.trade_vec[trade_size - 1];

	int label = current.label;
	long time_stamp = current.time_stamp;

	if(index > 0) {
		long max_time_diff = 0;
		long min_time_diff = 0;
		int con_login_cnt = 0;
		for(int id = index-1; id >= 0; id--) {
			//if(index - id > 5) {
			//if(current.time_stamp - login_list[id].time_stamp > time_threshold) {
			//	continue;
			//}
			if(login_list[index].device == login_list[id].device
				&& login_list[index].ip == login_list[id].ip
				&& login_list[index].city == login_list[id].city
				&& login_list[index].result == login_list[id].result) {
				if(max_time_diff <= 0)
					max_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				min_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				con_login_cnt++;
			}else {
				break;
			}
		}
		ss << ++feature_index << ":" << log(min_time_diff + 1) << " ";
		ss << ++feature_index << ":" << log(max_time_diff + 1) << " ";
		ss << ++feature_index << ":" << con_login_cnt << " ";

		/*max_time_diff = 0;
		min_time_diff = 0;
		con_login_cnt = 0;
		for(int id = index-1; id >= 0; id--) {
			if(index - id > 5) {
			//if(current.time_stamp - login_list[id].time_stamp > time_threshold) {
				continue;
			}
			if(login_list[index].device == login_list[id].device
				&& login_list[index].ip == login_list[id].ip
				&& login_list[index].city == login_list[id].city
				&& login_list[index].result != login_list[id].result) {
				if(max_time_diff <= 0)
					max_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				min_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				con_login_cnt++;
			}else {
				break;
			}
		}
		ss << ++feature_index << ":" << log(min_time_diff + 1) << " ";
		ss << ++feature_index << ":" << log(max_time_diff + 1) << " ";
		ss << ++feature_index << ":" << con_login_cnt << " ";*/

		max_time_diff = 0;
		min_time_diff = 0;
		con_login_cnt = 0;
		for(int id = index-1; id >= 0; id--) {
			if(index - id > 5) {
			//if(current.time_stamp - login_list[id].time_stamp > time_threshold) {
				continue;
			}
			if(login_list[index].device != login_list[id].device
				&& login_list[index].ip == login_list[id].ip
				&& login_list[index].city == login_list[id].city
				&& login_list[index].result == login_list[id].result) {
				if(max_time_diff <= 0)
					max_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				min_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				con_login_cnt++;
			}else {
				break;
			}
		}
		ss << ++feature_index << ":" << log(min_time_diff + 1) << " ";
		ss << ++feature_index << ":" << log(max_time_diff + 1) << " ";
		ss << ++feature_index << ":" << con_login_cnt << " ";

		/*max_time_diff = 0;
		min_time_diff = 0;
		con_login_cnt = 0;
		for(int id = index-1; id >= 0; id--) {
			//if(index - id > 5) {
			if(current.time_stamp - login_list[id].time_stamp > time_threshold) {
				continue;
			}
			if(login_list[index].device != login_list[id].device
				&& login_list[index].ip == login_list[id].ip
				&& login_list[index].city == login_list[id].city
				&& login_list[index].result == login_list[id].result) {
				if(max_time_diff <= 0)
					max_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				min_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				con_login_cnt++;
			}else {
				break;
			}
		}
		ss << ++feature_index << ":" << log(min_time_diff + 1) << " ";
		ss << ++feature_index << ":" << log(max_time_diff + 1) << " ";
		ss << ++feature_index << ":" << con_login_cnt << " ";*/

		/*max_time_diff = 0;
		min_time_diff = 0;
		con_login_cnt = 0;
		for(int id = index-1; id >= 0; id--) {
			if(index - id > 5) {
			//if(current.time_stamp - login_list[id].time_stamp > time_threshold) {
				continue;
			}
			if(login_list[index].device == login_list[id].device
				&& login_list[index].ip != login_list[id].ip
				&& login_list[index].city == login_list[id].city
				&& login_list[index].result == login_list[id].result) {
				if(max_time_diff <= 0)
					max_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				min_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				con_login_cnt++;
			}else {
				break;
			}
		}
		ss << ++feature_index << ":" << log(min_time_diff + 1) << " ";
		ss << ++feature_index << ":" << log(max_time_diff + 1) << " ";
		ss << ++feature_index << ":" << con_login_cnt << " ";*/

		max_time_diff = 0;
		min_time_diff = 0;
		con_login_cnt = 0;
		for(int id = index-1; id >= 0; id--) {
			if(index - id > 5) {
			//if(current.time_stamp - login_list[id].time_stamp > time_threshold) {
				continue;
			}
			if(login_list[index].device != login_list[id].device
				&& login_list[index].ip != login_list[id].ip) {
				if(max_time_diff <= 0)
					max_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				min_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				con_login_cnt++;
			}else {
				break;
			}
		}
		ss << ++feature_index << ":" << log(min_time_diff + 1) << " ";
		ss << ++feature_index << ":" << log(max_time_diff + 1) << " ";
		ss << ++feature_index << ":" << con_login_cnt << " ";

		/*max_time_diff = 0;
		min_time_diff = 0;
		con_login_cnt = 0;
		for(int id = index-1; id >= 0; id--) {
			//if(index - id > 5) {
			if(current.time_stamp - login_list[id].time_stamp > time_threshold) {
				continue;
			}
			if(login_list[index].device != login_list[id].device
				&& login_list[index].ip != login_list[id].ip) {
				if(max_time_diff <= 0)
					max_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				min_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				con_login_cnt++;
			}else {
				break;
			}
		}
		ss << ++feature_index << ":" << log(min_time_diff + 1) << " ";
		ss << ++feature_index << ":" << log(max_time_diff + 1) << " ";
		ss << ++feature_index << ":" << con_login_cnt << " ";*/

		set<string> history_device_set;
		set<string> history_ip_set;
		long device_time_diff = 0;
		long ip_time_diff = 0;
		int time_long_cnt = 0;
		for(int id = 0; id < index; id++) {
			if(index - id > 5) {
			//if(current.time_stamp - login_list[id].time_stamp > time_threshold) {
				continue;
			}
			if(login_list[id].city == login_list[index].city
				&& login_list[id].device != login_list[index].device) {
				history_device_set.insert(login_list[id].device);
				device_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
			}
			if(login_list[id].city == login_list[index].city
				&& login_list[id].ip != login_list[index].ip) {
				history_ip_set.insert(login_list[id].ip);
				ip_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
			}
			if(login_list[id].time_long > (time_stamp - login_list[id].time_stamp)) {
				time_long_cnt++;
			}
		}
		ss << ++feature_index << ":" << time_long_cnt << " ";
		ss << ++feature_index << ":" << history_device_set.size() << " ";
		ss << ++feature_index << ":" << device_time_diff << " ";
		ss << ++feature_index << ":" << history_ip_set.size() << " ";
		ss << ++feature_index << ":" << ip_time_diff << " ";
	}

	return ss.str();
}

string update_user_history_trade_info(int feature_index, vector<login_data> login_list, int index, long time_threshold, int trade_size) {
	stringstream ss;
	if(index == 0) {
		return "";
	}
	long max_diff = 0;
	long min_diff = 17280000;
	double avg_diff = 0.0;
	int cnt = 0;
	int time_diff = 0;
	int device_status = 0;
	int ip_status = 0;
	for(int id = 0; id < index; id++) {
		if(login_list[index].time_stamp - login_list[id].time_stamp > time_threshold)
			continue;
		login_data login = login_list[id];
		int trade_size = login.trade_vec.size();
		if(trade_size > 0) {
			time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
			if(login_list[index].device == login_list[id].device)
				device_status = 2;
			else
				device_status = 1;
			if(login_list[index].ip == login_list[id].ip)
				ip_status = 2;
			else
				ip_status = 1;
		}
		if(trade_size > 1) {
			for(int i = 0; i < trade_size - 1; i++) {
				long diff = login.trade_vec[i + 1].time_stamp - login.trade_vec[i].time_stamp;
				if(diff > max_diff) {
					max_diff = diff;
				}
				if(diff < min_diff) {
					min_diff = diff;
				}
				avg_diff += diff;
				cnt++;
			}
		}
	}
	if(cnt == 0)
		return "";
	avg_diff = avg_diff/cnt;

	ss << ++feature_index << ":" << time_diff << " ";
	ss << ++feature_index << ":" << device_status << " ";
	ss << ++feature_index << ":" << ip_status << " ";
	ss << ++feature_index << ":" << max_diff << " ";
	ss << ++feature_index << ":" << log(min_diff + 1) << " ";
	ss << ++feature_index << ":" << avg_diff << " ";

	return ss.str();
}

string update_multi_trade_info(int feature_index, vector<login_data> login_list, int index, int trade_size) {
	stringstream ss;
	login_data login = login_list[index];
	trade_data current = login.trade_vec[trade_size - 1];

	int label = current.label;
	long time_stamp = current.time_stamp;
	if(trade_size > 1) {
		ss << ++feature_index << ":" << 
			(login.trade_vec[trade_size - 1].time_stamp - login.trade_vec[trade_size - 2].time_stamp) << " ";
		ss << ++feature_index << ":" << 
			(login.trade_vec[trade_size - 1].time_stamp - login.trade_vec[0].time_stamp) << " ";

		long max_diff = 0;
		long min_diff = 17280000;
		double avg_diff = 0.0;
		for(int id = 0; id < trade_size - 1; id++) {
			long diff = login.trade_vec[id + 1].time_stamp - login.trade_vec[id].time_stamp;
			if(diff > max_diff) {
				max_diff = diff;
			}
			if(diff < min_diff) {
				min_diff = diff;
			}
			avg_diff += diff;
		}
		avg_diff = avg_diff/(trade_size - 1);
		ss << ++feature_index << ":" << max_diff << " ";
		ss << ++feature_index << ":" << log(min_diff + 1) << " ";
		ss << ++feature_index << ":" << avg_diff << " ";
	}else {
		feature_index += 5;
	}
	if(trade_size > 2) {
		ss << ++feature_index << ":" << 
			(login.trade_vec[trade_size - 1].time_stamp - login.trade_vec[trade_size - 3].time_stamp) << " ";
	}else{
		feature_index += 1;
	}

	return ss.str();
}

string update_login_error_info(int feature_index, vector<login_data> login_list, int index, int i, int trade_size) {
	stringstream ss;
	login_data login = login_list[index];
	trade_data current = login.trade_vec[trade_size - 1];

	int label = current.label;
	long time_stamp = current.time_stamp;
	if(i != index) {
		/*if(login_list[i].device == login.device) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;

		if(login_list[i].log_from == login.log_from) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;
		if(login_list[i].ip == login.ip) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;
		if(login_list[i].city == login.city) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;

		if(login_list[i].result == login.result) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;

		if(login_list[i].type == login.type) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;*/

		ss << ++feature_index << ":" << time_stamp - login_list[i].time_stamp << " ";

		//ss << ++feature_index << ":" << login.time_stamp - login_list[i].time_stamp << " ";
		ss << ++feature_index << ":" << log(max_time_long + login.time_long - login_list[i].time_long + 1) << " ";
		//ss << ++feature_index << ":" << login.time_long - login_list[i].time_long << " ";
	}else{
		feature_index += 10;
	}

	return ss.str();
}

string update_disc_info(int feature_index, vector<login_data> login_list, int index, int i, int trade_size) {
	stringstream ss;
	login_data login = login_list[index];
	trade_data current = login.trade_vec[trade_size - 1];

	int label = current.label;
	long time_stamp = current.time_stamp;

	int result_size = 10;
	int type_size = 3;
	int log_from = 11;

	//11+3+(11+10+3)*2
	int log_from_index = (discrete_map.at("log_from")).at(login.log_from);
	ss << 1 + log_from_index + feature_index << ":1" << " ";
	feature_index += log_from;

	int type_index = (discrete_map.at("type")).at(login.type);
	ss << 1 + type_index + feature_index << ":1" << " ";
	feature_index += type_size;

	if(index > 0) {
		log_from_index = (discrete_map.at("log_from")).at(login_list[index - 1].log_from);
		ss << 1 + log_from_index + feature_index << ":1" << " ";
		feature_index += log_from;

		int result_index = (discrete_map.at("result")).at(login_list[index - 1].result);
		ss << 1 + result_index + feature_index << ":1" << " ";
		feature_index += result_size;

		type_index = (discrete_map.at("type")).at(login_list[index - 1].type);
		ss << 1 + type_index + feature_index << ":1" << " ";
		feature_index += type_size;
	}else {
		feature_index += log_from + result_size + type_size;
	}

	/*if(i != index) {
		log_from_index = (discrete_map.at("log_from")).at(login_list[i].log_from);
		ss << 1 + log_from_index + feature_index << ":1" << " ";
		feature_index += log_from;

		int result_index = (discrete_map.at("result")).at(login_list[i].result);
		ss << 1 + result_index + feature_index << ":1" << " ";
		feature_index += result_size;

		type_index = (discrete_map.at("type")).at(login_list[i].type);
		ss << 1 + type_index + feature_index << ":1" << " ";
		feature_index += type_size;
	}else{
		feature_index += log_from + result_size + type_size;
	}*/

	return ss.str();
}

bool generate_sample(vector<login_data> login_list, int i, int index, string user_id, int trade_size) {
	stringstream ss;
	
	login_data login = login_list[index];
	trade_data current = login.trade_vec[trade_size - 1];

	int label = current.label;
	long time_stamp = current.time_stamp;
	//cout << "debug " << login.user_id << endl;

	ss << label << " ";
	int feature_index = 0;

	ss << update_current_info(feature_index, login_list, index, i, trade_size);//8
	feature_index += 100;
	//if(DEBUG_TEST)
	//	cout << ss.str() << endl;
	//cout << feature_index << endl;

	ss << update_global_info(feature_index, user_id, login_list, index, trade_size);//6
	feature_index += 100;
	//cout << feature_index << endl;

	ss << update_user_history_info(feature_index, login_list, index, EIGHT_MONTH, trade_size);//45
	feature_index += 100;
	//cout << feature_index << endl;

	//update_user_history_info(ss, feature_index, login_list, index, FIVE_MONTH, trade_size);//45
	//feature_index += 100;
	//cout << feature_index << endl;

	ss << update_last_login_info(feature_index, login_list, index, trade_size);//9
	feature_index += 100;
	//cout << feature_index << endl;

	//ss << update_last_last_login_info(feature_index, login_list, index, trade_size);//17
	//feature_index += 100;
	//cout << feature_index << endl;

	ss << update_user_history_login_info1(feature_index, login_list, index, 600L, trade_size); //16
	feature_index += 100;

	ss << update_user_history_login_info2(feature_index, login_list, index, 600L, trade_size); //16
	feature_index += 100;

	//update_user_history_login_info2(ss, feature_index, login_list, index, ONE_DAY, trade_size); //16
	//feature_index += 100;

	//update_user_history_login_info2(ss, feature_index, login_list, index, ONE_MONTH, trade_size); //16
	//feature_index += 100;

	//update_user_history_login_info2(ss, feature_index, login_list, index, THREE_MONTH, trade_size); //16
	//feature_index += 100;
	//cout << feature_index << endl;

	ss << update_user_history_trade_info(feature_index, login_list, index, THREE_MONTH, trade_size);//3
	feature_index += 100;
	//cout << feature_index << endl;

	ss << update_multi_trade_info(feature_index, login_list, index, trade_size);//5
	feature_index += 100;
	//cout << feature_index << endl;

	ss << update_login_error_info(feature_index, login_list, index, i, trade_size);//9
	feature_index += 100;
	//cout << feature_index << endl;

	ss << update_disc_info(feature_index, login_list, index, i, trade_size);//62
	feature_index += 100;
	//cout << feature_index << endl;
	

	if((time_stamp < 1430409600L)){
		if(label) {
			train_pos++;
			case1_pos++;
		}
		else {
			train_neg++;
			case1_neg++;
		}
		ss << "10000:" << current.row_key << " ";
		train_out_file << ss.str() << endl;
	} else if(time_stamp > 1430409600L && time_stamp < 1433088000L) {
		if(label)
			test_pos++;
		else
			test_neg++;
		//ss << "0:" << current.row_key << " ";
		ss << "10000:" << current.row_key << " ";
		train_out_file << ss.str() << endl;
		//test_out_file << ss.str() << endl;
	} else if((time_stamp > 1433260800L && time_stamp < 1434124800L) || 
		(time_stamp > 1434729600L && time_stamp < 1435680000L)){
		if(label)
			valid_pos++;
		else
			valid_neg++;
		ss << "0:" << current.row_key << " ";
		test_out_file << ss.str() << endl;
		//valid_out_file << ss.str() << endl;
	}else if(time_stamp > 1435680000L){
		ss << "0:" << current.row_key << " ";
		predict_out_file << ss.str() << endl;
	}
	if(time_stamp > 1433260800L && time_stamp < 1435680000L) {
		ss << "0:" << current.row_key << " ";
		valid_out_file << ss.str() << endl;
	}

	if(DEBUG_TEST) {
		cout << ss.str() << endl;
	}
}

bool generate_zero(trade_data data, int index, vector<trade_data> trade_list) {
	std::stringstream ss;

	long time_stamp = data.time_stamp;
	int label = data.label;
	ss << data.label << " ";
	trade_data current = trade_list[index];

	int feature_index = 0;
	ss << ++feature_index << ":" << index << " ";
	ss << ++feature_index << ":" << (time_stamp/3600 + 8)%24 << " ";
	ss << ++feature_index << ":" << ((time_stamp/3600 + 8)/24 + 4)%7 << " ";
	//ss << ++feature_index << ":" << (time_stamp > 1422720000L) << " ";

	if(index > 0) {
		ss << ++feature_index << ":" << 
		(trade_list[index].time_stamp - trade_list[index - 1].time_stamp) << " ";
		ss << ++feature_index << ":" << 
		(trade_list[index].time_stamp - trade_list[0].time_stamp) << " ";

		long max_diff = 0;
		long min_diff = 17280000;
		double avg_diff = 0.0;
		for(int id = 0; id < index; id++) {
			long diff = trade_list[id + 1].time_stamp - trade_list[id].time_stamp;
			if(diff > max_diff) {
				max_diff = diff;
			}
			if(diff < min_diff) {
				min_diff = diff;
			}
			avg_diff += diff;
		}
		avg_diff = avg_diff/index;
		ss << ++feature_index << ":" << max_diff << " ";
		ss << ++feature_index << ":" << min_diff << " ";
		ss << ++feature_index << ":" << avg_diff << " ";
	}else {
		feature_index += 5;
	}

	if(index > 1) {
		ss << ++feature_index << ":" << 
		 (trade_list[index].time_stamp - trade_list[index - 2].time_stamp) << " ";
	}

	if(DEBUG_TEST) {
		cout << ss.str() << endl;
	}

	if((time_stamp < 1430409600L)){
		if(label) {
			train_pos++;
			case1_pos++;
		}
		else {
			train_neg++;
			case1_neg++;
		}
		ss << "10000:" << current.row_key << " ";
		train_out_file << ss.str() << endl;
	} else if(time_stamp > 1430409600L && time_stamp < 1433088000L) {
		if(label)
			test_pos++;
		else
			test_neg++;
		ss << "10000:" << current.row_key << " ";
		//ss << "0:" << current.row_key << " ";
		train_out_file << ss.str() << endl;
		//test_out_file << ss.str() << endl;
	} else if((time_stamp > 1433260800L && time_stamp < 1434124800L) || 
		(time_stamp > 1434729600L && time_stamp < 1435680000L)){
		if(label)
			valid_pos++;
		else
			valid_neg++;
		ss << "0:" << current.row_key << " ";
		test_out_file << ss.str() << endl;
		//valid_out_file << ss.str() << endl;
	}else if(time_stamp > 1435680000L){
		ss << "0:" << current.row_key << " ";
		predict_out_file << ss.str() << endl;
	}
	if(time_stamp > 1433260800L && time_stamp < 1435680000L) {
		ss << "0:" << current.row_key << " ";
		valid_out_file << ss.str() << endl;
	}
}

int part1_size = 0;
int part2_size = 0;
int part3_size = 0;

bool transfer_data() {
	map<string, int> detail_trade_map1;
	map<string, int> detail_trade_map2;

	int size_num = 0, total_num = 0;
	int user_cnt = 0;
	cout << "begin to generate sample" << endl;
	for(map<string, set<trade_data> >::iterator it = trade_info.begin(); it != trade_info.end(); ++it) {
		string user_id = it->first;
		user_cnt++;
		if(user_cnt%3000 == 0)
			cout << "parse user data size " << user_cnt << endl;
		if(user_id.length() < 1)
			continue;
		set<trade_data> trade_detail = it->second;
		vector<trade_data> trade_list;
		for(set<trade_data>::iterator iter = trade_detail.begin(); iter != trade_detail.end(); ++iter) {
			trade_list.push_back(*iter);
		}
		if(login_info.find(it->first) == login_info.end()) {
			int i = 0;
			for(set<trade_data>::iterator iter = trade_detail.begin(); iter != trade_detail.end(); ++iter) {
				trade_data trade = *iter;
				i++;
				generate_zero(trade, i - 1, trade_list);
				part1_size++;
				size_num++;
			}
			continue;
		}
		set<login_data> login_detail = login_info.find(it->first)->second;
		
		vector<login_data> login_list;
		for(set<login_data>::iterator iter = login_detail.begin(); iter != login_detail.end(); ++iter) {
			/*if(login_list.size() > 0 && login_list[login_list.size() - 1] == (*iter)) 
				continue;*/
			login_list.push_back(*iter);
		}
		int i = 0;
		int index = -1, befor_index = 0;
		clear_history_map();
		int trade_cnt = 0;
		total_device_map.clear();
		total_ip_map.clear();
		set<login_data>::iterator total_it = total_login_map.begin();
		for(set<trade_data>::iterator iter = trade_detail.begin(); iter != trade_detail.end(); ++iter) {
			total_num++;
			trade_cnt++;
			trade_data trade = *iter;
			for(; i < login_list.size(); i++) {
				if(login_list[i].time_stamp > trade.time_stamp)
					break;
				if(login_list[i].result == "1") {
					update_history_login(login_list[i]);
					index = i;
					for(int j = 0; j < login_list[index].trade_vec.size(); j++) {
						if(login_list[index].trade_vec[j].label == 0 
							&& (trade.time_stamp - login_list[index].trade_vec[j].time_stamp) > 3600*24*30) {
							update_history_valid(login_list[index]);
						}else if(login_list[index].trade_vec[j].label == 1 
							&& (trade.time_stamp - login_list[index].trade_vec[j].time_stamp) > 3600*24*30) {
							update_history_black(login_list[index]);
						}
					}
				}
				if(DEBUG_TEST) {
					out_login(login_list[i]);
					cout << " " << endl;
				}
			}
			if(DEBUG_TEST) {
				out_trade(trade);
			}
			if(index < 0) {
				size_num++;
				part2_size++;
				generate_zero(trade, trade_cnt - 1, trade_list);
				continue;
			}
			update_history_trade(login_list[index]);
			login_list[index].trade_vec.push_back(trade);
			part3_size++;
			/*for(; total_it != total_login_map.end(); total_it++) {
				login_data login_tmp = (*total_it);
				if(login_tmp.time_stamp < trade.time_stamp) {
					update_total_device_map(login_tmp, login_tmp.user_id);
					update_total_ip_map(login_tmp, login_tmp.user_id);
				}
			}*/
			generate_sample(login_list, i - 1, index, user_id, login_list[index].trade_vec.size());
			size_num++;
		}
	}

	cout << "total sample size " << size_num << " " << total_num << endl;

	vector<int> trade_tmp(101);
	for(map<string, int>::iterator it = detail_trade_map2.begin(); it != detail_trade_map2.end(); ++it) {
		int show = it->second;
		int pos = 0;
		if(detail_trade_map1.find(it->first) != detail_trade_map1.end())
			pos = detail_trade_map1.at(it->first);

		int index = pos * 100/show;
		trade_tmp[index] = trade_tmp[index] + 1;
	}
}

bool transfer_login_and_trade() {
	for(map<string, set<trade_data> >::iterator it = trade_info.begin(); it != trade_info.end(); ++it) {
		string user_id = it->first;
		set<trade_data> trade_detail = it->second;
		vector<trade_data> trade_list;
		for(set<trade_data>::iterator iter = trade_detail.begin(); iter != trade_detail.end(); ++iter) {
			trade_list.push_back(*iter);
		}
		total_trade_vec.insert(make_pair(user_id, trade_list));
	}
	//trade_info.clear();
	for(map<string, set<login_data> >::iterator it = login_info.begin(); it != login_info.end(); ++it) {
		string user_id = it->first;
		set<login_data> login_detail = it->second;
		vector<login_data> login_list;
		login_data last_login_data;
		for(set<login_data>::iterator iter = login_detail.begin(); iter != login_detail.end(); ++iter) {
			login_data data = (*iter);
			login_list.push_back(data);
			total_login_map.insert(data);
		}
		total_login_vec.insert(make_pair(user_id, login_list));
	}
	//login_info.clear();
}

bool write_raw_data(vector<trade_data> trade_list) {
	zero_raw_data_out << trade_list[0].user_id << " " << trade_list.size() << " " << endl;
	for(int i = 0; i < trade_list.size(); i++) {
		zero_raw_data_out << trade_list[i].time_stamp << " " 
							<< trade_list[i].row_key << " " << trade_list[i].label << " " << endl;
	}
	//zero_raw_data_out << "0 0" << endl;
	zero_raw_data_out << endl;
}

bool upload_zero_raw_data() {
	ifstream file_in("zero_raw_data");
	if(!file_in) {
		raw_data_cache = false;
		return false;
	}else {
		raw_data_cache = true;
	}
	while(!file_in.eof()) {
		string user_id;
		int size;
		file_in >> user_id;
		file_in >> size;
		vector<trade_data> trade_list_data(size);
		int i = 0;
		while(i < size) {
			trade_data data;
			file_in >> data.time_stamp >> data.row_key >> data.label;
			trade_list_data[i] = data;
			generate_zero(data, i, trade_list_data);
			i++;
		}
	}
	return true;
}

bool write_sample_raw_data(map<string, vector<login_data> > total_login_vec) {
	for(map<string, vector<login_data> >::iterator it = total_login_vec.begin(); it != total_login_vec.end(); it++) {
		vector<login_data> data = it->second;
		sample_raw_data_out << it->first << " " << data.size() << " " << endl;
		for(int i = 0; i < data.size(); i++) {
			login_data login = data[i];
			sample_raw_data_out << login.device << " " << login.log_from << " " << login.ip << " " << login.city
					<< " " << login.result << " " << login.time_stamp << " " << 
					login.type << " " << login.trade_vec.size() << " " << endl;
			for(int j = 0; j < login.trade_vec.size(); j++) {
				sample_raw_data_out << login.trade_vec[j].time_stamp << " " << login.trade_vec[j].row_key
					<< " " << login.trade_vec[j].label << " " << endl;
			}
		}
		sample_raw_data_out << endl;
	}
}

bool upload_sample_raw_data() {
	ifstream file_in("sample_raw_data");
	if(!file_in) {
		raw_data_cache = false;
		return false;
	}else {
		raw_data_cache = true;
	}
	while(!file_in.eof()) {
		string user_id;
		int size;
		file_in >> user_id;
		file_in >> size;
		vector<login_data> login_list_data(size);
		int i = 0;
		while(i < size) {
			login_data data;
			file_in >> data.device >> data.log_from >> data.ip;
			file_in >> data.city >> data.result >> data.time_stamp >> data.type;
			int trade_size;
			file_in >> trade_size;
			int j = 0;
			while(j < trade_size) {
				trade_data trade_tmp;
				file_in >> trade_tmp.time_stamp >> trade_tmp.row_key >> trade_tmp.label;
				data.trade_vec.push_back(trade_tmp);
				j++;
			}
			login_list_data[i] = data;
			i++;
		}
		for(i = 0; i < size; i++) {
			login_data login = login_list_data[i];
			if(login.trade_vec.size() == 0)
				continue;
			for(int index = 0; index < login.trade_vec.size(); index++) {
				int login_index = i;
				long current_stamp = login.trade_vec[index].time_stamp;
				while(login_index < size - 1) {
					if(login_list_data[login_index + 1].time_stamp > current_stamp
						&& login_list_data[login_index + 1].result == "1") {
						break;
					}else{
						login_index++;
					}
				}
				generate_sample(login_list_data, login_index, i, user_id, index);
			}
			int j = i;
			while(j < size - 1) {
				//if(login_list_data[j+1].time_stamp )
			}
		}

	}
	return true;
}

bool transfer_data2() {
	int size_num = 0;
	cout << "begin to generate sample------------------------" << endl;

	transfer_login_and_trade();

	set<login_data>::iterator total_login_it = total_login_map.begin();
	map<string, int> user_trade_cnt_map;
	
	vector<login_data> black_trade_login_info;
	vector<login_data> gray_trade_login_info;
	vector<login_data> valid_trade_login_info;
	int black_id = 0, valid_id = 0, gray_id = 0;
	bool debug_ttt = 0;
	for(set<trade_data>::iterator total_trade_it = total_trade_map.begin(); 
				total_trade_it != total_trade_map.end(); ++total_trade_it) {
		trade_data current = *total_trade_it;
		if(debug_ttt == 0 && current.time_stamp > 1422720000L) {
			debug_ttt = 1;
			cout << "process data size " << size_num << ", time " << current.time_stamp << endl;
		}else if (debug_ttt == 1 && current.time_stamp > 1425139200L) {
			debug_ttt = 2;
			cout << "process data size " << size_num << ", time " << current.time_stamp << endl;
		}else if (debug_ttt == 2 && current.time_stamp > 1427817600L) {
			debug_ttt = 3;
			cout << "process data size " << size_num << ", time " << current.time_stamp << endl;
		}else if (debug_ttt == 3 && current.time_stamp > 1430409600L) {
			debug_ttt = 4;
			cout << "process data size " << size_num << ", time " << current.time_stamp << endl;
		}else if (debug_ttt == 4 && current.time_stamp > 1433088000L) {
			debug_ttt = 5;
			cout << "process data size " << size_num << ", time " << current.time_stamp << endl;
		}else if (debug_ttt == 5 && current.time_stamp > 1435680000L) {
			debug_ttt = 6;
			cout << "process data size " << size_num << ", time " << current.time_stamp << endl;
		}
		//if(size_num%10000 == 0)
		//	cout << "process data size " << size_num << ", time " << current.time_stamp << endl; 

		string user_id = current.user_id;
		if(user_id.length() < 1)
			continue;
		long time_stamp = current.time_stamp;

		for(; total_login_it != total_login_map.end(); total_login_it++) {
			if((*total_login_it).time_stamp < current.time_stamp) {
				update_total_device_map(*total_login_it, (*total_login_it).user_id);
				update_total_ip_map(*total_login_it, (*total_login_it).user_id);
			}else {
				break;
			}
		}

		if(black_id < black_trade_login_info.size()) {
			for(; black_id < black_trade_login_info.size(); black_id++) {
				if(current.time_stamp - black_trade_login_info[black_id].time_stamp > ONE_MONTH) {
					update_black_total_device_map(black_trade_login_info[black_id], 
						black_trade_login_info[black_id].user_id);
					update_black_total_ip_map(black_trade_login_info[black_id], 
						black_trade_login_info[black_id].user_id);
				}else
					break;
			}
		}

		if(gray_id < gray_trade_login_info.size()) {
			for(; gray_id < gray_trade_login_info.size(); gray_id++) {
				if(current.time_stamp - gray_trade_login_info[gray_id].time_stamp > ONE_MONTH) {
					update_gray_total_device_map(gray_trade_login_info[gray_id], 
						gray_trade_login_info[gray_id].user_id);
					update_gray_total_ip_map(gray_trade_login_info[gray_id], 
						gray_trade_login_info[gray_id].user_id);
				}else
					break;
			}
		}

		if(valid_id < valid_trade_login_info.size()) {
			for(; valid_id < valid_trade_login_info.size(); valid_id++) {
				if(current.time_stamp - valid_trade_login_info[valid_id].time_stamp > ONE_MONTH) {
					update_valid_total_device_map(valid_trade_login_info[valid_id], 
						valid_trade_login_info[valid_id].user_id);
					update_valid_total_ip_map(valid_trade_login_info[valid_id], 
						valid_trade_login_info[valid_id].user_id);
				}else
					break;
			}
		}

		vector<trade_data> trade_list = total_trade_vec.at(user_id);
		int trade_index = 0;
		if(user_trade_cnt_map.find(user_id) == user_trade_cnt_map.end()) {
			user_trade_cnt_map.insert(make_pair(user_id, 0));
		}
		
		if(total_login_vec.find(user_id) == total_login_vec.end()) {
			out_trade(current);
			map<string, int>::iterator it = user_trade_cnt_map.find(user_id);
			trade_index = it->second;
			it->second = trade_index + 1;
			part1_size++;
			generate_zero(current, trade_index, trade_list);
			size_num++;
			continue;
		}
		map<string, vector<login_data> >::iterator login_it = total_login_vec.find(user_id);
		vector<login_data> login_list = login_it->second;

		int index = -1;
		int login_index = 0;
		for(; login_index < login_list.size(); login_index++) {
			if(login_list[login_index].time_stamp > current.time_stamp)
				break;
			if(login_list[login_index].result == "1") {
				index = login_index;
			}
			out_login(login_list[login_index]);
			for(int idx = 0; idx < login_list[login_index].trade_vec.size(); idx++) {
				out_trade(login_list[login_index].trade_vec[idx]);
			}
		}

		if(index < 0) {
			out_trade(current);
			map<string, int>::iterator it = user_trade_cnt_map.find(user_id);
			trade_index = it->second;
			it->second = trade_index + 1;
			part2_size++;
			generate_zero(current, trade_index, trade_list);
			size_num++;
			continue;
		}

		login_list[index].trade_vec.push_back(current);
		int tmp_size = login_list[index].trade_vec.size();
		login_it->second = login_list;
		
		out_trade((login_it->second)[index].trade_vec[(login_it->second)[index].trade_vec.size() - 1]);
		part3_size++;
		generate_sample(login_list, login_index - 1, index, user_id, login_list[index].trade_vec.size());
		update_trade_total_device_map(login_list[index], login_list[index].user_id);
		update_trade_total_ip_map(login_list[index], login_list[index].user_id);
		if(current.label == 0) {
			if(index > 0)
				valid_trade_login_info.push_back(login_list[index - 1]);
			/*for(int tmp_id = 0; tmp_id < index; tmp_id++) {
				if(current.time_stamp - login_list[tmp_id].time_stamp < ONE_MONTH) {
					valid_trade_login_info.push_back(login_list[tmp_id]);
				}
			}*/
			valid_trade_login_info.push_back(login_list[index]);
		}
		else {
			if(index > 0)
				black_trade_login_info.push_back(login_list[index - 1]);
			/*for(int tmp_id = 0; tmp_id < index; tmp_id++) {
				if(current.time_stamp - login_list[tmp_id].time_stamp < ONE_MONTH) {
					black_trade_login_info.push_back(login_list[tmp_id]);
				}
			}*/
			black_trade_login_info.push_back(login_list[index]);
		}
		size_num++;
	}
	cout << "finish generate sample, total sample size " << size_num << endl;
	cout << "total change label " << change_label_cnt << endl;

	if(DEBUG_TEST)
		return 0;
	/*write_sample_raw_data(total_login_vec);

	cout << "begin to write zero sample" << endl;
	for(map<string, int>::iterator map_it = user_trade_cnt_map.begin(); map_it != user_trade_cnt_map.end(); map_it++) {
		string user_id = map_it->first;
		int index = map_it->second;
		//cout << user_id << " " << index << endl;
		if(index == 0)
			continue;
		vector<trade_data> trade_list = total_trade_vec.at(user_id);
		vector<trade_data> sub_list;
		for(int i = 0; i < index; i++)
			sub_list.push_back(trade_list[i]);
		write_raw_data(sub_list);
	}*/
}

int main(int argc, char **argv) {
	srand((unsigned)time(NULL));
	string login_path = argv[1];
	string trade_path = argv[2];
	if(argc > 3) {
		test_user = argv[3];
		DEBUG_TEST = true;
	}else {
		DEBUG_TEST = false;
	}

	DEBUG_TEST = true;

	//if(upload_sample_raw_data() && upload_zero_raw_data())
	//	return 0;

	init_discrete_map();
	cout << "file cache is " << is_cache << endl;

	load_login_data(login_path);

	load_trade_data(trade_path);

	//transfer_data();
	transfer_data2();

	output_discrete_map();

	if(DEBUG_LOG) {
		cout << "train pos, train neg, test pos, test neg " << train_pos << " "
			<< train_neg << " " << test_pos << " " << test_neg << " " << 
			valid_pos << " " << valid_neg << endl;
		cout << "case1 and case2 " << case1_pos << " " << case1_neg << " "
			 << case2_pos << " " << case2_neg << endl;
		cout << "max_time_long " << max_time_long << endl;
	}
	cout << "part size " << part1_size << " " << part2_size << " " << part3_size << endl;
}
