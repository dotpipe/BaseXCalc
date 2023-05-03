#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <math.h>
#include <exception>
#include <sstream>

using namespace std;

class air {

	public:
	unsigned int range_size = 256;
	int min_size = 0;
	int max_size = 2;
	int MAX_ALL = 0;
	int MAX_Q = MAX_ALL;
	vector<unsigned long int> answers, results;
	unsigned long long int total = 0;
	size_t y = 0;
	ifstream in_file;
	ofstream out_file;
	bool idleMethod();
	bool airAlgo(unsigned long long int);
	void returnBytes();
	inline unsigned long long int recIdle() {
		// unsigned long long int total = 0;
		unsigned long long int powr = 1;
		for (int i = 0 ; i < answers.size() ; i++) {
			powr = 1;
			for (int u = 0 ; u < i ; u++) {
				powr *= range_size;
			}
			// cout << total << " + (" << powr << "*" << answers[i] << ")\n" << flush;
    		total = total + (powr * answers[i]);
			if (total > pow(2,54))
			{
				while (i > 0 && answers.size() > 0)
				{
					answers.erase(answers.begin());
					i--;
				}
				returnBytes();
				return 0;
				// total = recIdle();
			}
		}
		returnBytes();
		// cout << "Largess: " << total << endl;
		return 0;
	}
	air() {};
	~air() {};
};

bool air::idleMethod() {
	unsigned long long int p = recIdle();
	airAlgo(p);
	int r = 0;
	for (unsigned int x : results) {
		if (x == answers[r]) { }
		else {
			// cout << endl;
			// for (unsigned int e : answers)
			// 	cout << e << " ";
			// cout << endl;
			// for (unsigned int e : results)
			// 	cout << e << " ";
			// cout << endl;
			// cout << "Byte error: " << x << "!=" << answers[r] << " ";// << total << flush;
			return 0;
		}
		r++;
	}
  returnBytes();
	return 1;
}

bool air::airAlgo (unsigned long long int Ti) {
	unsigned long long int n = Ti%range_size, m = 0;
	results.push_back(Ti%range_size);
	int i = 0;
	while (results.size() < MAX_Q) {
		int j = 0;
		for (int x : results) {
			unsigned long long powr = 1;
			for (unsigned int u = 0 ; u < j ; u++)
				powr *= range_size;
			m = m + (powr * x);
			j++;
		}
		n = Ti;
		n -= m;
		m = 0;

		for (int o = 0 ; o < results.size() ; o++)
			n = n/range_size;
		results.push_back(n%range_size);
	}
	return 1;
}

void air::returnBytes()
{
  // cout << total << flush;
	uint64_t p = total;
	string p_Str = "";
	while (p > 0)
	{
		p_Str += (p%256 > 0) ? (char)(p%256) : (char)(0);
		p >>= 8;
	}
	// p_Str += ';';
	out_file.write(p_Str.c_str(), p_Str.length());
}

int main(int argc, char ** argv) {
	air * x = new air();

	if (argc < 2) {
		printf("Usage: \n\t%s -a <maximum> <minimum> <integers...>", argv[0]);
		printf("\n\t%s -c <maximum> <minimum> <no of vars> <long long int>", argv[0]);
		return 0;
	}

	if (strcmp(argv[1],"-a") == 0)
	{
		// for (int i = 2; i < argc; i++)
		// 	x->answers.push_back(stoi(argv[i]));
		// x->MAX_ALL = x->answers.size();
		uint8_t bytes = 64;
		x->in_file.open("test.in", std::ios_base::in);
		x->out_file.open("test.out", std::ios_base::out | std::ios::trunc);
		stringstream k;
		k << x->in_file.rdbuf();
		string inf = k.str();
	  	k.str("");
		x->range_size = x->max_size;
		for (uint32_t n = 0; n < inf.length(); n++)
		{
			while (n%bytes && inf.length() > n+1)
			{
				n++;
				x->answers.push_back(inf.at(n-1));
				if ((unsigned int)(inf.at(n - 1)) > x->max_size)
					x->max_size = (int)(inf.at(n-1));
			}
			x->total = x->recIdle();
			x->returnBytes();
		}
	}
	if (strcmp(argv[1],"-c") == 0) {
		if (argc == 6)
			x->range_size = x->max_size + abs(x->min_size);
		x->MAX_ALL = stoi(argv[4]);
		x->MAX_Q = x->MAX_ALL;
		unsigned long long int t = stoll(argv[5]);
		x->airAlgo(t);
		x->answers = x->results;
		x->results.clear();
		x->idleMethod();
		printf("Series: ");
		for (unsigned long long int x : x->results)
			printf("%d ", x);
		if (x->answers == x->results)
			printf("\nLargess verified. Input and output are same");
		else
			printf("\nLargess unverified. Assume more integers?");
	}
	return 1; 
}