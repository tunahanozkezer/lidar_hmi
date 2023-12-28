#include "lidarprocessor.h"

LidarProcessor::LidarProcessor()
{
    // İşlem görmüş veri başlatılabilir
}

void LidarProcessor::processLidarData(const QVector<LidarPoint>& data)
{
    // Lidar verilerini işleyin, örneğin bir filtreleme veya düzenleme yapın
    // Bu örnekte sadece veriyi kopyalayacağız
    processedData = data;
}

QVector<LidarPoint> LidarProcessor::getProcessedData() const
{
    return processedData;
}
