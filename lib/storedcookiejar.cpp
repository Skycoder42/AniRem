#include "storedcookiejar.h"
#include <QDir>
#include <QNetworkCookie>
#include <QStandardPaths>

const QString StoredCookieJar::CookieKey = QStringLiteral("cookies");
const QString StoredCookieJar::CookieDataKey = QStringLiteral("raw");

StoredCookieJar::StoredCookieJar(QObject *parent) :
	QNetworkCookieJar(parent),
	_cookieStore(nullptr),
	_scheduled(false)
{
	QDir cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
	cacheDir.mkpath(QStringLiteral("."));
	_cookieStore = new QSettings(cacheDir.absoluteFilePath(QStringLiteral("api-cookie-jar.ini")),
								 QSettings::IniFormat,
								 this);
	auto cnt = _cookieStore->beginReadArray(CookieKey);
	QList<QNetworkCookie> cookies;
	cookies.reserve(cnt);
	for(auto i = 0; i < cnt; i++) {
		_cookieStore->setArrayIndex(i);
		cookies.append(QNetworkCookie::parseCookies(_cookieStore->value(CookieDataKey).toByteArray()));
	}
	_cookieStore->endArray();
	setAllCookies(cookies);
}

StoredCookieJar::~StoredCookieJar()
{
	if(_scheduled)
		store();
}

bool StoredCookieJar::insertCookie(const QNetworkCookie &cookie)
{
	auto ok = QNetworkCookieJar::insertCookie(cookie);
	if(ok && !_scheduled)
		_scheduled = QMetaObject::invokeMethod(this, "store", Qt::QueuedConnection);
	return ok;
}

bool StoredCookieJar::deleteCookie(const QNetworkCookie &cookie)
{
	auto ok = QNetworkCookieJar::deleteCookie(cookie);
	if(ok && !_scheduled)
		_scheduled = QMetaObject::invokeMethod(this, "store", Qt::QueuedConnection);
	return ok;
}

void StoredCookieJar::store()
{
	auto cookies = allCookies();
	_cookieStore->remove(CookieKey);
	_cookieStore->beginWriteArray(CookieKey, cookies.size());
	auto index = 0;
	for(auto cookie : cookies) {
		_cookieStore->setArrayIndex(index++);
		_cookieStore->setValue(CookieDataKey, cookie.toRawForm(QNetworkCookie::Full));
	}
	_cookieStore->endArray();
	_scheduled = false;
}
