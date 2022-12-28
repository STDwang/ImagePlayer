//***************************************************************************
// Author:        stdWang
// Date:		  2022/09/12
//***************************************************************************
#ifndef IMAGESEVER_H
#define IMAGESEVER_H
#include <qthread.h>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QVariant>
#include <QMetaObject>
#include <QCoreApplication>
#include <QVariantMap>
#include <QImage>
#include "imageprovider.h"
#include "rawimage.h"
class imageSever : public QObject
{
	Q_OBJECT
public:
	imageSever(QObject* parent = 0);
    ImageProvider* m_pImgProvider;
    ImageProvider* m_pImgAerialProvider;
signals:
	void sendSetViewSizeBySon(int row, int col);
	void sendGetImageSizeBySon(QString path);
    void sendOpenImageBySon(int row, int col, QString path, qreal w, qreal h, QString d, bool reverse);
	void sendGetPixValueBySon(int row, int col, int x, int y);
	void sendSaveImageBySon(int row, int col, QString path, QString type);

	void sendImageSizeToUI(qint64 size);
	void sendImageToUI(int row, int col, bool tip, qreal w, qreal h);
	void sendPixValueToUI(bool tip, int x, int y, int v);
	void sendSaveTipToUI(bool tip);
public slots:
	void setViewSize(int row, int col);
	void getImageSize(QString path);
    void openImage(int row, int col, QString path, qreal w, qreal h, QString d, bool reverse);
	void saveImage(int row, int col, QString path, QString type);
	void getPixValue(int row, int col, int x, int y);
    //设置图像缩放比
    void setImageScaled(double scale);
    //设置鸟瞰图图像缩放比
    void setImageAerialScaled(double scale);
private slots:
	void getImageSizeFromSon(qint64 size);
	void openImageFromSon(int row, int col, bool tip, QImage img);
	void getPixValueFromSon(bool tip, int x, int y, int v);
	void saveImageFromSon(bool tip);
private:
	QThread* rawThread;
	RawImage* rawImage;
};

#endif // IMAGESEVER_H
