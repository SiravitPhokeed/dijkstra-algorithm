#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>

using namespace std;

class link
{
public:
	double weight;
	int index;
	link(int MyIndex, double MyWeight)
	{
		index = MyIndex;
		weight = MyWeight;
	}
};

class node
{
	string name;
	int FindLinkIndex(int linkIndex); //Explain in Graph.cpp.
public:
	vector<link> linkTo;
	double timeTake; // total time it take to go to each town 
	int PrevIndex;
	int status; // 0 is unexplored 1 is explored 2 exploring
	node(string MyName) { name = MyName; status=0; timeTake=2147483647;}
	~node() { linkTo.clear(); }
	string Name() { return name; }
	void SetLink(int linkIndex, double weight); //Explain in Graph.cpp.
};

class Graph
{
	bool IsDirectGraph;
	string ErrorCase(int e); //Explain in Graph.cpp.
public:
	vector<node> nodeList;
	Graph(bool IsDirect = false) { IsDirectGraph = IsDirect; }
	~Graph() { nodeList.clear(); }
	bool LoadGraph(string FileName);						   //Explain in Graph.cpp.
	bool SaveGraph(string FileName);						   //Explain in Graph.cpp.
	bool AddNode(string name);								   //Explain in Graph.cpp.
	bool AddLink(int BeginIndex, int EndIndex, double weight); //Explain in Graph.cpp.
	int IndexOf(string Name);								   //Explain in Graph.cpp.
	// vector<node> shortPath;
	// string shortestPath(string start, string end);
	// string listShortestPath(int startIndex, int endIndex);
	string ShortestPath(string start, string end);
	void Dijkstra(int startIndex, int endIndex);
	void Explorer(int startIndex, int endIndex);
	void ShortestEstimate(int startIndex, int endIndex);
	string ListShortestPath(int startIndex, int endIndex);
protected:
};

//====================== Method of node =================================
int node::FindLinkIndex(int linkIndex)
{
	for (int i = 0; i < linkTo.size(); i++)
		if (linkTo[i].index == linkIndex)
			return i;
	return -1;
}

void node::SetLink(int linkIndex, double weight)
{
	int Index = FindLinkIndex(linkIndex);
	if (Index < 0)
		linkTo.push_back(link(linkIndex, weight));
	else
		linkTo[Index].weight = weight;
}

//====================== Method of Graph ================================
bool Graph::LoadGraph(string FileName)
{
	bool result = true;
	try
	{
		ifstream Datafile(FileName.data());
		if (!Datafile.is_open())
			throw(101);
		else
		{
			string tp;
			getline(Datafile, tp);
			if (tp.compare("#== Graph Link Data File ==#") != 0)
				throw(102);
			int index;
			double weight;
			while (getline(Datafile, tp))
			{
				nodeList.push_back(node(tp));
				while (getline(Datafile, tp))
				{
					if (tp.compare("#----------#") != 0)
					{
						stringstream LinkInfo(tp);
						LinkInfo >> index;
						LinkInfo >> weight;
						nodeList.back().SetLink(index, weight);
					}
					else
						break;
				}
			}
		}
		Datafile.close();
	}
	catch (int e)
	{
		cout << "exception: " << ErrorCase(e) << '\n';
		nodeList.clear();
		result = false;
	}
	return result;
}
bool Graph::SaveGraph(string FileName)
{
	bool result = true;
	try
	{
		ofstream Datafile(FileName.data());
		if (!Datafile.is_open())
			throw(201);
		else
		{
			Datafile << "#== Graph Link Data File ==#" << endl;
			for (int i = 0; i < nodeList.size(); i++)
			{
				Datafile << nodeList[i].Name() << endl;
				for (int j = 0; j < nodeList[i].linkTo.size(); j++)
					Datafile << nodeList[i].linkTo[j].index << " " << nodeList[i].linkTo[j].weight << endl;
				Datafile << "#----------#" << endl;
			}
		}
	}
	catch (int e)
	{
		cout << "exception: " << ErrorCase(e) << '\n';
		result = false;
	}

	return result;
}
bool Graph::AddNode(string name)
{
	bool result = false;
	if (IndexOf(name) < 0)
	{
		nodeList.push_back(node(name));
		result = true;
	}
	else
		cout << "exception: " << ErrorCase(301) << '\n';
	return result;
}
bool Graph::AddLink(int BeginIndex, int EndIndex, double weight)
{
	bool result = true;
	try
	{
		int NodeMax = nodeList.size() - 1;
		if (BeginIndex > NodeMax || EndIndex > NodeMax)
			throw(401);
		else
		{
			nodeList[BeginIndex].SetLink(EndIndex, weight);
			if (!IsDirectGraph)
				nodeList[EndIndex].SetLink(BeginIndex, weight);
		}
	}
	catch (int e)
	{
		cout << "exception: " << ErrorCase(e) << '\n';
		result = false;
	}

	return true;
}
int Graph::IndexOf(string Name)
{
	int result = -1;
	for (int i = 0; i < nodeList.size(); i++)
	{
		if (nodeList[i].Name() == Name)
		{
			result = i;
			break;
		}
	}
	return result;
}

