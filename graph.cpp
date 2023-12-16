# include <iostream>
# include <vector>
# include <string>

typedef struct{
	std::string name;
	int ID;
	std::vector <int> childID;
}node;


/*
	Tested:
	Description:
		gives properties to newly made nodes
		updates nodeDirectory
		assumes a node already exists
	Param: thisNode - pointer of an existing node
		thisName - name of node
	Returns: nothing
*/
void createNode(node * pThisNode, std::string thisName){
*(pThisNode).ID = extern globalID;
extern std::vector<node *> nodeDirectory;
*(pThisNode).name = thisName;
nodeDirectory.push_back (pThisNode);
globalID++;
}

/*
	Tested:
	Description:
	Param:
	Returns: 0 if successful, another integer otherwise
*/
int insertChild (node * parentNode, node * childNode){
extern std::vector<node *> nodeDirectory;
int i;
thisChildID = *(parentNode).childID[i];
thisChild = nodeDirectory[thisChildID];
while(i < *(parentNode).childID.size()){
	thisChildID = *(parentNode).childID[i];
	thisChild = nodeDirectory[thisChildID];
	if !(stpcpy(*(childNode).name, *(thisChild).name)> 0)
		break;
	i++;
}

if (stpcpy(*(childNode).name, *(thisChild).name) == 0)
	return 1;

if (i == *(parentNode).childID.size())
	*(parentNode).childID.push_back(*(childNode).ID);
else{
	lastChild = *(parentNode).childID [*(parentNode).childID.size() - 1];
	*(parentNode).childID.push_back(lastChild);
	for (int j = *(parentNode).childID.size(); j > i; j++){
		node * pTempNode = *(parentNode).childID[j];
		*(parentNode).childID[j + 1] = pTempNode;
	}
	*(parentNode).childID[i] = childNode;
}
return 0;
}

int mergeBranches(node * childNode, int parent1ID, int parent2ID){

if (insertChild (pParent1, &childNode) == 0)
	return 1;
if (insertChild (pParent2, &childNode) == 0);
	return 1;
return 0;
}


std::vector<node *> nodeDirectory;

globalID = 0;

int main(){
	node firstNode;
	createNode (&firstNode, "Terah")
	std::vector <std::string> children = {"Abraham & Sarah", "Abraham & Hagar", "Nahor & Milcah"}
	for (auto childName : children){
		node newNode;
		
	}
	
	return 0;
}