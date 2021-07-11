#include "Graph.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
//====================== Method of node =================================
int node::FindLinkIndex(int linkIndex) {
	for (int i=0;i<linkTo.size();i++)	
		if(linkTo[i].index==linkIndex) 
			return i; 
	return -1; 
}	

void node::SetLink(int linkIndex,double weight) { 
	int Index = FindLinkIndex(linkIndex);
	if(Index<0)
    	linkTo.push_back(link(linkIndex,weight));
	else
		linkTo[Index].weight = weight; 	
}

//====================== Method of Graph ================================
bool Graph::LoadGraph(string FileName){
	bool result = true;
	try	{
		ifstream Datafile(FileName.data());
   		if (!Datafile.is_open())
   			throw (101);
		else {   
      		string tp;
      		getline(Datafile, tp);
      		if(tp.compare("#== Graph Link Data File ==#")!=0) throw(102);
      		int index;
      		double weight;
      		while(getline(Datafile, tp)){
      			nodeList.push_back(node(tp));
				while(getline(Datafile, tp)){ 
         			if(tp.compare("#----------#")!=0){
         				stringstream LinkInfo(tp);
         				LinkInfo >> index;
						LinkInfo >> weight;  
						nodeList.back().SetLink(index,weight);
				 	}
         			else break;
            	}	
			}
      	}
      	Datafile.close(); 
	} catch(int e) {
		cout <<  "exception: " << ErrorCase(e) << '\n';
		nodeList.clear();
		result =false;
	}
	return result;
}
bool Graph::SaveGraph(string FileName){
	bool result = true;
	try{
		ofstream Datafile(FileName.data());
		if (!Datafile.is_open())
			throw(201);
		else {
			Datafile << "#== Graph Link Data File ==#" << endl;
			for (int i=0; i<nodeList.size();i++) {
				Datafile << nodeList[i].Name() << endl;
				for (int j=0;j<nodeList[i].linkTo.size() ;j++)
					Datafile << nodeList[i].linkTo[j].index << " " << nodeList[i].linkTo[j].weight << endl;	
				Datafile <<	"#----------#" << endl;		
			}
		}
	} catch(int e) {
		cout <<  "exception: " << ErrorCase(e) << '\n';
		result =false;		
	}
	
	return result;
}
bool Graph::AddNode(string name){
	bool result=false;
	if(IndexOf(name)<0){
		nodeList.push_back(node(name));
		result = true;
	} else
	  cout <<  "exception: " << ErrorCase(301) << '\n';
	return result;
}
bool Graph::AddLink(int BeginIndex,int EndIndex,double weight){
	bool result = true;
	try{
		int NodeMax = nodeList.size()-1;
		if (BeginIndex>NodeMax || EndIndex>NodeMax)
		   	throw(401);	
		else {
			nodeList[BeginIndex].SetLink(EndIndex,weight);
			if (!IsDirectGraph) 
				nodeList[EndIndex].SetLink(BeginIndex,weight);
		}
	} catch(int e) {
		cout <<  "exception: " << ErrorCase(e) << '\n';
		result =false;			
	}
 
	return true;
}
int Graph::IndexOf(string Name){
	int result=-1;
	for (int i=0;i<nodeList.size();i++){
		if(nodeList[i].Name()==Name){
			result=i;
			break;
		}
	}
	return result;
}
//------------------ private Method --------------------

string Graph::ErrorCase(int e){
	string result;
	switch(e){
		case 101:
			result ="File not found."; 
			break;
		case 102:
			result ="File not graph format.";
			break;
		case 201:
		    result ="Can't open file for write.";
			break;
		case 301:
			result ="Duplicate node in add node.";
			break;	
		case 401:
			result ="IndexNode out of range in add link.";		
			break;
		default:
			result ="Internal error";		
	}
	return result;
}




