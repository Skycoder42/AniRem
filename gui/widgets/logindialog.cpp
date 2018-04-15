#include "logindialog.h"
#include "ui_logindialog.h"
#include <QtMvvmCore/Binding>
#include <dialogmaster.h>

LoginDialog::LoginDialog(QtMvvm::ViewModel *viewModel, QWidget *parent) :
	QDialog(parent),
	_viewModel(static_cast<LoginViewModel*>(viewModel)),
	_ui(new Ui::LoginDialog)
{
	_ui->setupUi(this);
	DialogMaster::masterDialog(this, true);

	QtMvvm::bind(_viewModel, "userName",
				 _ui->usernameLineEdit, "text",
				 QtMvvm::Binding::TwoWay,
				 nullptr,
				 "editingFinished()");
	QtMvvm::bind(_viewModel, "password",
				 _ui->passwordLineEdit, "text",
				 QtMvvm::Binding::TwoWay,
				 nullptr,
				 "editingFinished()");
	QtMvvm::bind(_viewModel, "authCode",
				 _ui->faCodeLineEdit, "text",
				 QtMvvm::Binding::TwoWay,
				 nullptr,
				 "editingFinished()");
	QtMvvm::bind(_viewModel, "storePassword",
				 _ui->rememberPasswordCheckBox, "checked");

	connect(_viewModel, &LoginViewModel::loadingChanged,
			this, &LoginDialog::setDisabled);
	connect(_viewModel, &LoginViewModel::loginSuccessful,
			this, &LoginDialog::close);

	//FEATURE disable for now
	_ui->rememberPasswordCheckBox->hide();
}

LoginDialog::~LoginDialog()
{
	delete _ui;
}

void LoginDialog::accept()
{
	_viewModel->login();
}
