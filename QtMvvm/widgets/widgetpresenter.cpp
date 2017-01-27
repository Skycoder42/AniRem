#include "ipresentingwidget.h"
#include "settingsdialog.h"
#include "widgetpresenter.h"

#include <QDebug>
#include <QMainWindow>
#include <QDialog>
#include <QDockWidget>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QMessageBox>
#include <QInputDialog>
#include <QPushButton>
#include <QApplication>
#include <float.h>

WidgetPresenter::WidgetPresenter() :
	implicitMappings(),
	explicitMappings(),
	activeControls()
{}

void WidgetPresenter::registerWidget(const QMetaObject &widgetType)
{
	auto presenter = static_cast<WidgetPresenter*>(CoreApp::instance()->presenter());
	if(!presenter) {
		presenter = new WidgetPresenter();
		CoreApp::setMainPresenter(presenter);
	}
	presenter->implicitMappings.append(widgetType);
}

void WidgetPresenter::registerWidgetExplicitly(const char *controlName, const QMetaObject &widgetType)
{
	auto presenter = static_cast<WidgetPresenter*>(CoreApp::instance()->presenter());
	if(!presenter) {
		presenter = new WidgetPresenter();
		CoreApp::setMainPresenter(presenter);
	}
	presenter->explicitMappings.insert(controlName, widgetType);
}

void WidgetPresenter::present(Control *control)
{
	auto active = activeControls.value(control);
	if(active) {
		extendedShow(active);
		return;
	}

	auto ok = false;
	auto widgetMetaObject = findWidgetMetaObject(control->metaObject(), ok);
	if(ok) {
		auto parentControl = control->parentControl();
		auto parentWidget = parentControl ? activeControls.value(parentControl, nullptr) : nullptr;

		auto widget = qobject_cast<QWidget*>(widgetMetaObject.newInstance(Q_ARG(Control *, control),
																		  Q_ARG(QWidget*, parentWidget)));
		if(!widget) {
			qCritical() << "Failed to create widget of type"
						<< widgetMetaObject.className()
						<< "(did you mark the constructor as Q_INVOKABLE? Required signature: \"Contructor(Control *, QWidget*);\")";
			return;
		}

		auto presented = false;
		auto tPresenter = dynamic_cast<IPresentingWidget*>(parentWidget);
		if(tPresenter)
			presented = tPresenter->tryPresent(widget);
		if(!presented)
			presented = tryPresent(widget, parentWidget);

		if(presented) {
			activeControls.insert(control, widget);
			widget->setAttribute(Qt::WA_DeleteOnClose);
			QObject::connect(widget, &QWidget::destroyed, [=](){
				activeControls.remove(control);
				control->onClose();
			});
			control->onShow();
		} else
			widget->deleteLater();
	} else {
		qCritical() << "Unable to find widget for control of type"
					<< control->metaObject()->className();
	}
}

void WidgetPresenter::withdraw(Control *control)
{
	auto widget = activeControls.value(control);
	if(widget)
		widget->close();
}

void WidgetPresenter::showMessage(MessageResult *result, CoreApp::MessageType type, const QString &title, const QString &text, const QString &positiveAction, const QString &negativeAction, const QString &neutralAction, int inputType)
{
	QDialog *dialog = nullptr;
	if(type == CoreApp::Input)
		dialog = createInputDialog(title, text, inputType, positiveAction, negativeAction, neutralAction);
	else {
		auto msgBox = new QMessageBox();
		if(title.isEmpty())
			msgBox->setText(text);
		else {
			msgBox->setText(QStringLiteral("<b>%1</b>").arg(title));
			msgBox->setInformativeText(text);
		}

		QPushButton *aBtn = nullptr;
		QPushButton *rBtn = nullptr;
		QPushButton *nBtn = nullptr;
		if(!positiveAction.isNull())
			aBtn = msgBox->addButton(positiveAction, QMessageBox::AcceptRole);
		if(!negativeAction.isNull())
			rBtn = msgBox->addButton(negativeAction, QMessageBox::RejectRole);
		if(!neutralAction.isNull())
			nBtn = msgBox->addButton(neutralAction, QMessageBox::DestructiveRole);

		//default button
		if(aBtn)
			msgBox->setDefaultButton(aBtn);
		else if(rBtn)
			msgBox->setDefaultButton(rBtn);
		else if(nBtn)
			msgBox->setDefaultButton(nBtn);

		//escape button
		if(rBtn)
			msgBox->setEscapeButton(rBtn);
		else if(aBtn)
			msgBox->setEscapeButton(aBtn);
		else if(nBtn)
			msgBox->setEscapeButton(nBtn);

		switch (type) {
		case CoreApp::Information:
			msgBox->setIcon(QMessageBox::Information);
			msgBox->setWindowTitle("Information");
			break;
		case CoreApp::Question:
			msgBox->setIcon(QMessageBox::Question);
			msgBox->setWindowTitle(tr("Question"));
			break;
		case CoreApp::Warning:
			msgBox->setIcon(QMessageBox::Warning);
			msgBox->setWindowTitle(tr("Warning"));
			break;
		case CoreApp::Critical:
			msgBox->setIcon(QMessageBox::Critical);
			msgBox->setWindowTitle(tr("Error"));
			break;
		default:
			Q_UNREACHABLE();
			break;
		}

		dialog = msgBox;
	}

	if(dialog) {
		dialog->setAttribute(Qt::WA_DeleteOnClose);

		//"dialog master" stuff
		Qt::WindowFlags flags = Qt::MSWindowsFixedSizeDialogHint | Qt::WindowCloseButtonHint;
		dialog->setSizeGripEnabled(false);
		dialog->setModal(true);
		if(dialog->parentWidget()) {
			dialog->setWindowModality(Qt::WindowModal);
			flags |= Qt::Sheet;
		} else {
			dialog->setWindowModality(Qt::ApplicationModal);
			flags |= Qt::Dialog;
		}
		dialog->setWindowFlags(flags);

		QObject::connect(dialog, &QDialog::finished, dialog, [=](int diagRes){
			auto res = getResult(diagRes);
			QVariant value;
			if(type == CoreApp::Input && res == MessageResult::PositiveResult)
				value = extractInputResult(dialog);
			result->complete(res, value);
		});
		result->setCloseTarget(dialog, QDialog::staticMetaObject.method(QDialog::staticMetaObject.indexOfSlot("close()")));

		dialog->open();
	} else
		result->complete(MessageResult::NegativeResult, {});
}

