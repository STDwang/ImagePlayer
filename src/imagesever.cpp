#include "imagesever.h"
#include <QFile>
#include <QDir>
#include <./src/rawdatatype.h>
imageSever::imageSever(QObject* parent) : QObject(parent)
{
	rawThread = new QThread;
    connect(rawThread, SIGNAL(finished()), rawThread, SLOT(deleteLater()));

	rawImage = new RawImage();
    rawImage->moveToThread(rawThread);
    connect(this, SIGNAL(sendSetViewSizeBySon(int, int)), rawImage, SLOT(setViewSizeBySon(int, int)));

	connect(this, SIGNAL(sendGetImageSizeBySon(QString)), rawImage, SLOT(getImageSizeBySon(QString)));
	connect(rawImage, SIGNAL(sendFileSize(qint64)), this, SLOT(getImageSizeFromSon(qint64)));

    connect(this, SIGNAL(sendOpenImageBySon(int, int, QString, qreal, qreal, QString, bool)), rawImage, SLOT(openImageBySon(int, int, QString, qreal, qreal, QString, bool)));
	connect(rawImage, SIGNAL(sendImage(int, int, bool, QImage, QVariant)), this, SLOT(openImageFromSon(int, int, bool, QImage, QVariant)));

	connect(this, SIGNAL(sendGetPixValueBySon(int, int, int, int)), rawImage, SLOT(getPixValueBySon(int, int, int, int)));
	connect(rawImage, SIGNAL(sendPixValue(bool, int, int, int)), this, SLOT(getPixValueFromSon(bool, int, int, int)));

	connect(this, SIGNAL(sendSaveImageBySon(int, int, QString, QString)), rawImage, SLOT(saveImageBySon(int, int, QString, QString)));
	connect(rawImage, SIGNAL(sendSaveTip(bool)), this, SLOT(saveImageFromSon(bool)));

    connect(this, SIGNAL(sendAutoImageBySon(int, int)), rawImage, SLOT(autoImageBySon(int, int)));
    connect(rawImage, SIGNAL(sendAutoTip(bool)), this, SLOT(autoImageFromSon(bool)));

    connect(this, SIGNAL(sendReverseImageBySon(int, int)), rawImage, SLOT(reverseImageBySon(int, int)));
    connect(rawImage, SIGNAL(sendReverseTip(bool)), this, SLOT(reverseImageFromSon(bool)));

    connect(rawImage, SIGNAL(sendMinMax(int, int, int, int, int, int)), this, SLOT(minMaxFromSon(int, int, int, int, int, int)));

	m_pImgProvider = new ImageProvider();
    m_pImgProvider->setImageViewSize(1, 1);
    m_pImgAerialProvider = new ImageProvider();
    m_pImgAerialProvider->setImageViewSize(1, 1);

    emit sendSetViewSizeBySon(1, 1);

    rawThread->start();
}

imageSever::~imageSever() {
    rawThread->quit();
    rawThread = nullptr;

    delete rawImage;
    rawImage = nullptr;
    delete m_pImgProvider;
    m_pImgProvider = nullptr;
    delete m_pImgAerialProvider;
    m_pImgAerialProvider = nullptr;
}

void imageSever::setViewSize(int row, int col) {
	m_pImgProvider->setImageViewSize(row, col);
	emit sendSetViewSizeBySon(row, col);
}

void imageSever::setImageScaled(double scale) {
    m_pImgProvider->setImageScaled(scale);
}

void imageSever::setImageAerialScaled(double scale) {
    m_pImgAerialProvider->setImageScaled(scale);
}

void imageSever::getImageSize(QString path) {
	emit sendGetImageSizeBySon(path);
}

void imageSever::getPixValue(int row, int col, int x, int y) {
    emit sendGetPixValueBySon(row, col, x, y);
}

void imageSever::openImage(int row, int col, QString path, qreal w, qreal h, QString d, bool reverse) {
    emit sendOpenImageBySon(row, col, path, w, h, d, reverse);
}

void imageSever::saveImage(int row, int col, QString path, QString type) {
	emit sendSaveImageBySon(row, col, path, type);
}

void imageSever::autoImage(int row, int col){
    emit sendAutoImageBySon(row, col);
}

void imageSever::reverseImage(int row, int col){
    emit sendReverseImageBySon(row, col);
}

void imageSever::getImageSizeFromSon(qint64 size) {
    emit sendImageSizeToUI(size);
}

void imageSever::getPixValueFromSon(bool tip, int x, int y, int v) {
    emit sendPixValueToUI(tip, x, y, v);
}

void imageSever::openImageFromSon(int row, int col, bool tip, QImage img, QVariant hist) {
	m_pImgProvider->imgs[row][col] = img;
	emit sendImageToUI(row, col, tip, img.width(), img.height(), hist);
}

void imageSever::saveImageFromSon(bool tip) {
	emit sendSaveTipToUI(tip);
}

void imageSever::autoImageFromSon(bool tip){
    if(tip) {
        emit sendRefreshToUI();
    }
}

void imageSever::reverseImageFromSon(bool tip){
    if(tip) {
        emit sendRefreshToUI();
    }
}

void imageSever::minMaxFromSon(int row, int col, int min, int max, int autoMin, int autoMax) {
    emit sendMinMaxToUI(row, col, min, max, autoMin, autoMax);
}
