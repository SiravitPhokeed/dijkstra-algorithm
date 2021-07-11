#include <iostream>
#include <vector>
#include "Graph.h"

void ShowHelp();

int main(int argc, char **argv)
{
	Graph MyGraph;
	ShowHelp();
	string cmd;
	bool IsLoop = true;
	do
	{
		cout << ">";
		cin >> cmd;
		if (cmd == "node")
		{
			string name;
			cout << "node name =";
			cin >> name;
			if (MyGraph.AddNode(name))
				cout << "Add node success" << endl;
		}
		else if (cmd == "link")
		{
			string FromNodeName;
			string ToNodeName;
			double weight;
			cout << "from node =";
			cin >> FromNodeName;
			cout << "to node =";
			cin >> ToNodeName;
			cout << "weight =";
			cin >> weight;
			if (MyGraph.AddLink(MyGraph.IndexOf(FromNodeName), MyGraph.IndexOf(ToNodeName), weight))
				cout << "Add link success" << endl;
		}
		else if (cmd == "save")
		{
			string Filename;
			cout << "file name =";
			cin >> Filename;
			if (MyGraph.SaveGraph(Filename))
				cout << "Save file success" << endl;
		}
		else if (cmd == "load")
		{
			string Filename;
			cout << "file name =";
			cin >> Filename;
			if (MyGraph.LoadGraph(Filename))
				cout << "Load file success" << endl;
		}
		else if (cmd == "list")
		{
			for (int i = 0; i < MyGraph.nodeList.size(); i++)
			{
				cout << "node " << i << " = " << MyGraph.nodeList[i].Name() << endl;
				for (int j = 0; j < MyGraph.nodeList[i].linkTo.size(); j++)
					cout << "  ->" << MyGraph.nodeList[i].linkTo[j].index << " weight =" << MyGraph.nodeList[i].linkTo[j].weight << endl;
			}
		}
		else if (cmd == "short")
		{
			string StartNode;
			string EndNode;
			cout << "start node =";
			cin >> StartNode;
			cout << "end node =";
			cin >> EndNode;
			cout << "a -> b -> d" << endl;
			/*			cout << "+---------------------------+" << endl;
			cout << "|    find shortest path     |" << endl;
			cout << "+---------------------------+" << endl;man	*/
		}
		else if (cmd == "help")
		{
			ShowHelp();
		}
		else if (cmd == "exit")
		{
			IsLoop = false;
		}
	} while (IsLoop);
	return 0;
}

void ShowHelp()
{
	cout << "----------------------" << endl;
	cout << "Please select command " << endl;
	cout << "node = add node, link = add link, save = save link, load = load link," << endl;
	cout << "list = list graph info, short = find shortest path, help = show help, exit = exit program." << endl;
}
