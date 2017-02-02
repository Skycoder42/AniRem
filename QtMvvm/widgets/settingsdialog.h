#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "inputwidgetfactory.h"

#include <QDialog>
#include <QStyledItemDelegate>
#include <QFormLayout>
#include <functional>
#include <QAbstractButton>
#include <settingscontrol.h>
#include <QGroupBox>
#include <QLabel>
#include <QSignalMapper>
#include <QSet>

namespace Ui {
class SettingsDialog;
}

class CategoryItemDelegate : public QStyledItemDelegate
{
public:
	CategoryItemDelegate(std::function<void(int)> updateFunc,
						 const QSize &iconSize,
						 int layoutSpacing,
						 QObject *parent = nullptr);

	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
	QSize iconSize;
	std::function<void(int)> updateFunc;
};

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	Q_INVOKABLE SettingsDialog(Control *mControl, QWidget *parent = nullptr);
	~SettingsDialog();

private slots:
	void resetListSize();
	void updateWidth(int width);

	void propertyChanged();
	void saveValues();
	void restoreValues();

	void on_buttonBox_clicked(QAbstractButton *button);
	void on_filterLineEdit_textChanged(const QString &searchText);

private:
	SettingsControl *control;
	Ui::SettingsDialog *ui;
	CategoryItemDelegate *delegate;
	int maxWidthBase;

	typedef QPair<SettingsEntry, QMetaProperty> EntryInfo;
	QHash<QWidget*, EntryInfo> entryMap;
	QSet<QWidget*> changedEntries;

	int calcSpacing(Qt::Orientation orientation);

	void createUi(const SettingsSetup &setup);
	void createCategory(const SettingsCategory &category);
	void createSection(const SettingsSection &section, QTabWidget *tabWidget);
	void createGroup(const SettingsGroup &group, QWidget *contentWidget, QFormLayout *layout);
	void createEntry(const SettingsEntry &entry, QWidget *sectionWidget, QFormLayout *layout);

	void searchInDialog(const QRegularExpression &regex);
	bool searchInCategory(const QRegularExpression &regex, QTabWidget *tab);
	bool searchInSection(const QRegularExpression &regex, QWidget *contentWidget);
	bool searchInGroup(const QRegularExpression &regex, QGroupBox *groupWidget);
	bool searchInEntry(const QRegularExpression &regex, QLabel *label, QWidget *content);

	QIcon loadIcon(const QUrl &icon);
};

#endif // SETTINGSDIALOG_H
