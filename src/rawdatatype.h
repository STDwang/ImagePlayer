//***************************************************************************
// Author:        stdWang
// Date:		  2022/09/12
//***************************************************************************
#pragma once
#include <QObject>
#include <QImage>
#include <QMetaType>
#include <QVector>
#include <QString>

struct RawImageData
{
	QImage nowImage;//图片
	QString srcPath;//路径
    QString imageType;//路径
	QSizeF imageSize;//图片尺寸

	qint64 pixelCount;//像素个数
    std::vector<uchar> pData8;
	std::vector<std::vector<ushort>> imageData;
};

Q_DECLARE_METATYPE(RawImageData);
