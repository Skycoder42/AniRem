#ifndef GUIAPP_H
#define GUIAPP_H

#include <QtMvvmCore/CoreApp>

class AniRemApp : public QtMvvm::CoreApp
{
	Q_OBJECT

public:
	explicit AniRemApp(QObject *parent = nullptr);

	void updateAutoStartState();
	void updateAutoStartState(int interval);

protected:
	void performRegistrations() override;
	int startApp(const QStringList &arguments) override;

private:
	bool setAutoStart(bool autoStart);

	void updateCheck();
};

#undef coreApp
#define coreApp static_cast<AniRemApp*>(QtMvvm::CoreApp::instance())

#endif // GUIAPP_H
