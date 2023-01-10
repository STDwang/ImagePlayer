#include "rawimage.h"
#include <QFile>
#include <QTextCodec>
RawImage::RawImage(QObject* parent) : QObject(parent) {}

template <typename T>
T ReverseBytes(T oldValue)
{
	T newValue;
	int size = sizeof(T);
	memset(&newValue, 0, size);
	for (int i = 0; i < size; i++) {
		//挨个取字节
		char value = (oldValue >> (i * 8)) & 0xFF;
		memset((char*)&newValue + size - i - 1, value, 1);
	}
	return newValue;
}

std::vector<int> getHistogram(std::vector<uchar>& pData8) {
    std::vector<int> line(256, 0);
    for (int i = 0; i < pData8.size(); i++) {
        line[pData8[i]]++;
    }
    return line;
}

std::vector<int> getHistogram16(double min, double max, std::vector<ushort>& pData16) {
    std::vector<int> line(max - min + 1, 0);
    for (int i = 0; i < pData16.size(); i++) {
        line[pData16[i] - min]++;
    }
    return line;
}

void RawImage::getImageSizeBySon(QString path) {
    srcPathTemp = path.remove("file:///");
    // 读取Raw文件的数据
    QFile file(srcPathTemp);
    emit sendFileSize(file.size());
}

void RawImage::getPixValueBySon(int row, int col, int x, int y) {
    if (x <= 0) x = 0;
    if (y <= 0) y = 0;
    if (x >= rawDataList[row][col].nowImage.width() - 1) x = rawDataList[row][col].nowImage.width() - 1;
    if (y >= rawDataList[row][col].nowImage.height() - 1) y = rawDataList[row][col].nowImage.height() - 1;
    if (!rawDataList[row][col].imageType.compare("raw", Qt::CaseInsensitive)) {
        if (rawDataList[row][col].imageData.size() < 1) {
            emit sendPixValue(false, x, y, 0);
            return;
        }
        emit sendPixValue(true, x, y, rawDataList[row][col].imageData[y][x]);
    }
    else if (!rawDataList[row][col].imageType.compare("png", Qt::CaseInsensitive)
        || !rawDataList[row][col].imageType.compare("jpg", Qt::CaseInsensitive)
        || !rawDataList[row][col].imageType.compare("bmp", Qt::CaseInsensitive)) {
        emit sendPixValue(true, x, y, rawDataList[row][col].nowImage.pixelColor(x, y).red());
    }
}

void RawImage::setViewSizeBySon(int row, int col) {
	rawDataList.clear();
	rawDataList.resize(row);
	for (int i = 0; i < row; i++) {
		rawDataList[i].resize(col);
	}
}

void RawImage::setGrayscale(int row, int col, double newMin, double newMax) {
    rawDataList[row][col].tempMin = newMin;
    rawDataList[row][col].tempMax = newMax;
    double unit = (newMax - newMin) / 256.0;
    double temp = 0;
    for (int i = 0; i < rawDataList[row][col].pixelCount; i++)
    {
        temp = (rawDataList[row][col].pData16[i] - newMin) / unit;
        if (temp < 0)temp = 0;
        if (temp > 255)temp = 255;
        rawDataList[row][col].pData8[i] = uchar(temp);
    }
    rawDataList[row][col].nowImage = QImage(&rawDataList[row][col].pData8[0],
        int(rawDataList[row][col].nowImage.width()),
        int(rawDataList[row][col].nowImage.height()),
        QImage::Format_Indexed8);
}

