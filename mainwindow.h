#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QRandomGenerator>
#include <lidarpoint.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class LidarHMI;
}
QT_END_NAMESPACE

class LidarHMI : public QMainWindow
{
    Q_OBJECT

public:
    LidarHMI(QWidget *parent = nullptr);
    ~LidarHMI();
private slots:
    void on_connectButton_clicked();

    void on_refreshButton_clicked();
    void refresh_combo_box();

    void updateLidarData();
    void on_lidar_start_btn_clicked();

    void on_grafik_temizle_btn_clicked();

    void on_speed_btn_clicked();

private:
    void updateGraphicsView(const QVector<LidarPoint>& data);
    Ui::LidarHMI *ui;
    QGraphicsScene *scene;
    QTimer *lidarTimer;
};
#endif // MAINWINDOW_H
