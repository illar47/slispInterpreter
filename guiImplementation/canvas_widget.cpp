#include "canvas_widget.hpp"

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLayout>

CanvasWidget::CanvasWidget(QWidget * parent): QWidget(parent){
	scene = new QGraphicsScene(this); 
	QGraphicsView * view =  new QGraphicsView(scene); 
  // TODO: your code here...

	QHBoxLayout* layout = new QHBoxLayout;
	//idk why you no work
	layout->addWidget(view);

	this->setLayout(layout);
}

/**
* A public slot that accepts a signal in the form of a QGraphicsItem Pointer containing
* an object derived from QGraphicsItem to draw. 
*/
void CanvasWidget::addGraphic(QGraphicsItem * item){
  // TODO: your code here...
	scene->addItem(item); 
	

}
