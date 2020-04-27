#include <string>
#include <vector>
#include <stdio.h>

using namespace std;

void split(const string &src, /* out */ vector<string> &dest, const char delimiter){
	if (src.empty())
		return;

	size_t last = 0;
	size_t next;
	while ((next = src.find_first_of(delimiter, last)) != string::npos){
		dest.push_back(src.substr(last, next - last));
		last = next + 1;
	}
	dest.push_back(src.substr(last));
}

int main(){
	string a = "first,second,s,,nice,,,kk,k,ok";
	vector<string> dest;
	split(a, dest, ',');
	for (auto i = dest.cbegin(); i != dest.cend(); i++){
		puts(i->c_str());
	}
	return 0;
}
