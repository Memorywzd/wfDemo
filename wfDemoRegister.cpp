#include "wfDemoRegister.h"

#include <QJsonDocument>
#include <QDebug>
#include <QMessageBox>

using namespace std;

wfDemoRegister::wfDemoRegister(QWidget *parent, QJsonObject json, QString sql, QString tableName)
	: QWidget(parent), json(json), sql(sql), tableName(tableName)
{
	ui.setupUi(this);

	connect(ui.pushButton, &QPushButton::clicked, this, &wfDemoRegister::registerTemp);
	connect(ui.pushButton_2, &QPushButton::clicked, this, &wfDemoRegister::close);

	ui.textBrowser->setText(sql);
}

wfDemoRegister::~wfDemoRegister()
{}

void wfDemoRegister::registerTemp()
{
	string registerSql;
	registerSql = "insert into tempRegister(Tname, type, json, writeTable, tableName, ";
	registerSql += "writeTableSql, sql1, sql2) values(";
	registerSql += "'" + ui.lineEdit->text().toStdString() + "', ";
	registerSql += ui.comboBox->currentIndex() == 0 ? "1, " : "0, ";
	registerSql += "'" + QJsonDocument(json).toJson(QJsonDocument::Compact).toStdString() + "', ";
	registerSql += ui.comboBox_2->currentIndex() == 0 ? "1" : "0";
	registerSql += ", '" + tableName.toStdString() + "', ";
	registerSql += "'" + ui.textBrowser->toPlainText().toStdString() + "', ";
	registerSql += "'" + ui.textEdit->toPlainText().toStdString() + "', ";
	registerSql += "'" + ui.textEdit_2->toPlainText().toStdString() + "')";

	QMessageBox::information(nullptr, "注册器", QString::fromStdString(registerSql));
}
