# include <vector>
# include <string>


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
	Tested: 12/25/2023
	Description: disconnects the ith child ID from childList of pParent
	Param: pParent - pointer of parent node
		childID - child ID in pParent->childList
*/
void disconnectNodes(node * pParent, int childID){
	std::vector<int> ListOChildren = pParent->childList;
	
	if (ListOChildren.empty())
		return;
	
	int i = 0;
	while (i < ListOChildren.size() && childID != ListOChildren[i])
		i++;
	
	for (int j = i; j < ListOChildren.size() - 1; j++){
		ListOChildren [j] = ListOChildren [j + 1];
	}
	ListOChildren.pop_back();
	
	pParent->childList = ListOChildren;
	return;
}