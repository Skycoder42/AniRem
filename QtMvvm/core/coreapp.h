#ifndef COREAPP_H
#define COREAPP_H

#include "qtmvvm_core_global.h"
#include <QCommandLineParser>
#include <QObject>
#include <QPointer>
#include <QSharedPointer>
#include <type_traits>

#include "control.h"
#include "messageresult.h"
class IPresenter;

class QTMVVM_CORE_SHARED_EXPORT CoreApp : public QObject
{
	Q_OBJECT

public:
	enum MessageType {
		Information,
		Question,
		Warning,
		Critical,
		Input
	};
	Q_ENUM(MessageType)

	struct QTMVVM_CORE_SHARED_EXPORT MessageConfig {
		CoreApp::MessageType type;
		QString title;
		QString text;
		QString positiveAction;
		QString negativeAction;
		QString neutralAction;
		QByteArray inputType;
		QVariant defaultValue;
		QVariantMap editProperties;
	};

	explicit CoreApp(QObject *parent = nullptr);
	~CoreApp();

	static CoreApp *instance();
	static void setMainPresenter(IPresenter *presenter);
	static void disableBoot();

	QSharedPointer<QCommandLineParser> getParser() const;

	//internal use only!
	IPresenter *presenter() const;

public slots:
	void registerApp();
	void bootApp();

	void showControl(Control *control);
	void closeControl(Control *control);
	void showMessage(MessageResult *result, const CoreApp::MessageConfig &config);

protected:
	virtual void setupParser(QCommandLineParser &parser, bool &allowInvalid) const;
	virtual bool startApp(const QCommandLineParser &parser) = 0;

	bool autoShowHelpOrVersion(const QCommandLineParser &parser);

protected slots:
	virtual void aboutToQuit();

private:
	static bool _bootEnabled;
	static QPointer<CoreApp> _instance;

	QScopedPointer<IPresenter> _presenter;
};

template <typename T>
class SimpleCoreApp : public CoreApp
{
	static_assert(std::is_base_of<Control, T>::value, "T must inherit Control!");
public:
	explicit SimpleCoreApp(QObject *parent = nullptr);

protected:
	bool startApp(const QCommandLineParser &parser) override;
};

Q_DECLARE_METATYPE(CoreApp::MessageConfig)

#define REGISTER_CORE_APP(T) \
	static void _setup_ ## T ## _hook() { \
		auto app = new T(nullptr); \
		app->registerApp(); \
	} \
	Q_COREAPP_STARTUP_FUNCTION(_setup_ ## T ## _hook)

#define coreApp CoreApp::instance()

// ------------- Generic Implementation -------------

template<typename T>
SimpleCoreApp<T>::SimpleCoreApp(QObject *parent) :
	CoreApp(parent)
{}

template<typename T>
bool SimpleCoreApp<T>::startApp(const QCommandLineParser &parser)
{
	if(!autoShowHelpOrVersion(parser)) {
		auto control = new T(this);
		return showControl(control);
	} else
		return true;
}

#endif // COREAPP_H