void RawImage::openRawImage(int row, int col, bool reverse) {
	rawDataList[row][col].imageData.clear();
    rawDataList[row][col].pData8.clear();
    rawDataList[row][col].pData16.resize(rawDataList[row][col].pixelCount);

	// 读取Raw文件的数据
    QTextCodec* tc = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    QByteArray ba1 = rawDataList[row][col].srcPath.toLocal8Bit();
    FILE* rfid = fopen(ba1.data(), "rb");

	if (rfid == NULL) return;
    fread(&rawDataList[row][col].pData16[0], sizeof(ushort), rawDataList[row][col].pixelCount, rfid);
    //字节取反
    if(reverse){
        for (int i = 0; i < rawDataList[row][col].pixelCount; i++) {
            rawDataList[row][col].pData16[i] = ReverseBytes(rawDataList[row][col].pData16[i]);
        }
    }
	fclose(rfid);
    std::vector<ushort> temp16 = rawDataList[row][col].pData16;

    //原数据转二维
	rawDataList[row][col].imageData.resize(int(rawDataList[row][col].imageSize.height()));
	for (int i = 0; i < rawDataList[row][col].imageSize.height(); i++) {
		rawDataList[row][col].imageData[i].resize(int(rawDataList[row][col].imageSize.width()));
		for (int j = 0; j < rawDataList[row][col].imageSize.width(); j++) {
            rawDataList[row][col].imageData[i][j] = temp16[i * int(rawDataList[row][col].imageSize.width()) + j];
		}
	}

    //原数据转8字节，压缩灰度空间
    rawDataList[row][col].imin = rawDataList[row][col].tempMin = *min_element(temp16.begin(), temp16.end());
    rawDataList[row][col].imax = rawDataList[row][col].tempMax = *max_element(temp16.begin(), temp16.end());
    emit sendMinMax(row, col, rawDataList[row][col].imin, rawDataList[row][col].imax, rawDataList[row][col].imin, rawDataList[row][col].imax);
    double unit = (rawDataList[row][col].tempMax - rawDataList[row][col].tempMin) / 256.0;

    /*int linewidth = (rawDataList[row][col].imageSize.width() * 8 + 31)/32 * 4;
    rawDataList[row][col].pData8.resize(linewidth * rawDataList[row][col].imageSize.height());
    for (int i = 0; i < rawDataList[row][col].imageSize.height(); i++) {
        for (int j = 0; j < rawDataList[row][col].imageSize.width(); j++) {
            rawDataList[row][col].pData8[i * linewidth + j] = uchar((pData16[i * rawDataList[row][col].imageSize.width() + j] - tempMin) / unit == 256 ?
                255 : (pData16[i * rawDataList[row][col].imageSize.width() + j] - tempMin) / unit);
        }
    }*/

	rawDataList[row][col].pData8.resize(rawDataList[row][col].pixelCount);
	for (int i = 0; i < rawDataList[row][col].pixelCount; i++)
	{
        rawDataList[row][col].pData8[i] = uchar((temp16[i] - rawDataList[row][col].tempMin) / unit == 256 ?
            255 : (temp16[i] - rawDataList[row][col].tempMin) / unit);
	}

    rawDataList[row][col].histogram = getHistogram(rawDataList[row][col].pData8);
    QVariant hisVar;
    hisVar.setValue(getHistogram16(rawDataList[row][col].tempMin,
        rawDataList[row][col].tempMax,
        rawDataList[row][col].pData16));

    //数据格式转为QImage并转发
    rawDataList[row][col].nowImage = QImage(&rawDataList[row][col].pData8[0],
        int(rawDataList[row][col].imageSize.width()),
		int(rawDataList[row][col].imageSize.height()),
		QImage::Format_Indexed8);
	emit sendImage(row, col, true, rawDataList[row][col].nowImage, hisVar);
}

void RawImage::openOtherTypeImage(int row, int col) {
	rawDataList[row][col].nowImage = QImage(rawDataList[row][col].srcPath);

    QImage tempImg;
    if (!rawDataList[row][col].nowImage.allGray())
    {
        tempImg = rawDataList[row][col].nowImage.convertToFormat(QImage::Format_Grayscale8);
    }
    if (tempImg.isNull())
    {
        int nWidth = tempImg.width();
        int nHeight = tempImg.height();
        QVector<int> hist(256, 0);

        for (int j = 0; j < nHeight; j++)
        {
            for (int k = 0; k < nWidth; k++)
            {
                int nIndex = int(tempImg.bits()[j * nHeight + k]);
                hist[nIndex] = hist.at(nIndex) + 1;
            }
        }

        int histMin = 256, histMax = 0;
        for (auto it : hist) {
            if (it != 0) {
                histMin = it;
            }
        }
        for (int i = 255; i >= 0; i--) {
            if (i != 0) {
                histMax = i;
            }
        }

        QVariant hisVar;
        hisVar.setValue(hist);

        emit sendMinMax(row, col, histMin, histMax, histMin, histMax);
        emit sendImage(row, col, true, rawDataList[row][col].nowImage, hisVar);
    }
}

