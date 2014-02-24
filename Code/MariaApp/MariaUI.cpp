#include "MariaUI.h"


MariaUI::MariaUI() 
{
	initWindowTitle();
	initTextBox();

	resize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	show();
}


MariaUI::~MariaUI(void)
{
	delete inputBox;
}

void MariaUI::initWindowTitle()
{
	if (objectName().isEmpty())
            setObjectName(QStringLiteral("MariaUI"));

    setWindowTitle(QApplication::translate("MariaUI", "M.A.R.I.A", 0));
}
void MariaUI::initTextBox()
{
	inputBox = new QLineEdit(this);
	inputBox->setGeometry(QRect(30, 30, 400, 30));
}