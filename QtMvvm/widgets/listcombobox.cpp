#include "listcombobox.h"

#include <QDebug>

ListComboBox::ListComboBox(QWidget *parent) :
	QComboBox(parent)
{
	connect(this, &ListComboBox::currentTextChanged,
			this, &ListComboBox::currentValueChanged);
	connect(this, &ListComboBox::editTextChanged,
			this, &ListComboBox::currentValueChanged);
}

QVariant ListComboBox::currentValue() const
{
	if(currentText() != itemText(currentIndex()))
		return currentText();
	else
		return currentData();
}

void ListComboBox::setCurrentValue(const QVariant &data)
{
	auto index = findData(data);
	if(index != -1)
		setCurrentIndex(index);
	else
		setCurrentText(data.toString());
}
