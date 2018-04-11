#ifndef LOGINVIEWMODEL_H
#define LOGINVIEWMODEL_H

#include <QtMvvmCore/ViewModel>
#include <localsettings.h>
#include <userclass.h>

class LoginViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT

	Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
	Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
	Q_PROPERTY(QString authCode READ authCode WRITE setAuthCode NOTIFY authCodeChanged)
	Q_PROPERTY(bool storePassword READ storePassword WRITE setStorePassword NOTIFY storePasswordChanged)

	Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)

	QTMVVM_INJECT_PROP(LocalSettings*, settings, _settings)

public:
	Q_INVOKABLE explicit LoginViewModel(QObject *parent = nullptr);

	QString userName() const;
	QString password() const;
	QString authCode() const;
	bool storePassword() const;
	bool isLoading() const;

public slots:
	void setUserName(const QString &userName);
	void setPassword(const QString &password);
	void setAuthCode(const QString &authCode);
	void setStorePassword(bool storePassword);

	void login();

signals:
	void userNameChanged(const QString &userName);
	void passwordChanged(const QString &password);
	void authCodeChanged(const QString &authCode);
	void storePasswordChanged(bool storePassword);
	void loadingChanged(bool loading);

	void loginSuccessful();

protected:
	void onInit(const QVariantHash &params) override;

private slots:
	void apiError(const QString &errorString, int errorCode, QtRestClient::RestReply::ErrorType errorType);

private:
	LocalSettings *_settings;
	UserClass *_user;

	QString _userName;
	QString _password;
	QString _authCode;
	bool _storePassword;
	bool _loading;
};

#endif // LOGINVIEWMODEL_H
