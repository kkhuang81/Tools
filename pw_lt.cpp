#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdint>
#include <sstream>
#include <stdio.h>
// for mmap:
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

//for cusomer
#include <unistd.h>  //close open
#include <vector>
#include "sfmt/SFMT.h"
#include <fstream>
#include <unistd.h>
#include <string>
#include <time.h>
#include <cstdlib>


//typedef unsigned long long int;

#define ASSERT(v) {if (!(v)) {cerr<<"ASSERT FAIL @ "<<__FILE__<<":"<<__LINE__<<endl; exit(1);}}

using namespace std;

void handle_error(const char* msg) {
	perror(msg);
	exit(255);
}


void readNM(int &n, int& m)
{
	ifstream cin("attribute.txt");
	ASSERT(!cin == false);
	string s;
	while (cin >> s)
	{
		if (s.substr(0, 2) == "n=")
		{
			n = atoi(s.substr(2).c_str());
			continue;
		}
		if (s.substr(0, 2) == "m=")
		{
			m = atoi(s.substr(2).c_str());
			continue;
		}
		ASSERT(false);
	}	
	cin.close();
}



int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		cout << "Usage: ./pw dataset_name number" << endl;
		exit(0);
	}	

	int n, m;
	

	string folder = argv[1];
	int num = atoi(argv[2]);

	sfmt_t sfmtSeed;
	//srand(95082);
	srand(time(NULL));
	sfmt_init_gen_rand(&sfmtSeed, rand());

	readNM(n, m);
	vector<vector<int>> in_edge(n, vector<int>());
	vector<double>probT(n, 0.0);

	size_t length;
	string graph_file = "graph_ic.inf";
	int fd = open((graph_file).c_str(), O_RDWR);
	if (fd == -1)
		handle_error("open");
	struct stat sb;
	int rc = fstat(fd, &sb);
	if (rc == -1)
		handle_error("fstat");

	length = sb.st_size;
	auto ptr = static_cast<char*>(mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0u));  //byte by byte
	auto f = ptr;

	
	int gap = 2 * sizeof(int)+sizeof(double);
	//ASSERT(fin != false);
	
	for (int i = 0; i < m; i++)
	{
		int a, b;
		double p;
		//int c = fscanf(fin, "%d%d%lf", &a, &b, &p);
		memcpy(&a, f, sizeof(int));
		memcpy(&b, f + sizeof(int), sizeof(int));
		memcpy(&p, f + 2 * sizeof(int), sizeof(double));
		f += gap;
		
		ASSERT(a < n);
		ASSERT(b < n);
		
		in_edge[b].push_back(a);		
		probT[b] = p;
	}
	
	rc = munmap(ptr, length);
	close(fd);
	//LT
	for (int k = 0; k < num; k++)
	{
		string index = to_string(k);
		string outfile = folder + "_" + index + "_lt";
		ofstream output(outfile);

		for (int i = 0; i < n; i++)
		{
			if (in_edge[i].size() == 0)continue;
			int index = sfmt_genrand_uint32(&sfmtSeed) % in_edge[i].size();
			int v = in_edge[i][index];
			output << v << " " << i << endl;			
		}

		output.close();
	}

	return 0;
}