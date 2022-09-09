#include "message_widget.hpp"

#include <QLabel>
#include <QLayout>
#include <QLineEdit>

#include <iostream>

MessageWidget::MessageWidget(QWidget *parent) : QWidget(parent) {

	msg = new QLineEdit; 
	msg->setReadOnly(true); //make it so that it is not editable by user
	//are these necessary??
	//QObject::connect(msg, &QLineEdit::returnPressed, this, &MessageWidget::info);
	//QObject::connect(msg, &QLineEdit::returnPressed, this, &MessageWidget::error);
	//QObject::connect(msg, &QLineEdit::returnPressed, this, &MessageWidget::clear); //may need to change from dispText method

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(msg);

	this->setLayout(layout);
}

/**
* a public slot accepting informational message to display, clearing any error formmating 
*/
void MessageWidget::info(QString message) {
  // TODO: your code here...

	msg->setText(message); 
	//msg->setStyleSheet("{ background-color: white }");
	QPalette p = msg->palette();
	p.setColor(QPalette::Highlight, QColor("blue"));
	msg->setPalette(p);
	msg->deselect();
	emit msg->displayText(); //I think this should work???
}

/**
* a public slot accepting an error emssage to display as selected text highlighted with a red background
*/
void MessageWidget::error(QString message) {

	msg->setText(message);

	QPalette p = msg->palette();
	p.setColor(QPalette::Highlight, QColor("red"));
	msg->setPalette(p); 
	msg->selectAll(); 
	msg->displayText(); //need to display the text with red highlight
}

/**
* clear out any currently displayed text 
*/
void MessageWidget::clear() {
  // TODO: your code here...
	//msg->setStyleSheet("{ background-color: white }");
	QPalette p = msg->palette();
	p.setColor(QPalette::Highlight, QColor("blue"));
	msg->setPalette(p);
	msg->deselect();
	emit msg->clear(); 
}
