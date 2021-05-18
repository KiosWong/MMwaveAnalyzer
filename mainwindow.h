#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCodec>
#include <QVector>
#include "qcustomplot.h"
#include "layer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initStyle();
    Layer *addLayer(QCustomPlot *p);
    void setLofar();

private:
    Ui::MainWindow *ui;
    QCPColorMap *m_pColorMap;
    QCustomPlot *fftHeatmap;
    Layer *newLayer;
    double fftHeatmapValue[32][256];
    QCPItemTracer* newTracer;
    QVector <QCPItemTracer *> mouseTracerList;

private slots:
    void on_mousePressed(QMouseEvent *event);
    void on_mouseMoved(QMouseEvent *event);
};
#endif // MAINWINDOW_H
