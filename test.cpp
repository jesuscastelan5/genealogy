# include <iostream>
# include "string tools.h"

int main (){
	std::cout << SplitStr("abbz,james", ',')[1] << std::endl;
	if (SplitStr("", ',').empty())
		std::cout << "works" << std::endl;
	else
		std::cout << "no work" << std::endl;

	return 0;
}