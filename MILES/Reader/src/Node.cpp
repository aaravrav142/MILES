#include <Node.h>
#include <cmath> 

//This will sort the MACS from greatest to least
void Node::Sort(bool up){
	router rout;
	bool done=false;
	for(int x=0;x<(sizeof(this->routers)/sizeof(this->routers[0])-1);x++){
		int y=x;
		while(!done){
			if((y<0)||this->routers[y].Strength > this->routers[y+1].Strength){
				done=true;
			}
			else{
				rout=this->routers[y];
				this->routers[y]=this->routers[y+1];
				this->routers[y+1]=rout;
				y--;
			}		
		}
		done=false;
			
	}
	return;
}
//This will normalize the node with respect to the highest signal strength
void Node::Normalize(){
	int max=0;
	for(int x=0;x<(sizeof(this->routers)/sizeof(this->routers[0]));x++){
		if(this->routers[x].Strength!=0){
			if(this->routers[x].Strength<max)
				max=this->routers[x].Strength;
		}
	}
	for(int x=0;x<(sizeof(this->routers)/sizeof(this->routers[0]));x++){
		if(this->routers[x].Strength!=0)
			this->routers[x].Strength=this->routers[x].Strength/max;
	}
	//printf("%d\n",max);
}

void Node::Subtract(Node *curReading){
	//printf("Entered Subtraction\n");
	for(int x=0; x<curReading->routerCount;x++){
		for(int y=0; y<this->routerCount;y++){
			this->routers[y].Diff=1;
			if(curReading->routers[x].MAC==this->routers[y].MAC&&this->routers[y].MAC!=""&&curReading->routers[x].MAC!=""){
				this->routers[y].Diff=curReading->routers[x].Strength-this->routers[y].Strength;
				
				this->routers[y].Diff=fabs(this->routers[y].Diff);

				break;
				}
		}
	}
}

void Node::Add(){
	//printf("Entered Add\n");
	for(int x=0; x<this->routerCount;x++)
		this->prob+=this->routers[x].Diff;
	this->prob/=(routerCount+1);
	printf("\nProb: %f\n",this->prob);
	this->prob/=Pfactor;
	//printf("Pfactor: %f\n",this->Pfactor);
}

//standard toString() for the pose
std::string pose::toString(){
	return "x: "+std::to_string(this->x)+"\ty: "+std::to_string(this->y)+"\tz: "+std::to_string(this->z);
}
//standard toString() for the MAC
std::string router::toString(){
	return "MAC: "+this->MAC+"\tStr: "+std::to_string(this->Strength);
}
//standard toString()  for the Node for debugging
std::string Node::toString(){
	std::string ret;

	ret="____________________________________________\nProb:\n"+std::to_string(this->prob)+"\n"+this->pos.toString()+"\n";

	for(int x=0;x<(sizeof(this->routers)/sizeof(this->routers[0]));x++){
		if(this->routers[x].MAC!="")
			ret=ret+this->routers[x].toString()+"\n";
	}
	return ret;
}

