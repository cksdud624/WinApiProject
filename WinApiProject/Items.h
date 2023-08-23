#pragma once

#include <string>

using namespace std;

class Item
{
private:
	string name;
	int type;
	int num;
	int count;
public:
	string getName() { return name; }
	int getType() { return type; }
	int getNum() { return num; }
	int getCount() { return count; }

	void setName(string name) { this->name = name; }
	void setType(int type) { this->type = type; }
	void setNum(int num) { this->num = num; }
	void setCount(int count) {this->count = count;}
};