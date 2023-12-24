# include <iostream>
# include <vector>
# include <string>
# include <set>
# include <fstream>
# include "string tools.h"

typedef struct{
	std::string name;
	int ID;
	std::vector <int> childList;
}node;


/*
	Tested:
	Description:
		gives properties to newly made nodes
		updates nodeDirectory
		assumes a node already exists
	Param: thisNode - pointer of an existing node
		thisName - name of node
*/
void createNode(node * pThisNode, std::string thisName){
	extern int globalID;
	pThisNode->ID = globalID;
	extern std::vector<node *> nodeDirectory;
	pThisNode->name = thisName;
	nodeDirectory.push_back (pThisNode);
	globalID++;
	return;
}

/*
	Tested:
	Description:
	Param:
*/
void insertChild (node * parentNode, node * childNode){
	int childID = childNode->ID;
	int i;
	while (i < parentNode->childList.size() && childID> parentNode->childList[i]){
		i++;
	}
	if (i == parentNode->childList.size()){
		parentNode->childList.push_back(childID);
		return;
	}
	
	int lastChild = parentNode->childList[parentNode->childList.size() - 1];
	parentNode->childList.push_back(lastChild);
	for (int j = parentNode->childList.size(); j > i; j--){
		int tempChild = parentNode->childList[j];
		parentNode->childList[j + 1] = tempChild;
	}
	parentNode->childList[i] = childID;
	return;
}

/*
	Tested:
	Description:
	Param:
*/
void mergeBranches(node * childNode, int parent1ID, int parent2ID){
	extern std::vector<node *> nodeDirectory;

	node * pParent1 = nodeDirectory[parent1ID];
	node * pParent2 = nodeDirectory[parent2ID];

	insertChild (pParent1, childNode);
	insertChild (pParent2, childNode);
	return;
}


std::vector<node *> nodeDirectory;
int globalID = 0;

/*
	Tested:
	Description: gives users a list of possible matches to parentName
	Param: parentName - name part of the parent node
	Returns: possMatches - list of IDs that potentially match user's query
*/
std::vector<int> getParentID (std::string parentName){
	extern std::vector<node *> nodeDirectory;
	std::vector <int> possMatches;
	for (auto pThisNode : nodeDirectory){
		if (pThisNode->name == parentName){
			possMatches.push_back(pThisNode->ID);
		}
	}
	return possMatches;
}

/*
	Tested:
	Description: prompts user of name of a new node
	Param: prompt - the question for user,
		newNode - the pointer to be given a name
*/
void promptName(std::string prompt, node * newNode){
	std::string nodeName;
	std::cout << prompt << std::endl;
	std::cin >> nodeName;
	createNode (newNode, nodeName);
	return;
}

/*
	Tested:
	Description: lets the user create nodes (with or without a parent node) given a user inputted name
	Param:
*/
void addNodes(){
	extern int globalID;
	extern std::vector<node *> nodeDirectory;

	char userAns;
	std::cout << "Does this family member have a parent node (y/n)? " << std::endl;
	std::cin >> userAns;
	node newNode;
	if (userAns == 'n'){
		promptName ("What is the name of the family member / marriage? ", &newNode);
		return;
	}
	std::string parentName;
	std::cout << "What is the name of the parent / parent marriage? " << std::endl;
	std::cin >> parentName;
	// at some point, find a way to disambiguate apparent duplicate entries
	int parentID = getParentID (parentName)[0];
	node * pParentNode = nodeDirectory[parentID];
	promptName("What is the name of the new family member / marriage? ", &newNode);
	insertChild(pParentNode, &newNode);
	return;
}


/*
	Tested: 12/23/2023
	Description: makes a string list of child nodes from pParent
	Param: pParent (node ptr)
		delimiter - seperates IDs of children in string
	Returns: listOChildren
*/
std::string childToList (node * pParent, char delimiter){
	std::string listOChildren;
	int i;
	
	if (pParent->childList.size() == 0)
		return listOChildren;

	for (i = 0; i < pParent->childList.size() - 1; i++){
		int child = pParent->childList[i];
		listOChildren = listOChildren + IntToStr(child);
		listOChildren.push_back(delimiter);
	}
	int child = pParent->childList[i];
	listOChildren = listOChildren + IntToStr(child);
	
	return listOChildren;
}

/*
	Tested: 12/23/2023
	Description: saves nodes to a (txt) file
		csv file testing ongoing
	Param: fileName
*/
void saveGeneology(std::string fileName){
	extern std::vector<node *> nodeDirectory;
	std::string colDelimiter = ";";
	
	std::ofstream myFile;
	myFile.open(fileName + ".txt");
	// header
	myFile << "ID" << colDelimiter << "Name" << colDelimiter << "Children" << std::endl;
	for (auto entry : nodeDirectory){
		std::string childList = childToList (entry, ',');
		myFile << IntToStr(entry->ID) << colDelimiter << entry->name << colDelimiter << childList << std::endl;
	}

	myFile.close();
	return ;
}


int main(){
	std::string userResp;

	std::cout << "Type \quit to quit" << std::endl;
	
	while (userResp != "\quit"){
		addNodes();
		std::cin >> userResp;
	}
	
	return 0;
}