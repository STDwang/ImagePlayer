#pragma execution_character_set("utf-8")

#include <QtQuick>
#include <QVariantMap>
#include <QThread>

class QCustomPlot;
class QCPAbstractPlottable;
class QCPTextElement;
class QCPItemTracer;
class QCPItemText;

class DDuiBasePlotItem : public QQuickPaintedItem
{
	Q_OBJECT
public:
	DDuiBasePlotItem(QQuickItem* parent = 0);
	virtual ~DDuiBasePlotItem();

	void paint(QPainter* painter);
	virtual void initCustomPlotUI() {}

	QCustomPlot* getPlot();
protected:
	virtual void hoverMoveEvent(QHoverEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* event);

	void routeMouseEvents(QMouseEvent* event);
	void routeWheelEvents(QWheelEvent* event);

public slots:
	void onCustomReplot();
	void updateCustomPlotSize();
private:
	QCustomPlot* m_CustomPlot;
};

class CustomHistPlotItem : public DDuiBasePlotItem
{
	Q_OBJECT
public:
	CustomHistPlotItem(QQuickItem* parent = 0);
	virtual ~CustomHistPlotItem();

	Q_INVOKABLE void initCustomPlot();
	Q_INVOKABLE void histPlay(QVariantMap data);

signals:
	void sendPosTip(int posX, int posY);
	void sendLeaveTip();
public:
	std::vector<int> hist;
	int histMinX, histMaxX;
	QCPItemTracer* plottracer;
protected:
	virtual void hoverEnterEvent(QHoverEvent* event);
	virtual void hoverLeaveEvent(QHoverEvent* event);
	virtual void hoverMoveEvent(QHoverEvent* event);
};
