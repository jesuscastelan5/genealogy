# include <iostream>
# include <vector>
# include <string>
# include <set>
# include <fstream>
# include <iomanip>
# include "string tools.h"

// add DOB and DOD at some point
typedef struct{
	std::string name;
	int ID;
	std::vector <int> childList;
}node;


/*
	Tested: 12/17/2023
	Description: gives properties to newly made nodes
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
	Tested: 12/23/2023
	Description: adds child ID to childList for parentNode
		Ensures childList stays sorted in ascending order
	Param: parentNode, childNode
*/
void insertChild (node * parentNode, node * childNode){
	int childID = childNode->ID;
	int i = 0;
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
	Description: adds child ID to childList for two parent nodes
		does this indirectly by using the nodeDirectory
		rather than using the raw pointers
		figured it'd be safer
	Param: childNode, parent1ID, parent2ID
*/
void mergeBranches(node * childNode, int parent1ID, int parent2ID){
	extern std::vector<node *> nodeDirectory;

	node * pParent1 = nodeDirectory[parent1ID];
	node * pParent2 = nodeDirectory[parent2ID];

	insertChild (pParent1, childNode);
	insertChild (pParent2, childNode);
	return;
}

/*
	Tested:
	Description: gives users a list of possible matches to nodeName
	Param: nodeName - name part of the node
	Returns: possMatches - list of IDs that potentially match user's query
*/
std::vector<int> getNodeID (std::string nodeName){
	extern std::vector<node *> nodeDirectory;
	std::vector <int> possMatches;
	for (auto pThisNode : nodeDirectory){
		if (pThisNode->name == nodeName){
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
	Description: finds the ID of the most likely node a user is searching for
	Param: prompt - the question for user,
	Returns: the nodes' ID
*/
int findNodeID (std::string prompt){
	std::string nodeName;
	std::cout << prompt << std::endl;
	std::cin >> nodeName;
	// at some point, find a way to disambiguate apparent duplicate entries
	return getNodeID (nodeName)[0];
}

/*
	Tested:
	Description: finds existing node (parent or child),
		and connects it to a new node
	Param: prompt - the question for user,
		mode - 0 corresponds to child,
			1 corresponds to parent
*/
void findNAddNode (std::string prompt, int mode, node * pNewNode){
	int oldNodeID = findNodeID (prompt);
	node * pOldNode = nodeDirectory[oldNodeID];
	promptName("What is the name of the new family member / marriage? ", pNewNode);
	if (mode == 0)
		insertChild(pNewNode, pOldNode);
	else
		insertChild(pOldNode, pNewNode);
	return;
}


/*
	Tested:
	Description: lets the user create and connect nodes
	Param:
*/
void addNodes(){
	extern int globalID;
	extern std::vector<node *> nodeDirectory;

	char userAns;
	std::cout << "Does this family member have a parent node (y/n)? Type q to quit." << std::endl;
	std::cin >> userAns;
	node newNode;
	if (userAns == 'q')
		return
	else if (userAns == 'y'){
		findNAddNode ("What is the name of the parent / parent marriage?", 1, &newNode);
		return;
	}
	std::cout << "Does this family member have a child node (y/n)? Type q to quit." << std::endl;
	std::cin >> userAns;
	if (userAns == 'q')
		return;
	else if (userAns == 'n'){
		promptName ("What is the name of the family member / marriage? ", &newNode);
		return;
	}
	int childID = findNodeID ("What is the name of the child / child marriage?")
	findNAddNode ("What is the name of the child / child marriage?", 0, &newNode);
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
	Description: shows a list of current nodeDirectory
*/
void listGenealogy(){
	extern std::vector<node *> nodeDirectory;
	int ID_WIDTH = 5;
	int NAME_WIDTH = 41;
	int CHILD_WIDTH = 20;
	
	// header
	std::cout << std::setw(ID_WIDTH) << std::right << "ID";
	std::cout << " ";
	std::cout << std::setw(NAME_WIDTH) << std::left << "Name";
	std::cout << std::setw(CHILD_WIDTH) << std::left << "Children" << std::endl;
	for (auto entry : nodeDirectory){
		std::string childList = childToList (entry, ',');
		
		std::cout << std::setw(ID_WIDTH) << std::right << IntToStr(entry->ID);
		std::cout << " "; 
		std::cout << std::setw(NAME_WIDTH) << std::left << entry->name;
		std::cout << std::setw(CHILD_WIDTH) << std::left << childList << std::endl;
	}

	return;
}


/*
	Tested: 12/23/2023
	Description: saves nodes to a (txt/csv) file
	Param: fileName - name of file
*/
void saveGenealogy(std::string fileName){
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
	return;
}


std::vector<node *> nodeDirectory;
int globalID = 0;

int main(){
	int userResp;
	
	std::cout << "Type \\quit to quit" << std::endl;
	std::cout << "What would you like to do?" << std::endl;
	std::cout << "0 - quit\n" <<
		"1 - add family members / marriages\n" <<
		"2 - save genealogy to a file" <<
		"3 - list genealogy to screen" << std::endl;
	std::cin >> userResp;
	while (userResp != 0){
		if (userResp == 1)
			addNodes();
		else if (userResp == 2){
			std::string fileName;
			std::cout << "What would you like to call the txt file?" << std::endl;
			std::cin >> fileName;
			saveGenealogy(fileName);
		}else if (userResp == 3)
			listGenealogy();
		std::cout << "What would you like to do?" << std::endl;
 		std::cin >> userResp;
	}
	
	return 0;
}