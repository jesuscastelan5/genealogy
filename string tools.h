# include <vector>
# include <string>
# include <cmath>
# define TRUE 1
# define FALSE 0

/*
	Tested:
	Purpose:
	Param:
	Returns: boolean
		TRUE if the entire string is an integer
		FALSE otherwise
*/
int IsInt (std::string yourStr) {
	if (yourStr.empty())
		return FALSE;
	
	for (auto thisChar : yourStr){
		int thisCharASCII = (int)thisChar;
		if (thisCharASCII < 48 || thisCharASCII > 57)
			return FALSE;
	}
	
	return TRUE;
}


/*
    Tested: 12/30/2023 (jcastelan5)
    Purpose: splits a string by a given delimiter
    Param: 
    Returns: vector of strings split by delimiter
*/
std::vector<std::string> SplitStr(std::string yourStr, char delim){
  std::vector<std::string> splitStr;
  std::string tempStr;
  for (auto thisChar : yourStr){
	if (thisChar != delim){
		tempStr.push_back(thisChar);
    }else{
		splitStr.push_back(tempStr);
		tempStr = "";
    }
  }
  
  // for final split
  if (tempStr != "")
	splitStr.push_back(tempStr);
  return splitStr;
}


/*
	Tested: 12/30/2023
	Description:
		did not feel like importing cstring
		also, not hard to implement
	Param: inputStr
	Returns: all caps version of inputStr
*/
std::string capitalize (std::string inputStr){
	std::string retStr;
	
	for (auto thisChar : inputStr){
		if ((int) thisChar < 97 || (int) thisChar > 122)
			retStr.push_back (thisChar);
		else{
			int capChar = (int) thisChar - 32;
			retStr.push_back ((char) capChar);
		}
	}
	
	return retStr;
}

/*
	Tested: 12/28/2023
	Description: recursive loop to find digits of integer
		will push_back 0s if there is a gap of digits
		e.g. 10004, 1000006007, 1002, etc 
		ensures numbers ending with 0s will get those 0s in the string
	Param: n - integer to convert to string
		retStr - string with digits
		prevPower - power of the last digit found, used to find gaps in digits
	Returns: int in string format
*/
std::string findDigits (int n, std::string retStr, int prevPower){
	if (prevPower == 0)
		return retStr;
	
	int power = 0;
	while (n > pow(10, power + 1) - 1){
		power++;
	}
	int digit = n / pow(10, power);
	if (prevPower - power > 1){
		for (int i = 1; i < prevPower - power; i++){
			retStr.push_back('0');
		}
	}
	retStr.push_back ((char) 48 + digit);
	n = n - digit * pow(10, power);
	prevPower = power;
	
	
	retStr = findDigits (n, retStr, prevPower);
	
	return retStr;
}

/*
	Tested: 12/28/2023
	Description: converts a positive integer from 0 to ... into a string
		Ensures 0 does not go to findDigits
		Could not get std::to_string to work for some reason, thus leading to this function
	Param: n - integer to convert
	Returns: retStr - string form of n
*/
std::string IntToStr(int n) {
	
	std::string retStr;
	int prevPower = -1;
	
	if (n == 0){
		retStr.push_back('0');
		return retStr;
	}
	retStr = findDigits (n, retStr, prevPower);
	
	return retStr;
}