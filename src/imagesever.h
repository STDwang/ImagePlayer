//***************************************************************************
// Author:        stdWang
// Date:		  2022/09/12
//***************************************************************************
#ifndef IMAGESEVER_H
#define IMAGESEVER_H
#include <qthread.h>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QVariant>
#include <QMetaObject>
#include <QCoreApplication>
#include <QVariantMap>
#include <QImage>
#include "imageprovider.h"
#include "rawimage.h"
class imageSever : public QObject
{
	Q_OBJECT
public:
	imageSever(QObject* parent = 0);
    ~imageSever();
    //主视图图像源
    ImageProvider* m_pImgProvider;
    //鸟瞰图图像源
    ImageProvider* m_pImgAerialProvider;
signals:
    //发送命令给图像处理线程
	void sendSetViewSizeBySon(int row, int col);
	void sendGetImageSizeBySon(QString path);
    void sendOpenImageBySon(int row, int col, QString path, qreal w, qreal h, QString d, bool reverse);
	void sendGetPixValueBySon(int row, int col, int x, int y);
	void sendSaveImageBySon(int row, int col, QString path, QString type);
    void sendAutoImageBySon(int row, int col);
    void sendReverseImageBySon(int row, int col);

    //转发命令给UI界面
	void sendImageSizeToUI(qint64 size);
	void sendImageToUI(int row, int col, bool tip, qreal w, qreal h, QVariant hist);
	void sendPixValueToUI(bool tip, int x, int y, int v);
	void sendSaveTipToUI(bool tip);
    void sendRefreshToUI();
    void sendMinMaxToUI(int row, int col, int min, int max, int autoMin, int autoMax);
public slots:
    /**
     * @brief 设置二维存储视图
     * @param int	[视图行位置]
     * @param int	[视图列位置]
     */
	void setViewSize(int row, int col);


    /**
     * @brief 设置图像缩放比
     * @param scale	[缩放比]
     */
    void setImageScaled(double scale);

    /**
     * @brief 设置鸟瞰图图像缩放比
     * @param scale	[缩放比]
     */
    void setImageAerialScaled(double scale);

    /**
     * @brief 得到图片文件大小
     * @param QString	[图片路径]
     */
    void getImageSize(QString path);

    /**
     * @brief 根据(x,y)找灰度值v
     * @param int	[视图行位置]
     * @param int	[视图列位置]
     * @param int	[x坐标]
     * @param int	[y坐标]
     */
    void getPixValue(int row, int col, int x, int y);

    /**
     * @brief 打开图片
     * @param int		[视图行位置]
     * @param int		[视图列位置]
     * @param qreal		[宽度]
     * @param qreal		[高度]
     * @param QString	[位深]
     */
    void openImage(int row, int col, QString path, qreal w, qreal h, QString d, bool reverse);

    /**
     * @brief 保存图片
     * @param int		[所选图像视图行位置]
     * @param int		[所选图像视图列位置]
     * @param QString	[保存路径]
     * @param QString	[保存格式]
     */
	void saveImage(int row, int col, QString path, QString type);

    /**
     * @brief auto亮度
     * @param int		[所选图像视图行位置]
     * @param int		[所选图像视图列位置]
     */
    void autoImage(int row, int col);

    /**
     * @brief 反转灰度
     * @param int		[所选图像视图行位置]
     * @param int		[所选图像视图列位置]
     */
    void reverseImage(int row, int col);
private slots:
    /**
     * @brief 得到图片文件大小并发送给UI界面
     * @param int		[所选图像视图行位置]
     * @param int		[所选图像视图列位置]
     * @param QString	[保存路径]
     * @param QString	[保存格式]
     */
	void getImageSizeFromSon(qint64 size);

    /**
     * @brief 得到xyv并发送给UI界面
     * @param bool	[是否成功]
     * @param int	[x坐标]
     * @param int	[y坐标]
     * @param int	[灰度值]
     */
    void getPixValueFromSon(bool tip, int x, int y, int v);

    /**
     * @brief 得到图片并发送给UI界面
     * @param int		[所选图像视图行位置]
     * @param int		[所选图像视图列位置]
     * @param bool		[是否成功打开]
     * @param QImage	[图片]
     */
	void openImageFromSon(int row, int col, bool tip, QImage img, QVariant hist);

    /**
     * @brief 保存并发送结果给UI界面
     * @param bool	[是否成功保存]
     */
	void saveImageFromSon(bool tip);

    /**
     * @brief auto结果给UI界面
     * @param bool	[是否成功保存]
     */
    void autoImageFromSon(bool tip);

    /**
     * @brief reverse结果给UI界面
     * @param bool	[是否成功保存]
     */
    void reverseImageFromSon(bool tip);

    /**
     * @brief 灰度空间大小给UI界面
     * @param int		[所选图像视图行位置]
     * @param int		[所选图像视图列位置]
     * @param int		[原始灰度空间最小值]
     * @param int		[原始灰度空间最大值]
     * @param int		[当前灰度空间最小值]
     * @param int		[当前灰度空间最大值]
     */
    void minMaxFromSon(int row, int col, int min, int max, int autoMin, int autoMax);
private:
	QThread* rawThread;
	RawImage* rawImage;
};

#endif // IMAGESEVER_H
