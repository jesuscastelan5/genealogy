# include <iostream>
# include <vector>
# include <string>
# include <set>

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
extern std::vector<node *> nodeDirectory;
*(pThisNode).name = thisName;
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
extern std::vector<node*> nodeDirectory;

node * pParent1 = nodeDirectory[parent1ID]
node * pParent2 = nodeDirectory[parent2ID]

insertChild (pParent1, &childNode)
insertChild (pParent2, &childNode)
return 0;
}


std::vector<node *> nodeDirectory;
int globalID = 0;


int getParentID (std::string ){
	
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
	std::cout << "Does this family member have a parent node (y/n) " << std::endl;
	std::cin >> userAns;
	if (userAns == 'y')
		//
	else
		//
	
	
	
	return;
}


int main(){
		
	
	return 0;
}