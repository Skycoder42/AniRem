#include "loginviewmodel.h"
#include <QtMvvmCore/Messages>
#include <apihelper.h>
#include <proxerapi.h>

LoginViewModel::LoginViewModel(QObject *parent) :
	ViewModel(parent),
	_settings(nullptr),
	_user(ProxerApi::factory().user().instance(this)),
	_userName(),
	_password(),
	_authCode(),
	_storePassword(false)
{
	connect(_user, &UserClass::apiError,
			this, &LoginViewModel::apiError);
}

QString LoginViewModel::userName() const
{
	return _userName;
}

QString LoginViewModel::password() const
{
	return _password;
}

QString LoginViewModel::authCode() const
{
	return _authCode;
}

bool LoginViewModel::storePassword() const
{
	return _storePassword;
}

bool LoginViewModel::isLoading() const
{
	return _loading;
}

void LoginViewModel::setUserName(const QString &userName)
{
	if (_userName == userName)
		return;

	_userName = userName;
	emit userNameChanged(_userName);
}

void LoginViewModel::setPassword(const QString &password)
{
	if (_password == password)
		return;

	_password = password;
	emit passwordChanged(_password);
}

void LoginViewModel::setAuthCode(const QString &authCode)
{
	if (_authCode == authCode)
		return;

	_authCode = authCode;
	emit authCodeChanged(_authCode);
}

void LoginViewModel::setStorePassword(bool storePassword)
{
	if (_storePassword == storePassword)
		return;

	_storePassword = storePassword;
	emit storePasswordChanged(_storePassword);
}

void LoginViewModel::login()
{
	_loading = true;
	emit loadingChanged(_loading);

	QUrlQuery params;
	params.addQueryItem(QStringLiteral("username"), _userName);
	params.addQueryItem(QStringLiteral("password"), _password);
	if(!_authCode.isEmpty())
		params.addQueryItem(QStringLiteral("secretkey"), _authCode);
	auto nmReply = _user->restClass()->builder()
		.addPath(QStringLiteral("login"))
		.setBody(params.query().toUtf8(), "application/x-www-form-urlencoded")
		.setVerb(QtRestClient::RestClass::PostVerb)
		.send();

	auto reply = new QtRestClient::GenericRestReply<ProxerStatus, ProxerStatus> {
		nmReply,
		_user->restClient(),
		this
	};
	reply->onSucceeded([this](int code, ProxerStatus status){
		if(ApiHelper::testValid(code, status) ||
		   code == 3012) { // user already logged in
			_settings->account.username = _userName;
			_settings->account.storedPw = _storePassword;
			if(_storePassword) {
				//TODO store password to keystore
			} else {
				//TODO remove password from keystore
			}

			emit resultReady(true);
			emit loginSuccessful();
		 } else
			apiError(status.message(), status.code(), QtRestClient::RestReply::FailureError);
	});
	reply->onAllErrors([this](QString e, int c, QtRestClient::RestReply::ErrorType t) {
		apiError(e, c, t);
	});
}

void LoginViewModel::onInit(const QVariantHash &params)
{
	Q_UNUSED(params);
	setUserName(_settings->account.username);
	setStorePassword(_settings->account.storedPw);

	if(_storePassword) {
		//TODO load password from keystore
	}
}

void LoginViewModel::apiError(const QString &errorString, int errorCode, QtRestClient::RestReply::ErrorType errorType)
{
	QtMvvm::critical(tr("Login failed"),
					 ApiHelper::formatError(errorString, errorCode, errorType));

	_loading = false;
	emit loadingChanged(_loading);
}
