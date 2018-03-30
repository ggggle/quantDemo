#pragma once

#include <iostream>

using namespace std;

class Cfg
{
public:
	Cfg();
	~Cfg(){}
public:
	string hq_ip, hq_http, info_net;
	int hq_port;
	bool ok;
};