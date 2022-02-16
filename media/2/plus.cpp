#include <cstdlib>

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main()
{
//int sz = 65336 + 201;
//int sz = 65336 + 210;
/*
int sz = 65336 * 3;
char s[sz];
for(int i = 0; i < sz; ++i) s[i] = std::rand() % 10 + '0';
s[sz - 1] = 0;
cout << s << endl;
return 0;
*/
long long  sz = 1e10;
int yy = 0;
for(long long i = 0; i < sz; ++i) yy += std::rand() ;
//for(int i = 0; i < sz/sz; ++i) std::cout << std::rand() << " ";
std::cout << yy << " ";
return 0;



	int a, b;
	cin >> a >> b;
	//cout << 0;
	cout << max(a, b);
	//freopen("a.out", "w", stdout);
	std::cout << std::rand();
	for(int i = 0; i < sz; ++i) std::cout << std::rand();
	std::cout << std::rand();
	std::cout << std::rand();
}

