#ifndef STOREDCOOKIEJAR_H
#define STOREDCOOKIEJAR_H

#include <QNetworkCookieJar>
#include <QSettings>

class StoredCookieJar : public QNetworkCookieJar
{
	Q_OBJECT

public:
	explicit StoredCookieJar(QObject *parent = nullptr);
	~StoredCookieJar();

	bool insertCookie(const QNetworkCookie &cookie) override;
	bool deleteCookie(const QNetworkCookie &cookie) override;

private slots:
	void store();

private:
	static const QString CookieKey;
	static const QString CookieDataKey;

	QSettings *_cookieStore;
	bool _scheduled;
};

#endif // STOREDCOOKIEJAR_H
