#ifndef ENTRYDIALOG_H
#define ENTRYDIALOG_H

#include <QDialog>
#include <entryviewmodel.h>

namespace Ui {
class EntryDialog;
}

class EntryDialog : public QDialog
{
	Q_OBJECT

public:
	Q_INVOKABLE explicit EntryDialog(QtMvvm::ViewModel *viewModel, QWidget *parent = nullptr);
	~EntryDialog() override;

private slots:
	void on_treeView_activated(const QModelIndex &index);

private:
	EntryViewModel *_viewModel;
	Ui::EntryDialog *_ui;
};

#endif // ENTRYDIALOG_H
