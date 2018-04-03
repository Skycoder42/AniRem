#ifndef GUIAPP_H
#define GUIAPP_H

#include <QtMvvmCore/CoreApp>

class AniRemApp : public QtMvvm::CoreApp
{
	Q_OBJECT

public:
	explicit AniRemApp(QObject *parent = nullptr);

protected:
	void performRegistrations() override;
	int startApp(const QStringList &arguments) override;
};

#undef coreApp
#define coreApp static_cast<guiApp*>(CoreApp::instance())

#endif // GUIAPP_H
