
#include "stringutil/stringutil.h"
#include "encode/base64.h"
#include "time/data_time.h"
using namespace cetus;
int main()
{
	std::string str;
	int n = 123;
	float f = 3.14f;
	StringUtils::Format(str, "%d:%f", n, f);
	std::cout << str << std::endl;

	std::string basestr1 = Base64Encoding::Encode("cetus");
	std::string basestr2 = Base64Encoding::Encode("gone with the wind");
	std::cout << basestr1 << std::endl << basestr2 << std::endl;

	DateTime dt = DateTime::Now();
	std::cout << dt.ToString() << std::endl;

	std::cin.get();
	return 0;
}