QMetaObject WidgetPresenter::findWidgetMetaObject(const QMetaObject *controlMetaObject, bool &ok)
{
	ok = true;
	QByteArray cName = controlMetaObject->className();
	if(explicitMappings.contains(cName))
		return explicitMappings.value(cName);
	else {
		auto lIndex = cName.lastIndexOf("Control");
		if(lIndex > 0)
			cName.truncate(lIndex);
		foreach(auto metaObject, implicitMappings) {
			QByteArray wName = metaObject.className();
			if(wName.startsWith(cName))
				return metaObject;
		}
	}

	ok = false;
	return QMetaObject();
}

bool WidgetPresenter::tryPresent(QWidget *widget, QWidget *parent)
{
	auto metaObject = widget->metaObject();
	if(metaObject->inherits(&QDialog::staticMetaObject)) {
		qobject_cast<QDialog*>(widget)->open();
		return true;
	} else if(parent && parent->metaObject()->inherits(&QMainWindow::staticMetaObject)) {
		auto mainWindow = qobject_cast<QMainWindow*>(parent);
		if(metaObject->inherits(&QDockWidget::staticMetaObject)) {
			mainWindow->addDockWidget(Qt::LeftDockWidgetArea, qobject_cast<QDockWidget*>(widget));
			return true;
		} else if(metaObject->inherits(&QMdiSubWindow::staticMetaObject) &&
		   mainWindow->centralWidget() &&
		   mainWindow->centralWidget()->metaObject()->inherits(&QMdiArea::staticMetaObject)) {
			auto mdiArea = qobject_cast<QMdiArea*>(mainWindow->centralWidget());
			mdiArea->addSubWindow(widget);
			return true;
		}
	}

	extendedShow(widget);
	return true;
}

QDialog *WidgetPresenter::createInputDialog(const QString &title, const QString &text, int inputType, const QString &positiveText, const QString &negativeText, const QString &neutralText)
{
	Q_UNUSED(neutralText);

	auto dialog = new QInputDialog();
	switch (inputType) {
	case QMetaType::QString:
		dialog->setInputMode(QInputDialog::TextInput);
		break;
	case QMetaType::Int:
		dialog->setInputMode(QInputDialog::IntInput);
		dialog->setIntRange(INT_MIN, INT_MAX);
		break;
	case QMetaType::Double:
		dialog->setInputMode(QInputDialog::DoubleInput);
		dialog->setDoubleRange(DBL_MIN, DBL_MAX);
		break;
	default:
		return nullptr;
	}

	dialog->setWindowTitle(title);
	dialog->setLabelText(text);
	dialog->setOkButtonText(positiveText);
	dialog->setCancelButtonText(negativeText);

	return dialog;
}

QVariant WidgetPresenter::extractInputResult(QDialog *inputDialog)
{
	auto dialog = qobject_cast<QInputDialog*>(inputDialog);
	if(dialog) {
		switch (dialog->inputMode()) {
		case QInputDialog::TextInput:
			return dialog->textValue();
		case QInputDialog::IntInput:
			return dialog->intValue();
		case QInputDialog::DoubleInput:
			return dialog->doubleValue();
		default:
			Q_UNREACHABLE();
			break;
		}
	}

	return QVariant();
}

void WidgetPresenter::extendedShow(QWidget *widget) const
{
	widget->setWindowState(widget->windowState() & ~ Qt::WindowMinimized);
	widget->show();
	widget->raise();
	QApplication::alert(widget);
	widget->activateWindow();
}

MessageResult::ResultType WidgetPresenter::getResult(int dialogResult)
{
	switch (dialogResult) {
	case QDialog::Accepted:
		return MessageResult::PositiveResult;
	case QDialog::Rejected:
		return MessageResult::NegativeResult;
	default:
		return MessageResult::NeutralResult;
	}
}

void WidgetPresenter::registerDefaults()
{
	registerWidget<SettingsDialog>();
}
