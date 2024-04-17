#include "HEAD.h"
#include "KUPRF.h"
#include "Utils.h"

using namespace std;

class Client {
public:
	static unordered_map<string, pair<string, int>> SUM;
	static unordered_map<string, pair<string, string>> T;
	string Kw, Ks;
	KUPRF kuprf;

public:
	Client():kuprf() {
		kuprf.key_gen(reinterpret_cast<unsigned char*>(const_cast<char*>(Kw.c_str())));
		Ks = Utils::rand_str(LAMBDA);
		Ks = Utils::rand_str(LAMBDA);
	}
public:
	string w, op, ind, Kw_; // Kw'
	set<string> I;

public:
	void setOp(string op) {
		this->op = op;
	}

	void setW(string w) {
		this->w = w;
	}

	void setInd(string ind) {
		this->ind = ind;
	}

	void randomKw_() {
		Kw_ = Utils::rand_str(LAMBDA);
	}
};

std::unordered_map<string, pair<string, int>> Client::SUM;
std::unordered_map<string, pair<string, string>> Client::T;
