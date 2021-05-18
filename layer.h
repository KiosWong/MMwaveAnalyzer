#ifndef LAYER_H
#define LAYER_H
#include "qcustomplot.h"
#include <QMouseEvent>
#include <QDebug>
struct layerprivate;

class Layer : public QCPLayerable
{
    Q_OBJECT
public:
    Layer(QCustomPlot *plot);
    ~Layer();
    QCPPainter *mpainter;
    QCPLayer *layer;
    bool visible;
    QPoint movep;
    QString LayerName() const;
    void setVisible_(bool visible);  //设置层是否绘制

    virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const{};
    virtual void draw(QCPPainter *painter);
//    void mousePressEvent(QMouseEvent *event, const QVariant &details);
public slots:
    void Mousepressevent(QMouseEvent *event);
    void MouseMoveEvent(QMouseEvent *event);
    void getvalue(int x);
private:
    QCustomPlot *parentPlot;
    QCPItemTracer *mouseTracer;
};

#endif // LAYER_H
