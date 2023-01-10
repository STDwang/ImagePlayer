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
    QString imageType;//图片格式
	QSizeF imageSize;//图片尺寸
    bool isReverse = false; //是否反转

    double tempMin;
    double tempMax;
    double imax;//图片最大灰度值
    double imin;//图片最小灰度值
	qint64 pixelCount;//像素个数
    std::vector<ushort> pData16;
    std::vector<uchar> pData8;
	std::vector<std::vector<ushort>> imageData;

    //auto
    int autoThreshold = 5000;
    int hmin = 0, hmax = 0;
    std::vector<int> histogram;
};

Q_DECLARE_METATYPE(RawImageData);
