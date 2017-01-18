#include "coreapp.h"
#include <QGuiApplication>
#include <iostream>
#include "coremessage.h"
#include "ipresenter.h"

QPointer<CoreApp> CoreApp::_instance;

CoreApp::CoreApp(QObject *parent) :
	QObject(parent),
	_presenter(nullptr)
{}

CoreApp::~CoreApp() {}

CoreApp *CoreApp::instance()
{
	return _instance;
}

void CoreApp::setMainPresenter(IPresenter *presenter)
{
	Q_ASSERT_X(_instance, Q_FUNC_INFO, "The presenter must be set after construction of the CoreApp/Qt App");
	_instance->_presenter.reset(presenter);
}

IPresenter *CoreApp::presenter() const
{
	return _presenter.data();
}

void CoreApp::registerApp()
{
	//register metatypes
	qRegisterMetaType<QMetaMethod>("QMetaMethod");
	qRegisterMetaType<MessageResult*>();
	qRegisterMetaType<CoreApp::MessageType>();
	qRegisterMetaType<MessageResult::ResultType>();

	setParent(qApp);
	_instance = this;
	QMetaObject::invokeMethod(this, "initiate", Qt::QueuedConnection);
}

void CoreApp::showControl(Control *control)
{
	_presenter->present(control);
}

void CoreApp::closeControl(Control *control)
{
	_presenter->withdraw(control);
}

void CoreApp::showMessage(MessageResult *result, MessageType type, const QString &title, const QString &text, const QString &positiveAction, const QString &negativeAction, const QString &neutralAction, int inputType)
{
	_presenter->showMessage(result, type, title, text, positiveAction, negativeAction, neutralAction, inputType);
}

void CoreApp::setupParser(QCommandLineParser &parser, bool &allowInvalid)
{
	parser.addHelpOption();
	parser.addVersionOption();
	allowInvalid = true;
}

bool CoreApp::autoShowHelpOrVersion(const QCommandLineParser &parser)
{
	if(parser.isSet("help")) {
		if(!CoreMessage::information(tr("Usage"), parser.helpText()))
			std::cout << parser.helpText().toStdString() << std::endl;
		return true;
	} else if(parser.isSet("version")) {
		auto text = QGuiApplication::applicationDisplayName() +
					QLatin1Char(' ') +
					QGuiApplication::applicationVersion();
		if(!CoreMessage::information(tr("Version"), text))
			std::cout << text.toStdString() << std::endl;
		return true;
	} else
		return false;
}

void CoreApp::aboutToQuit() {}

void CoreApp::initiate()
{
	QCommandLineParser parser;
	bool allowInvalid = false;
	setupParser(parser, allowInvalid);

	if(parser.parse(QCoreApplication::arguments()) || allowInvalid) {
		if(startApp(parser)) {
			connect(qApp, &QCoreApplication::aboutToQuit,
					this, &CoreApp::aboutToQuit);
		} else
			qApp->exit(EXIT_FAILURE);
	} else {
		if(!CoreMessage::critical(tr("Invalid Arguments"), parser.errorText()))
			std::cerr << parser.errorText().toStdString() << std::endl;
	}
}
