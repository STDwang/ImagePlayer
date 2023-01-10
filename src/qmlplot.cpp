#include "qmlplot.h"
#include "qcustomplot.h"
#include <QDebug>
#include <random>

int getMax(int a, int b) {
	return a > b ? a : b;
}

int getMin(int a, int b) {
	return a < b ? a : b;
}

DDuiBasePlotItem::DDuiBasePlotItem(QQuickItem* parent) : QQuickPaintedItem(parent), m_CustomPlot(new QCustomPlot())
{
	setFlag(QQuickItem::ItemHasContents, true);
	setAcceptedMouseButtons(Qt::AllButtons);
	setAcceptHoverEvents(true);
	connect(this, &QQuickPaintedItem::widthChanged, this, &DDuiBasePlotItem::updateCustomPlotSize);
	connect(this, &QQuickPaintedItem::heightChanged, this, &DDuiBasePlotItem::updateCustomPlotSize);
}

DDuiBasePlotItem::~DDuiBasePlotItem()
{
	delete m_CustomPlot;
	m_CustomPlot = nullptr;
}

void DDuiBasePlotItem::paint(QPainter* painter)
{
	if (m_CustomPlot)
	{
		QPixmap picture(boundingRect().size().toSize());
		QCPPainter qcpPainter(&picture);
		m_CustomPlot->toPainter(&qcpPainter);
		painter->drawPixmap(QPoint(), picture);
	}
}

QCustomPlot* DDuiBasePlotItem::getPlot()
{
	if (!m_CustomPlot) {
		m_CustomPlot = new QCustomPlot;
	}
	return m_CustomPlot;
}

void DDuiBasePlotItem::mousePressEvent(QMouseEvent* event)
{
	routeMouseEvents(event);
}

void DDuiBasePlotItem::mouseReleaseEvent(QMouseEvent* event)
{
	routeMouseEvents(event);
}

void DDuiBasePlotItem::mouseMoveEvent(QMouseEvent* event)
{
	routeMouseEvents(event);
}

void DDuiBasePlotItem::mouseDoubleClickEvent(QMouseEvent* event)
{
	routeMouseEvents(event);
}

void DDuiBasePlotItem::wheelEvent(QWheelEvent* event)
{
	routeWheelEvents(event);
}

void DDuiBasePlotItem::routeMouseEvents(QMouseEvent* event)
{
	if (m_CustomPlot)
	{
		QMouseEvent* newEvent = new QMouseEvent(event->type(), event->localPos(), event->button(), event->buttons(), event->modifiers());
		QCoreApplication::postEvent(m_CustomPlot, newEvent);
	}
}

void DDuiBasePlotItem::routeWheelEvents(QWheelEvent* event)
{
	if (m_CustomPlot)
	{
		QWheelEvent* newEvent = new QWheelEvent(event->pos(), event->delta(), event->buttons(), event->modifiers(), event->orientation());
		QCoreApplication::postEvent(m_CustomPlot, newEvent);
	}
}

void DDuiBasePlotItem::hoverMoveEvent(QHoverEvent* event)
{
}

void DDuiBasePlotItem::updateCustomPlotSize()
{
	if (m_CustomPlot)
	{
		m_CustomPlot->setGeometry(0, 0, (int)width(), (int)height());
		m_CustomPlot->setViewport(QRect(0, 0, (int)width(), (int)height()));
	}
}

void DDuiBasePlotItem::onCustomReplot()
{
	update();
}

////
/// \brief CustomPlotItem::CustomPlotItem
/// \param parent
///
CustomHistPlotItem::CustomHistPlotItem(QQuickItem* parent) : DDuiBasePlotItem(parent)
{
}

CustomHistPlotItem::~CustomHistPlotItem()
{
}

void CustomHistPlotItem::initCustomPlot()
{
	//getPlot()->setBackground(QColor(45, 51, 59, 200));//外层背景颜色
	//getPlot()->axisRect()->setBackground(QBrush(QColor::fromRgb(34, 39, 46)));//里层背景黑色

	//getPlot()->xAxis->setBasePen(QPen(Qt::white, 1));  // 轴线的画笔
	//getPlot()->yAxis->setBasePen(QPen(Qt::white, 1));  // 轴线的画笔
	//getPlot()->xAxis->setLabelColor(Qt::white);
	//getPlot()->yAxis->setLabelColor(Qt::white);
	//getPlot()->xAxis->setSubTickPen(QPen(Qt::white, 1)); // 轴子刻度线的画笔
	//getPlot()->yAxis->setSubTickPen(QPen(Qt::white, 1)); // 轴子刻度线的画笔
	//getPlot()->xAxis->setTickLabelColor(Qt::white);		 // 轴刻度文字颜色
	//getPlot()->yAxis->setTickLabelColor(Qt::white);	     // 轴刻度文字颜色

	//getPlot()->xAxis->grid()->setZeroLinePen(Qt::NoPen);
	//getPlot()->yAxis->grid()->setZeroLinePen(Qt::NoPen);

	getPlot()->addGraph();
	getPlot()->graph(0)->setPen(QPen(QColor(49, 109, 202)));

	QPen pen;
	pen.setColor(QColor(Qt::red));
	pen.setStyle(Qt::DashLine);//虚线
	plottracer = new QCPItemTracer(getPlot());
	plottracer->setPen(pen);

	getPlot()->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	connect(getPlot(), &QCustomPlot::afterReplot, this, &CustomHistPlotItem::onCustomReplot);
}

void CustomHistPlotItem::histPlay(QVariantMap data)
{
	hist.clear();
	hist = data["data"].value<std::vector<int>>();
	if (hist.empty()) return;
	histMinX = data["xmin"].toInt();
	histMaxX = data["xmax"].toInt();

	getPlot()->graph(0)->data().data()->clear();

	int ymax = *max_element(hist.begin(), hist.end());
	int ymin = *min_element(hist.begin(), hist.end());
	getPlot()->xAxis->setRange(histMinX, histMaxX);
	getPlot()->yAxis->setRange(ymin, ymax / 100);
	for (int i = histMinX; i < histMinX + hist.size(); i++) {
		getPlot()->graph(0)->addData(i, hist[i - histMinX]);
	}
	plottracer->setGraph(getPlot()->graph(0));
	getPlot()->replot();
}

void CustomHistPlotItem::hoverEnterEvent(QHoverEvent* event) {
}

void CustomHistPlotItem::hoverLeaveEvent(QHoverEvent* event) {
	emit sendLeaveTip();
}

void CustomHistPlotItem::hoverMoveEvent(QHoverEvent* event)
{
	if (hist.size() > 0) {
		// 像素坐标转成实际的x,y轴的坐标
		int x_val = getPlot()->xAxis->pixelToCoord(event->pos().x());
		if (x_val < histMinX) x_val = histMinX;
		if (x_val > hist.size() - 1 + histMinX) x_val = hist.size() - 1 + histMinX;
		plottracer->setGraphKey(x_val);
		getPlot()->replot();
		emit sendPosTip(x_val, hist[x_val - histMinX]);
	}
}
