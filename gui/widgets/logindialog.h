#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <loginviewmodel.h>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
	Q_OBJECT

public:
	Q_INVOKABLE explicit LoginDialog(QtMvvm::ViewModel *viewModel, QWidget *parent = nullptr);
	~LoginDialog() override;

	void accept() override;

private:
	LoginViewModel *_viewModel;
	Ui::LoginDialog *_ui;
};

#endif // LOGINDIALOG_H
