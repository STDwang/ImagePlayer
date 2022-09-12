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
	QImage nowImage;//Í¼Æ¬
	QString srcPath;//Â·¾¶
        QString imageType;//Â·¾¶
	QSizeF imageSize;//Í¼Æ¬³ß´ç

	qint64 pixelCount;//ÏñËØ¸öÊý
        std::vector<uchar> pData8;
	std::vector<std::vector<ushort>> imageData;
};

Q_DECLARE_METATYPE(RawImageData);
