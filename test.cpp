# include <iostream>
# include "loadGenealogy.h"

int main (){
	std::vector <fileRow> myG = readGenealogyCSV ("testG.csv");
	
	std::cout << "Hi" << std::endl;

	return 0;
}