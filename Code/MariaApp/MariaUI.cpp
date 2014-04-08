#include <QKeyEvent>
#include "MariaUI.h"
#include "MariaLogic.h"

const float MariaUI::FLOW_FACTOR = 0.1;
const float MariaUI::VALUE_THRESHOLD = 1.0;
const float MariaUI::CLOSE_BUTTON_X_OFFSET = 0.0;
const float MariaUI::CLOSE_BUTTON_Y_OFFSET = 0.0;
const float MariaUI::WINDOW_DEFAULT_COLOR_R = 186;
const float MariaUI::WINDOW_DEFAULT_COLOR_G = 199;
const float MariaUI::WINDOW_DEFAULT_COLOR_B = 22;

QPixmap *MariaUI::_taskTypeIconHandler[AMOUNT_OF_IMAGES]={};

MariaUI::MariaUI(MariaLogic *mariaLogic, QWidget *parent) : QMainWindow(parent) {
	_mariaLogic = mariaLogic;

	initWindow();
	initButtons();
	initBackgroundColor(WINDOW_DEFAULT_COLOR_R, WINDOW_DEFAULT_COLOR_G, WINDOW_DEFAULT_COLOR_B);
	_commandBar = new MariaUICommandBar(this);
	_commandBar->getTextbox()->setFocus();
	show();

	trayIcon = new QSystemTrayIcon(QIcon(QString::fromStdString("Resources/marialogo16x16.png")));
	trayIcon->setToolTip("M.A.R.I.A.");

	loadImages();

	QObject::connect(this, SIGNAL(triggerShowHideEvent()), this, SLOT(showHideEvent()));
}

MariaUI::~MariaUI() {
	unloadImages();
	delete trayIcon;
	delete _btClose;	
	delete _commandBar;
	delete _bkgColorUpdateTimer;
}

void MariaUI::initWindow() {
	_expandView = false;

	QFont newFont("Century Gothic");
	newFont.setStyleStrategy(QFont::PreferAntialias);
	QApplication::setFont(newFont);

	if (objectName().isEmpty()) {
		setObjectName(QStringLiteral("MariaUI"));
	}
	setWindowFlags( Qt::FramelessWindowHint );
	setWindowTitle(QApplication::translate("MariaUI", "M.A.R.I.A", 0));

	resize(WINDOW_DEFAULT_SIZE_X, WINDOW_DEFAULT_SIZE_Y);
}

void MariaUI::initButtons() {
	QPixmap imageHandleCloseButton("./Resources/ui_bt_close.png");
	_btClose = new QToolButton(this);
	_btClose->setAutoFillBackground(true);;
	_btClose->setIcon(QIcon(imageHandleCloseButton));
	_btClose->setIconSize(QSize(imageHandleCloseButton.width(), imageHandleCloseButton.height()));
	_btClose->setFixedSize(QSize(imageHandleCloseButton.width(), imageHandleCloseButton.height()));
	_btClose->setToolTip("Close Program");
	_btClose->setStyleSheet("border:0px;");
	_btClose->setGeometry(QRect(width()-imageHandleCloseButton.width() + CLOSE_BUTTON_X_OFFSET, CLOSE_BUTTON_Y_OFFSET, imageHandleCloseButton.width(), imageHandleCloseButton.height()));	

	connect(_btClose, SIGNAL(clicked()), this , SLOT(quitAction()));
}

void MariaUI::initBackgroundColor(int r, int g, int b) {
	_bkgColor.setRed(r);
	_bkgColor.setGreen(g);
	_bkgColor.setBlue(b);
	_targetBkgColor.setRed(r);
	_targetBkgColor.setGreen(g);
	_targetBkgColor.setBlue(b);

	_bkgColorUpdateTimer = new QTimer(this);
	connect(_bkgColorUpdateTimer, SIGNAL(timeout()), this, SLOT(updateBackgroundColor()));
	updateBackgroundColor();
}

void MariaUI::loadImages() {	
	_taskTypeIconHandler[0] = new QPixmap("./Resources/ui_task_type_floating.png");
	_taskTypeIconHandler[1] = new QPixmap("./Resources/ui_task_type_deadline.png");
	_taskTypeIconHandler[2] = new QPixmap("./Resources/ui_task_type_timed.png");
	_taskTypeIconHandler[3] = new QPixmap("./Resources/ui_completed.png");
	_taskTypeIconHandler[4] = new QPixmap("./Resources/ui_not_completed.png");
	_taskTypeIconHandler[5] = new QPixmap("./Resources/ui_maria_logo_1.png");
	_taskTypeIconHandler[6] = new QPixmap("./Resources/ui_maria_logo_2.png");
	_taskTypeIconHandler[7] = new QPixmap("./Resources/ui_maria_logo_3.png");
	_taskTypeIconHandler[8] = new QPixmap("./Resources/ui_maria_logo_4.png");
	_taskTypeIconHandler[9] = new QPixmap("./Resources/ui_maria_logo_5.png");
	_taskTypeIconHandler[10] = new QPixmap("./Resources/ui_maria_logo_6.png");
	_taskTypeIconHandler[11] = new QPixmap("./Resources/ui_loadingdots.png");
}

void MariaUI::unloadImages() {
	for(int i = 0 ; i < AMOUNT_OF_IMAGES ; i++ ) {
		delete _taskTypeIconHandler[i];
	}
}

