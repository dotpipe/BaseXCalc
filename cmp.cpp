#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <math.h>
#include <exception>
#include <sstream>
#include <chrono>
#include <bitset>

using namespace std;

class air
{

	public:
	unsigned int range_size = 256;
	int min_size = 256;
	int max_size = 2;
	int MAX_ALL = 0;
	int MAX_Q = MAX_ALL;
	uint64_t output_total = 0;
	vector<unsigned long int> answers, results;
	unsigned long long int total = 0;
	string all_output = "";
	size_t y = 0;
	ifstream in_file;
	ofstream out_file;
	bool idleMethod();
	bool airAlgo(unsigned long long int);
	void returnBytes();
	void collect(ifstream&, string);
	string bitsToBytes (string str_xweight, uint64_t& return_len);
	string retrieve(string xweight, uint64_t return_len);
	bool recollect(ifstream& in);
	inline unsigned long long int recIdle() {
		unsigned long long int powr = 1;
		for (int i = 0 ; 0 < answers.size() ; i) {
			powr = 1;
			for (int u = 0 ; u < i ; u++) {
				powr *= range_size;
			}
    		total = total + (powr * ((uint8_t)(answers.front() - min_size)%range_size));
			answers.erase(answers.begin());
			if (i > 16 || answers.empty() || total > pow(2,54))
			{
				i = 0;
				returnBytes();
				total = 0;
			}
		}
		if (total > 0)
			returnBytes();
// std::cout << "Largess: " << total << endl;
		return 0;
	}
	air() {};
	~air() {};
};

// This function turns 8 bits to one byte till the string
// str_xweight is empty. return_len is the number accrued since last call
string air::bitsToBytes (string str_xweight, uint64_t& return_len)
{
	string bin = "", catchThis = "";
	size_t z = 0;
	for (z = 0; z + 1 < str_xweight.length() ; z += 8)
	{
		catchThis = (z < str_xweight.length()) ? str_xweight.substr(z,8) : str_xweight.substr(z);
		uint8_t xweight = bitset<8>(catchThis).to_ullong();
		if (xweight%256 == 0)
			bin.push_back((char)(0));
		else
			bin.push_back((char)(xweight%256));
	}
	catchThis = str_xweight.substr(z%8);
	uint8_t xweight = bitset<8>(catchThis).to_ullong();
	if (xweight%256 == 0)
		bin.push_back((char)(0));
	else
		bin.push_back((char)(xweight%256));
	return_len += bin.length();
	return bin;
}

// get files in order to represent decompressable data
bool air::recollect(ifstream& in)
{
	stringstream rd;
	rd << in.rdbuf();
	in.close();
	string buf = rd.str();
	rd.str("");
	string filename_str = "";
	uint64_t full_size = 0;

	uint64_t buf_len = buf.length(), bin_len = 0, buf_left = buf_len;
	// find this string and get the length toitand its further length
	uint8_t fn_s = buf.find("START---"); // 8 chars long
	// find where data frame begins by getting to the S
	uint8_t fn_t = buf.find("-----------------S",18 + fn_s + 8);
	// get the size of the file to output
	string full_size_str = buf.substr(18, fn_s-18);
	// shorten up string
	buf = buf.substr(fn_s+8);
	// turn full size string to a uin64_t
	full_size = stoi(full_size_str);
	// get length of number for full_size_str
	uint8_t fss = full_size_str.length();
	// recapture data point break aft buf manipulation
	fn_t = buf.find("-----------------S");
	// get filename
	filename_str = buf.substr(0,fn_t);
	// remove non compressed data from buf string
	buf = buf.substr(18+fss+fn_t-6);
	// std::cout << dictionary_str << "*" << flush;
	uint64_t eOf = buf.find("-----------------END--------------------S");
	if (eOf != string::npos)
		buf = buf.substr(0,eOf); // assert this is a real file of ours
	else
	{
		std::cout << "\r\n * Invalid archive" << flush;
		exit(0);
	}

	uint64_t buf_cntA = 0, buf_cntB = 0, cntAB= 0, return_len = 0;
	string binary_return_str = "";
	ofstream out {filename_str.c_str(), std::ios_base::out | std::ios_base::trunc };
	for (size_t i = 0; buf.length() > 0 ; i++)
	{
		string bf = buf.substr(0,buf.find("8;"));
		buf = buf.substr(bf.length() + 2);
		while (bf.length() > 0)
		{
			// find the first occurrence of --&
			buf_cntA = buf.find("?$");
			// find next occurence. our data is between
			// get data ready
			if (buf_cntA == string::npos)
				break;
			string buf_keep = buf.substr(0, buf_cntA);
			range_size = (int)(buf_keep[1]) + abs((int)(buf_keep[0]));
			uint64_t buff = 0;
			// ring around the rosey
			buf_keep = buf_keep.substr(2);
			for (uint8_t j = 0 ; j < buf_keep.length() ; j++)
			{
				buff <<= 8;
				buff += buf_keep.at(j);
			}
			binary_return_str += airAlgo(buff);
			buf = buf.substr(buf_cntA);
		}
		if (binary_return_str.length() > 1000000 || binary_return_str.length() >= (full_size-1))
		{
			out << binary_return_str << flush;
			binary_return_str.clear();
		}
		std::cout << "\rLeft: " << ((full_size) - return_len - binary_return_str.length()) << "         \r\n" << flush;
		// out.write(return_str.c_str(),return_str.length());
	}
	std::cout << "\rLeft: " << ((full_size) - return_len - binary_return_str.length()) << "         \r\n" << flush;
	out << binary_return_str << flush;
	return (buf.length() > 0);
}

