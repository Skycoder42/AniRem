#ifndef LISTCOMBOBOX_H
#define LISTCOMBOBOX_H

#include <QComboBox>

class ListComboBox : public QComboBox
{
	Q_OBJECT

	Q_PROPERTY(QVariant currentValue READ currentValue WRITE setCurrentValue NOTIFY currentValueChanged USER true)

public:
	explicit ListComboBox(QWidget *parent = nullptr);

	QVariant currentValue() const;

public slots:
	void setCurrentValue(const QVariant &data);

signals:
	void currentValueChanged();
};

#endif // LISTCOMBOBOX_H