void MariaUI::quitAction() {
	_mariaLogic->terminateProgram();
}

void MariaUI::updateBackgroundColor() {

	if(abs(_bkgColor.red()-_targetBkgColor.red())>VALUE_THRESHOLD ||
		abs(_bkgColor.green()-_targetBkgColor.green())>VALUE_THRESHOLD ||
		abs(_bkgColor.blue()-_targetBkgColor.blue())>VALUE_THRESHOLD) {
			_bkgColor.setRed(_bkgColor.red() + (_targetBkgColor.red()-_bkgColor.red())*FLOW_FACTOR);
			_bkgColor.setGreen(_bkgColor.green() + (_targetBkgColor.green()-_bkgColor.green())*FLOW_FACTOR);
			_bkgColor.setBlue(_bkgColor.blue() + (_targetBkgColor.blue()-_bkgColor.blue())*FLOW_FACTOR);
	} else {
		if(_bkgColorUpdateTimer->isActive()) {
			_bkgColorUpdateTimer->stop();
		}
	}
	QString backgroundcolor = QString::number(_bkgColor.red()) + ", " + QString::number(_bkgColor.green()) + ", " + QString::number(_bkgColor.blue());
	this->setStyleSheet("QMainWindow {background-color: rgb(" + backgroundcolor + ");min-width:400px;min-height:120px;}");
}

void MariaUI::showHideEvent() {
	if(isVisible()) {
		setWindowState(Qt::WindowState::WindowMinimized);
		trayIcon->show();
		trayIcon->showMessage("M.A.R.I.A. is still running!", "Press Ctrl + Space to show M.A.R.I.A.\nType \'exit\' to quit the program.");
		hide();
	} else {
		setWindowState(Qt::WindowState::WindowActive);
		trayIcon->hide();
		show();
	}
}

void MariaUI::resizeEvent(QResizeEvent* event) {
	QWidget::resizeEvent(event);
}

bool MariaUI::eventFilter(QObject* obj, QEvent *event) {
	if (obj == _commandBar->getTextbox()->getInputBoxReference()) {
		if (event->type() == QEvent::KeyPress) {
			QKeyEvent* keyPressed = static_cast<QKeyEvent*>(event);
			if(keyPressed->key() == Qt::Key_Backspace) {
				//-1 is required because is checks the length BEFORE backspace is applied to QLineEdit.
				if(_commandBar->getTextbox()->getUserInput().length()-1 == 0) {
					_commandBar->getStatus()->setStatus(MariaUIStatus::NONE);
				}
			} else {
				_commandBar->getTextbox()->setSuggestText("");
			}
		}
	}
	return QMainWindow::eventFilter(obj, event);
}

void MariaUI::keyReleaseEvent(QKeyEvent* event) {
	int keyPressed = event->key();
	if(event->key() == Qt::Key_Z && event->modifiers().testFlag(Qt::ControlModifier)) {
		if(_mariaLogic->processUndo()) {
			getCommandBar()->getTextbox()->setQuestionText("Undo was successful");
		} else {
			getCommandBar()->getTextbox()->setQuestionText("Nothing to Undo.");
		}
	} else if(keyPressed == Qt::Key_Return || keyPressed == Qt::Key_Enter) {
		//_mariaLogic->processCommand(_commandBar->getTextbox()->getUserInput());
		_mariaLogic->processCommand_New(_commandBar->getTextbox()->getUserInput());
	} else if(keyPressed == Qt::Key_Up) {
		_mariaLogic->processCommand_New("up");
	} else if(keyPressed == Qt::Key_Down) {
		_mariaLogic->processCommand_New("down");
	} else {
		if(_mariaLogic->checkValidCommand(_commandBar->getTextbox()->getUserInput())) {
			_commandBar->getStatus()->setStatus(MariaUIStatus::OK);
		} else {
			if(_commandBar->getTextbox()->getUserInput().length() > 0) {
				_commandBar->getStatus()->setStatus(MariaUIStatus::WAIT);
			} else {
				_commandBar->getStatus()->setStatus(MariaUIStatus::NONE);
			}

		}
		event->ignore();
	}
}

void MariaUI::forceLogicCall() {
	//Overwrite for UI classes to initiate a call to logic instead of through keyboards.
	_mariaLogic->generateTextforUI();
}

void MariaUI::setExpand(bool value) {
	_expandView = value;

	if(_expandView) {
		resize(WINDOW_DEFAULT_EXPAND_SIZE_X, WINDOW_DEFAULT_EXPAND_SIZE_Y);
	} else {
		resize(WINDOW_DEFAULT_SIZE_X, WINDOW_DEFAULT_SIZE_Y);
	}
}

bool MariaUI::getExpand() {
	return _expandView;
}

void MariaUI::setBackgroundColor(int r, int g, int b) {
	_targetBkgColor.setRed(r);
	_targetBkgColor.setGreen(g);
	_targetBkgColor.setBlue(b);

	if(!_bkgColorUpdateTimer->isActive()) {
		_bkgColorUpdateTimer->start(50);
	}
}

QPixmap* MariaUI::getImageHandler(int index) {
	return _taskTypeIconHandler[index];
}

MariaUICommandBar * MariaUI::getCommandBar() {
	return _commandBar;
}
