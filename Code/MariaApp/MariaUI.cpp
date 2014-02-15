#include "MariaUI.h"


MariaUI::MariaUI(void)
{
	setupUI();
}


MariaUI::~MariaUI(void)
{
}

void MariaUI::setupUI()
{
	initWindowTitle();
	resize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	initTextBox();
} 
void MariaUI::initWindowTitle()
{
	if (this->objectName().isEmpty())
            this->setObjectName(QStringLiteral("MariaUI"));

        this->setWindowTitle(QApplication::translate("MariaUI", "M.A.R.I.A", 0));
}
void MariaUI::initTextBox()
{
	inputBox = new QLineEdit(this);
	inputBox->setGeometry(QRect(30, 30, 400, 30));
}