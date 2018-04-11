#include "entrydialog.h"
#include "ui_entrydialog.h"

EntryDialog::EntryDialog(QtMvvm::ViewModel *viewModel, QWidget *parent) :
	QDialog(parent),
	_viewModel(static_cast<EntryViewModel*>(viewModel)),
	_ui(new Ui::EntryDialog)
{
	_ui->setupUi(this);

	_ui->treeView->setModel(_viewModel->model());
	connect(_viewModel, &EntryViewModel::modelChanged,
			this, [this](ProxerEntryModel *model){
		_ui->treeView->setModel(model);
	});
}

EntryDialog::~EntryDialog()
{
	delete _ui;
}
