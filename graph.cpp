# include <iostream>
# include <vector>
# include <string>
# include <set>
# include <fmt>

typedef struct{
	std::string name;
	int ID;
	std::set <int> childList;
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
	*(pThisNode).ID = extern int globalID;
	extern std::vector<node> nodeDirectory;
	*(pThisNode).name = thisName;
	nodeDirectory.push_back (*(pThisNode));
	globalID++;
	return;
}

/*
	Tested:
	Description:
	Param:
*/
void insertChild (node * parentNode, node * childNode){
	int childID = *(childNode).ID;
	*(parentNode).childList.insert(childID);
	return;
}

/*
	Tested:
	Description:
	Param:
*/
void mergeBranches(node * childNode, int parent1ID, int parent2ID){
	extern std::vector<node> nodeDirectory;

	node parent1 = nodeDirectory[parent1ID]
	node parent2 = nodeDirectory[parent2ID]

	insertChild (&parent1, &childNode)
	insertChild (&parent2, &childNode)
	return;
}


std::vector<node> nodeDirectory;
int globalID = 0;

/*
	Description: gives users a list of possible matches to parentName
*/
std::vector<int> getParentID (std::string parentName){
	extern std::vector<node> nodeDirectory;
	std::vector <int> possMatches;
	for (auto thisNode : nodeDirectory){
		if (thisNode.name == parentName){
			possMatches.push_back(thisNode.ID);
		}
	}
	return possMatches;
}

void promptName(std::string prompt, node * newNode){
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
	extern std::vector<node> nodeDirectory;

	char userAns;
	std::cout << "Does this family member have a parent node (y/n)? " << std::endl;
	std::cin >> userAns;
	std::string nodeName;
	if (userAns == 'n'){
		node newNode;
		promptName ("What is the name of the family member / marriage? ", &newNode);
		return;
	}
	std::string parentName, nodeName;
	std::cout << "What is the name of the parent / parent marriage? " << std::endl;
	std::cin >> parentName;
	// at some point, find a way to disambiguate apparent duplicate entries
	int parentID = getParentID (parentName)[0];

	node newNode;
	promptName("What is the name of the new family member / marriage? ", &newNode)
	node parentNode = nodeDirectory[parentID];
	insertChild(&parentNode, &newNode);
	return;
}


std::string childToList (node * pParent){
	std::string listOChildren;
	for (int i = 0; i < *(pParent).childList.size() - 1; i++){
		child = *(pParent).childList[i];
		listOChildren = listOChildren + fmt::format("{}", child);
		listOChildren.push_back(',');
	}
	child = *(pParent).childList[i];
	listOChildren = listOChildren + fmt::format("{}", child);
	
	return listOChildren;
}

void saveGeneology(std::string fileName){
	extern std::vector<node> nodeDirectory;
	
	std::ofstream myFile (fileName + ".csv");
	for (auto entry : nodeDirectory){
		std::string childList = childToList (entry);
		myFile << entry.ID << ";" << entry.name << ";" << childList;
	}

	myFile.close();
	return 0;
}


int main(){
	
	
	return 0;
}