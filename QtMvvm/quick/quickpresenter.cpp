#include "quickpresenter.h"
#include "settingsuibuilder.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QGuiApplication>
#include <quickextras.h>
#include <settingscontrol.h>

static QObject *createQuickPresenterQmlSingleton(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

QuickPresenter::QuickPresenter() :
	_singleton(nullptr)
{}

void QuickPresenter::registerViewExplicitly(const char *controlName, const QUrl &viewUrl)
{
	auto presenter = static_cast<QuickPresenter*>(CoreApp::instance()->presenter());
	if(!presenter) {
		presenter = new QuickPresenter();
		doRegister(presenter);
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
			return QUrl(QStringLiteral("qrc:///qml/views/") +
						searchDir.relativeFilePath(iterator.filePath()));
		}
	}

	qWarning() << "No QML-URL found for" << controlMetaObject->className();
	return QUrl();
}

bool QuickPresenter::tryPresentView(QObject *qmlPresenter, QObject *viewObject)
{
	auto meta = qmlPresenter->metaObject();
	auto index = -1;
	if(viewObject->inherits("QQuickItem"))
		index = meta->indexOfMethod("presentItem(QVariant)");
	if(viewObject->inherits("QQuickPopup"))
		index = meta->indexOfMethod("presentPopup(QVariant)");

	QVariant presented = false;
	if(index != -1) {
		meta->method(index).invoke(qmlPresenter,
								   Q_RETURN_ARG(QVariant, presented),
								   Q_ARG(QVariant, QVariant::fromValue(viewObject)));
	}
	return presented.toBool();
}

bool QuickPresenter::tryWithdrawView(QObject *qmlPresenter, QObject *viewObject)
{
	auto meta = qmlPresenter->metaObject();
	auto index = -1;
	if(viewObject->inherits("QQuickItem"))
		index = meta->indexOfMethod("withdrawItem(QVariant)");
	if(viewObject->inherits("QQuickPopup"))
		index = meta->indexOfMethod("withdrawPopup(QVariant)");

	QVariant withdrawen = false;
	if(index != -1) {
		meta->method(index).invoke(qmlPresenter,
								   Q_RETURN_ARG(QVariant, withdrawen),
								   Q_ARG(QVariant, QVariant::fromValue(viewObject)));
	}
	return withdrawen.toBool();
}

QUrl QuickPresenter::resolveInputType(int inputType)
{
	switch (inputType) {
	case QMetaType::QString:
		return QStringLiteral("qrc:/qtmvvm/qml/MsgTextField.qml");
	case QMetaType::Int:
		return QStringLiteral(":/qtmvvm/qml/MsgSpinBox.qml");
	case QMetaType::Double:
		return QStringLiteral("qrc:/qtmvvm/qml/MsgDblSpinBox.qml");
	default:
		return {};
	}
}

