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
#define Name(X) #X
#define Out(X) {string xname = Name(X); cout << xname.substr(5, xname.size()-1) << ": " << X << " ";}
using namespace std;

typedef struct trade_data{
	long time_stamp;
	string user_id;
	int label;
	int row_key;
}trade_data;

bool operator<(const trade_data & x,const trade_data & y)
{
	if(x.time_stamp == y.time_stamp)
		return x.user_id < y.user_id;
	return x.time_stamp < y.time_stamp;  
}

bool operator==(const trade_data & x,const trade_data & y)
{
	return x.time_stamp == y.time_stamp
			&& x.user_id == y.user_id
			&& x.label == y.label
			&& x.row_key == y.row_key;
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
}login_data;

void out_login(login_data data){
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
}

bool operator<(const login_data & x,const login_data & y)
{
	if(x.time_stamp == y.time_stamp)
		return x.user_id < y.user_id;
	return x.time_stamp < y.time_stamp;  
}

bool operator==(const login_data & x,const login_data & y)
{
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
	Out(data.time_stamp);
	Out(data.user_id);
	Out(data.label);
	Out(data.row_key);
}

bool DEBUG_LOG = true;
bool DEBUG_TEST = true;
bool is_cache = false;
int max_time_long = 0;
string test_user = "";

map<string, set<login_data> > login_info;
map<string, set<trade_data> > trade_info;
map<string, vector<login_data> > login_vec;
map<string, vector<trade_data> > trade_vec;
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

string train_file = "train_result";
ofstream train_out_file(train_file.c_str());
string test_file = "test_result";
ofstream test_out_file(test_file.c_str());
string valid_file = "valid_result";
ofstream valid_out_file(valid_file.c_str());
string predict_file = "predict_result";
ofstream predict_out_file(predict_file.c_str());

bool train_or_test = true;

int result_size = 10;
int type_size = 3;
int log_from = 11;

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
			if(user_id != test_user)
				continue;
			cout << "log info: ";
			out_login(login);
			cout << " " << endl;
		}
		if(login_info.find(user_id) == login_info.end()) {
			set<login_data> log_list;
			log_list.insert(login);
			login_info.insert(make_pair(user_id, log_list));
		}else {
			map<string, set<login_data> >::iterator it = login_info.find(user_id);
			set<login_data> log_list = it->second;
			log_list.insert(login);
			it->second = log_list;
		}
		total_login_map.insert(login);
		update_discrete_map(login, user_id);
	}
	if(DEBUG_LOG) {
		cout << "total login user " << login_info.size()  
			<< ", times " << line_num << " login map size " << total_login_map.size() << endl;
	}
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
			if(user_id != test_user)
				continue;
			cout << line_num << "trade info1: ";
			out_trade(trade);
			cout << " " << endl;
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
			int size1 = trade_list.size();
			trade_list.insert(trade);
			int size2 = trade_list.size();
			if(size1 == size2) {
				cout << line_num << " trade info2: ";
				out_trade(trade);
				cout << " " << size2 << " " << endl;
			}
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

void get_max_min_avg(stringstream& ss, int& feature_index, vector<double> data) {
	double max = 0;
	double min = 17280000.0;
	double avg = 0;

	for(int i = 0; i < data.size(); i++) {
		//cout << data[i] << endl;
		if(max < data[i])
			max = data[i];
		if(min > data[i])
			min = data[i];
		avg += data[i];
	}
	avg = avg/data.size();

	ss << ++feature_index << ":" << max << " ";
	ss << ++feature_index << ":" << min << " ";
	ss << ++feature_index << ":" << avg << " ";
}

