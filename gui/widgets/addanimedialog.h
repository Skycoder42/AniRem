#ifndef ADDANIMEDIALOG_H
#define ADDANIMEDIALOG_H

#include <QDialog>
#include <QMovie>
#include <animeinfo.h>
#include <addanimeviewmodel.h>

namespace Ui {
	class AddAnimeDialog;
}

class AddAnimeDialog : public QDialog
{
	Q_OBJECT

	Q_PROPERTY(int uiId READ uiId WRITE setUiId NOTIFY uiIdChanged)

public:
	Q_INVOKABLE AddAnimeDialog(QtMvvm::ViewModel *viewModel, QWidget *parent = nullptr);
	~AddAnimeDialog() override;

	void accept() override;

signals:
	void uiIdChanged();

private slots:
	void setUiId(int id);
	void loadingChanged(bool loading);

	void imageLoaded(int id, const QImage &image);
	void imageLoadFailed(int id, const QString &error);

private:
	enum PmState {
		PmCleared,
		PmLoading,
		PmLoaded,
		PmError
	};

	AddAnimeViewModel *_viewModel;
	Ui::AddAnimeDialog *_ui;

	QMovie *_loadingMovie;
	QPixmap _currentPixmap;
	PmState _pmState;

	void updatePm();

	int uiId() const;
};

#endif // ADDANIMEDIALOG_H
