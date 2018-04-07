#ifndef QMLTOAST_H
#define QMLTOAST_H

#include <QObject>

class QmlToast : public QObject
{
	Q_OBJECT

public:
	enum Duration {
		Short = 0x00000000,
		Long = 0x00000001
	};
	Q_ENUM(Duration)

	explicit QmlToast(QObject *parent = nullptr);

public slots:
	void toast(const QString &text, Duration duration = Short);
};

#endif // QMLTOAST_H
