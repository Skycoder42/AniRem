#ifndef COREMESSAGE_H
#define COREMESSAGE_H

#include "qtmvvm_core_global.h"
#include "messageresult.h"
#include "coreapp.h"
#include <QUrl>
#include <functional>

namespace CoreMessage
{

QTMVVM_CORE_SHARED_EXPORT MessageResult *message(const CoreApp::MessageConfig &config);

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

QTMVVM_CORE_SHARED_EXPORT bool about(const QString &content, bool includeCompany = true, const QUrl &companyUrl = QUrl(), bool includeQtVersion = true);

QTMVVM_CORE_SHARED_EXPORT MessageResult *getInput(const QString &title,
												  const QString &text,
												  const char *inputType,
												  const QVariant &defaultValue = {},
												  const QVariantMap &editProperties = {},
												  const QString &okText = {},
												  const QString &cancelText = {});
QTMVVM_CORE_SHARED_EXPORT MessageResult *getInput(const QString &title,
												  const QString &text,
												  int inputType,
												  const QVariant &defaultValue = {},
												  const QVariantMap &editProperties = {},
												  const QString &okText = {},
												  const QString &cancelText = {});
QTMVVM_CORE_SHARED_EXPORT bool getInput(const QString &title,
										const QString &text,
										const char *inputType,
										std::function<void(QVariant)> onResult,
										const QVariant &defaultValue = {},
										const QVariantMap &editProperties = {},
										const QString &okText = {},
										const QString &cancelText = {});
QTMVVM_CORE_SHARED_EXPORT bool getInput(const QString &title,
										const QString &text,
										int inputType,
										std::function<void(QVariant)> onResult,
										const QVariant &defaultValue = {},
										const QVariantMap &editProperties = {},
										const QString &okText = {},
										const QString &cancelText = {});

}

#endif // COREMESSAGE_H
