# include <iostream>
# include <vector>
# include <string>
# include <fstream>
# include <iomanip>
//# include "string tools.h"
# include "node operations.h"
# include "loadGenealogy.h"


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
	
	int childID;
	
	int oldNodeID = findNodeID (prompt);
	if (oldNodeID == -1)
		return;
	if (mode < 2){
		std::string nodeName = promptName("What is the name of the new family member / marriage?");
		createNode (pNewNode, nodeName);
	}else if (mode == 2){
		childID = findNodeID ("What is the name of the child member / marriage of child?");
		if (childID == -1)
			return;
	}if (mode == 0)
		insertChild(pNewNode->ID, oldNodeID);
	else if (mode == 1)
		insertChild(oldNodeID, pNewNode->ID);
	else if (mode == 2)
		insertChild(oldNodeID, childID);
	return;
}


/*
	Tested:
	Description: to be used with addNodes, takes away the repetitiveness of that subroutine
		ensures user puts a valid answer to prompts
		userAns is a string so we don't have to worry about flushing the input buffer
	Param:
		pNewNode - new node for findNAddNode, used if user answers 'y' to prompt1
		prompt1 - first question for user
		prompt2 - second question for user if the user answers 'y' to prompt1
		mode - mode for findNAddNode
	Returns: 'q' if the user wants to quit, 'y' or 'n' as an answer to prompt1
*/
char addNodesPrompts (node * pNewNode, std::string prompt1, std::string prompt2, int mode){
	std::string userAns;
	
	do{
		std::cout << prompt1 << std::endl;
		std::cin >> userAns;
		
		if (userAns == "q" || userAns == "Q" )
			return 'q';
		else if (userAns == "y" || userAns == "Y" ){
			findNAddNode (prompt2, mode, pNewNode);
			return 'y';
		}else if (userAns == "n" || userAns == "N" )
			return 'n';
		std::cout << "\nSorry, that's not a valid answer.\n" << std::endl;

	}while (1 == 1);
}


/*
	Tested: 12/24/2023
	Description: lets the user create and connect nodes
	Param: pNewNode - ptr of a new created node in main()
		(C/C++ needs globals to be created in main or outside of functions)
*/
void addNodes(node * pNewNode){
	char userAns;
	
	userAns = addNodesPrompts (pNewNode, "Does this family member have an existing parent node (y/n)? Type q to quit.",
		"What is the name of the parent / parent marriage? Type \\quit to quit.", 1);
	
	if (userAns != 'n')
		return;
	
	userAns = addNodesPrompts (pNewNode, "Does this family member have an existing child node (y/n)? Type q to quit.",
		"What is the name of the child / marriage of child? Type \\quit to quit.", 0);
	
	if (userAns != 'n')
		return;
	
	std::string nodeName = promptName ("What is the name of the new family member / marriage?");
	createNode (pNewNode, nodeName);
	return;
}


/*
	Tested: 12/25/2023
	Description: connects a parent and a child nodes without creating a new node
*/
void connectNodes(){
	findNAddNode ("What is the name of the parent / parent marriage? Type \\quit to quit.", 2, NULL);
	return;
}


/*
	Tested: 12/25/2023
	Description: prints out the list of child IDs with the row number
	Param: ListOChildren - list of child IDs
*/
void listAllChildren (std::vector<int> ListOChildren){
	extern std::vector<node *> nodeDirectory;

	int i = 1;
	for (auto childID : ListOChildren){
		node * pChild = nodeDirectory[childID];
		std::cout << IntToStr(i) << " - "<< pChild->name << std::endl;
		i++;
	}
	return;
}


/*
	Tested: 12/25/2023
	Description: takes away a child from the list of child IDs from a given parent
*/
void disconnectNodesPrompt(){
	extern std::vector <node *> nodeDirectory;
	
	int parentID = findNodeID("What is the name of the parent?");
	node * pParent = nodeDirectory[parentID];
	std::vector<int> ListOChildren = pParent->childList;
	
	std::cout << "Which of these should be disconnected from " << pParent->name << "?" << std::endl;
	listAllChildren(ListOChildren);
	int userAns;
	std::cin >> userAns;
	while (userAns > (ListOChildren.size()) || userAns < 1){
		std::cout << "That's not a valid answer." << std::endl;
		std::cout << "Which of these should be disconnected from " << pParent->name << "?" << std::endl;
		listAllChildren(ListOChildren);
		std::cin >> userAns;
	}
	
	int childID = ListOChildren[userAns - 1];
	
	disconnectNodes(pParent, childID);
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
		if (entry == NULL)
			continue;
		std::string childList = childToList (entry, ',');
		
		std::cout << std::setw(ID_WIDTH) << std::right << IntToStr(entry->ID);
		std::cout << " "; 
		std::cout << std::setw(NAME_WIDTH) << std::left << entry->name;
		std::cout << std::setw(CHILD_WIDTH) << std::left << childList << std::endl;
	}

	return;
}


