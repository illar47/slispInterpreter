#ifndef REPL_WIDGET_HPP
#define REPL_WIDGET_HPP

#include <QWidget>
#include <QLineEdit>
#include <QString>
#include <QVector>

class QLineEdit; //idk if needed???

class REPLWidget: public QWidget{
Q_OBJECT

public:

  REPLWidget(QWidget * parent = nullptr);

signals:

  void lineEntered(QString entry);

private slots:

  void changed();

private: 
	QLineEdit * repl; 
};

#endif
