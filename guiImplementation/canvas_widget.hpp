#ifndef CANVAS_WIDGET_HPP
#define CANVAS_WIDGET_HPP

#include <QWidget>

class QGraphicsItem;
class QGraphicsScene;

class CanvasWidget: public QWidget{
  Q_OBJECT

public:

  CanvasWidget(QWidget * parent = nullptr);

public slots:

  void addGraphic(QGraphicsItem * item);

private:

  QGraphicsScene* scene;
};

#endif
