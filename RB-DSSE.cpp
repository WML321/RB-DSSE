// RB-DSSE.cpp: 定义应用程序的入口点。
//

#include "RB-DSSE.h"
#include "Utils.h"
#include "Client.h"
#include "Server.h"
#include <chrono>
using namespace std;
string Ks;

void Search_Client(Client &client, Server &server) {
	auto start = std::chrono::high_resolution_clock::now();
	string tw = Utils::F(Ks, client.w);
	pair<string, int> ST;
	try
	{
		ST = Client::SUM.at(client.w);
	}
	catch (const std::exception&)
	{
		std::cout << "Value for key1: " << client.w << std::endl;
		return;
	}

	server.setTw(tw);
	server.setSt_ct(ST.first);
	server.setCt(ST.second);
	auto end = std::chrono::high_resolution_clock::now();
	auto duration_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	cout << "Search_Client: " << duration_milliseconds.count() << "ms" << endl;
	return;
}
void Search_Server(Client &client, Server &server) {
	auto start = std::chrono::high_resolution_clock::now();
	try {
		set<string> I;
		string st_i = server.st_ct;
		for (int i = server.ct; i > 0; i++) {
			string u = Utils::H(server.tw + Utils::num2str(i));
			tuple<string, string, string> ele = Server::EDB[u];
			string k_i = Utils::newxor(get<1>(ele), Utils::H(st_i + Utils::num2str(i)));
			string out;
			client.kuprf.Eval(reinterpret_cast<unsigned char*>(const_cast<char*>(out.c_str())), reinterpret_cast<unsigned char*>(const_cast<char*>(client.Kw.c_str())), st_i, op_srh);
			string e_i = Utils::newxor(get<0>(ele), out);
			if (e_i.find(',') == string::npos) {
				st_i = Utils::P_(k_i, st_i);
				continue;
			}
			vector<string> detail = Utils::split(e_i, ','); // 0为op，info为其他
			string op = detail[0], info = detail[1];
			if (op == "add") {
				I.insert(info);
			}
			else {
				string st_ind = st_i;
				int times = 0;
				while (st_ind != ST_0) {
					string f1;
					client.kuprf.Eval(reinterpret_cast<unsigned char*>(const_cast<char*>(f1.c_str())), reinterpret_cast<unsigned char*>(const_cast<char*>(client.Kw.c_str())), st_ind, op_del);
					string u = Utils::H(server.tw + st_ind);
					tuple<string, string, string> ele = Server::EDB[u];
					st_ind = Utils::newxor(get<2>(ele), f1);
					string out;
					client.kuprf.update(reinterpret_cast<unsigned char*>(const_cast<char*>(out.c_str())), reinterpret_cast<unsigned char*>(const_cast<char*>(info.c_str())), reinterpret_cast<unsigned char*>(const_cast<char*>(f1.c_str())));

					tuple<string, string, string> new_ele = make_tuple(Utils::newxor(get<0>(ele), out), get<0>(ele), Utils::newxor(get<2>(ele), out));

					Server::EDB[u] = new_ele;
					times++;
					if (times >= 2) {
						st_ind = ST_0;
					}
				}
			}
		}
		client.I = I;
	}
	catch (const std::exception& e) {
		cout <<"error";
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	cout << "Search_Server: " << duration_milliseconds.count() << "ms" << endl;
	
}

void Update_Client(Client &client, Server &server) {
	auto start = std::chrono::high_resolution_clock::now();
	pair<string, int> ele = Client::SUM[client.w];
	string st_ct;
	int ct;

	try
	{
		ele = Client::SUM.at(client.w);
		ct = ele.second;
		st_ct = ele.first;
	}
	catch (const std::exception&)
	{
		ct = 0;
		st_ct = ST_0;
	}

	try {

		string tw = Utils::F(Ks, client.w);
		string k_ct_1 = Utils::rand_str(LAMBDA);
		string st_ct_1 = Utils::P(k_ct_1, Utils::num2str(ct + 1));
		string e_ct_1;
		if (client.op == "add") {
			string out;
			client.kuprf.Eval(reinterpret_cast<unsigned char*>(const_cast<char*>(out.c_str())), reinterpret_cast<unsigned char*>(const_cast<char*>(client.Kw.c_str())), st_ct_1, op_srh);
			e_ct_1 = Utils::newxor(out, ("add" + string(",") + client.ind));
		}
		else {
			if (client.Kw_ == "" or client.op == "del") {
				client.randomKw_();
			}
			string delta;
			client.kuprf.update_token(reinterpret_cast<unsigned char*>(const_cast<char*>(delta.c_str())),
				reinterpret_cast<unsigned char*>(const_cast<char*>(client.Kw.c_str())),
				reinterpret_cast<unsigned char*>(const_cast<char*>(client.Kw_.c_str())));

			string out;
			client.kuprf.Eval(reinterpret_cast<unsigned char*>(const_cast<char*>(out.c_str())), reinterpret_cast<unsigned char*>(const_cast<char*>(client.Kw.c_str())), st_ct_1, op_srh);
			string e_ct_1 = Utils::newxor(out, (client.op + "," + delta));
		}
		k_ct_1 = Utils::newxor(k_ct_1, Utils::H(st_ct_1 + Utils::num2str(ct + 1)));
		pair<string, string> ele_ = Client::T[client.ind];
		string st_ind_ct = ele_.first;
		string c_ind_1_ct = ele_.second; // 1_ct  ct-1; ct_1 ct+1
		string out;
		client.kuprf.Eval(reinterpret_cast<unsigned char*>(const_cast<char*>(out.c_str())), reinterpret_cast<unsigned char*>(const_cast<char*>(client.Kw.c_str())), st_ct_1, op_srh);
		string c_ind_ct = Utils::newxor(out, Utils::H(st_ind_ct));
		if (client.op == "del") {
			Client::T[client.ind] = make_pair(ST_0, 0);
		}
		else if (client.op == "add") {
			Client::T[client.ind] = make_pair(st_ct_1, c_ind_ct);
		}
		else {
			string out;
			client.kuprf.Eval(reinterpret_cast<unsigned char*>(const_cast<char*>(out.c_str())), reinterpret_cast<unsigned char*>(const_cast<char*>(client.Kw.c_str())), st_ind_ct, op_rol);
			Client::T[client.ind] = make_pair(Utils::newxor(out, c_ind_1_ct), 0);
		}

		string u = Utils::H(tw + st_ct_1);
		server.u = u;
		server.e = e_ct_1;
		server.k = k_ct_1;
		server.c = c_ind_ct;
	}
	catch (const std::exception& e) {
		cout << "error" << endl;
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	cout << "Update_Client: " << duration_milliseconds.count() << "ms" << endl;
	return;

}

void Update_Server(Client& client, Server& server) {
	auto start = std::chrono::high_resolution_clock::now();
	Server::EDB[server.u] = make_tuple(server.e, server.k, server.c);
	auto end = std::chrono::high_resolution_clock::now();
	auto duration_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	cout << "Update_Server: " << duration_milliseconds.count() << "ms" << endl;
	return;
}

int main()
{
	cout << "Hello CMake." << endl;
	return 0;
}
