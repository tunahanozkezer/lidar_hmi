// serial_comm.cpp
#include "serial_comm.h"
#include "qdebug.h"
#include <QSerialPortInfo>
#include <QtLogging>

SerialPortManager::SerialPortManager(QObject *parent)
    : QObject(parent), serialPort(new QSerialPort(this))
{
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
}

SerialPortManager::~SerialPortManager()
{
    closePort();
}

QList<QSerialPortInfo> SerialPortManager::scan_ports()
{
    // COM PORT lar taranıyor ve combobox'a yazılıyor.
    return QSerialPortInfo::availablePorts();
}


bool SerialPortManager::openPort(const QString &portName, qint32 baudRate)
{
    if (serialPort->isOpen())
        closePort();

    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);

    if (serialPort->open(QIODevice::ReadWrite)) {
        // qDebug() << "Serial port opened:" << portName;
        return true;
    } else {
        // qDebug() << "Failed to open serial port:" << portName;
        return false;
    }
}

void SerialPortManager::closePort()
{
    if (serialPort->isOpen()) {
        serialPort->close();
        // qDebug() << "Serial port closed";
    }
}

bool SerialPortManager::isOpen() const
{
    return serialPort->isOpen();
}

void SerialPortManager::writeData(const QByteArray &data)
{
    if (serialPort->isOpen())
        serialPort->write(data);
}

void SerialPortManager::handleReadyRead()
{
    QByteArray data = serialPort->readAll();
     emit dataReceived(data);
}
