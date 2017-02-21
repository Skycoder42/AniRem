#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <QJsonSerializer>

class JsonSerializer : public QJsonSerializer
{
public:
	JsonSerializer(QObject *parent = nullptr);

protected:
	QJsonValue serializeVariant(int propertyType, const QVariant &value) const override;
	QVariant deserializeVariant(int propertyType, const QJsonValue &value, QObject *parent) const override;
};

#endif // JSONSERIALIZER_H
