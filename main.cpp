#include "core.h"
#include "encode/base64.h"
using namespace terra;
int main()
{
	//std::string str;
	//auto v = str.find_last_of('t');
	//int n = 123;
	//float f = 3.14f;
	//StringUtils::Format(str, "%d:%f", n, f);
	//std::cout << str << std::endl;

	std::string basestr1 = Base64Encoding::Encode("你好");
	std::string basestr2;
	Base64Encoding::Decode(basestr1, basestr2);
	std::string basestr3 = Base64Encoding::Encode("gone with the wind");
	std::cout << basestr1 << std::endl << basestr2 << std::endl;

	//DateTime dt = DateTime::Now();
	//std::cout << dt.ToString() << std::endl;

	std::cin.get();
	return 0;
}