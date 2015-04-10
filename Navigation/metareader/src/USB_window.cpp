#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/qdude/USB_window.hpp"
#include <SerialStream.h>
namespace qdude {

using namespace Qt;
using namespace LibSerial ;
USBWindow::USBWindow(int argc, char** argv,Ui::MainWindowDesign *main_ui, QWidget *parent)
	: QDialog(parent)
	, qnode(argc,argv)
{
	m_parent=parent;
	m_main_ui= main_ui;
	ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.

	setWindowIcon(QIcon(":/images/icon.png"));
	
}
USBWindow::~USBWindow() {}

void USBWindow::on_buttonBox_accepted(){
	SerialStream serial_port ;
	printf("%s\n",ui.SerialPort->text().toStdString().c_str());
	serial_port.Open(ui.SerialPort->text().toStdString());
     // Set the baud rate of the serial port.
	serial_port.SetBaudRate( SerialStreamBuf::BAUD_9600 ) ;
	serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
	serial_port.SetNumOfStopBits(1) ;
	serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
	serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
	serial_port.SetVTime(15);
 	serial_port.SetVMin(255);

	if ( ! serial_port.good() )
	{
		printf("Serial Port did not open\n");
		//Display Message
		return;
	}
	//Write the command to get the metadata
	serial_port << "MD "<< ui.buildNo->currentText().toStdString().c_str()<<" "<< ui.FloorNo->currentText().toStdString().c_str()<<std::endl ;
	//then read lines until empty <END>
	printf("Finished send\n");
//
// Read a whole array of data from the serial port. 

	const int BUFFER_SIZE = 512 ;
	char input_buffer[BUFFER_SIZE] ; 
	while(true){
		//sleep(2) ;
		int i=0;
		serial_port.read( input_buffer, BUFFER_SIZE ) ;
		std::string answer(input_buffer);
		printf("%s Finished Read\n",answer.c_str());
		if(answer.find("<END>")>0)
			break;
		//Now add to the metadata
		QString lineString(answer.c_str());
		QStringList info= lineString.split("||");
		QStringList name=info[2].split(": ");

		QString Location= name[1]+"("+info[0]+","+info[1]+")";
		std::string print=Location.toStdString();
		printf("%s\n",print.c_str());

		QString Description=info[3];
		print=Description.toStdString();

		printf("%s\n",print.c_str());
		m_main_ui->Locations->addItem(Location,Description);
	}
}

}
