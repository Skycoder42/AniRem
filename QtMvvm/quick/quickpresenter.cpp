#include "quickpresenter.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QGuiApplication>

static QObject *createQuickPresenterQmlSingleton(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

QuickPresenter::QuickPresenter() :
	_singleton(nullptr)
{}

void QuickPresenter::registerAsPresenter()
{
	CoreApp::setMainPresenter(new QuickPresenter());
	qmlRegisterSingletonType<QuickPresenterQmlSingleton>("com.skycoder42.qtmvvm", 1, 0, "QuickPresenter", createQuickPresenterQmlSingleton);
}

void QuickPresenter::registerViewExplicitly(const char *controlName, const QUrl &viewUrl)
{
	auto presenter = static_cast<QuickPresenter*>(CoreApp::instance()->presenter());
	if(!presenter) {
		presenter = new QuickPresenter();
		CoreApp::setMainPresenter(presenter);
		qmlRegisterSingletonType<QuickPresenterQmlSingleton>("com.skycoder42.qtmvvm", 1, 0, "QuickPresenter", createQuickPresenterQmlSingleton);
	}
	presenter->_explicitMappings.insert(controlName, viewUrl);
}

void QuickPresenter::present(Control *control)
{
	if(_singleton)
		_singleton->present(control);
}

void QuickPresenter::withdraw(Control *control)
{
	if(_singleton)
		_singleton->withdraw(control);
}

void QuickPresenter::showMessage(MessageResult *result, CoreApp::MessageType type, const QString &title, const QString &text, const QString &positiveAction, const QString &negativeAction, const QString &neutralAction, int inputType)
{
	if(_singleton)
		_singleton->showMessage(result, type, title, text, positiveAction, negativeAction, neutralAction, inputType);
}

QUrl QuickPresenter::findViewUrl(const QMetaObject *controlMetaObject)
{
	QByteArray cName = controlMetaObject->className();
	if(_explicitMappings.contains(cName))
		return _explicitMappings.value(cName);
	else {
		auto lIndex = cName.lastIndexOf("Control");
		if(lIndex > 0)
			cName.truncate(lIndex);

		QDir searchDir(QStringLiteral(":/qml/views"),
					   QStringLiteral("%1*.qml").arg(QString::fromLatin1(cName)),
					   QDir::NoSort,
					   QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
		QDirIterator iterator(searchDir, QDirIterator::Subdirectories);
		if(iterator.hasNext()) {
			iterator.next();
			return QUrl::fromLocalFile(iterator.filePath());
		}
	}

	return QUrl();
}

void QuickPresenter::setQmlSingleton(QuickPresenterQmlSingleton *singleton)
{
	Q_ASSERT_X(!_singleton, Q_FUNC_INFO, "rigth now, only a single qml engine is supported!");
	_singleton = singleton;

	QObject::connect(singleton, &QuickPresenterQmlSingleton::destroyed,
					 CoreApp::instance(), [=](){
		_singleton = nullptr;
	});
}



QuickPresenterQmlSingleton::QuickPresenterQmlSingleton(QQmlEngine *engine, QObject *parent) :
	QObject(parent),
	_engine(engine),
	_presenter(static_cast<QuickPresenter*>(CoreApp::instance()->presenter())),
	_stackView(nullptr),
	_latestComponent(nullptr),
	_loadCache()
{
	_presenter->setQmlSingleton(this);
}

void QuickPresenterQmlSingleton::present(Control *control)
{
	Q_ASSERT(_stackView);
	auto url = _presenter->findViewUrl(control->metaObject());
	if(!url.isValid())
		return;

	if(_latestComponent) {
		disconnect(_latestComponent, &QQmlComponent::progressChanged,
				   this, &QuickPresenterQmlSingleton::loadingProgressChanged);
	}

	_latestComponent = new QQmlComponent(_engine, this);
	_loadCache.insert(_latestComponent, control);

	//setup ui status
	emit viewLoadingChanged(true);
	emit loadingProgressChanged(0.0);
	connect(_latestComponent, &QQmlComponent::progressChanged,
			this, &QuickPresenterQmlSingleton::loadingProgressChanged);

	connect(_latestComponent, &QQmlComponent::statusChanged,
			this, &QuickPresenterQmlSingleton::statusChanged);
	_latestComponent->loadUrl(url, QQmlComponent::Asynchronous);
}

void QuickPresenterQmlSingleton::withdraw(Control *control)
{

}

void QuickPresenterQmlSingleton::showMessage(MessageResult *result, CoreApp::MessageType type, const QString &title, const QString &text, const QString &positiveAction, const QString &negativeAction, const QString &neutralAction, int inputType)
{

}

bool QuickPresenterQmlSingleton::isViewLoading() const
{
	return _latestComponent;
}

qreal QuickPresenterQmlSingleton::loadingProgress() const
{
	return _latestComponent ? _latestComponent->progress() : 0.0;
}

void QuickPresenterQmlSingleton::statusChanged(QQmlComponent::Status status)
{
	auto component = qobject_cast<QQmlComponent*>(sender());
	if(!component)
		return;

	switch(status) {
	case QQmlComponent::Ready:
		addObject(component, _loadCache.value(component));
		break;
	case QQmlComponent::Error:
		qCritical() << "Failed to create component"
					<< component->url()
					<< "with error:"
					<< component->errorString();
		break;
	default:
		return;
	}

	_loadCache.remove(component);
	if(_latestComponent == component) {
		_latestComponent = nullptr;
		emit viewLoadingChanged(false);
	}
}

void QuickPresenterQmlSingleton::addObject(QQmlComponent *component, Control *control)
{
	auto item = qobject_cast<QQuickItem*>(component->create());
	if(!item) {
		qCritical() << "Unable to create quick item from the loaded component"
					<< component->url();
		return;
	}

	item->setProperty("control", QVariant::fromValue(control));

	item->setParentItem(_stackView);
	QMetaObject::invokeMethod(_stackView, "itemPush", Q_ARG(QVariant, QVariant::fromValue(item)));
}



static QObject *createQuickPresenterQmlSingleton(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
	Q_UNUSED(jsEngine)
	return new QuickPresenterQmlSingleton(qmlEngine);
}
