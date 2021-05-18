#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1200,850);
    initStyle();
    setLofar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initStyle()
{
    //加载样式表
    QString qss;
//    QFile file(":/qss/psblack.css");
    QFile file(":/qss/flatwhite.css");
//    QFile file(":/qss/lightblue.css");
    if (file.open(QFile::ReadOnly)) {
#if 1
        //用QTextStream读取样式文件不用区分文件编码 带bom也行
        QStringList list;
        QTextStream in(&file);
        //in.setCodec("utf-8");
        while (!in.atEnd()) {
            QString line;
            in >> line;
            list << line;
        }

        qss = list.join("\n");
#else
        //用readAll读取默认支持的是ANSI格式,如果不小心用creator打开编辑过了很可能打不开
        qss = QLatin1String(file.readAll());
#endif
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
}

Layer *MainWindow::addLayer(QCustomPlot *p)
{
    Layer *newLayer = new Layer(p);
    connect(this,SIGNAL(sendlayer(int)), newLayer, SLOT(getvalue(int)));
    bool visible = true;
    newLayer->setVisible_(visible);
    return newLayer;
}


void MainWindow:: setLofar()//显示Lofar瀑布图
{
    fftHeatmap = ui->fftHeatMapCustomPlot; //创建一个画图指针

    fftHeatmap->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);//可拖拽+可滚轮缩放

    fftHeatmap->xAxis->setLabel("Range");//x轴显示文字
    fftHeatmap->yAxis->setLabel("Speed");//y轴显示文字
    fftHeatmap->xAxis->setRange(0,32);//当前X轴显示范围
    fftHeatmap->yAxis->setRange(0,256);//当前y轴显示范围
    connect(ui->fftHeatMapCustomPlot, SIGNAL(mousePress(QMouseEvent*)),this, SLOT(on_mousePressed(QMouseEvent*)));
    connect(ui->fftHeatMapCustomPlot, SIGNAL(mouseMove(QMouseEvent*)),this, SLOT(on_mouseMoved(QMouseEvent*)));
    newLayer = addLayer(fftHeatmap);



    //通过传递的轴的QCustomPlot进行注册,简洁理解QCPColorMap的数据为（x轴，y轴;颜色，值value）
    m_pColorMap = new QCPColorMap(fftHeatmap->xAxis, fftHeatmap->yAxis);
    m_pColorMap->data()->setSize(32, 256);//设置整个图（x,y）点数
    m_pColorMap->data()->setRange(QCPRange(0, 32),QCPRange(0,256));//setRange是设置X轴以及Y轴的范围

    for(int x = 0; x < 32; x++) {
        for(int y = 0; y < 256; y++) {
            fftHeatmapValue[x][y] = x + y;
            m_pColorMap->data()->setCell(x, y, fftHeatmapValue[x][y]);
        }
    }
    m_pColorMap->setGradient(QCPColorGradient::gpSpectrum);//设置默认渐进色变化（可在QCPColorGradient中查看）
    m_pColorMap->rescaleDataRange(true);



// 立即刷新图像
    fftHeatmap->rescaleAxes();//自适应大小
    ui->fftHeatMapCustomPlot->replot();

}

void MainWindow::on_mousePressed(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton) {
        QPointF ChickedPoint = event->pos();
        //排除区间外鼠标点
        if(!fftHeatmap->viewport().contains(event->pos())){return;}
        //将像素坐标转换为轴值
        int16_t currentx = fftHeatmap->xAxis->pixelToCoord(ChickedPoint.x());
        int16_t currenty = fftHeatmap->yAxis->pixelToCoord(ChickedPoint.y());
        for(int i = 0; i < mouseTracerList.size(); i++) {
            qDebug() << "x: "<<mouseTracerList.at(i)->position->coords().x() << ";y: " << mouseTracerList.at(i)->position->coords().y();
            if(mouseTracerList.at(i)->position->coords().x() == currentx && mouseTracerList.at(i)->position->coords().y() == currenty) {
                delete mouseTracerList.at(i);
                mouseTracerList.remove(i);
                goto replot;
            }
        }
        newTracer = new QCPItemTracer(fftHeatmap);
        newTracer->setStyle(QCPItemTracer::tsCircle);//锚点的类型设置为圆形
        newTracer->setPen(QPen(Qt::white, 4, Qt::DashLine));//画笔类型
        newTracer->setBrush(Qt::NoBrush);
        newTracer->setLayer("newlayer");
        newTracer->setVisible(true);
        newTracer->position->setCoords(currentx, currenty);
        mouseTracerList.append(newTracer);
        qDebug() << "currentx: "<<currentx << ";currenty: " << currenty;

replot:
        fftHeatmap->replot();
    }
    else if(event->button() == Qt::LeftButton) {
        QPointF ChickedPoint = event->pos();
        //排除区间外鼠标点
        if(!fftHeatmap->viewport().contains(event->pos())){return;}
        //将像素坐标转换为轴值
        int16_t currentx = fftHeatmap->xAxis->pixelToCoord(ChickedPoint.x());
        int16_t currenty = fftHeatmap->yAxis->pixelToCoord(ChickedPoint.y());
        for(int i = 0; i < mouseTracerList.size(); i++) {
//            qDebug() << "x: "<<mouseTracerList.at(i)->position->coords().x() << ";y: " << mouseTracerList.at(i)->position->coords().y();
            if(mouseTracerList.at(i)->position->coords().x() == currentx && mouseTracerList.at(i)->position->coords().y() == currenty) {
                QColor color = mouseTracerList.at(i)->pen().color();
                if(color == Qt::black)
                    mouseTracerList.at(i)->setPen(QPen(Qt::red, 4, Qt::DashLine));
                else
                    mouseTracerList.at(i)->setPen(QPen(Qt::white, 4, Qt::DashLine));
            }
        }
        fftHeatmap->replot();
    }
}

void MainWindow::on_mouseMoved(QMouseEvent *event)
{
    QPointF ChickedPoint = event->pos();
    //排除区间外鼠标点
    if(!fftHeatmap->viewport().contains(event->pos())){return;}
    //将像素坐标转换为轴值
    int16_t currentx = fftHeatmap->xAxis->pixelToCoord(ChickedPoint.x());
    int16_t currenty = fftHeatmap->yAxis->pixelToCoord(ChickedPoint.y());
    if(currentx >= 0 && currentx < 32 && currenty >= 0 && currenty < 256) {
        for(int i = 0; i < mouseTracerList.size(); i++) {
//            qDebug() << "x: "<<mouseTracerList.at(i)->position->coords().x() << ";y: " << mouseTracerList.at(i)->position->coords().y();
            if(mouseTracerList.at(i)->pen().color() != Qt::red) {
                if(mouseTracerList.at(i)->position->coords().x() == currentx && mouseTracerList.at(i)->position->coords().y() == currenty)
                    mouseTracerList.at(i)->setPen(QPen(Qt::black, 4, Qt::DashLine));
                else
                    mouseTracerList.at(i)->setPen(QPen(Qt::white, 4, Qt::DashLine));
            }
        }
        qDebug() << fftHeatmapValue[currentx][currenty];
    }

}

