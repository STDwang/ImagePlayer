#include "imagesever.h"
#include <QFile>
#include <QDir>
#include <./src/rawdatatype.h>
/**
 * @brief 函数简要说明-构造函数,连接信号-槽
 *
 * @return 返回说明
 *     -<em>imageSever</em> 一个imageSever对象
 */
imageSever::imageSever(QObject* parent) : QObject(parent)
{
	rawThread = new QThread;
	rawImage = new RawImage();
	rawImage->moveToThread(rawThread);
	connect(this, SIGNAL(sendSetViewSizeBySon(int, int)), rawImage, SLOT(setViewSizeBySon(int, int)));

	connect(this, SIGNAL(sendGetImageSizeBySon(QString)), rawImage, SLOT(getImageSizeBySon(QString)));
	connect(rawImage, SIGNAL(sendFileSize(qint64)), this, SLOT(getImageSizeFromSon(qint64)));

    connect(this, SIGNAL(sendOpenImageBySon(int, int, QString, qreal, qreal, QString, bool)), rawImage, SLOT(openImageBySon(int, int, QString, qreal, qreal, QString, bool)));
	connect(rawImage, SIGNAL(sendImage(int, int, bool, QImage)), this, SLOT(openImageFromSon(int, int, bool, QImage)));

	connect(this, SIGNAL(sendGetPixValueBySon(int, int, int, int)), rawImage, SLOT(getPixValueBySon(int, int, int, int)));
	connect(rawImage, SIGNAL(sendPixValue(bool, int, int, int)), this, SLOT(getPixValueFromSon(bool, int, int, int)));

	connect(this, SIGNAL(sendSaveImageBySon(int, int, QString, QString)), rawImage, SLOT(saveImageBySon(int, int, QString, QString)));
	connect(rawImage, SIGNAL(sendSaveTip(bool)), this, SLOT(saveImageFromSon(bool)));

	rawThread->start();
	m_pImgProvider = new ImageProvider();

    m_pImgProvider->setImageViewSize(1, 1);
    emit sendSetViewSizeBySon(1, 1);
}
/**
 * @brief 函数简要说明-设置二维存储视图
 * @param int	[视图行位置]
 * @param int	[视图列位置]
 *
 * @return null
 */
void imageSever::setViewSize(int row, int col) {
	m_pImgProvider->setImageViewSize(row, col);
	emit sendSetViewSizeBySon(row, col);
}
/**
 * @brief 函数简要说明-得到图片文件大小
 * @param QString	[图片路径]
 *
 * @return null
 */
void imageSever::getImageSize(QString path) {
	emit sendGetImageSizeBySon(path);
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
void imageSever::openImage(int row, int col, QString path, qreal w, qreal h, QString d, bool reverse) {
    emit sendOpenImageBySon(row, col, path, w, h, d, reverse);
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
void imageSever::getPixValue(int row, int col, int x, int y) {
	emit sendGetPixValueBySon(row, col, x, y);
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
void imageSever::saveImage(int row, int col, QString path, QString type) {
	emit sendSaveImageBySon(row, col, path, type);
}

/**
 * @brief 函数简要说明-得到图片文件大小并发送给UI界面
 * @param int		[所选图像视图行位置]
 * @param int		[所选图像视图列位置]
 * @param QString	[保存路径]
 * @param QString	[保存格式]
 *
 * @return null
 */
void imageSever::getImageSizeFromSon(qint64 size) {
	emit sendImageSizeToUI(size);
}
/**
 * @brief 函数简要说明-得到图片并发送给UI界面
 * @param int		[所选图像视图行位置]
 * @param int		[所选图像视图列位置]
 * @param bool		[是否成功打开]
 * @param QImage	[图片]
 *
 * @return null
 */
void imageSever::openImageFromSon(int row, int col, bool tip, QImage img) {
	m_pImgProvider->imgs[row][col] = img;
	emit sendImageToUI(row, col, tip, img.width(), img.height());
}
/**
 * @brief 函数简要说明-得到xyv并发送给UI界面
 * @param bool	[是否成功]
 * @param int	[x坐标]
 * @param int	[y坐标]
 * @param int	[灰度值]
 *
 * @return null
 */
void imageSever::getPixValueFromSon(bool tip, int x, int y, int v) {
	emit sendPixValueToUI(tip, x, y, v);
}
/**
 * @brief 函数简要说明-保存并发送结果给UI界面
 * @param bool	[是否成功保存]
 *
 * @return null
 */
void imageSever::saveImageFromSon(bool tip) {
	emit sendSaveTipToUI(tip);
}
