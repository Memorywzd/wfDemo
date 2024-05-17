#pragma once

#include <QWidget>
#include <QJsonObject>
#include "ui_wfDemoRegister.h"

class wfDemoRegister : public QWidget
{
	Q_OBJECT

public:
	wfDemoRegister(QWidget *parent, QJsonObject json, 
				   QString sql, QString tableName);
	~wfDemoRegister();

	void registerTemp();

private:
	Ui::Register ui;

	QJsonObject json;
	QString sql;
	QString tableName;
};
