#include "qt_interpreter.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <fstream>

#include <QBrush>
#include <QDebug>

#include "qgraphics_arc_item.hpp"

#include "interpreter_semantic_error.hpp"

QtInterpreter::QtInterpreter(QObject * parent): QObject(parent){
  // TODO: your code here...

}

//slot 

void QtInterpreter::parseAndEvaluate(QString entry){
  // TODO: your code here...
	std::string program = entry.toStdString(); 
	std::istringstream iss(program);
	QString msg; 
	bool ok = interp.parse(iss);
	if (!ok) {
		//std::cerr << "parsing error when calling:  " << program << std::endl;
		//update error - emmit
	    msg = QString::fromStdString("Error: parsing failure"); 
		emit error(msg); 
	}
	else {
		Expression result;
		try {
			result = interp.eval();
			//check for graphic stuffs
			
			//issue with the type not being NoneType
			 
			if (result.head.type == NoneType && result.head.value.sym_value == "draw") {
				
				//create graphics for each item in tail 
				for (int i = 0; i < result.tail.size(); i++) {
					
					Expression curr = result.tail[i]; 
					if (curr.head.type == PointType) {

						Point currPoint = curr.head.value.point_value; 
						QGraphicsEllipseItem* pnt = new QGraphicsEllipseItem(currPoint.x, currPoint.y, 1, 1); 
						//pnt->setRect(currPoint.x, currPoint.y, 0, 0); 

						emit drawGraphic(pnt); 
						
						
					}
					else if (curr.head.type == LineType) {
						Line currLine = curr.head.value.line_value; 
						QGraphicsLineItem* line = new QGraphicsLineItem(currLine.first.x, currLine.first.y, currLine.second.x, currLine.second.y); 
						//Line currLine = curr.head.value.line_value; 
						emit drawGraphic(line); 
					}
					else if (curr.head.type == ArcType) {

						
						Arc currArc = curr.head.value.arc_value; 
						//currArc.start.x, currArc.start.y, currArc.center.x, currArc.center.y
						double bigD = std::sqrt(std::pow((currArc.start.x - currArc.center.x), 2) + std::pow((currArc.start.y - currArc.center.y), 2)); 
						qreal xVal = currArc.center.x - bigD; 
						qreal yVal = currArc.center.y - bigD; 
						qreal theRealBigD = bigD * 2; 
						QGraphicsArcItem* arc = new QGraphicsArcItem(xVal, yVal, theRealBigD, theRealBigD);
						double calculatingTheBitch = currArc.span * (180 / atan2(0, -1) )* 16; 
						arc->setSpanAngle(calculatingTheBitch); //I wannnnaaaaaaaa dieeeeeee -_-
						emit drawGraphic(arc); 

					}
					//else error
	
				}
				
			}
			else {
				std::string toPrint = result.printExpression(result);
				msg = QString::fromStdString(toPrint);
				emit info(msg);
			}
			
		}
		catch (InterpreterSemanticError) {
			msg = QString::fromStdString("Error: evaluation failure");
			emit error(msg);
		}
		
	}
}

/**
* helper function to help read files 
*/
void QtInterpreter::fileRead( std::string inputFile){
	//Interpreter interp; 
	std::ifstream input(inputFile); 
	Expression result;
	std::string toPrint;
	QString msg;

	//is valid filename
		if (input) {
			bool ok = this->parse(input); //parse the input 
			try {
				result = this->eval();
			}
			catch (InterpreterSemanticError) {
				//output handled in eval() method. 
				msg = QString::fromStdString("Error: bad parse"); 
				emit error(msg);
			}
			
			//add check if valid 
			if (ok) {
				//std::cout << toPrint << std::endl;

				for (int i = 0; i < this->toDraw.size(); i++) {
					Expression curr = toDraw[i];
					if (curr.head.type == PointType) {

						Point currPoint = curr.head.value.point_value;
						QGraphicsEllipseItem* pnt = new QGraphicsEllipseItem(currPoint.x, currPoint.y, 1, 1);
						//pnt->setRect(currPoint.x, currPoint.y, 0, 0); 

						emit drawGraphic(pnt);


					}
					else if (curr.head.type == LineType) {
						Line currLine = curr.head.value.line_value;
						QGraphicsLineItem* line = new QGraphicsLineItem(currLine.first.x, currLine.first.y, currLine.second.x, currLine.second.y);
						//Line currLine = curr.head.value.line_value; 
						emit drawGraphic(line);
					}
					else if (curr.head.type == ArcType) {


						Arc currArc = curr.head.value.arc_value;
						//currArc.start.x, currArc.start.y, currArc.center.x, currArc.center.y
						double bigD = std::sqrt(std::pow((currArc.start.x - currArc.center.x), 2) + std::pow((currArc.start.y - currArc.center.y), 2));
						qreal xVal = currArc.center.x - bigD;
						qreal yVal = currArc.center.y - bigD;
						qreal theRealBigD = bigD * 2;
						QGraphicsArcItem* arc = new QGraphicsArcItem(xVal, yVal, theRealBigD, theRealBigD);
						double calculatingTheBitch = currArc.span * (180 / atan2(0, -1)) * 16;

						arc->setSpanAngle(calculatingTheBitch); //I wannnnaaaaaaaa dieeeeeee -_-
						double angle = 0; 
						if ((currArc.start.x - currArc.center.x) < 0) {
							angle = atan2(abs(currArc.start.y - currArc.center.y), abs(currArc.start.x - currArc.center.x)) + atan2(0, -1);
						}
						else {
							angle = atan2(abs(currArc.start.y - currArc.center.y), abs(currArc.start.x - currArc.center.x));
						}
						arc->setStartAngle(angle * (180.0 / atan2(0, -1)) * 16.0);

						emit drawGraphic(arc);

					}
					//else error
				}
			}
		}
		else {
			std::cout << "Error: invalid filename" << std::endl;
			msg = QString::fromStdString("Error: bad file provided");
			emit error(msg);
		}
}

