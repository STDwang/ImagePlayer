#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QApplication>
#include <QIcon>
#include "t2d/Toou2D.h"
#include <./src/imagesever.h>
int main(int argc, char* argv[])
{
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication app(argc, argv);
	QQmlApplicationEngine engine;
    Toou2D t; t.create(&engine);
    app.setApplicationName("ImagePlayer");
	app.setWindowIcon(QIcon(":/res/png/sate(1).png"));

	imageSever* imagesever = new imageSever();
	engine.rootContext()->setContextProperty("imagesever", imagesever);
    engine.addImageProvider(QLatin1String("CodeImg"), imagesever->m_pImgProvider);
    engine.addImageProvider(QLatin1String("CodeImgAerial"), imagesever->m_pImgAerialProvider);

	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject* obj, const QUrl& objUrl) {
		if (!obj && url == objUrl) QCoreApplication::exit(-1);
		}, Qt::QueuedConnection);

	engine.load(url);
	return app.exec();
}
