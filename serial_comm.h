// serial_comm.h
#ifndef serial_comm_H
#define serial_comm_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>

class SerialPortManager : public QObject
{
    Q_OBJECT

public:
    QSerialPort *serialPort;
    explicit SerialPortManager(QObject *parent = nullptr);
    ~SerialPortManager();

    QList<QSerialPortInfo> scan_ports();

    bool openPort(const QString &portName, qint32 baudRate = QSerialPort::Baud9600);
    void closePort();
    bool isOpen() const;

public slots:
    void writeData(const QByteArray &data);

signals:
    void dataReceived(const QByteArray &data);

private slots:
    void handleReadyRead();

private:


};


#endif // serial_comm_H
