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
	QImage nowImage;//ͼƬ
	QString srcPath;//·��
        QString imageType;//·��
	QSizeF imageSize;//ͼƬ�ߴ�

	qint64 pixelCount;//���ظ���
        std::vector<uchar> pData8;
	std::vector<std::vector<ushort>> imageData;
};

Q_DECLARE_METATYPE(RawImageData);
