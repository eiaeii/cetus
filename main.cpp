
#include "stringutil/stringutil.h"
#include "encode/base64.h"
#include "time/data_time.h"
#include "container/vector_ex.h"
using namespace terra;
int main()
{
	TVector<int> vec;
	vec.push_unique(1);
	vec.push_unique(1);
	vec.push_unique(2);
	vec.push_unique(3);
	for (auto&& v : vec)
	{
		std::cout << v << std::endl;
	}
	vec.push_back(3);
	vec.push_back(3);
	vec.push_back(4);
	
	std::cout << vec.find(3) << std::ends << vec.find_last(3) << std::endl;

	std::string str;
	auto v = str.find_last_of('t');
	int n = 123;
	float f = 3.14f;
	StringUtils::Format(str, "%d:%f", n, f);
	std::cout << str << std::endl;

	std::string basestr1 = Base64Encoding::Encode("terra");
	std::string basestr2 = Base64Encoding::Encode("gone with the wind");
	std::cout << basestr1 << std::endl << basestr2 << std::endl;

	DateTime dt = DateTime::Now();
	std::cout << dt.ToString() << std::endl;

	std::cin.get();
	return 0;
}