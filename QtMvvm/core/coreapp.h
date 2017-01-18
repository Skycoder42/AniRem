#ifndef COREAPP_H
#define COREAPP_H

#include "qtmvvm_core_global.h"
#include <QCommandLineParser>
#include <QObject>
#include <QPointer>
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

	explicit CoreApp(QObject *parent = nullptr);
	~CoreApp();

	static CoreApp *instance();
	static void setMainPresenter(IPresenter *presenter);

	//internal use only!
	IPresenter *presenter() const;

public slots:
	void registerApp();

	void showControl(Control *control);
	void closeControl(Control *control);
	void showMessage(MessageResult *result,
					 CoreApp::MessageType type,
					 const QString &title,
					 const QString &text,
					 const QString &positiveAction = {},
					 const QString &negativeAction = {},
					 const QString &neutralAction = {},
					 int inputType = QMetaType::UnknownType);

protected:
	virtual void setupParser(QCommandLineParser &parser, bool &allowInvalid);
	virtual bool startApp(const QCommandLineParser &parser) = 0;

	bool autoShowHelpOrVersion(const QCommandLineParser &parser);

protected slots:
	virtual void aboutToQuit();

private slots:
	void initiate();

private:
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

#define REGISTER_CORE_APP(T) \
	static void _setup_ ## T ## _hook() { \
		auto app = new T(nullptr); \
		app->registerApp(); \
	} \
	Q_COREAPP_STARTUP_FUNCTION(_setup_ ## T ## _hook)

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
