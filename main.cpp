#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>

using namespace std;

vector<string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str);
	string tok;
	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}
	return internal;
}

string readFile(const char* path) {
	ifstream ifs(path, ios::binary);
	if(ifs.fail()) return "0";
	string s = "";
	while (!ifs.eof()){
		int result;
		ifs.read(reinterpret_cast<char*>(&result), sizeof(char));
		s.push_back(result);
	}
	s.erase(s.size() - 1, 1);
	ifs.close();
	return s;
}

string getTime() {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%H%M%S", timeinfo);
	string str(buffer);
	return str;
}

int main(int argc, char* argv[]) {
	if (argc < 2 || argc > 2){
		cerr << "Usage: " << argv[0] << " triFileName.txt" << endl;
		system("pause");
		return 1;
	}
	vector<vector<int> > tri;
	vector<int> track;
	string init = readFile(argv[1]);
	if (init == "0"){
		cerr << "error opening " << argv[1] << " file\n";
		system("pause");
		return 1;
	}
	vector<string> initV = split(init, '\n');
	vector<vector<string> > tri_s;
	tri_s.resize(initV.size());
	tri.resize(initV.size());
	for (size_t i = 0; i < initV.size(); i++){
		tri_s[i] = split(initV[i], ' ');
		for (size_t j = 0; j < tri_s[i].size(); j++){
			tri[i].push_back(stoi(tri_s[i][j]));
		}
	}
	
	int lastI1 = initV.size() - 2, lastI2 = 0;
	while (lastI1 >= 0){
		while (lastI2 <= lastI1){
			int _max = max(tri[lastI1 + 1][lastI2], tri[lastI1 + 1][lastI2 + 1]);
			tri[lastI1][lastI2] += _max;
			lastI2++;
		}
		lastI1--;
		lastI2 = 0;
	}

	track.push_back(0);
	lastI1 = 0; lastI2 = 0;
	while (lastI1 < initV.size() - 1){
		int _max = max(tri[lastI1 + 1][lastI2], tri[lastI1 + 1][lastI2 + 1]);
		if (tri[lastI1 + 1][lastI2] == _max) track.push_back(lastI2);		
		else{
			track.push_back(lastI2 + 1);
			lastI2++;
		}
		lastI1++;
	}

	ofstream ofs("result" + getTime() + ".txt");
	for (size_t i = 0; i < tri_s.size(); i++) ofs << "\t " << i;
	ofs << '\n';
	for (size_t i = 0; i < tri_s.size(); i++){
		ofs << track[i] << '\t';
		for (size_t j = 0; j < tri_s[i].size(); j++){
			if (j == track[i]) ofs << '[' << tri_s[i][j] << ']';
			else ofs << ' ' << tri_s[i][j] << ' ';
			if (j != tri_s[i].size() - 1) ofs << '\t';
		}
		ofs << '\n';
	}

	ofs << "\n^\nИндекс пути\n\nМаксимальный путь: " << tri[0][0];
	ofs.close();
	return 0;
}