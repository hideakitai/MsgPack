#include <iostream>
#include <sstream>

#include "msgpack-c/include/msgpack.hpp"

#include <queue>

using namespace std;
int main(int argc, char *argv[])
{
	std::stringstream buffer;

//	msgpack::pack(buffer, nullptr);

//	std::queue<int> q;
//	for (int i = 1; i <=5; ++i) q.push(i);
//	msgpack::pack(buffer, q);


//	std::map<std::string, int> m;
//	for (int i = 1; i <=5; ++i) m.emplace(std::make_pair(std::to_string(i), i));
//	msgpack::pack(buffer, m);

//	std::unordered_map<std::string, int> m;
//	for (int i = 1; i <=5; ++i) m.emplace(std::make_pair(std::to_string(i), i));
//	msgpack::pack(buffer, m);

//	std::multimap<std::string, int> m;
//	for (int i = 1; i <=5; ++i) m.emplace(std::make_pair(std::to_string(i), i));
//	msgpack::pack(buffer, m);

//	std::unordered_multimap<std::string, int> m;
//	for (int i = 1; i <=5; ++i) m.emplace(std::make_pair(std::to_string(i), i));
//	msgpack::pack(buffer, m);


//	std::deque<int> q;
//	for (int i = 1; i <=5; ++i) q.push_back(i);
//	msgpack::pack(buffer, q);

//	unsigned char test[5] {"test"};
//	msgpack::pack(buffer, "test");
//	msgpack::pack(buffer, test);

//	std::vector<char> vc {1, 2, 3, 4, 5};
//	msgpack::pack(buffer, vc);

//	std::list<int> ls {1, 2, 3, 4, 5};
//	msgpack::pack(buffer, ls);

//	std::forward_list<int> ls {1, 2, 3, 4, 5};
//	msgpack::pack(buffer, ls);

	std::set<int> s {1, 2, 3, 4, 5};
	msgpack::pack(buffer, s);

//	std::unordered_set<int> s {1, 2, 3, 4, 5};
//	msgpack::pack(buffer, s);

//	std::multiset<int> s {1, 2, 3, 4, 5};
//	msgpack::pack(buffer, s);

//	std::unordered_multiset<int> s {1, 2, 3, 4, 5};
//	msgpack::pack(buffer, s);

//	std::pair<std::string, int> pr = std::make_pair("1", 1);
//	msgpack::pack(buffer, pr);

//	std::tuple<int, float, std::string> tp = std::make_tuple(1, 2.2, "three");
//	msgpack::pack(buffer, tp);



	// deserialize the buffer into msgpack::object instance.
	std::string str(buffer.str());
	std::cout << std::hex;
	for (size_t i = 0; i < str.length(); ++i)
	{
		std::cout << (int)(uint8_t)str.c_str()[i] << " ";
	}
}