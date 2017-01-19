#ifndef QUICKPRESENTER_H
#define QUICKPRESENTER_H

#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <ipresenter.h>
class QuickPresenterQmlSingleton;

class QuickPresenter : public IPresenter
{
	friend class QuickPresenterQmlSingleton;

public:
	QuickPresenter();

	template <typename TPresenter = QuickPresenter>
	static void registerAsPresenter();
	template <typename TControl>
	static void registerViewExplicitly(const QUrl &viewUrl);
	static void registerViewExplicitly(const char *controlName, const QUrl &viewUrl);

	void present(Control *control) override;
	void withdraw(Control *control) override;
	void showMessage(MessageResult *result,
					 CoreApp::MessageType type,
					 const QString &title,
					 const QString &text,
					 const QString &positiveAction,
					 const QString &negativeAction,
					 const QString &neutralAction,
					 int inputType) override;

protected:
	virtual QUrl findViewUrl(const QMetaObject *controlMetaObject);
	virtual bool tryPresentView(QObject *qmlPresenter, QObject *viewObject);

private:
	QuickPresenterQmlSingleton *_singleton;
	QHash<QByteArray, QUrl> _explicitMappings;

	void setQmlSingleton(QuickPresenterQmlSingleton *singleton);
	static void doRegister(QuickPresenter *presenter);
};

class QuickPresenterQmlSingleton : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QObject* qmlPresenter MEMBER _qmlPresenter NOTIFY stackViewChanged)

	Q_PROPERTY(bool viewLoading READ isViewLoading NOTIFY viewLoadingChanged)
	Q_PROPERTY(qreal loadingProgress READ loadingProgress NOTIFY loadingProgressChanged)

public:
	QuickPresenterQmlSingleton(QQmlEngine *engine, QObject *parent = nullptr);

	void present(Control *control);
	void withdraw(Control *control);
	void showMessage(MessageResult *result,
					 CoreApp::MessageType type,
					 const QString &title,
					 const QString &text,
					 const QString &positiveAction,
					 const QString &negativeAction,
					 const QString &neutralAction,
					 int inputType);

	bool isViewLoading() const;
	qreal loadingProgress() const;

signals:
	void stackViewChanged(QObject *stackView);
	void viewLoadingChanged(bool viewLoading);
	void loadingProgressChanged(qreal loadingProgress);

private slots:
	void statusChanged(QQmlComponent::Status status);

private:
	QQmlEngine *_engine;
	QuickPresenter *_presenter;
	QObject *_qmlPresenter;

	QQmlComponent *_latestComponent;
	QHash<QQmlComponent*, Control*> _loadCache;

	void addObject(QQmlComponent *component, Control *control);
};

// ------------- Generic Implementation -------------

template<typename TPresenter>
void QuickPresenter::registerAsPresenter()
{
	doRegister(new TPresenter());
}

template<typename TControl>
void QuickPresenter::registerViewExplicitly(const QUrl &viewUrl)
{
	static_assert(std::is_base_of<Control, TControl>::value, "TControl must inherit Control!");
	registerViewExplicitly(TControl::staticMetaObject.className(), viewUrl);
}

#endif // QUICKPRESENTER_H
