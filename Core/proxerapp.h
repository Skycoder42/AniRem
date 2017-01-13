#ifndef PROXERAPP_H
#define PROXERAPP_H

#include <coreapp.h>

class ProxerApp : public CoreApp
{
	Q_OBJECT

public:
	explicit ProxerApp(QObject *parent = nullptr);

protected:
	void setupParser(QCommandLineParser &parser, bool &allowInvalid) override;
	bool startApp(const QCommandLineParser &parser) override;

protected slots:
	void aboutToQuit() override;
};

#endif // PROXERAPP_H
