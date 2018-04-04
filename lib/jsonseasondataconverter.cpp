#include "animeinfo.h"
#include "jsonseasondataconverter.h"

#include <QJsonObject>

bool JsonSeasonDataConverter::canConvert(int metaTypeId) const
{
	return metaTypeId == qMetaTypeId<AnimeInfo::SeasonMap>();
}

QList<QJsonValue::Type> JsonSeasonDataConverter::jsonTypes() const
{
	return {QJsonValue::Object};
}

QJsonValue JsonSeasonDataConverter::serialize(int propertyType, const QVariant &value, const QJsonTypeConverter::SerializationHelper *helper) const
{
	Q_UNUSED(propertyType)
	Q_UNUSED(helper)

	auto v = value.value<AnimeInfo::SeasonMap>();
	QJsonObject o;
	auto metaEnum = QMetaEnum::fromType<AnimeInfo::SeasonType>();
	foreach(auto key, v.keys()) {
		QJsonObject pair;
		pair[QStringLiteral("count")] = v.value(key).first;
		pair[QStringLiteral("changed")] = v.value(key).second;
		o.insert(QString::fromUtf8(metaEnum.valueToKey(key)), pair);
	}
	return o;
}

QVariant JsonSeasonDataConverter::deserialize(int propertyType, const QJsonValue &value, QObject *parent, const QJsonTypeConverter::SerializationHelper *helper) const
{
	Q_UNUSED(propertyType)
	Q_UNUSED(parent)
	Q_UNUSED(helper)

	auto o = value.toObject();
	AnimeInfo::SeasonMap v;
	auto metaEnum = QMetaEnum::fromType<AnimeInfo::SeasonType>();
	foreach(auto key, o.keys()) {
		auto pair = o.value(key).toObject();
		AnimeInfo::SeasonInfo info;
		info.first = pair[QStringLiteral("count")].toInt();
		info.second = pair[QStringLiteral("changed")].toBool();
		v.insert(static_cast<AnimeInfo::SeasonType>(metaEnum.keyToValue(key.toLatin1().constData())), info);
	}
	return QVariant::fromValue(v);
}
