#include "VkApiMethods.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>

using namespace VKMM;

void VkApiMethod::setFinished(bool isOk)
{
	emit finished(isOk);
}

VkUser VkApiMethod::parseUser(const QJsonObject &value)
{
	VkUser out;
	out.id = value["uid"].toInt();
	out.name = value["first_name"].toString() + " " + value["last_name"].toString();
	out.photo = QUrl(value["photo_100"].toString());
	return out;
}

VkAudioPtr VkApiMethod::parseAudio(const QJsonObject &value)
{
	VkAudioPtr out(new VkAudio());
	out->id = value["aid"].toInt();
	out->uid = value["owner_id"].toInt();
	out->title = value["title"].toString();
	out->artist = value["artist"].toString();
	out->duration = value["duration"].toInt();
	out->album = value["album_id"].toInt();
	out->url = QUrl(value["url"].toString());
	return out;
}

// -- USER

QUrlQuery VkApiGetUser::getQuery()
{
	QUrlQuery q;
	q.addQueryItem("user_ids", QString::number(uid));
	q.addQueryItem("fields", "uid,first_name,last_name,photo_100");
	return q;
}

bool VkApiGetUser::process(QJsonValueRef reply)
{
	QJsonArray arr = reply.toArray();
	if (arr.size() < 1) return false;
	if (!arr.first().isObject()) return false;
	user = VkApiMethod::parseUser(arr.first().toObject());
	return true;
}

// -- AUDIO

QUrlQuery VKMM::VkApiGetAudio::getQuery()
{
	QUrlQuery q;
	q.addQueryItem("owner_id", QString::number(uid));
	q.addQueryItem("offset", QString::number(offset));
	q.addQueryItem("count", QString::number(count));
	return q;
}

bool VkApiGetAudio::process(QJsonValueRef reply)
{
	QJsonArray dataArray = reply.toArray();
	audios.clear();

	// Total count
	countTotal = dataArray.first().toInt();
	dataArray.pop_front();

	for (QJsonValueRef v : dataArray)
	{
		audios.push_back(VkApiMethod::parseAudio(v.toObject()));
	}
	return true;
}