void RawImage::openImageBySon(int row, int col, QString path, qreal w, qreal h, QString d, bool reverse) {
	if (w == 0.0 || h == 0.0) {
		QImage temp;
        QVariant hisVar;
        hisVar.setValue(std::vector<int>());
		sendImage(row, col, false, temp, hisVar);
		return;
	}
	RawImageData data;
	data.srcPath = path.remove("file:///");
	data.imageType = data.srcPath.split(".")[1];
	data.imageSize = QSizeF(w, h);
	data.pixelCount = w * h;
	rawDataList[row][col] = data;

	if (!data.imageType.compare("raw", Qt::CaseInsensitive)) {
        openRawImage(row, col, reverse);
	}
	else if (!data.imageType.compare("png", Qt::CaseInsensitive)
		|| !data.imageType.compare("jpg", Qt::CaseInsensitive)
		|| !data.imageType.compare("bmp", Qt::CaseInsensitive)) {
		openOtherTypeImage(row, col);
	}
}

void RawImage::saveImageBySon(int row, int col, QString path, QString type) {
	if (rawDataList[row][col].nowImage.save(path.remove("file:///"))) {
		emit sendSaveTip(true);
	}
	else {
		emit sendSaveTip(false);
	}
}

void RawImage::autoImageBySon(int row, int col){
    int limit = rawDataList[row][col].pixelCount / 10;
    if (rawDataList[row][col].autoThreshold < 10 || (rawDataList[row][col].hmax == rawDataList[row][col].hmin && rawDataList[row][col].hmin == 255))
        rawDataList[row][col].autoThreshold = 5000;
    else
        rawDataList[row][col].autoThreshold /= 2;

    int threshold = rawDataList[row][col].pixelCount / rawDataList[row][col].autoThreshold;
    int i = -1;
    bool found = false;
    int count;
    do {
        i++;
        count = rawDataList[row][col].histogram[i];
        if (count > limit) count = 0;
        found = count > threshold;
    } while (!found && i < 255);
    rawDataList[row][col].hmin = i;
    i = 256;
    do {
        i--;
        count = rawDataList[row][col].histogram[i];
        if (count > limit) count = 0;
        found = count > threshold;
    } while (!found && i > 0);
    rawDataList[row][col].hmax = i;

    double newMin, newMax;
    if (rawDataList[row][col].hmax >= rawDataList[row][col].hmin) {
        newMin = rawDataList[row][col].imin + rawDataList[row][col].hmin * (rawDataList[row][col].imax - rawDataList[row][col].imin) / 256.0;
        newMax = rawDataList[row][col].imin + rawDataList[row][col].hmax * (rawDataList[row][col].imax - rawDataList[row][col].imin) / 256.0;
        if (newMin == newMax)
        {
            newMin = rawDataList[row][col].imin;
            newMax = rawDataList[row][col].imax;
        }
    }
    else {
        newMin = qMin(rawDataList[row][col].tempMin, rawDataList[row][col].tempMax);
        newMax = qMax(rawDataList[row][col].tempMin, rawDataList[row][col].tempMax);
    }

    emit sendMinMax(row, col, rawDataList[row][col].imin, rawDataList[row][col].imax, newMin, newMax);
    if (rawDataList[row][col].isReverse)
    {
        setGrayscale(row, col, newMax, newMin);
    }
    else
    {
        setGrayscale(row, col, newMin, newMax);
    }

    emit sendAutoTip(true);
}

void RawImage::reverseImageBySon(int row, int col){
    std::swap(rawDataList[row][col].tempMin, rawDataList[row][col].tempMax);
    double unit = (rawDataList[row][col].tempMax - rawDataList[row][col].tempMin) / 256.0;
    for (int i = 0; i < rawDataList[row][col].pixelCount; i++)
    {
        double temp = (rawDataList[row][col].pData16[i] - rawDataList[row][col].tempMin) / unit;
        if (temp < 0)temp = 0;
        if (temp > 255)temp = 255;
        rawDataList[row][col].pData8[i] = uchar(temp);
    }
    rawDataList[row][col].isReverse = !rawDataList[row][col].isReverse;
    rawDataList[row][col].nowImage = QImage(&rawDataList[row][col].pData8[0],
        int(rawDataList[row][col].nowImage.width()),
        int(rawDataList[row][col].nowImage.height()),
        QImage::Format_Indexed8);
    emit sendReverseTip(true);
}
