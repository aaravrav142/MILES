#include <NodeList.h>
#include <iostream>
#include <fstream>
#include <string>

#include <stdio.h>
#include <stdlib.h> 



int main(int argc, char** argv){
	NodeList list;	
	list.loadFromFile("pose.txt","trajectory.txt");
	list.SortAll(true);
	std::cout<< list.toString();
	std::cout<< "All are sorted"<<std::endl;
	list.NormalizeAll();
	std::cout<< list.toString();
}

