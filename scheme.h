#pragma once
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class scheme
{
private:
	vector<string> attributes;
public:
	scheme() = default;
	scheme(vector<string> names) : attributes(names) {}
	vector<string> getAttributes() { return attributes; }
	string toString()
	{
		stringstream out;
		out << attributes.size() << endl;
		for (unsigned int i = 0; i < attributes.size(); i++)
		{
			out << attributes.at(i) << endl;
		}
		return out.str();
	}
};