// string Graph::shortestPath(string start, string end){
// 	// vector<node> path;
// 	int startIndex = IndexOf(start);
// 	int endIndex = IndexOf(end);
// 	// cout << startIndex << endl; // debug
// 	if (start == end) {
// 		// shortPath.push_back(nodeList[endIndex]);
// 		string shortestString = "";
// 		for (int i=0; i<shortPath.size(); i++){
// 			shortestString += shortPath[i].Name();
// 			if (i < (shortPath.size() - 1)) {
// 				shortestString += " -> ";
// 			} else {
// 				shortestString += "\n";
// 			}
// 		}
// 		cout << shortestString;
// 		return shortestString;
// 	}
// 	// cout << endIndex << endl;  // debug
// 	nodeList[startIndex].status = 1;
// 	nodeList[0].timeTake = 0;
// 	shortPath.push_back(nodeList[startIndex]);
// 	vector<link> unexploredVector = nodeList[startIndex].linkTo;
// 	for (int i=0; i<unexploredVector.size(); i++) {
// 		// cout << unexploredVector[i].index << endl;  // debug
// 		// cout << unexploredVector[i].weight << endl;  // debug
// 		int currentNode = unexploredVector[i].index;
// 		if (nodeList[currentNode].status == 0 || nodeList[currentNode].status == 2) {
// 			nodeList[currentNode].status = 2;  // set nodes to exploring
// 			if (nodeList[startIndex].timeTake + unexploredVector[i].weight < nodeList[currentNode].timeTake) {
// 				// cout << nodeList[currentNode].Name() << endl;  // debug
// 				// cout << nodeList[startIndex].timeTake << " + " << unexploredVector[i].weight << endl;
// 				nodeList[currentNode].timeTake = nodeList[startIndex].timeTake + unexploredVector[i].weight;  // set time take to node
// 			}
// 			else shortPath.pop_back();
// 			// cout << nodeList[startIndex].Name() << " -> " << nodeList[currentNode].Name() << "\t"<< nodeList[currentNode].timeTake << endl; //debug
// 			shortestPath(nodeList[currentNode].Name(), end);
// 		}
// 	}
// 	shortPath.push_back(nodeList[endIndex]);
// 	return ""; // idk why this work but it has to be here
// }

// string Graph::listShortestPath(int startIndex, int endIndex) {
// 	;
// }

string Graph::ShortestPath(string start, string end) {
	int startIndex = IndexOf(start);
	int endIndex = IndexOf(end);
	Dijkstra(startIndex, endIndex);
	return ListShortestPath(startIndex, endIndex);
}

//------------------ private Method --------------------

void Graph::Dijkstra(int startIndex, int endIndex) {
	nodeList[startIndex].status = 1;  // set start node to explored
	nodeList[startIndex].timeTake = 0;  // set weight to 0
	nodeList[startIndex].PrevIndex = startIndex;  // set path marker
	Explorer(startIndex, endIndex);
}

void Graph::Explorer(int startIndex, int endIndex) {
	int linkIndex;
	double currentTotalWeight;
	vector<link> exploringList = nodeList[startIndex].linkTo;
	for (int i=0; i<exploringList.size(); i++) {
		linkIndex = exploringList[i].index;;
		currentTotalWeight = nodeList[startIndex].timeTake + exploringList[i].weight;
		// cout << nodeList[startIndex].timeTake << "+" << exploringList[i].weight << " = " << currentTotalWeight << endl;  // debug
		if (nodeList[linkIndex].status == 0) {  // if unexplored
			nodeList[linkIndex].timeTake = currentTotalWeight;  // set to current time taken
			nodeList[linkIndex].PrevIndex = startIndex;  // set path marker
			nodeList[linkIndex].status = 2;  // set to exploring
		} else if (nodeList[linkIndex].status == 2 && nodeList[linkIndex].timeTake > currentTotalWeight) {
			nodeList[linkIndex].timeTake = currentTotalWeight;
			nodeList[linkIndex].PrevIndex = startIndex;  // set path marker
		}
	}

	// find the closest exploring node to be the next explored node
	int nextExploredIndex = -1;
	float nextExploredWeight = 0;
	for (int i=0; i<nodeList.size(); i++) {
		if (nodeList[i].status == 2 &&
			(nextExploredIndex == -1 || nextExploredWeight > nodeList[i].timeTake)) {
			nextExploredIndex = i;
			nextExploredWeight = nodeList[i].timeTake;
		}
	}
	nodeList[nextExploredIndex].status = 1;
	if (nextExploredIndex != endIndex) {
		Explorer(nextExploredIndex, endIndex);
	} else if (nextExploredIndex == endIndex) {
		cout << "total weight: " << currentTotalWeight << endl;
	}
}

string Graph::ListShortestPath(int startIndex, int endIndex) {
	if (startIndex != nodeList[endIndex].PrevIndex)  // if not the one before the end, bring the end closer to the start
		return ListShortestPath(startIndex, nodeList[endIndex].PrevIndex) + " -> " + nodeList[endIndex].Name();
	else  // the end
		return nodeList[startIndex].Name() + " -> " + nodeList[endIndex].Name();
}

string Graph::ErrorCase(int e)
{
	string result;
	switch (e)
	{
	case 101:
		result = "File not found.";
		break;
	case 102:
		result = "File not graph format.";
		break;
	case 201:
		result = "Can't open file for write.";
		break;
	case 301:
		result = "Duplicate node in add node.";
		break;
	case 401:
		result = "IndexNode out of range in add link.";
		break;
	default:
		result = "Internal error";
	}
	return result;
}

#endif
