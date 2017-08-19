#include "animeinfo.h"
#include "jsonseasondataconverter.h"

#include <QJsonObject>

bool JsonSeasonDataConverter::canConvert(int metaTypeId) const
{
	return metaTypeId == qMetaTypeId<QMap<AnimeInfo::SeasonType,AnimeInfo::SeasonInfo>>();
}

QList<QJsonValue::Type> JsonSeasonDataConverter::jsonTypes() const
{
	return {QJsonValue::Object};
}

QJsonValue JsonSeasonDataConverter::serialize(int propertyType, const QVariant &value, const QJsonTypeConverter::SerializationHelper *helper) const
{
	Q_UNUSED(propertyType)
	Q_UNUSED(helper)

	auto v = value.value<QMap<AnimeInfo::SeasonType,AnimeInfo::SeasonInfo>>();
	QJsonObject o;
	auto metaEnum = QMetaEnum::fromType<AnimeInfo::SeasonType>();
	foreach(auto key, v.keys()) {
		QJsonObject pair;
		pair["first"] = v.value(key).first;
		pair["second"] = v.value(key).second;
		o.insert(metaEnum.valueToKey(key), pair);
	}
	return o;
}

QVariant JsonSeasonDataConverter::deserialize(int propertyType, const QJsonValue &value, QObject *parent, const QJsonTypeConverter::SerializationHelper *helper) const
{
	Q_UNUSED(propertyType)
	Q_UNUSED(parent)
	Q_UNUSED(helper)

	auto o = value.toObject();
	QMap<AnimeInfo::SeasonType,AnimeInfo::SeasonInfo> v;
	auto metaEnum = QMetaEnum::fromType<AnimeInfo::SeasonType>();
	foreach(auto key, o.keys()) {
		auto pair = o.value(key).toObject();
		AnimeInfo::SeasonInfo info;
		info.first = pair["first"].toInt();
		info.second = pair["second"].toBool();
		v.insert((AnimeInfo::SeasonType)metaEnum.keyToValue(key.toLatin1().constData()), info);
	}
	return QVariant::fromValue(v);
}
