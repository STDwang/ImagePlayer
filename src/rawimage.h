//***************************************************************************
// Author:        stdWang
// Date:		  2022/09/12
//***************************************************************************
#ifndef RAWIMAGE_H
#define RAWIMAGE_H

#include <QImage>
#include <qglobal.h>
#include <QVariant>
#include "./src/rawdatatype.h"
class RawImage : public QObject
{
	Q_OBJECT
public:
	explicit RawImage(QObject* parent = 0);
signals:
    //发送图片
	void sendImage(int row, int col, bool tip, QImage img, QVariant hist);
    //发送文件大小
	void sendFileSize(qint64 size);
    //发送像素查询结果
	void sendPixValue(bool tip, int x, int y, int v);
    //发送灰度空间大小
    void sendMinMax(int row, int col, int min, int max, int autoMin, int autoMax);
    //发送保存结果
	void sendSaveTip(bool tip);
    //发送auto结果
    void sendAutoTip(bool tip);
    //发送reverse结果
    void sendReverseTip(bool tip);
public slots:
    /**
     * @brief 函数简要说明-设置二维存储视图
     * @param row	[视图行位置]
     * @param col	[视图列位置]
     *
     * @return null
     */
	void setViewSizeBySon(int row, int col);

    /**
     * @brief 函数简要说明-得到图片文件大小
     * @param path	[图片路径]
     *
     * @return null
     */
	void getImageSizeBySon(QString path);

    /**
     * @brief 函数简要说明-打开图片
     * @param row		[视图行位置]
     * @param col		[视图列位置]
     * @param path		[图片路径]
     * @param w         [宽度]
     * @param h         [高度]
     * @param reverse	[是否反转字节]
     *
     * @return null
     */
    void openImageBySon(int row, int col, QString path, qreal w, qreal h, QString d, bool reverse);

    /**
     * @brief 函数简要说明-根据(x,y)找灰度值
     * @param row	[视图行位置]
     * @param col	[视图列位置]
     * @param x	[x坐标]
     * @param y	[y坐标]
     *
     * @return void
     */
	void getPixValueBySon(int row, int col, int x, int y);

    /**
     * @brief 函数简要说明-保存图片
     * @param row		[所选图像视图行位置]
     * @param col		[所选图像视图列位置]
     * @param path	[保存路径]
     * @param type	[保存格式]
     *
     * @return null
     */
	void saveImageBySon(int row, int col, QString path, QString type);

    /**
     * @brief 函数简要说明-auto亮度
     * @param row		[所选图像视图行位置]
     * @param col		[所选图像视图列位置]
     *
     * @return null
     */
    void autoImageBySon(int row, int col);

    /**
     * @brief 函数简要说明-反转灰度
     * @param row		[所选图像视图行位置]
     * @param col		[所选图像视图列位置]
     *
     * @return null
     */
    void reverseImageBySon(int row, int col);

private:
    /**
     * @brief 函数简要说明-打开raw图片
     * @param row		[视图行位置]
     * @param col		[视图列位置]
     * @param reverse	[是否反转字节]
     *
     * @return null
     */
    void openRawImage(int row, int col, bool reverse);

    /**
     * @brief 函数简要说明-打开其他格式图片
     * @param row		[视图行位置]
     * @param col		[视图列位置]
     *
     * @return null
     */
    void openOtherTypeImage(int row, int col);

    /**
     * @brief 函数简要说明-设置灰度空间大小
     * @param row		[视图行位置]
     * @param col		[视图列位置]
     * @param newMin	[最小值]
     * @param newMax	[最大值]
     *
     * @return null
     */
    void setGrayscale(int row, int col, double newMin, double newMax);
private:
    //图像相关数据
	std::vector<std::vector<RawImageData>> rawDataList;
    //临时路径
	QString srcPathTemp;
};
#endif // RAWIMAGE_H
