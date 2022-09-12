#include "rawimage.h"
#include <QFile>
/**
 * @brief 函数简要说明-构造函数,连接信号-槽
 *
 * @return 返回说明
 *     -<em>RawImage</em> 一个RawImage对象
 */
RawImage::RawImage(QObject* parent) : QObject(parent) {}
/**
 * @brief 函数简要说明-辅助函数，反转字节
 *
 * @return void
 */
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
/**
 * @brief 函数简要说明-设置二维存储视图
 * @param int	[视图行位置]
 * @param int	[视图列位置]
 *
 * @return null
 */
void RawImage::setViewSizeBySon(int row, int col) {
	rawDataList.clear();
	rawDataList.resize(row);
	for (int i = 0; i < row; i++) {
		rawDataList[i].resize(col);
	}
}
/**
 * @brief 函数简要说明-得到图片文件大小
 * @param QString	[图片路径]
 *
 * @return null
 */
void RawImage::getImageSizeBySon(QString path) {
	srcPathTemp = path.remove("file:///");
	// 读取Raw文件的数据
	QFile file(srcPathTemp);
	emit sendFileSize(file.size());
}
/**
 * @brief 函数简要说明-打开raw图片
 * @param int		[视图行位置]
 * @param int		[视图列位置]
 *
 * @return null
 */
void RawImage::openRawImage(int row, int col, bool reverse) {
	rawDataList[row][col].imageData.clear();
    rawDataList[row][col].pData8.clear();
    std::vector<ushort> pData16;
    pData16.resize(rawDataList[row][col].pixelCount);

	// 读取Raw文件的数据
	QByteArray ba1;
	ba1.append(rawDataList[row][col].srcPath);
	const char* rpath = ba1.data();
	FILE* rfid = fopen(rpath, "rb");
	if (rfid == NULL) return;
    fread(&pData16[0], sizeof(ushort), rawDataList[row][col].pixelCount, rfid);

    if(reverse){
        for (int i = 0; i < rawDataList[row][col].pixelCount; i++) {
            pData16[i] = ReverseBytes(pData16[i]);
        }
    }

	fclose(rfid);

	rawDataList[row][col].imageData.resize(int(rawDataList[row][col].imageSize.height()));
	for (int i = 0; i < rawDataList[row][col].imageSize.height(); i++) {
		rawDataList[row][col].imageData[i].resize(int(rawDataList[row][col].imageSize.width()));
		for (int j = 0; j < rawDataList[row][col].imageSize.width(); j++) {
            rawDataList[row][col].imageData[i][j] = pData16[i * int(rawDataList[row][col].imageSize.width()) + j];
		}
	}

    double tempMin = *min_element(pData16.begin(), pData16.end());
    double tempMax = *max_element(pData16.begin(), pData16.end());

    rawDataList[row][col].pData8.resize(rawDataList[row][col].pixelCount);
    double unit = (tempMax - tempMin) / 256;
	uchar tempP8;
	for (int i = 0; i < rawDataList[row][col].pixelCount; i++)
	{
        rawDataList[row][col].pData8[i] = uchar((pData16[i] - tempMin) / unit == 256 ?
            255 : (pData16[i] - tempMin) / unit);
	}

    rawDataList[row][col].nowImage = QImage(&rawDataList[row][col].pData8[0],
        int(rawDataList[row][col].imageSize.width()),
		int(rawDataList[row][col].imageSize.height()),
		QImage::Format_Indexed8);

	emit sendImage(row, col, true, rawDataList[row][col].nowImage);
}
/**
 * @brief 函数简要说明-打开其他格式图片
 * @param int		[视图行位置]
 * @param int		[视图列位置]
 *
 * @return null
 */
void RawImage::openOtherTypeImage(int row, int col) {
	rawDataList[row][col].nowImage = QImage(rawDataList[row][col].srcPath);
	emit sendImage(row, col, true, rawDataList[row][col].nowImage);
}
/**
 * @brief 函数简要说明-打开图片
 * @param int		[视图行位置]
 * @param int		[视图列位置]
 * @param qreal		[宽度]
 * @param qreal		[高度]
 * @param QString	[位深]
 *
 * @return null
 */
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
/**
 * @brief 函数简要说明-根据(x,y)找灰度值
 * @param int	[视图行位置]
 * @param int	[视图列位置]
 * @param int	[x坐标]
 * @param int	[y坐标]
 *
 * @return void
 */
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
/**
 * @brief 函数简要说明-保存图片
 * @param int		[所选图像视图行位置]
 * @param int		[所选图像视图列位置]
 * @param QString	[保存路径]
 * @param QString	[保存格式]
 *
 * @return null
 */
void RawImage::saveImageBySon(int row, int col, QString path, QString type) {
	if (rawDataList[row][col].nowImage.save(path.remove("file:///"))) {
		emit sendSaveTip(true);
	}
	else {
		emit sendSaveTip(false);
	}
}