bool air::idleMethod() {
	unsigned long long int p = recIdle();
	airAlgo(p);
	int r = 0;
	for (unsigned int x : results) {
		if (x == answers[r]) { }
		else {
			// std::cout << endl;
			// for (unsigned int e : answers)
			// 	std::cout << e << " ";
			// std::cout << endl;
			// for (unsigned int e : results)
			// 	std::cout << e << " ";
			// std::cout << endl;
			// std::cout << "Byte error: " << x << "!=" << answers[r] << " ";// << total << flush;
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
		results.push_back((n%range_size) + min_size);
	}
	return 1;
}

void air::returnBytes()
{
	uint64_t p = total;
	string p_Str = "";
	while (p > 0)
	{
		if (p%128 == 0) all_output += (uint8_t)(0);
		else all_output += (uint8_t)(p%128);
		p >>= 7;
	}
	all_output += (uint8_t)(max_size);
	if (min_size == 0) all_output += (uint8_t)(255);
	else all_output += (uint8_t)(min_size);
	all_output += "?$";
	if (all_output.length() > 100000)
	{
		output_total += all_output.length() + 2;
		all_output += "8;";
		out_file.write(all_output.c_str(), all_output.length());
		all_output.clear();
	}
}

void air::collect(ifstream& in_file, string filename)
{
	uint8_t bytes = 128;
	stringstream no;
	no << in_file.rdbuf();
	string inf = no.str();
	no.str("");
	out_file << "-----------------S" << inf.length() << "START---TEST" << filename << "-----------------S";
	// x->range_size = x->max_size;
	for (uint32_t n = 1; n <= inf.length(); n++)
	{
		max_size = 2;
		min_size = 256;
		while (n%bytes && inf.length() > n)
		{
			if ((int)inf.at(n-1) >= max_size)
				max_size = inf.at(n-1) + 1;
			if ((int)inf.at(n-1) < min_size)
				min_size = inf.at(n-1);
			answers.push_back(inf.at(n-1));
			n++;
		}
		range_size = max_size - min_size;
		total = recIdle();
		total = 0;
		std::cout << "\rOutput Total: " << output_total << "/" << (inf.length() - n) << flush;
	}
	if (all_output.length() > 0)
	{
		out_file.write(all_output.c_str(), all_output.length());
		std::cout << "\rOutput Total: " << output_total << "/" << 0 << flush;
	}
}

int main(int argc, char ** argv) {
	air * x = new air();

	if (argc < 2) {
		printf("Usage: \n\t%s -a <maximum> <minimum> <integers...>", argv[0]);
		printf("\n\t%s -c <maximum> <minimum> <no of vars> <long long int>", argv[0]);
		return 0;
	}
	vector<string> filenames;
	vector<ifstream> ifstreams;
	string fname = "";

    printf("Press, Copyright Aunk 2016\n\r\n\r");

	std::setlocale(LC_ALL, "en_US-UTF8");
	while (argc - 1 > 0)
	{
		if (string(argv[argc - 2]).length() > 1 && string(argv[argc - 2]) != "-o" && string(argv[argc - 1]) != "-o" )
			filenames.push_back(string(argv[argc - 1]));
		else if (string(argv[argc - 2]) == "-o")
			fname = argv[argc - 1];
		argc--;
	}
	std::cout << ".." << flush;
	x->out_file.open(fname.c_str(), std::ios_base::out | std::ios_base::trunc);
	if (filenames.size() > 0 && fname.length() > 0) {
		if (! x->out_file) {
			printf("You must choose a output filename to continue...");
			exit(1);
		}

		if (filenames.size() == 0) {
			printf("\n\rYou must choose a input filename to continue...");
			exit(1);
		}
		std::cout << "Data Loading..\n\r" << flush;

		auto time_start = std::chrono::steady_clock::now();
		for (size_t i = 0; i < filenames.size() ; i++)
		{
			ifstream in { filenames[i].c_str(), std::ios_base::in | std::ios_base::binary };
			x->collect(in, filenames[i].c_str());
			std::cout << "\n\r";
			x->out_file << "8;-----------------END--------------------S" << flush;
		}
		std::cout << "\n\rComplete.\r\n" << flush;

		auto time_end = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds = time_end - time_start;
		std::cout << "Press finished in " << (elapsed_seconds.count()) << "secs\r\n" << flush;
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
