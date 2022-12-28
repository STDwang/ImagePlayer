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
    QImage temp;
	if (indexList.size() < 2) {
		return temp;
	}
	int row = indexList[0].toInt();
	int col = indexList[1].toInt();
    temp = imgs[row][col];
    if (mScale < 1.0){
        temp = imgs[row][col].scaled(imgs[row][col].width() * mScale, imgs[row][col].height() * mScale, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    return temp;
}

QPixmap ImageProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize)
{
	QStringList indexList = id.left(id.indexOf("###")).split("_");
    QPixmap temp;
	if (indexList.size() < 2) {
		return temp;
	}
	int row = indexList[0].toInt();
	int col = indexList[1].toInt();
    temp = QPixmap::fromImage(imgs[row][col]);
    if (mScale < 1.0){
        temp = QPixmap::fromImage(imgs[row][col].scaled(imgs[row][col].width() * mScale, imgs[row][col].height() * mScale, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
    return temp;
}

void ImageProvider::setImageScaled(double scale) {
    mScale = scale;
}