QObject *QuickPresenter::qmlPresenter() const
{
	if(_singleton)
		return _singleton->qmlPresenter();
	else
		return nullptr;
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

void QuickPresenter::doRegister(QuickPresenter *presenter)
{
	CoreApp::setMainPresenter(presenter);
	qmlRegisterSingletonType<QuickPresenterQmlSingleton>("com.skycoder42.qtmvvm", 1, 0, "QuickPresenter", createQuickPresenterQmlSingleton);
	qmlRegisterUncreatableType<MessageResult>("com.skycoder42.qtmvvm", 1, 0, "MessageResult", "This type can only be passed to QML from the presenter!");
	qmlRegisterUncreatableType<SettingsControl>("com.skycoder42.qtmvvm", 1, 0, "SettingsControl", "Controls cannot be created!");
	qmlRegisterType<SettingsUiBuilder>("com.skycoder42.qtmvvm", 1, 0, "SettingsUiBuilder");
	qmlProtectModule("com.skycoder42.qtmvvm", 1);
}

QQmlApplicationEngine *QuickPresenter::createEngine(const QUrl &file)
{
	QuickExtras::registerQmlSingleton();

	auto engine = new QQmlApplicationEngine();
	QObject::connect(qApp, &QGuiApplication::aboutToQuit,
					 engine, &QQmlApplicationEngine::deleteLater);
	QuickExtras::setupEngine(engine);
	if(file.isValid())
		engine->load(file);
	return engine;
}



QuickPresenterQmlSingleton::QuickPresenterQmlSingleton(QQmlEngine *engine, QObject *parent) :
	QObject(parent),
	_engine(engine),
	_presenter(static_cast<QuickPresenter*>(CoreApp::instance()->presenter())),
	_qmlPresenter(nullptr),
	_activeControls(),
	_latestComponent(nullptr),
	_loadCache(),
	_componentCache()
{
	_presenter->setQmlSingleton(this);
}

QObject *QuickPresenterQmlSingleton::qmlPresenter() const
{
	return _qmlPresenter;
}

void QuickPresenterQmlSingleton::present(Control *control)
{
	auto url = _presenter->findViewUrl(control->metaObject());
	if(!url.isValid())
		return;

	auto component = _componentCache.object(url);
	if(component)
		addObject(component, control);
	else {
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
}

void QuickPresenterQmlSingleton::withdraw(Control *control)
{
	auto item = _activeControls.value(control);
	if(item) {
		auto withdrawn = false;
		auto parent = _activeControls.value(control->parentControl());
		if(parent)
			withdrawn = _presenter->tryWithdrawView(parent, item);
		if(!withdrawn)
			withdrawn = _presenter->tryWithdrawView(_qmlPresenter, item);

		if(!withdrawn) {
			qCritical() << "Failed to withdraw view for control"
						<< control->objectName();
		}
	}
}

void QuickPresenterQmlSingleton::showMessage(MessageResult *result, CoreApp::MessageType type, const QString &title, const QString &text, const QString &positiveAction, const QString &negativeAction, const QString &neutralAction, int inputType)
{
	Q_ASSERT(_qmlPresenter);//TODO ugly
	QUrl inputUrl;
	if(type == CoreApp::Input) {
		inputUrl = _presenter->resolveInputType(inputType);
		if(!inputUrl.isValid()) {
			result->complete(MessageResult::NegativeResult, {});
			return;
		}
	}

	QMetaObject::invokeMethod(_qmlPresenter, "showMessage",
							  Q_ARG(QVariant, QVariant::fromValue(result)),
							  Q_ARG(QVariant, (MessageType)type),
							  Q_ARG(QVariant, title),
							  Q_ARG(QVariant, text),
							  Q_ARG(QVariant, positiveAction),
							  Q_ARG(QVariant, negativeAction),
							  Q_ARG(QVariant, neutralAction),
							  Q_ARG(QVariant, inputUrl));
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
		_componentCache.insert(component->url(), component);
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
	Q_ASSERT(_qmlPresenter);//TODO ugly
	auto item = component->create();
	if(!item) {
		qCritical() << "Unable to create quick view from the loaded component"
					<< component->url();
		return;
	}

	item->setProperty("control", QVariant::fromValue(control));

	auto presented = false;
	auto parent = _activeControls.value(control->parentControl());
	if(parent)
		presented = _presenter->tryPresentView(parent, item);
	if(!presented)
		presented = _presenter->tryPresentView(_qmlPresenter, item);

	if(presented) {
		_activeControls.insert(control, item);
		connect(item, &QObject::destroyed, this, [=](){
			_activeControls.remove(control);
			control->onClose();
		});
		control->onShow();
		QQmlEngine::setObjectOwnership(item, QQmlEngine::JavaScriptOwnership);
	} else {
		qCritical() << "Failed to present item!";
		item->deleteLater();
	}
}



static QObject *createQuickPresenterQmlSingleton(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
	Q_UNUSED(jsEngine)
	return new QuickPresenterQmlSingleton(qmlEngine);
}