bool generate_sample(vector<login_data> login_list, int i, int index, string user_id) {
	stringstream ss;
	
	login_data login = login_list[index];
	int trade_size = login.trade_vec.size();
	trade_data current = login.trade_vec[trade_size - 1];

	int label = current.label;
	long time_stamp = current.time_stamp;
	ss << label << " ";
	int feature_index = 10;

//10
	ss << ++feature_index << ":" << (time_stamp/3600 + 8)%24 << " ";
	ss << ++feature_index << ":" << ((time_stamp/3600 + 8)/24 + 4)%7 << " ";
	ss << ++feature_index << ":" << trade_size - 1 << " ";     //trade cnt
	ss << ++feature_index << ":" << login.time_long << " ";            //login time

	ss << ++feature_index << ":" << (time_stamp - login.time_stamp) << " ";
	ss << ++feature_index << ":" << (login.time_long > (time_stamp - login.time_stamp)) << " ";
	ss << ++feature_index << ":" << i - index << " ";
	ss << ++feature_index << ":" << (login.time_stamp/3600 + 8)%24 << " ";
	ss << ++feature_index << ":" << ((login.time_stamp/3600 + 8)/24 + 4)%7 << " ";

//19

	if(device_user_map.find(login.device) != device_user_map.end()) {
		map<string, int> tmp = device_user_map.at(login.device);
		if(tmp.find(user_id) != tmp.end() && tmp.find("sum") != tmp.end()) {
			ss << ++feature_index << ":" << tmp.at(user_id) << " ";
			ss << ++feature_index << ":" << tmp.at(user_id)*1.0/tmp.at("sum") << " ";
		}
	}else{
		feature_index += 2;
	}
	if(ip_user_map.find(login.ip) != ip_user_map.end()) {
		map<string, int> tmp = ip_user_map.at(login.ip);
		if(tmp.find(user_id) != tmp.end() && tmp.find("sum") != tmp.end()) {
			ss << ++feature_index << ":" << tmp.at(user_id) << " ";
			ss << ++feature_index << ":" << tmp.at(user_id)*1.0/tmp.at("sum") << " ";
		}
	}else{
		feature_index += 2;
	}

	if(total_device_map.find(login.device) != total_device_map.end()) {
		map<string, int> tmp = total_device_map.at(login.device);
		if(tmp.find(user_id) != tmp.end() && tmp.find("sum") != tmp.end()) {
			ss << ++feature_index << ":" << tmp.at(user_id) << " ";
			ss << ++feature_index << ":" << tmp.at(user_id)*1.0/tmp.at("sum") << " ";
		}
	}else{
		feature_index += 2;
	}
	if(total_ip_map.find(login.ip) != total_ip_map.end()) {
		map<string, int> tmp = total_ip_map.at(login.ip);
		if(tmp.find(user_id) != tmp.end() && tmp.find("sum") != tmp.end()) {
			ss << ++feature_index << ":" << tmp.at(user_id) << " ";
			ss << ++feature_index << ":" << tmp.at(user_id)*1.0/tmp.at("sum") << " ";
		}
	}else{
		feature_index += 2;
	}
//27

	// if(trade_total_device_map.find(login.device) != trade_total_device_map.end()) {
	// 	map<string, int> tmp = trade_total_device_map.at(login.device);
	// 	if(tmp.find(user_id) != tmp.end() && tmp.find("sum") != tmp.end()) {
	// 		ss << ++feature_index << ":" << tmp.at(user_id) << " ";
	// 		ss << ++feature_index << ":" << tmp.at(user_id)*1.0/tmp.at("sum") << " ";
	// 	}
	// }else{
	// 	feature_index += 2;
	// }
	// if(trade_total_ip_map.find(login.ip) != trade_total_ip_map.end()) {
	// 	map<string, int> tmp = trade_total_ip_map.at(login.ip);
	// 	if(tmp.find(user_id) != tmp.end() && tmp.find("sum") != tmp.end()) {
	// 		ss << ++feature_index << ":" << tmp.at(user_id) << " ";
	// 		ss << ++feature_index << ":" << tmp.at(user_id)*1.0/tmp.at("sum") << " ";
	// 	}
	// }else{
	// 	feature_index += 2;
	// }

	// if(valid_total_device_map.find(login.device) != valid_total_device_map.end()) {
	// 	map<string, int> tmp = valid_total_device_map.at(login.device);
	// 	if(tmp.find(user_id) != tmp.end() && tmp.find("sum") != tmp.end()) {
	// 		ss << ++feature_index << ":" << tmp.at(user_id) << " ";
	// 		ss << ++feature_index << ":" << tmp.at(user_id)*1.0/tmp.at("sum") << " ";
	// 	}
	// }else{
	// 	feature_index += 2;
	// }
	// if(valid_total_ip_map.find(login.ip) != valid_total_ip_map.end()) {
	// 	map<string, int> tmp = valid_total_ip_map.at(login.ip);
	// 	if(tmp.find(user_id) != tmp.end() && tmp.find("sum") != tmp.end()) {
	// 		ss << ++feature_index << ":" << tmp.at(user_id) << " ";
	// 		ss << ++feature_index << ":" << tmp.at(user_id)*1.0/tmp.at("sum") << " ";
	// 	}
	// }else{
	// 	feature_index += 2;
	// }

	// if(black_total_device_map.find(login.device) != black_total_device_map.end()) {
	// 	map<string, int> tmp = black_total_device_map.at(login.device);
	// 	if(tmp.find(user_id) != tmp.end() && tmp.find("sum") != tmp.end()) {
	// 		ss << ++feature_index << ":" << tmp.at(user_id) << " ";
	// 		ss << ++feature_index << ":" << tmp.at(user_id)*1.0/tmp.at("sum") << " ";
	// 	}
	// }else{
	// 	feature_index += 2;
	// }
//
/*	if(black_total_ip_map.find(login.ip) != black_total_ip_map.end()) {
		map<string, int> tmp = black_total_ip_map.at(login.ip);
		if(tmp.find(user_id) != tmp.end() && tmp.find("sum") != tmp.end()) {
			ss << ++feature_index << ":" << tmp.at(user_id) << " ";
			ss << ++feature_index << ":" << tmp.at(user_id)*1.0/tmp.at("sum") << " ";
		}
	}else{
		feature_index += 2;
	}*/


	if(device_map.find(login.device) != device_map.end()) {
		ss << ++feature_index << ":" << device_map.at(login.device) << " ";
		ss << ++feature_index << ":" << device_map.at(login.device)*1.0/(device_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
	//cout << feature_index << " ";
	if(ip_map.find(login.ip) != ip_map.end()) {
		ss << ++feature_index << ":" << ip_map.at(login.ip) << " ";
		ss << ++feature_index << ":" << ip_map.at(login.ip)*1.0/(ip_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
	//cout << feature_index << " ";

	if(city_map.find(login.city) != city_map.end()) {
		ss << ++feature_index << ":" << city_map.at(login.city) << " ";
		ss << ++feature_index << ":" << city_map.at(login.city)*1.0/(city_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
	//cout << feature_index << " ";
	if(type_map.find(login.type) != type_map.end()) {
		ss << ++feature_index << ":" << type_map.at(login.type) << " ";
		ss << ++feature_index << ":" << type_map.at(login.type)*1.0/(type_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
//
	//cout << feature_index << " ";
	if(log_from_map.find(login.log_from) != log_from_map.end()) {
		ss << ++feature_index << ":" << log_from_map.at(login.log_from) << " ";
		ss << ++feature_index << ":" << log_from_map.at(login.log_from)*1.0/(log_from_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
//37
	//cout << feature_index << " ";
	if(trade_device_map.find(login.device) != trade_device_map.end()) {
		ss << ++feature_index << ":" << trade_device_map.at(login.device) << " ";
		ss << ++feature_index << ":" << trade_device_map.at(login.device)*1.0/(trade_device_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
	//cout << feature_index << " ";
	if(trade_ip_map.find(login.ip) != trade_ip_map.end()) {
		ss << ++feature_index << ":" << trade_ip_map.at(login.ip) << " ";
		ss << ++feature_index << ":" << trade_ip_map.at(login.ip)*1.0/(trade_ip_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
	//cout << feature_index << " ";


	if(trade_city_map.find(login.city) != trade_city_map.end()) {
		ss << ++feature_index << ":" << trade_city_map.at(login.city) << " ";
		ss << ++feature_index << ":" << trade_city_map.at(login.city)*1.0/(trade_city_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
	//cout << feature_index << " ";
	if(trade_type_map.find(login.type) != trade_type_map.end()) {
		ss << ++feature_index << ":" << trade_type_map.at(login.type) << " ";
		ss << ++feature_index << ":" << trade_type_map.at(login.type)*1.0/(trade_type_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
//
	//cout << feature_index << " ";
	if(trade_log_from_map.find(login.log_from) != trade_log_from_map.end()) {
		ss << ++feature_index << ":" << trade_log_from_map.at(login.log_from) << " ";
		ss << ++feature_index << ":" << trade_log_from_map.at(login.log_from)*1.0/(trade_log_from_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
//47
	//cout << feature_index << " ";
	if(valid_device_map.find(login.device) != valid_device_map.end()) {
		ss << ++feature_index << ":" << valid_device_map.at(login.device) << " ";
		ss << ++feature_index << ":" << valid_device_map.at(login.device)*1.0/(valid_device_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
	//cout << feature_index << " ";
	if(valid_ip_map.find(login.ip) != valid_ip_map.end()) {
		ss << ++feature_index << ":" << valid_ip_map.at(login.ip) << " ";
		ss << ++feature_index << ":" << valid_ip_map.at(login.ip)*1.0/(valid_ip_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
	//cout << feature_index << " ";


	if(valid_city_map.find(login.city) != valid_city_map.end()) {
		ss << ++feature_index << ":" << valid_city_map.at(login.city) << " ";
		ss << ++feature_index << ":" << valid_city_map.at(login.city)*1.0/(valid_city_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
	//cout << feature_index << " ";
	if(valid_type_map.find(login.type) != valid_type_map.end()) {
		ss << ++feature_index << ":" << valid_type_map.at(login.type) << " ";
		ss << ++feature_index << ":" << valid_type_map.at(login.type)*1.0/(valid_type_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
//
	//cout << feature_index << " ";
	if(valid_log_from_map.find(login.log_from) != valid_log_from_map.end()) {
		ss << ++feature_index << ":" << valid_log_from_map.at(login.log_from) << " ";
		ss << ++feature_index << ":" << valid_log_from_map.at(login.log_from)*1.0/(valid_log_from_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
//57
	//cout << feature_index << " ";
	if(black_device_map.find(login.device) != black_device_map.end()) {
		ss << ++feature_index << ":" << black_device_map.at(login.device) << " ";
		ss << ++feature_index << ":" << black_device_map.at(login.device)*1.0/(black_device_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
	//cout << feature_index << " ";
	if(black_ip_map.find(login.ip) != black_ip_map.end()) {
		ss << ++feature_index << ":" << black_ip_map.at(login.ip) << " ";
		ss << ++feature_index << ":" << black_ip_map.at(login.ip)*1.0/(black_ip_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
	//cout << feature_index << " ";


	if(black_city_map.find(login.city) != black_city_map.end()) {
		ss << ++feature_index << ":" << black_city_map.at(login.city) << " ";
		ss << ++feature_index << ":" << black_city_map.at(login.city)*1.0/(black_city_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
	//cout << feature_index << " ";
	if(black_type_map.find(login.type) != black_type_map.end()) {
		ss << ++feature_index << ":" << black_type_map.at(login.type) << " ";
		ss << ++feature_index << ":" << black_type_map.at(login.type)*1.0/(black_type_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
//
	//cout << feature_index << " ";
	if(black_log_from_map.find(login.log_from) != black_log_from_map.end()) {
		ss << ++feature_index << ":" << black_log_from_map.at(login.log_from) << " ";
		ss << ++feature_index << ":" << black_log_from_map.at(login.log_from)*1.0/(black_log_from_map.at("sum")) << " ";
	}
	else{
		feature_index += 2;
	}
//67
	int before_index = index -1;
	if(index>0) {
		for(; before_index > 0; before_index--) {
			if(login_list[index].device == login_list[before_index].device
				&& login_list[index].ip == login_list[before_index].ip
				&& login_list[index].city == login_list[before_index].city
				&& login_list[index].result == login_list[before_index].result
				&& login_list[index].time_stamp - login_list[before_index].time_stamp < 10) {
				continue;
			}else {
				break;
			}
		}
		ss << ++feature_index << ":" << index - 1 - before_index << " ";

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
		ss << ++feature_index << ":" << time_stamp - login_list[before_index].time_stamp << " ";
//
		ss << ++feature_index << ":" << login.time_stamp - login_list[before_index].time_stamp << " ";
//
		//ss << ++feature_index << ":" << login.time_long - login_list[before_index].time_long << " ";
		ss << ++feature_index << ":" << max_time_long + login.time_long - login_list[before_index].time_long << " ";
//77
		//ss << ++feature_index << ":" << login.is_scan - login_list[before_index].is_scan << " ";

		//ss << ++feature_index << ":" << login.is_sec - login_list[before_index].is_sec << " ";
		//ss << ++feature_index << ":" << (login.device == login_list[before_index].device) << " ";
		//ss << ++feature_index << ":" << (login.ip == login_list[before_index].ip) << " ";
		//ss << ++feature_index << ":" << (login.city == login_list[before_index].city) << " ";

		//ss << ++feature_index << ":" << (login.type == login_list[before_index].type) << " ";

		map<string, int> log_device_info;
		map<string, int> log_ip_info;
		map<string, int> log_city_info;
		map<string, int> trade_device_info;
		map<string, int> trade_ip_info;
		map<string, int> trade_city_info;
		int login_cnt = 0, trade_cnt = 0;
		long device_time_diff = 17280000;
		long ip_time_diff = 17280000;
		long city_time_diff = 17280000;
		vector<double> time_diff;
		int sum_login = 0;
		int sum_trade = 0;
		for(int id = 0; id <= index; id++) {
			if(((login_list[index].time_stamp - login_list[id].time_stamp) < 600 
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
		ss << ++feature_index << ":" << trade_device_info.size() << " ";
		if(trade_device_info.size() > 0)
			ss << ++feature_index << ":" << trade_device_info.at(login_list[index].device)*1.0/sum_trade << " ";
		else
			++feature_index;
		ss << ++feature_index << ":" << trade_ip_info.size() << " ";
		if(trade_ip_info.size() > 0)
			ss << ++feature_index << ":" << trade_ip_info.at(login_list[index].ip)*1.0/sum_trade << " ";
		else
			++feature_index;

		ss << ++feature_index << ":" << trade_city_info.size() << " ";
		if(trade_city_info.size() > 0)
			ss << ++feature_index << ":" << trade_city_info.at(login_list[index].city)*1.0/sum_trade << " ";
		else
			++feature_index;
//89
		ss << ++feature_index << ":" << sum_login << " ";
		ss << ++feature_index << ":" << sum_trade << " ";
		ss << ++feature_index << ":" << login_cnt << " ";
		ss << ++feature_index << ":" << trade_cnt << " ";

		ss << ++feature_index << ":" << device_time_diff << " ";
		ss << ++feature_index << ":" << ip_time_diff << " ";
		ss << ++feature_index << ":" << city_time_diff << " ";

		//cout << "debug " << time_diff.size() << endl;
		if(time_diff.size() > 0)
			get_max_min_avg(ss, feature_index, time_diff);
		else
			feature_index += 3;
//99
/*
		for(map<string, int>::iterator map_it = trade_device_info.begin();
			map_it != trade_device_info.end(); map_it++) {
			cout << map_it->first << " " << map_it->second << endl;
		}
		for(map<string, int>::iterator map_it = trade_ip_info.begin();
			map_it != trade_ip_info.end(); map_it++) {
			cout << map_it->first << " " << map_it->second << endl;
		}
		for(map<string, int>::iterator map_it = trade_city_info.begin();
			map_it != trade_city_info.end(); map_it++) {
			cout << map_it->first << " " << map_it->second << endl;
		}*/

/*		log_device_info.clear();
		log_ip_info.clear();
		log_city_info.clear();
		trade_device_info.clear();
		trade_ip_info.clear();
		trade_city_info.clear();
		login_cnt = 0;
		trade_cnt = 0;
		for(int id = 0; id < index; id++) {
			if((login_list[index].time_stamp - login_list[id].time_stamp) < 4320000 
				&& (login_list[index].time_stamp - login_list[id].time_stamp) > 10) {
				log_device_info.insert(login_list[id].device);
				log_ip_info.insert(login_list[id].ip);
				log_city_info.insert(login_list[id].city);
				if(login_list[id].result == "1")
					login_cnt++;
				if(login_list[id].trade_vec.size() > 0) {
					trade_cnt += login_list[id].trade_vec.size();
					trade_device_info.insert(login_list[id].device);
					trade_ip_info.insert(login_list[id].ip);
					trade_city_info.insert(login_list[id].city);
				}
			}
		}
		ss << ++feature_index << ":" << log_device_info.size() << " ";
		ss << ++feature_index << ":" << log_ip_info.size() << " ";
		ss << ++feature_index << ":" << log_city_info.size() << " ";
		ss << ++feature_index << ":" << trade_device_info.size() << " ";
		ss << ++feature_index << ":" << trade_ip_info.size() << " ";
		ss << ++feature_index << ":" << trade_city_info.size() << " ";
		ss << ++feature_index << ":" << login_cnt << " ";
		ss << ++feature_index << ":" << trade_cnt << " ";*/


/*		log_device_info.clear();
		log_ip_info.clear();
		log_city_info.clear();
		trade_device_info.clear();
		trade_ip_info.clear();
		trade_city_info.clear();
		login_cnt = 0;
		trade_cnt = 0;
		for(int id = 0; id < index; id++) {
			if((login_list[index].time_stamp - login_list[id].time_stamp) < 60 
			&& (login_list[index].time_stamp - login_list[id].time_stamp) > 30) {
				log_device_info.insert(login_list[id].device);
				log_ip_info.insert(login_list[id].ip);
				log_city_info.insert(login_list[id].city);
				login_cnt++;
				if(login_list[id].trade_vec.size() > 0) {
					trade_cnt += login_list[id].trade_vec.size();
					trade_device_info.insert(login_list[id].device);
					trade_ip_info.insert(login_list[id].ip);
					trade_city_info.insert(login_list[id].city);
				}
			}
		}
		ss << ++feature_index << ":" << log_device_info.size() << " ";
		ss << ++feature_index << ":" << log_ip_info.size() << " ";
		ss << ++feature_index << ":" << log_city_info.size() << " ";
		ss << ++feature_index << ":" << trade_device_info.size() << " ";
		ss << ++feature_index << ":" << trade_ip_info.size() << " ";
		ss << ++feature_index << ":" << trade_city_info.size() << " ";
		ss << ++feature_index << ":" << login_cnt << " ";
		ss << ++feature_index << ":" << trade_cnt << " ";*/
	}else {
		feature_index += 32;
	}
	//cout << feature_index << " ";

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
//
		if(login_list[before_index - 1].type == login.type) {
			ss << ++feature_index << ":1" << " ";
		}else
			++feature_index;

		ss << ++feature_index << ":" << time_stamp - login_list[before_index - 1].time_stamp << " ";

		ss << ++feature_index << ":" << login.time_stamp - login_list[before_index - 1].time_stamp << " ";
		ss << ++feature_index << ":" << max_time_long + login.time_long - login_list[before_index - 1].time_long << " ";
		//ss << ++feature_index << ":" << login.time_long - login_list[before_index - 1].time_long << " ";
		
		//ss << ++feature_index << ":" << login.is_scan - login_list[before_index - 1].is_scan << " ";
		//ss << ++feature_index << ":" << login.is_sec - login_list[before_index - 1].is_sec << " ";


		//ss << ++feature_index << ":" << (login.device == login_list[before_index - 1].device) << " ";
		//ss << ++feature_index << ":" << (login.ip == login_list[before_index - 1].ip) << " ";
		//ss << ++feature_index << ":" << (login.city == login_list[before_index - 1].city) << " ";

		//ss << ++feature_index << ":" << (login.type == login_list[before_index - 1].type) << " ";
//
	}else {
		feature_index += 9;
	}
//108
	//cout << "feature index " << feature_index << endl;
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
		ss << ++feature_index << ":" << min_diff << " ";
		ss << ++feature_index << ":" << avg_diff << " ";
	}else {
		feature_index += 5;
	}
//111
	if(index > 0) {
		long time_diff = 0;
		int con_login_cnt = 0;
		for(int id = index-1; id >= max(0, index - 5); id--) {
			if(login_list[index].device == login_list[id].device
				&& login_list[index].ip == login_list[id].ip
				&& login_list[index].city == login_list[id].city
				&& login_list[index].result == login_list[id].result) {
				if(time_diff <= 0)
					time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				con_login_cnt++;
			}else {
				break;
			}
		}
		ss << ++feature_index << ":" << time_diff << " ";
		ss << ++feature_index << ":" << con_login_cnt << " ";

		time_diff = 0;
		con_login_cnt = 0;
		for(int id = index-1; id >= max(0, index - 5); id--) {
			if(login_list[index].device == login_list[id].device
				&& login_list[index].ip == login_list[id].ip
				&& login_list[index].city == login_list[id].city
				&& login_list[index].result != login_list[id].result) {
				if(time_diff <= 0)
					time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				con_login_cnt++;
			}else {
				break;
			}
		}
		ss << ++feature_index << ":" << time_diff << " ";
		ss << ++feature_index << ":" << con_login_cnt << " ";

		time_diff = 0;
		con_login_cnt = 0;
		for(int id = index-1; id >= max(0, index - 5); id--) {
			if(login_list[index].device != login_list[id].device
				&& login_list[index].ip == login_list[id].ip
				&& login_list[index].city == login_list[id].city
				&& login_list[index].result == login_list[id].result) {
				if(time_diff <= 0)
					time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				con_login_cnt++;
			}else {
				break;
			}
		}
		ss << ++feature_index << ":" << time_diff << " ";
		ss << ++feature_index << ":" << con_login_cnt << " ";

		time_diff = 0;
		con_login_cnt = 0;
		for(int id = index-1; id >= max(0, index - 5); id--) {
			if(login_list[index].device == login_list[id].device
				&& login_list[index].ip != login_list[id].ip
				&& login_list[index].city == login_list[id].city
				&& login_list[index].result == login_list[id].result) {
				if(time_diff <= 0)
					time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
				con_login_cnt++;
			}else {
				break;
			}
		}
		ss << ++feature_index << ":" << time_diff << " ";
		ss << ++feature_index << ":" << con_login_cnt << " ";

		set<string> device_set;
		set<string> ip_set;
		long device_time_diff = 0, ip_time_diff = 0;
		int time_long_cnt = 0;
		for(int id = max(0, index - 5); id < index; id++) {
			if(login_list[id].city == login_list[index].city
				&& login_list[id].device != login_list[index].device) {
				device_set.insert(login_list[id].device);
				device_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
			}
			if(login_list[id].city == login_list[index].city
				&& login_list[id].ip != login_list[index].ip) {
				ip_set.insert(login_list[id].ip);
				ip_time_diff = login_list[index].time_stamp - login_list[id].time_stamp;
			}
			if(login_list[id].time_long > (time_stamp - login_list[id].time_stamp)) {
				time_long_cnt++;
			}
		}
		ss << ++feature_index << ":" << time_long_cnt << " ";
		//ss << ++feature_index << ":" << device_set.size() << " ";
		//ss << ++feature_index << ":" << device_time_diff << " ";
		//ss << ++feature_index << ":" << ip_set.size() << " ";
		//ss << ++feature_index << ":" << ip_time_diff << " ";
	}else {
		feature_index += 9;
	}

//122
	int log_from_index = (discrete_map.at("log_from")).at(login.log_from);//11
	ss << 1 + log_from_index + feature_index << ":1" << " ";
	feature_index += (discrete_map.at("log_from")).size();

	int result_index = (discrete_map.at("result")).at(login.result);//10
	ss << 1 + result_index + feature_index << ":1" << " ";
	feature_index += (discrete_map.at("result")).size();

	int type_index = (discrete_map.at("type")).at(login.type);//3
	ss << 1 + type_index + feature_index << ":1" << " ";
	feature_index += (discrete_map.at("type")).size();

	if(index > 0) {
		log_from_index = (discrete_map.at("log_from")).at(login_list[index - 1].log_from);
		ss << 1 + log_from_index + feature_index << ":1" << " ";
		feature_index += (discrete_map.at("log_from")).size();//11

		result_index = (discrete_map.at("result")).at(login_list[index - 1].result);
		ss << 1 + result_index + feature_index << ":1" << " ";
		feature_index += (discrete_map.at("result")).size();//10

		type_index = (discrete_map.at("type")).at(login_list[index - 1].type);
		ss << 1 + type_index + feature_index << ":1" << " ";
		feature_index += (discrete_map.at("type")).size();//3
	}else {
		feature_index += (discrete_map.at("log_from")).size() 
					+ (discrete_map.at("result")).size() 
					+ (discrete_map.at("type")).size();
	}
/*
	if(before_index >= 0) {
		log_from_index = (discrete_map.at("log_from")).at(login_list[before_index].log_from);
		ss << 1 + log_from_index + feature_index << ":1" << " ";
		feature_index += (discrete_map.at("log_from")).size();//11

		result_index = (discrete_map.at("result")).at(login_list[before_index].result);
		ss << 1 + result_index + feature_index << ":1" << " ";
		feature_index += (discrete_map.at("result")).size();//10

		type_index = (discrete_map.at("type")).at(login_list[before_index].type);
		ss << 1 + type_index + feature_index << ":1" << " ";
		feature_index += (discrete_map.at("type")).size();//3
	}else {
		feature_index += (discrete_map.at("log_from")).size() 
					+ (discrete_map.at("result")).size() 
					+ (discrete_map.at("type")).size();
	}*/

/*	int city_index = (discrete_map.at("city")).at(login.city);
	ss << city_index + feature_index << ":1" << " ";
	feature_index += (discrete_map.at("city")).size();

	if(index > 0) {
		int city_index = (discrete_map.at("city")).at(login_list[index-1].city);
		ss << city_index + feature_index << ":1" << " ";
		feature_index += (discrete_map.at("city")).size();
	}else {
		feature_index += (discrete_map.at("city")).size();
	}*/

	//cout << feature_index << " " << user_id << " " << login.time_stamp << endl;

/*	if((time_stamp < 1433088000L) || (time_stamp > 1433260800L && time_stamp < 1434124800L)
		|| (time_stamp > 1434729600L && time_stamp < 1435680000L)){
		int rand_key = rand()%100;
		if(rand_key < 70) {
			if(label) {
				train_pos++;
				case1_pos++;
			}
			else {
				train_neg++;
				case1_neg++;
			}
			train_out_file << ss.str() << endl;
		}else if (rand_key < 90) {
			if(label)
				test_pos++;
			else
				test_neg++;
			ss << "0:" << current.row_key << " ";
			test_out_file << ss.str() << endl;
		}else {
			if(label)
				valid_pos++;
			else
				valid_neg++;
			ss << "0:" << current.row_key << " ";
			valid_out_file << ss.str() << endl;
		}
	}else if(time_stamp > 1435680000L){
		ss << "0:" << current.row_key << " ";
		predict_out_file << ss.str() << endl;
	}*/

	if((time_stamp < 1433088000L)){
		if(label) {
			train_pos++;
			case1_pos++;
		}
		else {
			train_neg++;
			case1_neg++;
		}
		train_out_file << ss.str() << endl;
	} else if(time_stamp > 1433260800L && time_stamp < 1434124800L){
		if(label)
			test_pos++;
		else
			test_neg++;
		ss << "0:" << current.row_key << " ";
		test_out_file << ss.str() << endl;
	} else if(time_stamp > 1434729600L && time_stamp < 1435680000L){
		if(label)
			valid_pos++;
		else
			valid_neg++;
		ss << "0:" << current.row_key << " ";
		test_out_file << ss.str() << endl;
		valid_out_file << ss.str() << endl;
	}else if(time_stamp > 1435680000L){
		ss << "0:" << current.row_key << " ";
		predict_out_file << ss.str() << endl;
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

	if(time_stamp > 1433088000L)
		ss << "0:" << data.row_key << " ";

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

	if(DEBUG_TEST) {
		cout << ss.str() << endl;
	}

/*	if((time_stamp < 1433088000L) || (time_stamp > 1433260800L && time_stamp < 1434124800L)
		|| (time_stamp > 1434729600L && time_stamp < 1435680000L)){
		int rand_key = rand()%100;
		if(rand_key < 70) {
			if(label) {
				train_pos++;
				case1_pos++;
			}
			else {
				train_neg++;
				case1_neg++;
			}
			train_out_file << ss.str() << endl;
		}else if (rand_key < 90) {
			if(label)
				test_pos++;
			else
				test_neg++;
			ss << "0:" << current.row_key << " ";
			test_out_file << ss.str() << endl;
		}else {
			if(label)
				valid_pos++;
			else
				valid_neg++;
			ss << "0:" << current.row_key << " ";
			valid_out_file << ss.str() << endl;
		}
	}else if(time_stamp > 1435680000L){
		ss << "0:" << current.row_key << " ";
		predict_out_file << ss.str() << endl;
	}*/

	if(time_stamp < 1433088000L){
		if(label) {
			train_pos++;
			case2_pos++;
		}
		else {
			train_neg++;
			case2_neg++;
		}
		train_out_file << ss.str() << endl;
	} else if(time_stamp > 1433260800L && time_stamp < 1434124800L){
		if(label)
			test_pos++;
		else
			test_neg++;
		ss << "0:" << data.row_key << " ";
		test_out_file << ss.str() << endl;
	} else if(time_stamp > 1434729600L && time_stamp < 1435680000L){
		if(label)
			valid_pos++;
		else
			valid_neg++;
		ss << "0:" << data.row_key << " ";
		test_out_file << ss.str() << endl;
		valid_out_file << ss.str() << endl;
	}else if(time_stamp > 1435680000L){
		ss << "0:" << data.row_key << " ";
		predict_out_file << ss.str() << endl;
	}
}

bool transfer_data() {
	map<string, int> detail_trade_map1;
	map<string, int> detail_trade_map2;

	int size_num = 0, total_num = 0;
	cout << "begin to generate sample" << endl;
	for(map<string, set<trade_data> >::iterator it = trade_info.begin(); it != trade_info.end(); ++it) {
		string user_id = it->first;
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
				generate_zero(trade, i, trade_list);
				size_num++;
			}
			continue;
		}
		set<login_data> login_detail = login_info.find(it->first)->second;
		
		vector<login_data> login_list;
		for(set<login_data>::iterator iter = login_detail.begin(); iter != login_detail.end(); ++iter) {
			if(login_list.size() > 0 && login_list[login_list.size() - 1] == (*iter)) 
				continue;
			//update_total_map(*iter, user_id);
			login_list.push_back(*iter);
		}
		int i = 0;
		int index = 0, befor_index = 0;
		clear_history_map();
		//total_device_map.clear();
		//total_ip_map.clear();
		//set<login_data>::iterator total_it = total_login_map.begin();
		int trade_cnt = 0;
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
/*				for(int j = 0; j < login_list[index].trade_vec.size(); j++) {
					if(login_list[index].trade_vec[j].label == 0 
						&& (trade.time_stamp - login_list[index].trade_vec[j].time_stamp) > 3600*24*30) {
						update_history_valid(login_list[index]);
					}else if(login_list[index].trade_vec[j].label == 1 
						&& (trade.time_stamp - login_list[index].trade_vec[j].time_stamp) > 3600*24*30) {
						update_history_black(login_list[index]);
					}
				}*/
				if(DEBUG_TEST) {
					out_login(login_list[i]);
					cout << " " << endl;
				}
			}
			if(DEBUG_TEST) {
				out_trade(trade);
				cout << " " << endl;
			}
			if(i == 0) {
				size_num++;
				generate_zero(trade, trade_cnt, trade_list);
			}
			if(i == 0)
				continue;
			//cout << "update " << endl;
			update_history_trade(login_list[index]);
			login_list[index].trade_vec.push_back(trade);
/*			for(; total_it != total_login_map.end(); total_it++) {
				if((*total_it).time_stamp < trade.time_stamp) {
					update_total_device_map(*total_it, (*total_it).user_id);
					update_total_ip_map(*total_it, (*total_it).user_id);
				}
			}*/
			//cout << "generate " << endl;
			generate_sample(login_list, i - 1, index, user_id);
			size_num++;
		}
		for(i = 0; i < login_list.size(); i++) {
			if(login_list[i].trade_vec.size() > 0) {
				std::stringstream ss;
				ss << login_list[i].device;
				string trade_info = ss.str();
				for(int j = 0; j < login_list[i].trade_vec.size(); j++) {
					if(detail_trade_map2.find(trade_info) == detail_trade_map2.end()) {
						detail_trade_map2.insert(make_pair(trade_info, 1));
					}else {
						map<string, int>::iterator itt = detail_trade_map2.find(trade_info);
						int num = itt->second + 1;
						itt->second = num;
					}
					if(login_list[i].trade_vec[j].label > 0) {
						if(detail_trade_map1.find(trade_info) == detail_trade_map1.end()) {
							detail_trade_map1.insert(make_pair(trade_info, 1));
						}else {
							map<string, int>::iterator itt = detail_trade_map1.find(trade_info);
							int num = itt->second + 1;
							itt->second = num;
						}
					}
				}
			}
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
		trade_vec.insert(make_pair(user_id, trade_list));
	}
	trade_info.clear();
	for(map<string, set<login_data> >::iterator it = login_info.begin(); it != login_info.end(); ++it) {
		string user_id = it->first;
		set<login_data> login_detail = it->second;
		vector<login_data> login_list;
		for(set<login_data>::iterator iter = login_detail.begin(); iter != login_detail.end(); ++iter) {
			login_list.push_back(*iter);
		}
		login_vec.insert(make_pair(user_id, login_list));
	}
	login_info.clear();
}

bool transfer_data2() {
	int size_num = 0;
	cout << "begin to generate sample" << endl;

	transfer_login_and_trade();

	set<login_data>::iterator total_login_it = total_login_map.begin();
	map<string, int> user_trade_cnt_map;
	long last_time_stamp = 0;
	set<login_data>::iterator trade_login_it = total_login_map.begin();
	for(set<trade_data>::iterator total_trade_it = total_trade_map.begin(); 
				total_trade_it != total_trade_map.end(); ++total_trade_it) {
		trade_data current = *total_trade_it;
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

/*		for(; trade_login_it != total_login_map.end(); trade_login_it++) {
			if((*trade_login_it).time_stamp < current.time_stamp - 3600*24*30) {
				for(int j = 0; j < (*trade_login_it).trade_vec.size(); j++) {
					if((*trade_login_it).trade_vec[j].label == 0) {
						update_valid_total_device_map(*trade_login_it, (*trade_login_it).user_id);
						update_valid_total_ip_map(*trade_login_it, (*trade_login_it).user_id);
					}else if((*trade_login_it).trade_vec[j].label == 1) {
						update_black_total_device_map(*trade_login_it, (*trade_login_it).user_id);
						update_black_total_ip_map(*trade_login_it, (*trade_login_it).user_id);
					}
				}
			}else {
				break;
			}
		}*/

		vector<trade_data> trade_list = trade_vec.at(user_id);
		int trade_index = 0;
		if(user_trade_cnt_map.find(user_id) == user_trade_cnt_map.end()) {
			user_trade_cnt_map.insert(make_pair(user_id, 1));
		}else{
			map<string, int>::iterator it = user_trade_cnt_map.find(user_id);
			trade_index = it->second;
			it->second = trade_index + 1;
		}
		
		if(login_vec.find(user_id) == login_vec.end()) {
			generate_zero(current, trade_index, trade_list);
			size_num++;
			continue;
		}
		map<string, vector<login_data> >::iterator login_it = login_vec.find(user_id);
		vector<login_data> login_list = login_it->second;

		clear_history_map();
		int index = 0;
		int login_index = 0;
		for(; login_index < login_list.size(); login_index++) {
			if(login_list[login_index].time_stamp > current.time_stamp)
				break;
			if(login_list[login_index].result == "1") {
				update_history_login(login_list[login_index]);
				index = login_index;
				for(int j = 0; j < login_list[index].trade_vec.size(); j++) {
					if(login_list[index].trade_vec[j].label == 0 
						&& (current.time_stamp - login_list[index].trade_vec[j].time_stamp) > 3600*24*30) {
						update_history_valid(login_list[index]);
					}else if(login_list[index].trade_vec[j].label == 1 
						&& (current.time_stamp - login_list[index].trade_vec[j].time_stamp) > 3600*24*30) {
						update_history_black(login_list[index]);
					}
					update_history_trade(login_list[index]);
				}
			}
			if(DEBUG_TEST) {
				out_login(login_list[login_index]);
				cout << " " << endl;
			}
		}
		if(login_index == 0) {
			generate_zero(current, trade_index, trade_list);
			size_num++;
			continue;
		}

		login_list[index].trade_vec.push_back(current);
		login_it->second = login_list;
		for(int id = 0; id < (login_it->second)[index].trade_vec.size(); id++) {
			if(DEBUG_TEST) {
				out_trade((login_it->second)[index].trade_vec[id]);
				cout << " " << endl;
			}
		}
		generate_sample(login_list, login_index - 1, index, user_id);
		size_num++;
		//update_trade_total_device_map(login_list[index], login_list[index].user_id);
		//update_trade_total_ip_map(login_list[index], login_list[index].user_id);
	}
	cout << "finish generate sample, total sample size " << size_num << endl;
}

int main(int argc, char **argv)
{
	srand((unsigned)time(NULL));
	string login_path = argv[1];
	string trade_path = argv[2];
	train_or_test = atoi(argv[3]);
	if(argc > 4) {
		test_user = argv[4];
		DEBUG_TEST = true;
	}else {
		DEBUG_TEST = false;
	}

	init_discrete_map();
	cout << "file cache is " << is_cache << endl;
	load_login_data(login_path);

	load_trade_data(trade_path);

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
}