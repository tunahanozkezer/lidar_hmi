#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <serial_comm.h>
#include <QSerialPortInfo>
#include <hmi_interface.hpp>
#include <QGraphicsEllipseItem>
#include <lidarprocessor.h>

struct data_to_send
{
    uint16_t angle;
    uint16_t distance;
};

     QVector<LidarPoint> lidarData;

class LidarData {
public:
    QVector<QPointF> dataPoints; // Lidar verilerini tutan vektör

    // Lidar verilerini eklemek için bir fonksiyon
    void addDataPoint(float angle, float distance) {
        dataPoints.append(QPointF(angle, distance));
    }
};

SerialPortManager serialManager;
std::vector<uint8_t> res;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // QGraphicsScene ve QGraphicsView oluşturun
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    // LiDAR simülasyonu için timer'ı başlatın
    lidarTimer = new QTimer(this);
    connect(lidarTimer, SIGNAL(timeout()), this, SLOT(updateLidarData()));
    lidarTimer->start(100); // Örnek: 50 ms'de bir güncelleme

    // QChartView'ı içerecek olan QFrame'i oluşturun

    refresh_combo_box();


}
LidarProcessor lidarProcessor;
// mainwindow.cpp
void MainWindow::updateLidarData()
{

    // Lidar verilerini işle
    lidarProcessor.processLidarData(lidarData);

    // İşlenmiş verileri al
    QVector<LidarPoint> processedData = lidarProcessor.getProcessedData();

    // Grafik üzerinde temsil et
    updateGraphicsView(processedData);
}

void MainWindow::updateGraphicsView(const QVector<LidarPoint>& data)
{
    // Grafik üzerindeki önceki elemanları temizle
    scene->clear();
    QGraphicsEllipseItem *pointItem = new QGraphicsEllipseItem(0, 0, 5, 5);
    pointItem->setBrush(Qt::red);
    scene->addItem(pointItem);


    // Sahnenin sabit kalmasını sağlayan render hint'ini ayarlayın
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
    ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform, true);

    // FollowScrollHint özelliğini devre dışı bırakın
    ui->graphicsView->setOptimizationFlag(QGraphicsView::DontSavePainterState, true);

    // ScrollHandDrag özelliğini devre dışı bırakın
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);

    // Sahnenin boyutunu belirleyin (örneğin, 800x600)
    scene->setSceneRect(-300, -300, 600, 600);

    // QGraphicsView'a sahneyi ayarlayın
    ui->graphicsView->setScene(scene);


    // Lidar verilerini grafik elemanlarına dönüştür ve ekle
    for (const LidarPoint& point : data)
    {
        qreal x = point.distance * qCos(qDegreesToRadians(point.angle));
        qreal y = point.distance * qSin(qDegreesToRadians(point.angle));

        QGraphicsEllipseItem *pointItem = new QGraphicsEllipseItem(x, y, 2, 2);

        if(point.angle > 355)
        {
            scene->clear();
            QGraphicsEllipseItem *pointItem = new QGraphicsEllipseItem(0, 0, 5, 5);
            pointItem->setBrush(Qt::red);
            scene->addItem(pointItem);

        }
        scene->addItem(pointItem);

    }
}

MainWindow::~MainWindow()
{
    delete ui;
    serialManager.closePort();

}


void QByteArrayToVector(const QByteArray& byteArray, std::vector<uint8_t> &result) {
    // QByteArray'deki veriyi std::vector<uint8_t>'e dönüştür
    result.assign(byteArray.begin(), byteArray.end());

}

void MainWindow::on_connectButton_clicked()
{

    if (serialManager.isOpen())
    {
        serialManager.closePort();
        ui->connectButton->setText("Bağlan");
    }
    else
    {
        serialManager.openPort(ui->COMCombo->currentText(), QSerialPort::Baud115200);
        qDebug() << "KINGSLAYER!!";


        ui->connectButton->setText("Bağlantı Kes");
        connect(&serialManager, &SerialPortManager::dataReceived, [&](const QByteArray &data) {

            const uint8_t *rawData = reinterpret_cast<const uint8_t*>(data.constData());

            // Determine the size of the data
            size_t dataSize = static_cast<size_t>(data.size());

            // Add the received data to the global vector
            res.insert(res.end(), rawData, rawData + dataSize);

            // You can also perform any additional processing or use the data as needed
            // For example, print the received data

            uart_protocol::packet paket;
            LidarPoint lidar_datas;
            data_to_send  veri;
            if(res.size() > 9)
            {
                if(true == uart_protocol::unpack_packet(res,paket))
                {
                    qDebug() << "Received data: " << data;

                    std::memcpy(&veri, paket.payload.data(), sizeof(data_to_send));

                    QString string = "Mesafe: " + QString::number(veri.distance);


                    ui->distance_lbl->setText(string);
                    string = "Açı: " + QString::number(veri.angle * 0.01);
                    ui->angle_lbl->setText(string);

                    lidar_datas.angle = veri.angle * 0.01;
                    lidar_datas.distance = veri.distance;
                    lidarData.append(lidar_datas);
                }
                else
                {
                     qDebug()  << "Chekcsum";
                }
                res.clear();
            }
        });
    }
}

void MainWindow::on_refreshButton_clicked()
{
    refresh_combo_box();
}

void MainWindow::refresh_combo_box()
{
    QList<QSerialPortInfo> list = serialManager.scan_ports();
    ui->COMCombo->clear();
    for(int i=0; i<list.length(); i++)
    {
        ui->COMCombo->addItem(list[i].portName());
    }
}


void MainWindow::on_lidar_start_btn_clicked()
{
    static bool donu;
    uint32_t packet_size;
    if(donu)
    {
         ui->lidar_start_btn->setText("Başlat");

    donu = false;
    }
    else
    {
        ui->lidar_start_btn->setText("Durdur");
        scene->clear();
        QGraphicsEllipseItem *pointItem = new QGraphicsEllipseItem(0, 0, 5, 5);
        pointItem->setBrush(Qt::red);
        scene->addItem(pointItem);

        donu = true;
    }


    std::vector<uint8_t> dataToSend{donu, 1};
    std::unique_ptr<uint8_t[]> packedData = uart_protocol::pack_packet(uart_protocol::control_packet, dataToSend, packet_size);
    QByteArray byteArray(reinterpret_cast<const char*>(packedData.get()), packet_size);

    // Veriyi seri porta gönderin
    serialManager.writeData(byteArray);

}

