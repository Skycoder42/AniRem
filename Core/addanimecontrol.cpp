#include "addanimecontrol.h"
#include <ProxerApi/infoclass.h>
using namespace QtRestClient;

AddAnimeControl::AddAnimeControl(QObject *parent) :
	Control(parent),
	infoClass(new InfoClass(this)),
	_id(-1),
	_title(),
	_loading(false),
	_acceptable(false)
{
	connect(infoClass, &InfoClass::apiError, this, [this](QString error){
		setLoading(false);
		emit loadError(error);
	});
}

int AddAnimeControl::id() const
{
	return _id;
}

QString AddAnimeControl::title() const
{
	return _title;
}

bool AddAnimeControl::isLoading() const
{
	return _loading;
}

bool AddAnimeControl::isAcceptable() const
{
	return _acceptable;
}

void AddAnimeControl::accept(bool allowInvalid)
{
	if((allowInvalid || _acceptable) && _id != -1) {
		if(_title.isEmpty())
			setTitle(tr("Anime %1").arg(_id));
		auto info = new AnimeInfo(_id, _title, this);
		emit completed(info);
	}
}

void AddAnimeControl::retry()
{
	auto mId = _id;
	_id = -1;
	setId(mId);
}

void AddAnimeControl::setId(int id)
{
	if (_id == id)
		return;

	_id = id;
	emit idChanged(id);
	setTitle(QString());
	setLoading(true);

	infoClass->getEntry(_id)->onSucceeded([this](RestReply*, int code, ProxerEntry *entry){
		if(infoClass->testValid(code, entry)) {
			if(entry->data->id == _id) {
				setTitle(entry->data->name);//TODO choose name from combolist
				setLoading(false);
				setAcceptable(true);
			}
		}
		entry->deleteLater();
	});
}

void AddAnimeControl::setTitle(QString title)
{
	if (_title == title)
		return;

	_title = title;
	emit titleChanged(title);
}

void AddAnimeControl::setLoading(bool loading)
{
	if (_loading == loading)
		return;
	_loading = loading;
	emit loadingChanged(loading);
	if(loading)
		setAcceptable(false);
}

void AddAnimeControl::setAcceptable(bool acceptable)
{
	if (_loading == acceptable)
		return;
	_acceptable = acceptable;
	emit acceptableChanged(acceptable);
}
