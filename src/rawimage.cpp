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

void RawImage::setViewSizeBySon(int row, int col) {
	rawDataList.clear();
	rawDataList.resize(row);
	for (int i = 0; i < row; i++) {
		rawDataList[i].resize(col);
	}
}

void RawImage::getImageSizeBySon(QString path) {
	srcPathTemp = path.remove("file:///");
	// 读取Raw文件的数据
	QFile file(srcPathTemp);
	emit sendFileSize(file.size());
}

void RawImage::openRawImage(int row, int col, bool reverse) {
	rawDataList[row][col].imageData.clear();
    rawDataList[row][col].pData8.clear();
    std::vector<ushort> pData16;
    pData16.resize(rawDataList[row][col].pixelCount);

	// 读取Raw文件的数据
    QTextCodec* tc = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    QByteArray ba1 = rawDataList[row][col].srcPath.toLocal8Bit();
    FILE* rfid = fopen(ba1.data(), "rb");

	if (rfid == NULL) return;
    fread(&pData16[0], sizeof(ushort), rawDataList[row][col].pixelCount, rfid);
    //字节取反
    if(reverse){
        for (int i = 0; i < rawDataList[row][col].pixelCount; i++) {
            pData16[i] = ReverseBytes(pData16[i]);
        }
    }
	fclose(rfid);

    //原数据转二维
	rawDataList[row][col].imageData.resize(int(rawDataList[row][col].imageSize.height()));
	for (int i = 0; i < rawDataList[row][col].imageSize.height(); i++) {
		rawDataList[row][col].imageData[i].resize(int(rawDataList[row][col].imageSize.width()));
		for (int j = 0; j < rawDataList[row][col].imageSize.width(); j++) {
            rawDataList[row][col].imageData[i][j] = pData16[i * int(rawDataList[row][col].imageSize.width()) + j];
		}
	}

    //原数据转8字节，压缩灰度空间
    double tempMin = *min_element(pData16.begin(), pData16.end());
    double tempMax = *max_element(pData16.begin(), pData16.end());
    double unit = (tempMax - tempMin) / 256;

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
		rawDataList[row][col].pData8[i] = uchar((pData16[i] - tempMin) / unit == 256 ?
			255 : (pData16[i] - tempMin) / unit);
	}

    //数据格式转为QImage并转发
    rawDataList[row][col].nowImage = QImage(&rawDataList[row][col].pData8[0],
        int(rawDataList[row][col].imageSize.width()),
		int(rawDataList[row][col].imageSize.height()),
		QImage::Format_Indexed8);
	emit sendImage(row, col, true, rawDataList[row][col].nowImage);
}

void RawImage::openOtherTypeImage(int row, int col) {
	rawDataList[row][col].nowImage = QImage(rawDataList[row][col].srcPath);
	emit sendImage(row, col, true, rawDataList[row][col].nowImage);
}

void RawImage::openImageBySon(int row, int col, QString path, qreal w, qreal h, QString d, bool reverse) {
	if (w == 0.0 || h == 0.0) {
		QImage temp;
		sendImage(row, col, false, temp);
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

void RawImage::saveImageBySon(int row, int col, QString path, QString type) {
	if (rawDataList[row][col].nowImage.save(path.remove("file:///"))) {
		emit sendSaveTip(true);
	}
	else {
		emit sendSaveTip(false);
	}
}
