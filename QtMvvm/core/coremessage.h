#ifndef COREMESSAGE_H
#define COREMESSAGE_H

#include "qtmvvm_core_global.h"
#include "messageresult.h"
#include "coreapp.h"
#include <functional>

namespace CoreMessage
{

QTMVVM_CORE_SHARED_EXPORT MessageResult *message(CoreApp::MessageType type,
												 const QString &title,
												 const QString &text,
												 const QString &positiveAction = {},
												 const QString &negativeAction = {},
												 const QString &neutralAction = {},
												 int inputType = QMetaType::UnknownType);

QTMVVM_CORE_SHARED_EXPORT MessageResult *information(const QString &title,
													 const QString &text,
													 const QString &okText = {});
QTMVVM_CORE_SHARED_EXPORT bool information(const QString &title,
										   const QString &text,
										   std::function<void()> onResult,
										   const QString &okText = {});
QTMVVM_CORE_SHARED_EXPORT MessageResult *question(const QString &title,
												  const QString &text,
												  const QString &yesText = {},
												  const QString &noText = {});
QTMVVM_CORE_SHARED_EXPORT bool question(const QString &title,
										const QString &text,
										std::function<void(bool)> onResult,
										const QString &yesText = {},
										const QString &noText = {});
QTMVVM_CORE_SHARED_EXPORT MessageResult *warning(const QString &title,
												 const QString &text,
												 const QString &okText = {});
QTMVVM_CORE_SHARED_EXPORT bool warning(const QString &title,
									   const QString &text,
									   std::function<void()> onResult,
									   const QString &okText = {});
QTMVVM_CORE_SHARED_EXPORT MessageResult *critical(const QString &title,
												  const QString &text,
												  const QString &okText = {});
QTMVVM_CORE_SHARED_EXPORT bool critical(const QString &title,
										const QString &text,
										std::function<void()> onResult,
										const QString &okText = {});
QTMVVM_CORE_SHARED_EXPORT MessageResult *getInput(const QString &title,
												  const QString &text,
												  int inputType,
												  const QString &okText = {},
												  const QString &cancelText = {});
QTMVVM_CORE_SHARED_EXPORT bool getInput(const QString &title,
										const QString &text,
										int inputType,
										std::function<void(QVariant)> onResult,
										const QString &okText = {},
										const QString &cancelText = {});

}

#endif // COREMESSAGE_H
