#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <math.h>
#include <exception>

using namespace std;

class air {

	public:
	int range_size = 256;
	int min_size = 0;
	int max_size = 256;
	int MAX_ALL = 0;
	int MAX_Q = MAX_ALL;
	vector<long int> answers, results;
	long long int total = 0;
	size_t y = 0;
	ifstream in_file;
	ofstream out_file;
	bool idleMethod();
	bool airAlgo(long long int);
	inline long long int recIdle() {
		long long int total = 0;
		long long int powr = 1;
		for (int i = 0 ; i < answers.size() ; i++) {
			powr = 1;
			for (int u = 0 ; u < i ; u++) {
				powr *= range_size;
			}
			cout << total << " + (" << powr << "*" << answers[i] << ")\n" << flush;
			total = total + (powr * answers[i]);
		}
		long long int bitbyte = total;
		cout << "Largess: " << total << endl;
		return total;
	}
	air() {};
	~air() {};
};

bool air::idleMethod() {
	long long int p = recIdle();
	airAlgo(p);
	int r = 0;
	for (int x : results) {
		if (x == answers[r]) { }
		else {
			cout << endl;
			for (int e : answers)
				cout << e << " ";
			cout << endl;
			for (int e : results)
				cout << e << " ";
			cout << endl;
			cout << "Byte error: " << x << "!=" << answers[r] << " " << total << flush;
			return 0;
		}
		r++;
	}
	return 1;
}

bool air::airAlgo (long long int Ti) {
	long long int n = Ti%range_size, m = 0;
	results.push_back(Ti%range_size);
	int i = 0;
	while (results.size() < MAX_Q) {
		int j = 0;
		for (int x : results) {
			long long powr = 1;
			for (int u = 0 ; u < j ; u++)
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

int main(int argc, char ** argv) {
	air * x = new air();

	if (argc < 5) {
		printf("Usage: \n\t%s -a <maximum> <minimum> <integers...>", argv[0]);
		printf("\n\t%s -c <maximum> <minimum> <no of vars> <long long int>", argv[0]);
		return 0;
	}
	x->max_size = stoi(argv[2]);
	x->min_size = stoi(argv[3]);

	if (strcmp(argv[1],"-a") == 0) {
		for (int i = 4; i < argc; i++)
			x->answers.push_back(stoi(argv[i]));
		x->MAX_ALL = x->answers.size();
		x->range_size = x->max_size + abs(x->min_size);
		x->idleMethod();
	}
	if (strcmp(argv[1],"-c") == 0) {
		if (argc == 6)
			x->range_size = x->max_size + abs(x->min_size);
		x->MAX_ALL = stoi(argv[4]);
		x->MAX_Q = x->MAX_ALL;
		long long int t = stoll(argv[5]);
		x->airAlgo(t);
		x->answers = x->results;
		x->results.clear();
		x->idleMethod();
		printf("Series: ");
		for (long long int x : x->results)
			printf("%d ", x);
		if (x->answers == x->results)
			printf("\nLargess verified. Input and output are same");
		else
			printf("\nLargess unverified. Assume more integers?");
	}
	return 1; 
}