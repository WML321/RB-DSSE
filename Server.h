#include"HEAD.h"
#include"KUPRF.h"

using namespace std;

class Server {
public:
	static unordered_map<string, tuple<string, string, string>> EDB;
public:
	string tw, st_ct, u;
	int ct;
	string e, k, c;
public:
	void setTw(string tw) {
		this->tw = tw;
	}
	void setSt_ct(string st) {
		this->st_ct = st;
	}

	void setCt(int ct) {
		this->ct;
	}
};