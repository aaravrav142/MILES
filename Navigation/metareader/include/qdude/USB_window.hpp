#ifndef qdude_USB_WINDOW_H
#define qdude_USB_WINDOW_H

#include <QDialog>
#include "ui_USB_window.h"
#include "ui_main_window.h"
#include "qnode.hpp"

//#include "libusb-1.0/libusb.h"

namespace qdude {

class USBWindow : public QDialog {
Q_OBJECT

public:
	USBWindow(int argc, char** argv,Ui::MainWindowDesign *main_ui, QWidget *parent = 0);
	~USBWindow();

public Q_SLOTS:
	/******************************************
	** Auto-connections (connectSlotsByName())
	*******************************************/
    void on_buttonBox_accepted();
    //void on_Close_clicked(bool check );

private:
	Ui::USBWindowDesign ui;
	Ui::MainWindowDesign *m_main_ui;
	QNode qnode;
	//libusb_device **devs;
	//libusb_context *ctx;
	QWidget *m_parent;
};

}  // namespace qdude

#endif // qdude_USB_WINDOW_H