#include "trafficanalysis.h"
#include "ui_trafficanalysis.h"

PacketStream pStream;

TrafficAnalysis::TrafficAnalysis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrafficAnalysis)
{
    ui->setupUi(this);
    connect(ui->pushButton_open,SIGNAL(released()),this,SLOT(pushButton_open_pressed()));
    connect(ui->pushButton_find,SIGNAL(released()),this,SLOT(pushButton_find_pressed()));
    connect(ui->pushButton_clear,SIGNAL(released()),this,SLOT(pushButton_clear_pressed()));
}
TrafficAnalysis::~TrafficAnalysis()
{
    delete ui;
}

//------класс Packet------
Packet::Packet()
{
    pHeader.time1 = 0;
    pHeader.time2 = 0;
    pHeader.capLen = 0;
    pHeader.dataLen = 0;
}
Packet::Packet(PcapHeader pH)
{
    pHeader = pH;
}
Packet::Packet(qint32 t1, qint32 t2, qint32 cL, qint32 dL)
{
    pHeader.time1 = t1;
    pHeader.time2 = t2;
    pHeader.capLen = cL;
    pHeader.dataLen = dL;
}
Packet::Packet(const Packet &myPacket)
{
    pHeader = myPacket.getPHeader();
    data = new unsigned char [pHeader.capLen];
    memcpy(data, myPacket.getDataArr(), pHeader.capLen);
}
Packet::~Packet() {delete[] data;}
void Packet::setPHeader(PcapHeader pH) {pHeader = pH;}
void Packet::setTime1(qint32 t1) {pHeader.time1 = t1;}
void Packet::setTime2(qint32 t2) {pHeader.time2 = t2;}
void Packet::setCapLen(qint32 cL) {pHeader.capLen = cL;}
void Packet::setDataLen(qint32 dL) {pHeader.dataLen = dL;}
void Packet::setDataArr(qint32 n) {data = new unsigned char[n];}
PcapHeader Packet::getPHeader() {return pHeader;}
PcapHeader Packet::getPHeader() const {return pHeader;}
qint32 Packet::getTime1() {return pHeader.time1;}
qint32 Packet::getTime2() {return pHeader.time2;}
qint32 Packet::getCapLen() {return pHeader.capLen;}
qint32 Packet::getCapLen() const {return pHeader.capLen;}
qint32 Packet::getDataLen() {return pHeader.dataLen;}
unsigned char* Packet::getDataArr() {return data;}
unsigned char* Packet::getDataArr() const {return data;}
bool Packet::operator == (const Packet &myPacket)
{
    if(pHeader.capLen == myPacket.getCapLen()) return true;
    else return false;
}
bool Packet::operator < (const Packet &myPacket)
{
    if(pHeader.capLen < myPacket.getCapLen()) return true;
    else return false;
}
//-------класс Packet------

//-------класс PacketStream------
PacketStream::PacketStream()
{
    fHeader.magic = 0;
    fHeader.version_major = 0;
    fHeader.version_minor = 0;
    fHeader.thiszone = 0;
    fHeader.sigfigs = 0;
    fHeader.snaplen = 0;
    fHeader.linktype = 0;
}
PacketStream::PacketStream(PcapFHeader fH)
{
    fHeader = fH;
}
PacketStream::PacketStream(qint32 mag, qint16 v_maj, qint16 v_min, qint32 tZ, qint32 sF, qint32 sL, qint32 lT)
{
    fHeader.magic= mag;
    fHeader.version_major = v_maj;
    fHeader.version_minor = v_min;
    fHeader.thiszone = tZ;
    fHeader.sigfigs = sF;
    fHeader.snaplen = sL;
    fHeader.linktype = lT;
}
PacketStream::~PacketStream()
{
    packets.clear();
}
void PacketStream::setFHeader(PcapFHeader fH) {fHeader = fH;}
void PacketStream::setMagic(qint32 mag) {fHeader.magic = mag;}
void PacketStream::setVersion_major(qint16 v_maj) {fHeader.version_major = v_maj;}
void PacketStream::setVersion_minor(qint16 v_min) {fHeader.version_minor = v_min;}
void PacketStream::setThiszone(qint32 tZ) {fHeader.thiszone = tZ;}
void PacketStream::setSigfigs(qint32 sF) {fHeader.sigfigs = sF;}
void PacketStream::setSnaplen(qint32 sL) {fHeader.snaplen = sL;}
void PacketStream::setLinktype(qint32 lT) {fHeader.linktype = lT;}
PcapFHeader PacketStream::getFHeader() {return fHeader;}
qint32 PacketStream::getMagic() {return fHeader.magic;}
qint16 PacketStream::getVersion_major() {return fHeader.version_major;}
qint16 PacketStream::getVersion_minor() {return fHeader.version_minor;}
qint32 PacketStream::getThiszone() {return fHeader.thiszone;}
qint32 PacketStream::getSigfigs() {return fHeader.sigfigs;}
qint32 PacketStream::getSnaplen() {return fHeader.snaplen;}
qint32 PacketStream::getLinktype() {return fHeader.linktype;}
//-------класс PacketStream------


