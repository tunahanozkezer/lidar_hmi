#ifndef LIDARPROCESSOR_H
#define LIDARPROCESSOR_H

#include <QVector>
#include "lidarpoint.h"

class LidarProcessor
{
public:
    LidarProcessor();

    // Lidar verilerini işleyen bir metod ekleyin
    void processLidarData(const QVector<LidarPoint>& data);

    // İşlenmiş verileri almak için bir metod ekleyin
    QVector<LidarPoint> getProcessedData() const;

private:
    QVector<LidarPoint> processedData;
};


#endif // LIDARPROCESSOR_H
