#include "main_window.hpp"

#include <iostream>

#include <QLayout>

#include "message_widget.hpp"
#include "canvas_widget.hpp"
#include "repl_widget.hpp"
#include "interpreter_semantic_error.hpp"

//myIncludes
#include "qt_interpreter.hpp"
/**
* default constructor
*/
MainWindow::MainWindow(QWidget * parent): MainWindow("", parent){
  // TODO: your code here...

	REPLWidget *repl = new REPLWidget;
	CanvasWidget* canvas = new CanvasWidget; 
	MessageWidget *msg = new MessageWidget;

	//should the mainwindow also have the interpreter??? 
	QtInterpreter *interp = new QtInterpreter; 

	//add all the connections between them
	QObject::connect(repl, &REPLWidget::lineEntered, interp, &QtInterpreter::parseAndEvaluate); 
	QObject::connect(interp, &QtInterpreter::info, msg, &MessageWidget::info); 
	QObject::connect(interp, &QtInterpreter::error, msg, &MessageWidget::error); 
	QObject::connect(interp, &QtInterpreter::drawGraphic, canvas, &CanvasWidget::addGraphic); 


	//connect interp to message and canvas 

	//link everthing up 
	QVBoxLayout *layout = new QVBoxLayout; 
	layout->addWidget(repl); 
	layout->addWidget(canvas); 
	layout->addWidget(msg); 

	this->setLayout(layout);

}

/**
* default constructor using a filename as a script file 
*/
MainWindow::MainWindow(std::string filename, QWidget * parent): QWidget(parent){
  // TODO: your code here...

	REPLWidget* repl = new REPLWidget;
	CanvasWidget* canvas = new CanvasWidget;
	MessageWidget* msg = new MessageWidget;

	//should the mainwindow also have the interpreter??? 
	QtInterpreter* interp = new QtInterpreter;

	//add all the connections between them
	QObject::connect(repl, &REPLWidget::lineEntered, interp, &QtInterpreter::parseAndEvaluate);
	QObject::connect(interp, &QtInterpreter::info, msg, &MessageWidget::info);
	QObject::connect(interp, &QtInterpreter::error, msg, &MessageWidget::error);
	QObject::connect(interp, &QtInterpreter::drawGraphic, canvas, &CanvasWidget::addGraphic);


	//connect interp to message and canvas 

	//link everthing up 
	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(repl);
	layout->addWidget(canvas);
	layout->addWidget(msg);

	this->setLayout(layout);

	//make another method in qt_interpreter and call down here 
	interp->fileRead(filename); 
}