/*
	Tested: 12/27/2023
	Description: deletes and releases all nodes and clears all elements from nodeDirectory
		resets globalID to 0
*/
void deleteAllNodes(){
	extern std::vector<node *> nodeDirectory;
	extern int globalID;
	
	for (int i = nodeDirectory.size() - 1; i > -1; i--){
		node * pThisNode = nodeDirectory[i];
		delete pThisNode;
		nodeDirectory.pop_back();
	}
	
	globalID = 0;
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
	
	std::string fileName;
	std::cout << "What would you like to call the txt file? (Type \\quit to quit)" << std::endl;
	std::getline(std::cin >> std::ws, fileName);
	
	if (fileName == "\\quit" || fileName == "\\QUIT")
		return;
	
	std::ofstream myFile;
	myFile.open(fileName + ".txt");
	// header
	myFile << "ID" << colDelimiter << "Name" << colDelimiter << "Child IDs" << std::endl;
	for (auto entry : nodeDirectory){
		if (entry == NULL)
			continue;
		std::string childList = childToList (entry, ',');
		myFile << IntToStr(entry->ID) << colDelimiter << entry->name << colDelimiter << childList << std::endl;
	}

	myFile.close();
	return;
}


/*
	Tested: 12/29/2023
	Description:
		Assumes deleteAllNodes() was called prior to use
		If IDs from file are in a strange order 
		(e.g. 0, 5, 3, 1, 2 or 100, 5, 2, 8),
		it will respect those IDs to maintain referencial integrity to children.
	Param: vector of CSV rows
*/
void CSVToNodes(std::vector<fileRow> genCSV){
	extern std::vector<node *> nodeDirectory;
	extern int globalID;
	
	for (auto row : genCSV){
		node * pThisNode = new node;
		pThisNode->ID = row.memberID ;
		pThisNode->name = row.memberName;
		std::vector<std::string> StringIDs = SplitStr (row.memberChildren, ',');
		std::vector <int> IntIDs;
		if (!(StringIDs.empty())){
			for (auto SID : StringIDs)
				IntIDs.push_back(std::stoi(SID));
		}
		pThisNode->childList = IntIDs;
		if (row.memberID > globalID){
			for (int i = globalID; i < row.memberID; i++)
				nodeDirectory.push_back(NULL);
			globalID = nodeDirectory.size();
			nodeDirectory.push_back(pThisNode);
			globalID++;
		}else if (row.memberID < globalID){
			nodeDirectory[row.memberID] = pThisNode;
		}else{
			nodeDirectory.push_back(pThisNode);
			globalID++;
		}
	}
	
	return;
}



/*
	Tested:
	Description:
	Param:
	Returns:
*/
void readGenealogy(){
	deleteAllNodes();
	std::string fileName;
	
	std::cout << "What is the name of the CSV file?" << std::endl;
	std::cin >> fileName;
	
	fileName = fileName + ".csv";
	std::vector<fileRow> genCSV = readGenealogyCSV (fileName);
	if (genCSV.empty()){
		std::cout << "Warning: " << fileName << " does not exist, is empty, or is formatted incorrectly." << std::endl;
		std::cout << "Make sure the columns are separated by semicolons like so:" << std::endl;
		std::cout << "0;Abraham;1" << std::endl;
		std::cout << "1;Isaac;2" << std::endl;
		std::cout << "2;Jacob;" << std::endl;
		return;
	}
	
	CSVToNodes(genCSV);
	
	return;
}


/*
	Tested:
	Description:
	Param:
	Returns:
*/
void readGenealogyPrompt(){
	extern std::vector<node *> nodeDirectory;
	if (!nodeDirectory.empty()){
		std::string userAns;
		std::cout << "Warning: ";
		do{
			std::cout << "this will delete your current genealogy.  Do you want to proceed? (y/n)" << std::endl;
			std::cin >> userAns;
			if (userAns == "q" || userAns == "Q")
				return;
			else if (userAns == "n" || userAns == "N")
				return;
			else if (userAns == "y" || userAns == "Y"){
				readGenealogy();
				return;
			}
			std::cout << "That's not a valid answer" << std::endl;
		}while (1 == 1);
	}else
		readGenealogy();
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
		}else if (userResp == 2)
			connectNodes();
		else if (userResp == 3)
			readGenealogyPrompt();
		else if (userResp == 4)
			disconnectNodesPrompt();
		else if (userResp == 5)
			deleteAllNodes();
		else if (userResp == 6)
			listGenealogy();
		else if (userResp == 7){
			saveGenealogy(fileName);
		}
		std::cout << std::endl;
		std::cout << "What would you like to do?" << std::endl;
		std::cout << "0 - quit\n" <<
		"1 - create family members / marriages\n" <<
		"2 - connect family members / marriages\n" <<
		"3 - load existing genealogy\n"
		"4 - disconnect family members / marriages\n" <<
		"5 - delete all family members / marriages\n" <<
		"6 - list genealogy members to screen\n" <<
		"7 - save genealogy to a file" << std::endl;
		std::cin >> userResp;
	}
	
	return 0;
}