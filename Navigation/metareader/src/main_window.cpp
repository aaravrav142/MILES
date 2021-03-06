/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date February 2011
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/qdude/main_window.hpp"
#include "../include/qdude/USB_window.hpp"
#include "../include/qdude/Port_window.hpp"

#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace qdude {
using namespace boost;
using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
	: QMainWindow(parent)
	, qnode(argc,argv)
{
	ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.

	setWindowIcon(QIcon(":/images/MILES.png"));

}

MainWindow::~MainWindow() {}

/*****************************************************************************
** Implementation [Slots]
*****************************************************************************/

/*
 * These triggers whenever the button is clicked, regardless of whether it
 * is already checked or not.
 */
void MainWindow::on_Locations_currentIndexChanged(int index){
	ui.Description->setText(ui.Locations->itemData(index).toString());
}
void MainWindow::on_Go_clicked(bool check ) {
	QString goal=ui.Locations->currentText();
	if(goal.isEmpty()||goal.isNull())
		return;
	QStringList Coords= goal.split("(x: ");
	goal=Coords[1];
        goal.replace(")","");
        Coords=goal.split(",y: ");
	std::string xstr= Coords[0].toStdString();
	std::string ystr= Coords[1].toStdString();
	double x=atof(xstr.c_str());
	double y=atof(ystr.c_str());
	printf("%f\n",x);
	printf("%f\n",y);
	qnode.SetGoal(x,y);
}
void MainWindow::on_Close_clicked(bool check ) {
	QWidget::close();
}
void MainWindow::on_actionPort_triggered(){
	int argc=0;
	char **argv=NULL;
	qdude::PortWindow w(argc,argv,this);
	w.exec();
}
void MainWindow::on_actionMetadata_From_File_triggered(){
	//Get the filename
	QString fileName = QFileDialog::getOpenFileName(this, tr("Metadata File"),
		                                        "/home",
		                                        tr("Metadata File (*.txt)"));
	//Now we parse the file and add to the Description
	FILE* metafile=fopen(fileName.toStdString().c_str(),"r");

	char line[10000];
	while(fgets(line,1000,metafile)!=NULL){
		QString lineString(line);
		QStringList info= lineString.split("||");
		QStringList name=info[2].split(": ");

		QString Location= name[1]+"("+info[0]+","+info[1]+")";
		std::string print=Location.toStdString();
		printf("%s\n",print.c_str());

		QString Description=info[3];
		print=Description.toStdString();

		printf("%s\n",print.c_str());
		ui.Locations->addItem(Location,Description);

	}
	fclose(metafile);
}
void MainWindow::on_actionMetadata_From_USB_triggered(){
	int argc=0;
	 char **argv=NULL;
    qdude::USBWindow w(0,m_port,&ui);
    w.exec();
}
void MainWindow::on_actionPose_Estimate_triggered(){
asio::io_service io;
	asio::serial_port port(io);
	port.open(this->m_port.c_str());
	port.set_option(asio::serial_port_base::baud_rate(9600));
	port.set_option(asio::serial_port::flow_control(asio::serial_port::flow_control::none));
      	port.set_option(asio::serial_port::parity(asio::serial_port::parity::none));
     	port.set_option(asio::serial_port::stop_bits(asio::serial_port::stop_bits::one));
     	port.set_option(asio::serial_port::character_size(8));

	std::string command= "LONCE\n";

	asio::write(port,asio::buffer(command.c_str(),command.length()));

	std::string answer="";
	while(1){
		char c;
		//printf("Before\n");
		asio::read(port, asio::buffer(&c,1));
		//printf("After\n");
		if(c=='\n'){
			printf("Reached the end of line\n");
			break;	
		}
		answer=answer+c;
	
	}
	printf("%s\n",answer.c_str());
	port.close();
}

/*****************************************************************************
** Implementation [Configuration]
*****************************************************************************/

void MainWindow::ReadSettings() {
    QSettings settings("Qt-Ros Package", "qdude");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
   
    
}

void MainWindow::WriteSettings() {
    QSettings settings("Qt-Ros Package", "qdude");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	WriteSettings();
	QMainWindow::closeEvent(event);
}

}  // namespace qdude

