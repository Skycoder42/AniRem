#ifndef CORE_H
#define CORE_H

#include <QObject>

class QNetworkAccessManager;
class Core
{
public:
	static const QString ProxerRest;

	static QNetworkAccessManager *createImageLoaderNam(QObject *parent = nullptr);
};

#endif // CORE_H
