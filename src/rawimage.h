//***************************************************************************
// Author:        stdWang
// Date:		  2022/09/12
//***************************************************************************
#ifndef RAWIMAGE_H
#define RAWIMAGE_H

#include <QImage>
#include <qglobal.h>
#include "./src/imageprovider.h"
#include "./src/rawdatatype.h"
class RawImage : public QObject
{
	Q_OBJECT
public:
	explicit RawImage(QObject* parent = 0);
    void openRawImage(int row, int col, bool reverse);
	void openOtherTypeImage(int row, int col);
signals:
	void sendImage(int row, int col, bool tip, QImage img);
	void sendFileSize(qint64 size);
	void sendPixValue(bool tip, int x, int y, int v);
	void sendSaveTip(bool tip);
public slots:
	void setViewSizeBySon(int row, int col);
	void getImageSizeBySon(QString path);
    void openImageBySon(int row, int col, QString path, qreal w, qreal h, QString d, bool reverse);
	void getPixValueBySon(int row, int col, int x, int y);
	void saveImageBySon(int row, int col, QString path, QString type);
public:
	std::vector<std::vector<RawImageData>> rawDataList;
	QString srcPathTemp;
};
#endif // RAWIMAGE_H
