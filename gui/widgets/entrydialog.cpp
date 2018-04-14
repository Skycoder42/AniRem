#include "entrydialog.h"
#include "ui_entrydialog.h"
#include <QtMvvmCore/Binding>
#include <dialogmaster.h>

EntryDialog::EntryDialog(QtMvvm::ViewModel *viewModel, QWidget *parent) :
	QDialog(parent),
	_viewModel(static_cast<EntryViewModel*>(viewModel)),
	_ui(new Ui::EntryDialog)
{
	_ui->setupUi(this);
	DialogMaster::masterDialog(this);

	_ui->typeComboBox->addItems(_viewModel->listTypes());
	QtMvvm::bind(_viewModel, "typeIndex",
				 _ui->typeComboBox, "currentIndex");

	connect(_viewModel, &EntryViewModel::modelChanged,
			this, [this](ProxerEntryModel *model){
		_ui->treeView->setModel(model);

		_ui->treeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
		_ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
		_ui->treeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
		_ui->treeView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
	});
}

EntryDialog::~EntryDialog()
{
	delete _ui;
}

void EntryDialog::on_treeView_activated(const QModelIndex &index)
{
	if(index.isValid())
		_viewModel->addAnime(_viewModel->model()->getId(index));
}
