#include "animeinfo.h"
#include "jsonserializer.h"

JsonSerializer::JsonSerializer(QObject *parent) :
	QJsonSerializer(parent)
{}

QJsonValue JsonSerializer::serializeVariant(int propertyType, const QVariant &value) const
{
	if(propertyType == qMetaTypeId<QMap<AnimeInfo::SeasonType,AnimeInfo::SeasonInfo>>()) {
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
	} else
		return QJsonSerializer::serializeVariant(propertyType, value);
}

QVariant JsonSerializer::deserializeVariant(int propertyType, const QJsonValue &value, QObject *parent) const
{
	if(propertyType == qMetaTypeId<QMap<AnimeInfo::SeasonType,AnimeInfo::SeasonInfo>>()) {
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
	} else
		return QJsonSerializer::deserializeVariant(propertyType, value, parent);
}
