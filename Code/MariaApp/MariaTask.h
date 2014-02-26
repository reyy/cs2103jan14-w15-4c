#pragma once
#include<string>
using namespace std;

class MariaTask{
private:
	int type;
	string title;
	string description;
	time_t start;
	time_t end;

public:
	MariaTask(void);
	~MariaTask(void);
};

