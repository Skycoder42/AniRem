#ifndef INPUTVIEWFACTORY_H
#define INPUTVIEWFACTORY_H

#include <QUrl>
#include <QVariantMap>

class InputViewFactory
{
public:
	virtual ~InputViewFactory();

	virtual int metaTypeId(const QByteArray &type, const QVariantMap &properties);
	virtual QUrl getInput(const QByteArray &type, const QVariantMap &properties);
	virtual QUrl getDelegate(const QByteArray &type, const QVariantMap &properties);
};

#endif // INPUTVIEWFACTORY_H
