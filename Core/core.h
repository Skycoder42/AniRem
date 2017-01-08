#ifndef CORE_H
#define CORE_H

#include "core_global.h"

#include <QObject>

class QNetworkAccessManager;
class CORESHARED_EXPORT Core
{
public:
	static QNetworkAccessManager *createImageLoaderNam(QObject *parent = nullptr);
};

#endif // CORE_H
