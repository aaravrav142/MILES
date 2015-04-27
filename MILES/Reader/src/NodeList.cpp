
#include <NodeList.h>
#include <Node.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath> 
#include <stdio.h>
#include <stdlib.h> 
//Normalize all the nodes in the nodelist
void NodeList::NormalizeAll(){
	for(int x=0;x<this->nodeCount;x++){
		this->list[x].Normalize();
	}
}

//Sort the MACs of all the nodes in the nodelist
void NodeList::SortAll(bool up){
	for(int x=0;x<this->nodeCount;x++){
		this->list[x].Sort(up);
	}
}
//Standard toString() for debugging. Prints out all nodes
std::string NodeList::toString(){
	std::string ret="";
	for(int x=0;x<this->nodeCount;x++){
		ret+="\n"+std::to_string(x)+this->list[x].toString();
	}
	return ret;
}
//Standard toString() for debugging. Prints a single node
std::string NodeList::toString(int x){
	return this->list[x].toString();
}
//Standard toString() for debugging. Prints node section from begin to end
std::string NodeList::toString(int begin, int end){
	std::string ret;
	if(begin>end){
		if(begin>this->nodeCount)
			begin=this->nodeCount;
		for(int x=end;x<begin;x++)
			ret+=this->list[x].toString();
	}	
	else{
		if(end>this->nodeCount)
			end=this->nodeCount;
		for(int x=begin;x<end;x++)
			ret+=this->list[x].toString();
	}
	return ret;
}
//Prints out a single router from all nodes. Show the change in strength over change in position
std::string NodeList::RouterInfo(std::string MAC){
	std::string ret=MAC+" info:\n";
	for(int x=0;x<this->nodeCount;x++){
		int y=0;
		while(this->list[x].routers[y].Strength!=0){
			if(this->list[x].routers[y].MAC==MAC){
				ret+="\n"+this->list[x].pos.toString()+" Strength: "+this->list[x].routers[y].toString();
				break;
			}
			y++;	
		}
		
	}
	return ret;
}

//Load the NodeList from file.
//The files needed will be the wifi file created from the wifi listener, 
//and the trajectory file created by rgbdslam
void NodeList::loadFromFile(std::string wifiFILE)
{
	//Read from both the trajectory file from rgbdslam and the wifi file from wifi scanner
	FILE* wfile=fopen(wifiFILE.c_str(),"r");

	
	char line[10000];
	this->nodeCount =0;
	//Go through the wifi file line by line
	while(fgets(line,10000,wfile)!=NULL){
		std::string lineString = line;
		
		//get x
		int front= lineString.find(":");
		
		int back = lineString.find("||");
		back=back-1;
		std::string x = lineString.substr(front+1,back-front);
		//std::cout <<"x "<<x<<" "<<front<<" "<<back<< std::endl;
		lineString=lineString.substr(back+2,std::string::npos);
				
		//get y
		front= lineString.find(":");

		back = lineString.find("||");
		back=back-1;
	
		std::string y = lineString.substr(front+1,back-front);
		//std::cout <<"y "<<y<<" "<<front<<" "<<back<< std::endl;
		lineString=lineString.substr(back+2,std::string::npos);

		//get z
		front= lineString.find(":");
		
		back = lineString.find("||");
		back=back-1;
		
		std::string z = lineString.substr(front+1,back-front);
		//std::cout <<"z "<<z<< std::endl;
		lineString=lineString.substr(back+2,std::string::npos);

		//get time
		front= 0;

		back = lineString.find("||");
		back=back-1;
		
		std::string time = lineString.substr(front+1,back-front);
		//std::cout <<"time "<<time<< std::endl;
		lineString=lineString.substr(back+2,std::string::npos);
		
		this->list[this->nodeCount].pos.x=std::stod(x.c_str());
		this->list[this->nodeCount].pos.y=std::stod(y.c_str());
		this->list[this->nodeCount].pos.z=std::stod(z.c_str());
		std::string MAC;
		std::string Str;
		this->list[this->nodeCount].routerCount=0;
		
		while(lineString.find("MAC")!=std::string::npos){

			front=lineString.find("[MAC: ");
			back=lineString.find(",");
			MAC = lineString.substr(front+6,back-front-6);
			//std::cout <<"MAC "<<MAC<< std::endl;
			
			front=lineString.find("Str ");
			back=lineString.find("]");
			Str = lineString.substr(front+4,back-front-4);
			//std::cout <<"Str " <<Str<< std::endl;
			double num= std::stod(Str.c_str());
			lineString=lineString.substr(back+1,std::string::npos);
			//std::cout <<lineString<< std::endl;
			//convert from string and put in the router array
			this->list[this->nodeCount].routers[this->list[this->nodeCount].routerCount].MAC=MAC;
			this->list[this->nodeCount].routers[this->list[this->nodeCount].routerCount].Strength=num;
			this->list[this->nodeCount].routerCount++;
			//break;
		}
		//break;
		this->list[this->nodeCount].id=this->nodeCount;
		this->list[this->nodeCount].Pfactor=1;
		this->nodeCount++;
		
	}
        fclose(wfile);
}

void NodeList::toFile(std::string filename){
	FILE* outfile=fopen(filename.c_str(),"w");
	fprintf(outfile,"%s\n",this->toString().c_str());
	fclose(outfile);

}

void NodeList::Subtract(Node *curReading){
	for(int x=0; x<this->nodeCount;x++){
		this->list[x].Subtract(curReading);		
	}
}

void NodeList::Add(){
	for(int x=0; x<this->nodeCount;x++){
		this->list[x].Add();		
	}
	//printf("Add finished\n");
}

std::vector<int> NodeList::findMins(){
	std::vector<int> mins;
	double minimum=1;
	int abs=0;
	for(int x=0;x<this->nodeCount;x++){
		if(this->list[x].prob<minimum){
			minimum=this->list[x].prob;	
			abs=x;
			mins.push_back(x);
		}
	}
	printf("Minimum :%f\n",minimum);
	for(int x=0;x<this->nodeCount;x++){
		if(this->list[x].prob>minimum-.05&&this->list[x].prob<minimum+.05){
			if(x!=abs)
				mins.push_back(x);
	}
	}
	return mins;
}



void NodeList::FindNN(NodeList *plist, int index){
	//Make a 2x2 square with the center being the index position
	//double Llimitx=this->list[index].pos.x-1;
	//double Llimity=this->list[index].pos.y-1;
	//double Rlimitx=this->list[index].pos.x+1;
	//double Rlimity=this->list[index].pos.y+1;
	int test=0;
	for(int x=0; x<this->nodeCount;x++){
		double compx= fabs(this->list[index].pos.x-this->list[x].pos.x);
		double compy= fabs(this->list[index].pos.y-this->list[x].pos.y);
		
		if(compy<4&&compx<4){
			double factor= (8-compx+compy)/300;
			//printf("Factor1 %f\n", this->list[x].Pfactor);
			this->list[x].Pfactor+=factor;
			if(this->list[x].Pfactor<0)
				this->list[x].Pfactor=0;
			//printf("Factor2 %f\n", this->list[x].Pfactor);
			test++;
		}	
		
	}
	printf("x: %f y: %f factored: %d\n",this->list[index].pos.x,this->list[index].pos.y,test);
	

}
