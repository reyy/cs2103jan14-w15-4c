#pragma once
#include <QtWidgets/QMainWindow>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QlineEdit>

class MariaUI :	public QMainWindow
{
public:
	MariaUI(void);
	~MariaUI(void);
	QLineEdit *inputBox;

    void setupUi()
    {
        if (this->objectName().isEmpty())
            this->setObjectName(QStringLiteral("MariaUI"));

        this->resize(480, 120);

		inputBox = new QLineEdit(this);
		inputBox->setGeometry(QRect(30, 30, 400, 30));

        retranslateUi();

        //QMetaObject::connectSlotsByName(this);
    } 

private:
    void retranslateUi()
    {
        this->setWindowTitle(QApplication::translate("MariaUI", "M.A.R.I.A", 0));
    } 

};

