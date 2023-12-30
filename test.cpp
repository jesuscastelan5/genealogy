# include <iostream>
# include <string>
# include <vector>
# include "string tools.h"

int main (){
	std::vector<std::string> myArr = {"", "a1", "2e", "tefgr", "14"};
	for (auto elem : myArr)
		std::cout << IntToStr(IsInt(elem)) << std::endl;

	return 0;
}