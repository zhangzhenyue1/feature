#include<bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unordered_map>
#define N 1024*1024
#define CONTROL_NUM 50
#define DOUBLE_MAX 1.79e+308
#define DOUBLE_MIN -2.22e-308
#define GAP_THRESHOLD 60

using namespace std;

bool debug_out = true;

string feature_id = "1";
map<string, int> pos_feature_map;
map<string, int> neg_feature_map;

void SplitString(const string& s, vector<string>& v, const string& c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
         
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

void load_data(string input_file) {
	std::ifstream read_data(input_file.c_str());
	if(!read_data) {
		std::cerr << "Data file open failed!" << std::endl;
		exit(-1);
	}
	while(!read_data.eof()) {
		char buf[N];
		read_data.getline(buf, sizeof(buf));
		string line = buf;
		vector<string> feature_list;
		SplitString(line, feature_list, " ");
		if(feature_list.size() < 1)
			continue;
		int label = atoi(feature_list[0].c_str());
		for(int i = 1; i < feature_list.size(); i++) {
			string feature = feature_list[i];
			vector<string> feature_info;
			SplitString(feature, feature_info, ":");
			if(feature_info.size() == 2 && feature_info[0] == feature_id) {
				string value = feature_info[1];
				if(label > 0) {
					map<string, int>::iterator it = pos_feature_map.find(value);
					if(it == pos_feature_map.end()) {
						pos_feature_map.insert(make_pair(value, 1));
					}else {
						int num = it->second;
						it->second = num + 1;
					}
				}else {
					map<string, int>::iterator it = neg_feature_map.find(value);
					if(it == neg_feature_map.end()) {
						neg_feature_map.insert(make_pair(value, 1));
					}else {
						int num = it->second;
						it->second = num + 1;
					}
				}
			}
		}
	}

	for(map<string, int>::iterator it = pos_feature_map.begin(); it != pos_feature_map.end(); it++) {
		cout << "pos key " << it->first << " " << it->second << endl;
	}
	for(map<string, int>::iterator it = neg_feature_map.begin(); it != neg_feature_map.end(); it++) {
		cout << "neg key " << it->first << " " << it->second << endl;
	}
}

int main(int argc, char* argv[]) {
	if(argc < 4) {
		cout << "input args: " << endl;
		return -1;
	}

	string input_file = argv[1];
	string feature_id = argv[2];
	int size = atoi(const_cast<const char *>(argv[3]));


}