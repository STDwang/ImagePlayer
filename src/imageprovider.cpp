#include "imageprovider.h"

ImageProvider::ImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

void ImageProvider::setImageViewSize(int row, int col) {
	imgs.clear();
	imgs.resize(row);
	for (int i = 0; i < row; i++) {
		imgs[i].resize(col);
	}
}

QImage ImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize)
{
	QStringList indexList = id.left(id.indexOf("###")).split("_");
	if (indexList.size() < 2) {
		QImage temp;
		return temp;
	}
	int row = indexList[0].toInt();
	int col = indexList[1].toInt();

	return imgs[row][col];
}

QPixmap ImageProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize)
{
	QStringList indexList = id.left(id.indexOf("###")).split("_");
	if (indexList.size() < 2) {
		QPixmap temp;
		return temp;
	}
	int row = indexList[0].toInt();
	int col = indexList[1].toInt();
	return QPixmap::fromImage(imgs[row][col]);
}
