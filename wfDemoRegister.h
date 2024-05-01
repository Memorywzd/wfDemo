#pragma once

#include <QWidget>
#include "ui_wfDemoRegister.h"

class wfDemoRegister : public QWidget
{
	Q_OBJECT

public:
	wfDemoRegister(QWidget *parent = nullptr);
	~wfDemoRegister();

private:
	Ui::Register ui;
};
