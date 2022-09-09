#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <string>

#include <QWidget>

#include "qt_interpreter.hpp"

//my includes 
#include "message_widget.hpp"
#include "canvas_widget.hpp"
#include "repl_widget.hpp"
#include "interpreter_semantic_error.hpp"

class MainWindow: public QWidget{
  Q_OBJECT

public:

  MainWindow(QWidget * parent = nullptr);
  MainWindow(std::string filename, QWidget * parent = nullptr);

private:

  QtInterpreter interp;

  //my additons 
  REPLWidget * repl;
  MessageWidget * msg;
  CanvasWidget * canvas;
};


#endif
