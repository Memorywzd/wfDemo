#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QMouseEvent>
#include <QScrollArea>

#include "verifier.h"

#include "ui_widget.h" // include organizationtree and uicontent 

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

	// oringinal workflow manager
    void CommitWorkflow();

    void onTempNameChanged(int index);
    void onFlowNameChanged(const QString& text);

public slots:
    void onVerifyRequest();
    void onVerifyFalse(QString errMsg);
    void onVerifySuccess();
    void onCreateNewWorkflow();

private:
    void setUpConfig();

	// original workflow manager
    Verifier* verifier{ nullptr };

    int tempName = 0;
    QString flowName;

private:
    Ui::Widget *ui;
};



#endif // WIDGET_H
