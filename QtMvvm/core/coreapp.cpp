#include "coreapp.h"
#include <iostream>

QPointer<CoreApp> CoreApp::_instance;

CoreApp::CoreApp(QObject *parent) :
	QObject(parent),
	_presenter(nullptr)
{}

CoreApp *CoreApp::instance()
{
	return _instance;
}

void CoreApp::setMainPresenter(IPresenter *presenter)
{
	Q_ASSERT_X(_instance, Q_FUNC_INFO, "The presenter must be set after construction of the CoreApp/Qt App");
	_instance->_presenter.reset(presenter);
}

void CoreApp::registerApp()
{
	setParent(qApp);
	_instance = this;
	QMetaObject::invokeMethod(this, "initiate", Qt::QueuedConnection);
}

bool CoreApp::showControl(Control *control)
{
	return _presenter->present(control);
}

bool CoreApp::closeControl(Control *control)
{
	return _presenter->withdraw(control);
}

void CoreApp::setupParser(QCommandLineParser &parser, bool &allowInvalid)
{
	parser.addHelpOption();
	parser.addVersionOption();
	allowInvalid = true;
}

bool CoreApp::autoShowHelpOrVersion(const QCommandLineParser &parser)
{
	Q_UNUSED(parser);
	Q_UNIMPLEMENTED();
	//TODO do so...
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
		//TODO pass error to ui
		std::cerr << parser.errorText().toStdString() << std::endl;
	}
}