void readPackets(QFile &file)
{
    PcapFHeader tempFHeader;
    PcapHeader tempPHeader;
    unsigned char *tempDataArr;
    file.read((char *)&tempFHeader,24);
    pStream.setFHeader(tempFHeader);
    while (file.pos() < file.size())
    {
        file.read((char *)&tempPHeader,16);
        Packet myPacket(tempPHeader);

        myPacket.setDataArr(myPacket.getCapLen());
        tempDataArr = myPacket.getDataArr();
        file.read((char *)tempDataArr, myPacket.getCapLen());
        pStream.packets.push_back(myPacket);
    }
}

void TrafficAnalysis::pushButton_find_pressed()
{
    QString data = "";
    QString jStr = ui->LineEdit_search->text();
    int j = jStr.toInt() - 1;
    if(pStream.packets.length() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Open .pcap file!"));
        return;
    }
    if(j < 0 || j > pStream.packets.length())
    {
        QMessageBox::critical(this, tr("Error"), tr("Incorrect packet's number"));
        return;
    }
    ui->textEdit_packetView->append("################# Packet № " + jStr + " #################");
    ui->textEdit_packetView->append("----------------------------------pcap header----------------------------------");
    data = QDateTime::fromTime_t(pStream.packets[j].getTime1()).toString();
    data += " : " + QString::number(pStream.packets[j].getTime2());
    ui->textEdit_packetView->append("time:    " + data);
    data = QString::number(pStream.packets[j].getCapLen());
    ui->textEdit_packetView->append("caplen:   " + data);
    data = QString::number(pStream.packets[j].getDataLen());
    ui->textEdit_packetView->append("datalen:  " + data);
    ui->textEdit_packetView->append("-------------------------------------Packet-------------------------------------");
    int modulo = pStream.packets[j].getCapLen() % 16;
    for(int i = 0; i < pStream.packets[j].getCapLen() - modulo; i+=16)
    {
        data = "";
        for(int k = 0; k < 8; k++)
        {
            if(QString::number(pStream.packets[j].getDataArr()[i+k], 16).length() == 2) data += QString::number(pStream.packets[j].getDataArr()[i+k], 16) + " ";
            else data += "0" + QString::number(pStream.packets[j].getDataArr()[i+k], 16) + " ";
        }
        data += "\t";
        for(int k = 9; k < 18; k++)
        {
            if(QString::number(pStream.packets[j].getDataArr()[i+k], 16).length() == 2) data += QString::number(pStream.packets[j].getDataArr()[i+k], 16) + " ";
            else data += "0" + QString::number(pStream.packets[j].getDataArr()[i+k], 16) + " ";
        }
        ui->textEdit_packetView->append(data);
    }
    for(int i = modulo; i < pStream.packets[j].getCapLen(); i++)
    {
        data = "";
        if(QString::number(pStream.packets[j].getDataArr()[i], 16).length() == 2) data += QString::number(pStream.packets[j].getDataArr()[i], 16) + " ";
        else data += "0" + QString::number(pStream.packets[j].getDataArr()[i], 16) + " ";
    }
    ui->textEdit_packetView->append("");

}

void TrafficAnalysis::pushButton_clear_pressed()
{
    QString data = "";
    ui->textEdit_packetView->setText(data);
    ui->LineEdit_search->setText(data);
}

void TrafficAnalysis::pushButton_open_pressed()
{
    pStream.packets.clear();
    QString fName = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::currentPath(), tr("PCAP files (*.cap)"));
    if(fName == "")
    {
        QMessageBox::critical(this, tr("Error"), tr("Could not read file's name"));
        return;
    }
    QFile file(fName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
        return;
    }
    readPackets(file);
    file.close();
    QString data = "";
    ui->lineEdit_number->setText(data);
    ui->lineEdit_maxLength->setText(data);
    ui->lineEdit_minLength->setText(data);
    ui->lineEdit_averageLength->setText(data);
    data = QString::number(pStream.packets.length());
    ui->lineEdit_number->setText(data);

    qint32 maxTemp, minTemp, averTemp;
    maxTemp = minTemp = averTemp = pStream.packets[0].getCapLen();
    for(int i = 1; i < pStream.packets.length(); i++)
    {
        if(pStream.packets[i].getCapLen() > maxTemp) maxTemp = pStream.packets[i].getCapLen();
        if(pStream.packets[i].getCapLen() < minTemp) minTemp = pStream.packets[i].getCapLen();
        averTemp += pStream.packets[i].getCapLen();
    }
    averTemp /= pStream.packets.length();
    data = QString::number(maxTemp);
    ui->lineEdit_maxLength->setText(data);
    data = QString::number(minTemp);
    ui->lineEdit_minLength->setText(data);
    data = QString::number(averTemp);
    ui->lineEdit_averageLength->setText(data);
}
