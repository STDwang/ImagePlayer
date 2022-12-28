//***************************************************************************
// Author:        stdWang
// Date:		  2022/09/12
//***************************************************************************
#include <QQuickImageProvider>
#pragma once
class ImageProvider : public QQuickImageProvider
{
public:
	ImageProvider();
	QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize);
	QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize);
	void setImageViewSize(int row, int col);
    void setImageScaled(double scale);

	QVector<QVector<QImage>> imgs;
private:
    //缩放比
    double mScale = 1.0;
};
