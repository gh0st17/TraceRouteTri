#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#pragma warning(disable : 4996)

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

template<typename T>
size_t count(T i){
	stringstream ss;
	ss << i;
	return ss.str().size();
}

string genW(unsigned i){
	string s = "";
	for (; i > 0; i--){
		s += ' ';
	}
	return s;
}

void initTri(vector<vector<__int64> > &tri, vector<vector<string> > &tri_s, vector<string> &initV){
	tri_s.resize(initV.size());
	tri.resize(initV.size());
	for (size_t i = 0; i < initV.size(); i++){
		tri_s[i] = split(initV[i], ' ');
		for (size_t j = 0; j < tri_s[i].size(); j++){
			tri[i].push_back(stoi(tri_s[i][j]));
		}
	}
}


void traceRoute(vector<vector<__int64> >	&tri,
				unsigned					_size){
	unsigned lastI1 = _size - 2, lastI2 = 0;
	while ((int)lastI1 >= 0){
		while (lastI2 <= lastI1){
			tri[lastI1][lastI2] += max(tri[lastI1 + 1][lastI2], tri[lastI1 + 1][lastI2 + 1]);
			lastI2++;
		}
		lastI1--;
		lastI2 = 0;
	}
}

vector<unsigned> findRoute(vector<vector<__int64> >	&tri,
							unsigned				_size){
	vector<unsigned> track;
	track.push_back(0);
	unsigned lastI1 = 0, lastI2 = 0;
	while (lastI1 < _size - 1){
		if (tri[lastI1 + 1][lastI2] == max(tri[lastI1 + 1][lastI2], tri[lastI1 + 1][lastI2 + 1]))
			track.push_back(lastI2);
		else{
			track.push_back(lastI2 + 1);
			lastI2++;
		}
		lastI1++;
	}
	return track;
}

void writeResult(vector<vector<__int64> >	&tri,
				vector<vector<string> >		&tri_s,
				vector<string>				&initV,
				vector<unsigned>			&track){
	__int64 _max = tri[0][0];
	tri.clear();
	initTri(tri, tri_s, initV);
	ofstream ofs("result" + getTime() + ".txt");
	ofs << genW(count(track.back()) + 1);
	for (size_t i = 0; i < tri.size(); i++) ofs << i << genW(4 - count(i));
	ofs << '\n';
	for (size_t i = 0; i < tri.size(); i++){
		ofs << track[i] << genW(count(track.back()) - count(track[i]));
		for (size_t j = 0; j < tri[i].size(); j++){
			if (j == track[i]) ofs << '[' << (count(tri[i][j]) < 2 ? "0" : "") << tri[i][j] << ']';
			else ofs << ' ' << (count(tri[i][j]) < 2 ? "0" : "") << tri[i][j] << ' ';
			if (j == tri_s[i].size() - 1) ofs << '\n';
		}
	}

	ofs << '\n';
	for (unsigned i = 0; i < count(track.back()); i++) ofs << '^';
	ofs << "\nИндекс пути\n\nМаксимальный путь: " << _max;
	ofs.close();
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "russian");
	if (argc < 2 || argc > 2){
		string exe(argv[0]);
		cerr << "Usage: " << split(exe, '\\').back() << " triFileName.txt" << endl;
		system("pause");
		return 1;
	}
	string init = readFile(argv[1]);
	if (init == "0"){
		cerr << "Error opening " << argv[1] << " file\n";
		system("pause");
		return 1;
	}
	vector<vector<__int64> > tri;
	vector<unsigned> track;
	vector<string> initV = split(init, '\n');
	vector<vector<string> > tri_s;

	initTri(tri, tri_s, initV);
	traceRoute(tri, initV.size());
	track = findRoute(tri, initV.size());

	writeResult(tri, tri_s, initV, track);
	return 0;
}