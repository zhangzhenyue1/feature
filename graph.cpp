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
#include <unordered_map>

#define N 9999
#define DOUBLE_MAX 1.79e+308
#define DOUBLE_MIN -2.22e-308
#define Name(X) #X
#define Out(X) {string xname = Name(X); cout << xname.substr(5, xname.size()-1) << ": " << X << " ";}
using namespace std;

bool is_cache = false;

unordered_map<string, unordered_map<string, int> > device_user_map;
unordered_map<string, vector<string> > user_device_vector;
unordered_map<string, unordered_map<string, int> > device_device_map;
unordered_map<string, int> device_graph;

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
		unordered_map<string, int> tmp;
		while(!file_in.eof()) {
			file_in >> value_key >> value_value;
			//cout << value_key << " " << value_value << " ";
			if(value_key == "sum")
				continue;
			if(value_key == "0" && value_value == 0)
				break;
			tmp.insert(make_pair(value_key, value_value));
		}
		device_user_map.insert(make_pair(key, tmp));
		
	}
	if(device_user_map.size() > 0) {
		cout << "device size " << device_user_map.size() << endl;
		cout << "835072 " << device_user_map.at("835072").size() << endl;
		is_cache = true;
	}else {
		is_cache = false;
	}
	return true;
}

bool get_user_device_vector() {
	for(unordered_map<string, unordered_map<string, int> >::iterator it = device_user_map.begin(); 
			it != device_user_map.end(); it++) {
		string device_id = it->first;
		unordered_map<string, int> device_map = it->second;
		if(device_map.size() < 2)
			continue;
		for(unordered_map<string, int>::iterator mp_it = device_map.begin(); mp_it != device_map.end(); mp_it++) {
			string user_id = mp_it->first;
			unordered_map<string, vector<string> >::iterator st_it = user_device_vector.find(user_id);
			if(st_it == user_device_vector.end()) {
				vector<string> device;
				device.push_back(device_id);
				user_device_vector.insert(make_pair(user_id, device));
			}else {
				vector<string> device = st_it->second;
				device.push_back(device_id);
				st_it->second = device;
			}
		}
	}
	cout << "user size " << user_device_vector.size() << endl;
}

bool calc_device_device_map() {
	int cnt = 0;
	for(unordered_map<string, vector<string> >::iterator it = user_device_vector.begin(); it != user_device_vector.end(); it++) {
		vector<string> device_vec = it->second;
		cnt += device_vec.size();
		for(int i =0; i < device_vec.size(); i++) {
			for(int j = i+1; j < device_vec.size(); j++) {
				string device1 = device_vec[i];
				string device2 = device_vec[j];

				unordered_map<string, unordered_map<string, int> >::iterator it1 = device_device_map.find(device1);
				if(it1 == device_device_map.end()) {
					unordered_map<string, int> tmp;
					tmp.insert(make_pair(device2, 1));
					device_device_map.insert(make_pair(device1, tmp));
				}else {
					unordered_map<string, int> tmp = it1->second;
					unordered_map<string, int>::iterator it2 = tmp.find(device2);
					if(it2 == tmp.end()) {
						tmp.insert(make_pair(device2, 1));
					}else {
						int size = it2->second;
						size = size + 1;
						it2->second = size;
					}
					it1->second = tmp;
				}

				unordered_map<string, unordered_map<string, int> >::iterator it_1 = device_device_map.find(device2);
				if(it_1 == device_device_map.end()) {
					unordered_map<string, int> tmp;
					tmp.insert(make_pair(device1, 1));
					device_device_map.insert(make_pair(device2, tmp));
				}else {
					unordered_map<string, int> tmp = it_1->second;
					unordered_map<string, int>::iterator it2 = tmp.find(device1);
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
	cout << "device total " << cnt << endl;
}

bool init_device_graph() {
	int index = 10;
	for(unordered_map<string, unordered_map<string, int> >::iterator it = device_device_map.begin(); 
			it != device_device_map.end(); ++it) {
		string device_id = it->first;
		device_graph.insert(make_pair(device_id, index));
		index++;
	}
}

bool calc_continus_graph() {
	bool status = true;
	int round_id = 0;
	while(status) {
		round_id++;
		cout << "calc graph, round " << round_id << endl;
		status = false;
		for(unordered_map<string, unordered_map<string, int> >::iterator it = device_device_map.begin(); 
				it != device_device_map.end(); ++it) {
			unordered_map<string, int>::iterator graph_it = device_graph.find(it->first);
			int index = graph_it->second;
			unordered_map<string, int> tmp = it->second;
			for(unordered_map<string, int>::iterator iter = tmp.begin(); 
					iter != tmp.end(); ++iter) {
				if(device_graph.at(iter->first) < index) {
					status = true;
					index = device_graph.at(iter->first);
				}
			}
			graph_it->second = index;
		}
		//if(round_id > 2)
		//	break;
	}
	int size_tmp = device_graph.at("835072");
	map<int, int> graph_map;
	for(unordered_map<string, int>::iterator iter = device_graph.begin(); 
					iter != device_graph.end(); ++iter) {
		int index = iter->second;
		//if(index != size_tmp)
		//	cout << "debug " << iter->first << " " << iter->second << endl;
		map<int, int>::iterator it = graph_map.find(index);
		if(it == graph_map.end()) {
			graph_map.insert(make_pair(index, 1));
		}else{
			int size = it->second;
			it->second = size + 1;
		}
	}

	for(map<int, int>::iterator iter = graph_map.begin(); iter != graph_map.end(); iter++) {
		cout << "graph index " << iter->first << ", graph size " << iter->second << endl;
	}
}

bool write_device_device_map(string path) {
	ofstream file_out(path.c_str());
	std::stringstream ss;
	for(unordered_map<string, unordered_map<string, int> >::iterator it = device_device_map.begin(); 
			it != device_device_map.end(); ++it) {
		file_out << it->first << " ";
		for(unordered_map<string, int>::iterator iter = (it->second).begin(); 
			iter != (it->second).end(); ++iter) {
			file_out << iter->first << " " << iter->second << " ";
		}
		file_out << "0 0";
		file_out << " " << endl;
	}
	cout << "save file " << path << ", size " << device_device_map.size() << endl;

	ofstream graph_file((path + "_graph").c_str());
	for(unordered_map<string, int>::iterator it = device_graph.begin(); it != device_graph.end(); it++) {
		graph_file << it->first << " " << it->second << endl;
	}
}

int main(int argc, char **argv)
{
	srand((unsigned)time(NULL));
	string input_file = argv[1];
	string output_file = argv[2];
	cout << "upload data" << endl;
	upload_device_map(input_file);

	cout << "get user data" << endl;
	get_user_device_vector();

	cout << "get device device map" << endl;
	calc_device_device_map();

	cout << "init graph" << endl;
	init_device_graph();

	cout << "calculate graph" << endl;
	calc_continus_graph();

	cout << "write file" << endl;
	write_device_device_map(output_file);
}