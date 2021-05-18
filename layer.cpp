#include "layer.h"

struct layerprivate
{
    QPoint mousepoint;
};

Layer::Layer(QCustomPlot *plot) : QCPLayerable(plot)
{
    parentPlot = plot;
    mpainter = new QCPPainter;
    mpainter = nullptr;
    visible = true;
    mParentPlot->addLayer(LayerName());
    setLayer(LayerName());
    setVisible_(visible);
    connect(mParentPlot,&QCustomPlot::mouseMove, this, &Layer::MouseMoveEvent);
    connect(mParentPlot,&QCustomPlot::mousePress,this,&Layer::Mousepressevent);
}

Layer::~Layer()
{

}

//void Layer::Mousepressevent(QMouseEvent *event)
//{
//    qDebug()<<"layer:"<<event->pos();
//}

void Layer::draw(QCPPainter *painter)
{
#if 1
    Q_UNUSED(painter);
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    pen.setStyle(Qt::PenStyle::DashLine);
    mpainter = painter;
    mpainter->setPen(pen);

    QPoint p1,p2;
    p1.setX(100);
    p1.setY(100);
    p2.setX(200);
    p2.setY(100);

    QCPAxis *axis = mParentPlot->axisRect(0)->axis(QCPAxis::atBottom);

    p1.setX(axis->axisRect()->left());
    p1.setY(movep.y());
    p2.setX(axis->axisRect()->right());
    p2.setY(movep.y());
    mpainter->drawLine(p1,p2);
    p1.setX(movep.x());
    p1.setY(0);
    p2.setX(movep.x());
    p2.setY(axis->axisRect()->bottom());
    mpainter->drawLine(p1, p2);
    mpainter->drawEllipse(movep.x() - 2,movep.y() - 2, 4, 4);
    QPoint coordPoint;
    coordPoint.setX(movep.x());
    coordPoint.setY(movep.y() - 5);

    char coordText[20] = {0};
    int16_t currentx = parentPlot->xAxis->pixelToCoord(movep.x());
    int16_t currenty = parentPlot->yAxis->pixelToCoord(movep.y());
    sprintf(coordText, "(%d, %d)", currentx, currenty);

    mpainter->drawText(coordPoint,coordText);
    QFont font;
    font.setPointSize(30);
    font.setUnderline(true);
    mpainter->setFont(font);
    mpainter->setPen(QPen(Qt::black,2));
#else
    Q_UNUSED(painter);
        mpainter = painter;
        mpainter->setPen(QPen(Qt::green,1,Qt::DashDotDotLine));
        QPoint p1,p2;
        p1.setX(100);
        p1.setY(100);
        p2.setX(200);
        p2.setY(100);
    //    mpainter->drawLine(p1,p2);
    //    mpainter->drawEllipse(200,100,100,100);
        QCPAxis *axis = mParentPlot->axisRect(0)->axis(QCPAxis::atBottom);
        p1.setX(axis->axisRect()->left());
        p1.setY(movep.y());
        p2.setX(axis->axisRect()->right());
        p2.setY(movep.y());
        mpainter->drawLine(p1,p2);
        p1.setX(movep.x());
        p1.setY(0);
        p2.setX(movep.x());
        p2.setY(axis->axisRect()->bottom());
        mpainter->drawLine(p1,p2);
#endif

}

void Layer::Mousepressevent(QMouseEvent *event)
{

}

//void Layer::mousePressEvent(QMouseEvent *event, const QVariant &details)
//{
//    qDebug()<<"haha:"<<event->pos();
//}

void Layer::MouseMoveEvent(QMouseEvent *event)
{
    movep = event->pos();
    parentPlot->replot();
//    qDebug()<<"haha:"<<event->pos();
}

void Layer::getvalue(int x)
{
    qDebug()<<"fromcustom:"<<x;
}

QString Layer::LayerName() const
{
    return QStringLiteral("newlayer");
}

void Layer::setVisible_(bool visible)
{
    layer = mParentPlot->layer(LayerName());
    if(layer)
    {
        layer->setVisible(visible);
    }

}
