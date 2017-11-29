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

map<string, map<string, int> > device_user_map;
map<string, vector<string> > user_device_vector;
map<string, map<string, int> > device_device_map;

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

bool get_user_device_set() {
	for(map<string, map<string, int> >::iterator it = device_user_map.begin(); it != device_user_map.end(); it++) {
		string device_id = it->first;
		map<string, int> device_map = it->second;
		if(device_map.size() < 3)
			continue;
		for(map<string, int>::iterator mp_it = device_map.begin(); mp_it != device_map.end(); mp_it++) {
			string user_id = mp_it->first;
			map<string, vector<string> >::iterator st_it = user_device_vector.find(user_id);
			if(st_it == user_device_vector.end()) {
				vector<string> device;
				device.push_back(device_id);
				user_device_vector.insert(make_pair(user_id, device));
			}else {
				vector<string> device = st_it->second;
				device.push_back(device_id);
				st_it->second = device
			}
		}
	}
}

bool calc_device_device_map() {
	for(map<string, vector<string> >::iterator it = user_device_vector.begin(); it != user_device_vector.end(); it++) {
		vector<string> device_vec = it->second;
		for(int i =0; i < device_vec.size(); i++) {
			for(int j = i+1; j < device_vec.size(); j++) {
				string device1 = device_vec[i];
				string device2 = device_vec[j];

				map<string, map<string, int> >::it1 = device_device_map.find(device1);
				if(it1 == device_device_map.end()) {
					map<string, int> tmp;
					tmp.insert(make_pair(device2, 1));
					device_device_map.insert(make_pair(device1, tmp));
				}else {
					map<string, int> tmp = it1->second;
					map<string, int>::iterator it2 = tmp.find(device2);
					if(it2 == tmp.end()) {
						tmp.insert(make_pair(device2, 1));
					}else {
						int size = it2->second;
						size = size + 1;
						it2->second = size;
					}
					it1->second = tmp;
				}

				map<string, map<string, int> >::it_1 = device_device_map.find(device2);
				if(it_1 == device_device_map.end()) {
					map<string, int> tmp;
					tmp.insert(make_pair(device1, 1));
					device_device_map.insert(make_pair(device2, tmp));
				}else {
					map<string, int> tmp = it_1->second;
					map<string, int>::iterator it2 = tmp.find(device1);
					if(it2 == tmp.end()) {
						tmp.insert(make_pair(device1, 1));
					}else {
						int size = it2->second;
						size = size + 1;
						it2->second = size;
					}
					it_1->second = tmp;
				}
			}
		}
	}
}

bool write_device_device_map(string path) {
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

int main(int argc, char **argv)
{
	srand((unsigned)time(NULL));
	string input_file = argv[1];
	string output_file = argv[2];
	upload_device_map(input_file);

	get_user_device_set();

	calc_device_device_map();

	write_device_device_map(output_file);
}