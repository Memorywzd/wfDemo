#include "components.h"
#include <QInputDialog>

Components::Components(QWidget* parent) : QWidget(parent)
{}

void Components::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		bool ok;
		QString newText = QInputDialog::getText(parentWidget(),
			"修改描述文字", "请输入新的描述文字:", QLineEdit::Normal, getText(), &ok);
		if (ok && !newText.isEmpty())
			setText(newText);
	}
}

void Components::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		offset = event->globalPos() - frameGeometry().topLeft();
	}
}

void Components::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton) {
		QPoint newPos = event->globalPos() - offset;
		move(newPos);
		//qDebug() << "move: " << newPos;
	}
}

int TextForm::Type = 2;

TextForm::TextForm(QWidget* parent) : Components(parent)
{
	lineEdit = new QLineEdit(this);
	lineEdit->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

void TextForm::setText(const QString& text)
{
	lineEdit->setText(text);
}

QString TextForm::getText() const
{
	return lineEdit->text();
}

int AreaForm::Type = 3;

AreaForm::AreaForm(QWidget* parent) : Components(parent)
{
	textEdit = new QTextEdit(this);
	textEdit->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

void AreaForm::setText(const QString& text)
{
	textEdit->setText(text);
}

QString AreaForm::getText() const
{
	return textEdit->toPlainText();
}

int ButtonForm::Type = 4;

ButtonForm::ButtonForm(QWidget* parent) : Components(parent)
{
	button = new QPushButton("Button", this);
	button->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

void ButtonForm::setText(const QString& text)
{
	button->setText(text);
}

QString ButtonForm::getText() const
{
	return button->text();
}
