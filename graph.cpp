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
	Tested: 12/24/2023
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
	Tested: 12/24/2023
	Description: prompts user of name of a new node
	Param: prompt - the question for user
	Returns: name of the node
*/
std::string promptName(std::string prompt){
	std::string nodeName;
	std::cout << prompt << std::endl;
	std::getline (std::cin >> std::ws, nodeName);
	return nodeName;
}

/*
	Tested: 12/24/2023
	Description: finds the ID of the most likely node a user is searching for
	Param: prompt - the question for user,
	Returns: the nodes' ID
		-1 if the user quits from here
*/
int findNodeID (std::string prompt){
	std::vector<int> possMatches = {-1};
	std::string nodeName;

	do{
		if (possMatches.empty())
			std::cout << "Sorry, "<< nodeName << " does not exist in the genealogy." << std::endl;
		nodeName = promptName (prompt);
		if (nodeName == "\\quit")
			return -1;
		// at some point, find a way to disambiguate apparent duplicate entries
		possMatches = getNodeID (nodeName);
	}while (possMatches.empty());

	return possMatches[0];
}

/*
	Tested: 12/24/2023
	Description: finds existing node (parent or child),
		and connects it to a new node
	Param: prompt - the question for user,
		mode - 0 corresponds to child,
			1 corresponds to parent
*/
void findNAddNode (std::string prompt, int mode, node * pNewNode){
	extern std::vector<node *> nodeDirectory;
	
	int oldNodeID = findNodeID (prompt);
	if (oldNodeID == -1)
		return;
	node * pOldNode = nodeDirectory[oldNodeID];
	std::string nodeName = promptName("What is the name of the new family member / marriage?");
	createNode (pNewNode, nodeName);
	if (mode == 0)
		insertChild(pNewNode, pOldNode);
	else
		insertChild(pOldNode, pNewNode);
	return;
}


/*
	Tested: 12/24/2023
	Description: lets the user create and connect nodes
	Param: pNewNode - ptr of a new created node in main()
		(C/C++ needs globals to be created in main or outside of functions)
*/
void addNodes(node * pNewNode){
	extern int globalID;

	char userAns;
	std::cout << "Does this family member have an existing parent node (y/n)? Type q to quit." << std::endl;
	std::cin >> userAns;
	if (userAns == 'q')
		return;
	else if (userAns == 'y'){
		findNAddNode ("What is the name of the parent / parent marriage? Type \\quit to quit.", 1, pNewNode);
		return;
	}
	std::cout << "Does this family member have an existing child node (y/n)? Type q to quit." << std::endl;
	std::cin >> userAns;
	if (userAns == 'q')
		return;
	else if (userAns == 'n'){
		std::string nodeName = promptName ("What is the name of the family member / marriage? ");
		createNode (pNewNode, nodeName);
		return;
	}
	findNAddNode ("What is the name of the child / marriage of child? Type \\quit to quit.", 0, pNewNode);
	return;
}

/*
	Tested:
	Description: prints out the list of child IDs with the row number
	Param: ListOChildren - list of child IDs
*/
void listAllChildren (std::vector<int> ListOChildren){
	int i = 1;
	for (auto childID : ListOChildren){
		node * pChild = nodeDirectory[childID];
		std::cout << IntToStr(i) << " - "<< pChild->name << std::endl;
		i++;
	}
	return;
}


/*
	Tested:
	Description: disconnects the ith + 1 child ID from childList of pParent
	Param: pParent - pointer of parent node
		userAns - position (from index = 1) 
			of child in pParent->childList
*/
void disconnectNodes(node * pParent, int userAns){
	std::vector<int> ListOChildren = pParent->childList;
	
	if (userAns == ListOChildren.size()){
		ListOChildren.pop();
	}else{
		for (int i = userAns; i < ListOChildren.size() - 1; i++){
			ListOChildren [i - 1] = ListOChildren [i];
		}
		ListOChildren.pop();
	}
	
	pParent->childList = ListOChildren;
	return;
}


/*
	Tested:
	Description: takes away a child from the list of child IDs from a given parent
*/
void disconnectNodesPrompt(){
	extern std::vector <node *> nodeDirectory;
	
	int parentID = findNodeID("What is the name of the parent?");
	node * pParent = nodeDirectory[parentID];
	std::vector<int> ListOChildren = pParent->childList;
	
	std::cout << "Which of these should be disconnected from " << pParent->name << "?" << std::endl;
	listAllChildren();
	int userAns;
	std::cin >> userAns;
	while (userAns > (ListOChildren.size()) || userAns < 1){
		std::cout << "That's not a valid answer." << std::endl;
		std::cout << "Which of these should be disconnected from " << pParent->name << "?" << std::endl;
		listAllChildren();
		std::cin >> userAns;
	}
	
	disconnectNodes(pParent, userAns);
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
	int i = 0;
	
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
	std::cout << std::setw(CHILD_WIDTH) << std::left << "Child IDs" << std::endl;
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
	Tested: 12/24/2023
	Description: saves nodes to a (txt/csv) file
	Param: fileName - name of file
*/
void saveGenealogy(std::string fileName){
	extern std::vector<node *> nodeDirectory;
	std::string colDelimiter = ";";
	
	std::ofstream myFile;
	myFile.open(fileName + ".txt");
	// header
	myFile << "ID" << colDelimiter << "Name" << colDelimiter << "Child IDs" << std::endl;
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
	int userResp = -1;
	
	while (userResp != 0){
		if (userResp == 1){
			node * pNewNode = new node;
			addNodes(pNewNode);
			if (pNewNode->name == "")
				delete pNewNode;
		}else if (userResp == 2){
			disconnectNodesPrompt();
		}else if (userResp == 3)
			listGenealogy();
		else if (userResp == 4){
			std::string fileName;
			std::cout << "What would you like to call the txt file?" << std::endl;
			std::getline(std::cin >> std::ws, fileName);
			saveGenealogy(fileName);
		}
		std::cout << "What would you like to do?" << std::endl;
		std::cout << "0 - quit\n" <<
		"1 - create family members / marriages\n" <<
		"2 - disconnect family members / marriages\n" <<
		"3 - list genealogy to screen\n" <<
		"4 - save genealogy to a file" << std::endl;
		std::cin >> userResp;
	}
	
	return 0;
}