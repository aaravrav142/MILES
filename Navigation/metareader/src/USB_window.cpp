#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/qdude/USB_window.hpp"
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>
#include <std_msgs/String.h>

namespace qdude {
using namespace boost;
using namespace Qt;
//using namespace LibSerial ;
USBWindow::USBWindow(int argc, char** argv,Ui::MainWindowDesign *main_ui, QWidget *parent)
	: QDialog(parent)
	, qnode(argc,argv)
{
	m_parent=parent;
	m_main_ui= main_ui;
	ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.

	//setWindowIcon(QIcon(":/images/icon.png"));
	
}
USBWindow::~USBWindow() {}

void USBWindow::on_buttonBox_accepted(){
	asio::io_service io;
	asio::serial_port port(io);
	port.open(ui.SerialPort->text().toStdString().c_str());
	port.set_option(asio::serial_port_base::baud_rate(9600));
	std::string command="MD "+ui.buildNo->currentText().toStdString()+" "+ui.FloorNo->currentText().toStdString()+"\n";
	asio::write(port,asio::buffer(command.c_str(),command.length()-1));
	printf("Reading\n");
	std::string answer="";
	while(1){
		while(1){
			char c;
			printf("Before\n");
			asio::read(port, asio::buffer(&c,1));
			printf("After\n");
			if(c=='\n')
				break;	
			answer=answer+c;
			printf("%s\n",answer.c_str());
		}
		if(answer.find("<END>")!=std::string::npos)
			break;		
		QString lineString(answer.c_str());
		QStringList info= lineString.split("||");
		QStringList name=info[2].split(": ");

		QString Location= name[1]+"("+info[0]+","+info[1]+")";
		std::string print=Location.toStdString();
		printf("%s\n",print.c_str());

		QString Description=info[3];
		print=Description.toStdString();

		printf("%s\n",print.c_str());
		
		printf("Finished Read\n");
	
	}
	port.close();
}

}