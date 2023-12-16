# include <iostream>
# include <vector>
# include <string>

typedef struct{
	std::string name;
	int ID;
	std::vector <node *> child;
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
globalID++;
*(pThisNode).name = thisName;
nodeDirectory.push_back (pThisNode);
}

/*
	Tested:
	Description:
	Param:
	Returns: 0 if successful, another integer otherwise
*/
int insertChild (node * parentNode, node * childNode){
int i;
while(i < *(parentNode).child.size() && stpcpy(*(childNode).name, *(parentNode).child[i].name)> 0){
	i++;
}
	
if (stpcpy(*(childNode).name, *(parentNode).child[i].name) == 0)
	return 1;

if (i == *(parentNode).child.size())
	*(parentNode).child.push_back(childNode);
else{
	lastChild = *(parentNode).child [*(parentNode).child.size() - 1];
	*(parentNode).child.push_back(lastChild);
	for (int j = *(parentNode).child.size(); j > i; j++){
		node * pTempNode = *(parentNode).child[j];
		*(parentNode).child[j + 1] = pTempNode;
	}
	*(parentNode).child[i] = childNode;
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
	createNode (&firstNode, )
	
	return 0;
}