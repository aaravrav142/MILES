/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
#include "../include/qdude/qnode.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace qdude {

/*****************************************************************************
** Implementation
*****************************************************************************/

QNode::QNode(int argc, char** argv )
{
	ros::init(argc,argv,"qdude");
	ros::NodeHandle n;
	chatter_publisher = n.advertise<std_msgs::String>("chatter", 1000);
}

QNode::~QNode() {
}
bool QNode::SetGoal(QString Location){
	
}
}
