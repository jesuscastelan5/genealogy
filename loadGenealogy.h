# include <string>
# include <fstream>
# include <sstream>
# include <vector>
# include "string tools.h"

typedef struct{
	int memberID;
	std::string memberName;
	std::string memberChildren;
}fileRow;

/*
	Tested: 12/30/2023
	Description: reads CSV with a delimiter of ';'
		If file with name of fileName doesn't exist,
		it will return an empty vector of rowsRead
		If file is empty, it will return an empty vector
		Will ignore header
	Param: fileName
	Returns: contents of CSV file
*/
std::vector <fileRow> loadGenealogyCSV (std::string fileName){
	std::vector <fileRow> rowsRead;

	char sepChar = ';';
	std::fstream myFile;
	myFile.open(fileName);
	
	std::string line;
	fileRow thisRow;
	
	while(std::getline(myFile, line)){
		std::stringstream inputString (line);
		
		std::vector <std::string> dividedString = SplitStr(line, sepChar);
		if (dividedString.size() < 2){
			std::vector <fileRow> emptyVector;
			return emptyVector;
		}else if (dividedString[0] == "ID")
			continue;
		else{
			thisRow.memberID = std::stoi (dividedString[0]);
			thisRow.memberName = dividedString[1];
			if (dividedString.size()>2)
				thisRow.memberChildren = dividedString[2];
			else
				thisRow.memberChildren = "";
			rowsRead.push_back(thisRow);
		}
	}
	
	myFile.close();
	
	return rowsRead;
}