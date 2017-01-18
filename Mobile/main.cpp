#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <quickpresenter.h>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);
	QGuiApplication::setApplicationName(QStringLiteral(TARGET));
	QGuiApplication::setApplicationVersion(QStringLiteral(VERSION));
	QGuiApplication::setOrganizationName(QStringLiteral(COMPANY));
	QGuiApplication::setOrganizationDomain(QStringLiteral("com.Skycoder42"));
	QGuiApplication::setApplicationDisplayName(DISPLAY_NAME);
	QGuiApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/main.ico")));

	QuickPresenter::registerAsPresenter();
	QQmlApplicationEngine engine;
	engine.load(QUrl(QLatin1String("qrc:/qtmvvm/qml/App.qml")));

	return app.exec();
}
