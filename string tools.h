# include <vector>
# include <string>
# include <cmath>

/*
    Tested: 10/22/2023 (jcastelan5)
    Purpose: splits a string by a given delimiter
    Param: 
    Returns: vector of strings split by delimiter
*/
std::vector<std::string> SplitStr(std::string yourStr, char delim){
  std::vector<std::string> splitStr;
  std::string tempStr;
  for (int i = 0; i < yourStr.length(); i++){
    if (yourStr[i] != delim){
     tempStr = tempStr + yourStr[i];
    }else{
      splitStr.push_back(tempStr);
      tempStr = "";
    }
  }
  // for final split
  splitStr.push_back(tempStr);
  return splitStr;
}



/*
	Tested: 12/17/2023
	Description: recursive loop to find digits of integer
		will push_back 0s if there is a gap of digits
		e.g. 10004, 1000006007, 1002, etc 
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
	Tested: 12/17/2023
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