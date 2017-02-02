#include "proxerstatus.h"

ProxerStatus::ProxerStatus(QObject *parent) :
	RestObject(parent),
	error(1),
	message(),
	code(0)
{}
