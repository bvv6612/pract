#ifndef TRAFFICANALYSIS_H
#define TRAFFICANALYSIS_H

#include <QList>
#include <QWidget>
#include <QObject>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>

namespace Ui {
class TrafficAnalysis;
}

class TrafficAnalysis : public QWidget
{
    Q_OBJECT

public:
    explicit TrafficAnalysis(QWidget *parent = 0);
    ~TrafficAnalysis();
    void friend readPackets(QFile); //чтение пакетов с файла

private:
    Ui::TrafficAnalysis *ui;

private slots:
    void pushButton_open_pressed();
    void pushButton_find_pressed();
    void pushButton_clear_pressed();
};

struct PcapHeader
{
    qint32 time1;
    qint32 time2;
    qint32 capLen;
    qint32 dataLen;
};

struct PcapFHeader
{
    qint32 magic;
    qint16 version_major;
    qint16 version_minor;
    qint32 thiszone;
    qint32 sigfigs;
    qint32 snaplen;
    qint32 linktype;
};

class Packet
{
public:
    Packet(); //пустой конструктор
    Packet(PcapHeader pH); //конструктор с заданием заголовка
    Packet(qint32 t1, qint32 t2, qint32 cL, qint32 dL); //конструктор с напрямую заданными значениями заголовка
    Packet(const Packet &myPacket); //конструктор копирования
    ~Packet(); //деструктор
    void setPHeader(PcapHeader fH); //задание заголовка
    void setTime1(qint32 t1); //прямое задание значений заголовка
    void setTime2(qint32 t2);
    void setCapLen(qint32 cL);
    void setDataLen(qint32 dL);
    void setDataArr(int n); //выделение памяти под пакет(массив)
    PcapHeader getPHeader(); //получение заголовка
    PcapHeader getPHeader() const;
    qint32 getTime1(); //получение значений напрямую
    qint32 getTime2();
    qint32 getCapLen();
    qint32 getCapLen() const;
    qint32 getDataLen();
    unsigned char* getDataArr(); //получение указателя на пакет(массив)
    unsigned char* getDataArr() const;
    bool operator == (const Packet &myPacket);
    bool operator < (const Packet &myPacket);
private:
    PcapHeader pHeader; //заголовок
    unsigned char *data; //пакет
};

class PacketStream
{
public:
    QList <Packet> packets; //вектор пакетов
    PacketStream(); //пустой конструктор
    PacketStream(PcapFHeader fH);//конструктор с заданием заголовка
    PacketStream(qint32 mag, qint16 v_maj, qint16 v_min,
                     qint32 tZ, qint32 sF, qint32 sL, qint32 lT); //конструктор с напрямую заданными значениями заголовка
    ~PacketStream(); //деструктор
    void setFHeader(PcapFHeader fH); //задание заголовка
    void setMagic(qint32 mag);           //прямое задание значений заголовка
    void setVersion_major(qint16 v_maj);
    void setVersion_minor(qint16 v_min);
    void setThiszone(qint32 tZ);
    void setSigfigs(qint32 sF);
    void setSnaplen(qint32 sL);
    void setLinktype(qint32 lT);
    PcapFHeader getFHeader(); //получение заголовка
    qint32 getMagic();         //получение значений напрямую
    qint16 getVersion_major();
    qint16 getVersion_minor();
    qint32 getThiszone();
    qint32 getSigfigs();
    qint32 getSnaplen();
    qint32 getLinktype();
private:
    PcapFHeader fHeader; //заголовок файла
};

#endif // TRAFFICANALYSIS_H
