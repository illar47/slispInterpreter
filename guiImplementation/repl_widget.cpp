#include "repl_widget.hpp"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QKeyEvent>

REPLWidget::REPLWidget(QWidget * parent): QWidget(parent){
  // TODO: your code here
	repl = new QLineEdit; 

	//connect repl to this 
	QObject::connect(repl, &QLineEdit::returnPressed, this, &REPLWidget::changed); 

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(repl);

	this->setLayout(layout);
}

/**
* slot that indicates when the input has been changed. 
*/
void REPLWidget::changed() {

	//get input - provide to lineEntered 
	QString input = repl->text(); 
	repl->clear(); 
	emit lineEntered(input); 
	//user input modifying the string??? 
}
