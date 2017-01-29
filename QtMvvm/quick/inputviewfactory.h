#ifndef INPUTVIEWFACTORY_H
#define INPUTVIEWFACTORY_H

#include <QUrl>

class InputViewFactory
{
public:
	virtual ~InputViewFactory();

	virtual int metaTypeId(const QByteArray &type);
	virtual QUrl getInput(const QByteArray &type);
	virtual QUrl getDelegate(const QByteArray &type);
};

#endif // INPUTVIEWFACTORY_H
