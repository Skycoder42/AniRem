#ifndef CACHINGNAMFACTORY_H
#define CACHINGNAMFACTORY_H

#include <QQmlNetworkAccessManagerFactory>
#include <QNetworkAccessManager>

class CachingNamFactory : public QQmlNetworkAccessManagerFactory
{
public:
	CachingNamFactory();

	QNetworkAccessManager *create(QObject *parent) override;
};

#endif // CACHINGNAMFACTORY_H
