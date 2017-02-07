#include "proxerstatus.h"

ProxerStatus::ProxerStatus(QObject *parent) :
	QObject(parent),
	error(1),
	message(),
	code(0)
{}
