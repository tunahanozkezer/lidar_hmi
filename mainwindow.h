#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QRandomGenerator>
#include <lidarpoint.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_connectButton_clicked();

    void on_refreshButton_clicked();
    void refresh_combo_box();

    void updateLidarData();
    void on_lidar_start_btn_clicked();

private:
    void updateGraphicsView(const QVector<LidarPoint>& data);
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QTimer *lidarTimer;
};
#endif // MAINWINDOW_H
