#include "passwordmanager.h"
#include "ui_passwordmanager.h"
#include "animatedqlineedit.h"
#include "animatedqpushbutton.h"
#include "accountwidgetabstractlabel.h"
#include "accountwidget.h"
#include "copydata.h"

#include <QDebug>
#include <QApplication>
#include <QToolTip>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QCommonStyle>
#include <QTimer>
#include <QStatusBar>
#include <QFileDialog>
#include <QStandardPaths>

PasswordManager::PasswordManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PasswordManager),
    m_controller(&m_model),
    m_editing(nullptr),
    m_hideAnimationDuration(200)
{
    ui->setupUi(this);
    if(!m_model.m_accountManager.load(QApplication::applicationDirPath() + "/data/data.cfg")){
        throw std::logic_error("Unable to open config file.");
    }
    setFixedSize(size()); /// to prevent resizing
    createDesigns();
    createCustomContextMenu();

    ui->loginInfo->setText(ui->loginInfo->text().arg(MINIMUM_LOGIN_LENGTH));
    ui->passwordInfo->setText(ui->passwordInfo->text().arg(MINIMUM_PASSWORD_LENGTH));

    ui->versionInfo->setText(ui->versionInfo->text().arg(m_model.m_version));
    ui->versionInfo->adjustSize();
    ui->releaseDateInfo->setText(ui->releaseDateInfo->text().arg(m_model.m_releaseDate));
    ui->releaseDateInfo->adjustSize();
    ui->authorInfo->setText(ui->authorInfo->text().arg(m_model.m_author));
    ui->authorInfo->adjustSize();

    connect(ui->registerPasswordEdit, &AnimatedQLineEdit::returnPressed, this, &PasswordManager::on_createAccountButton_clicked);
    connect(ui->registerLoginEdit, &AnimatedQLineEdit::returnPressed, this, &PasswordManager::on_createAccountButton_clicked);

    connect(ui->loginEdit, &AnimatedQLineEdit::returnPressed, this, &PasswordManager::on_loginButton_clicked);
    connect(ui->passwordEdit, &AnimatedQLineEdit::returnPressed, this, &PasswordManager::on_loginButton_clicked);

    connect(ui->loginLineEdit, &AnimatedQLineEdit::returnPressed, this, &PasswordManager::on_addAccountButton_clicked);
    connect(ui->passwordLineEdit, &AnimatedQLineEdit::returnPressed, this, &PasswordManager::on_addAccountButton_clicked);
    connect(ui->forLineEdit, &AnimatedQLineEdit::returnPressed, this, &PasswordManager::on_addAccountButton_clicked);

    ui->scrollArea->verticalScrollBar()->setStyle(new QCommonStyle);

    m_hideAnimation.addAnimation(new QPropertyAnimation(this, "minimumSize"));
    m_hideAnimation.addAnimation(new QPropertyAnimation(this, "maximumSize"));

    connect(&m_hideAnimation, &QParallelAnimationGroup::finished, this, &PasswordManager::hideAnimationFinished);

    setWindowIcon(QIcon(":/key.png"));
}

PasswordManager::~PasswordManager()
{
    m_model.m_userManager.encrypt();
    delete ui;
}

void PasswordManager::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    setFocus();
}

void PasswordManager::createDesigns()
{
    {
        PushButtonDesign design;
        design.m_defaultColor = QColor(100, 100, 100, 255);
        design.m_hoverColor = QColor(110, 110, 110, 255);
        design.m_textColor = QColor(0, 0, 0, 255);
        design.m_pressedColor = QColor(115, 115, 115, 255);
        design.m_disabledColor = QColor(80, 80, 80, 255);
        design.m_focusedColor = QColor(115, 115, 115, 255);
        design.m_focusedText = QColor(255, 255, 255, 255);
        design.m_checkedColor = QColor(155, 155, 155, 255);
        design.m_delay = 300;
        m_buttonManager.addDesign(design);
        m_buttonManager.setCurrent(0);
        for(auto& itr : findChildren<AnimatedQPushButton*>()){
            itr->setManager(&m_buttonManager);
            itr->updateVisually();
            itr->setFocusPolicy(Qt::TabFocus);
            itr->setDefault(true);
        }
    }

    {
        LineEditDesign design;
        design.m_enablePressedColor = true;
        design.m_defaultColor = QColor(100, 100, 100, 255);
        design.m_hoverColor = QColor(110, 110, 110, 255);
        design.m_selectionColor = QColor(90, 90, 90, 255);
        design.m_pressedColor = QColor(150, 150, 150, 255);
        design.m_pressedBorderColor = QColor(70, 70, 70, 255);
        design.m_textColor = QColor(0, 0, 0, 255);
        design.m_disabledColor = QColor(90, 90, 90, 255);
        design.m_goodColor = QColor(0, 255, 0, 128);
        design.m_wrongColor = QColor(255, 0, 0, 128);
        design.m_delay = 500;
        m_lineEditManager.addDesign(design);
        m_lineEditManager.setCurrent(0);
        for(auto& itr : findChildren<AnimatedQLineEdit*>()){
            itr->setManager(&m_lineEditManager);
            itr->updateVisually();
            itr->setContextMenuPolicy(Qt::NoContextMenu);
        }
    }

    {
        AccountWidgetDesign design;
        design.m_defaultColor = QColor(0, 0, 0, 25);
        design.m_hoverColor = QColor(0, 0, 0, 35);
        design.m_childDefaultColor = QColor(255, 255, 255, 65);
        design.m_childHoverColor =  QColor(255, 255, 255, 95);
        design.m_delay = 300;

        m_accWidgetManager.addDesign(design);
        m_accWidgetManager.setCurrent(0);
    }
}

void PasswordManager::on_moreButton_clicked()
{
    animateToMorePage();
}

void PasswordManager::on_backButton_clicked()
{
    animateToLoginPage();
}

void PasswordManager::on_aboutButton_clicked()
{
    animateToAboutPage();
}

void PasswordManager::on_registerButton_clicked()
{
    toggleRegisterWidgets();
}

void PasswordManager::on_addButton_clicked()
{
    m_controller.on_addButton_clicked();
    animateToAddPage();
}

void PasswordManager::on_createAccountButton_clicked()
{
    bool success = m_model.createAccount(ui->registerLoginEdit->text().toLower(), ui->registerPasswordEdit->text());

    Account acc;
    acc.m_login = ui->registerLoginEdit->text();
    acc.m_password = ui->registerPasswordEdit->text();

    if(m_model.registerLoginState() == State::Good){
        if(m_model.m_accountManager.exists(acc)){
            m_model.setLoginState(State::Wrong);
            success = false;
            ui->loginInfo->setText(tr("Account with that login already exists"));
        } else{
            ui->loginInfo->setText(tr("Your login is good"));
        }
    } else {
        ui->loginInfo->setText(tr("At least %1 characters").arg(MINIMUM_LOGIN_LENGTH));
    }

    ui->registerLoginEdit->setState(m_model.registerLoginState());
    ui->registerPasswordEdit->setState(m_model.registerPasswordState());
    if(success){
        m_model.m_accountManager.registerAccount(acc);
        animateToLoginPage();
    }
}

void PasswordManager::on_logoutButton_clicked()
{
    m_model.m_userManager.encrypt();
    m_model.m_userManager.save();
    m_model.m_userManager.unload();
    m_model.m_userManager.setUser("");
    ui->scrollArea->widget()->deleteLater();
    animateToLoginPage();
}

void PasswordManager::on_backToMorePageButton_clicked()
{
    animateToMorePage();
}

void PasswordManager::on_loginButton_clicked()
{
    if(m_model.m_accountManager.size()){
        Account acc;
        acc.m_login = ui->loginEdit->text();
        acc.m_password = ui->passwordEdit->text();

        bool success = m_model.login(acc);

        ui->loginEdit->setState(m_model.loginLoginState());
        ui->passwordEdit->setState(m_model.loginPasswordState());

        if(success){
            m_controller.login();
            loginWithUser(acc);
        }
    }
}

void PasswordManager::on_addAccountButton_clicked()
{
    UserAccount acc;
    acc.m_for = ui->forLineEdit->text();
    acc.m_login = ui->loginLineEdit->text();
    acc.m_password = ui->passwordLineEdit->text();
    bool success = m_model.addUserAccount(acc);

    ui->forLineEdit->setState(m_model.addForLineState());
    ui->loginLineEdit->setState(m_model.addLoginLineState());
    ui->passwordLineEdit->setState(m_model.addPasswordLineState());

    if(success){
        if(m_model.addPageState() == AddPageState::Edit){
            m_model.m_userManager.replace(idOfWidget(m_editing), acc);
            m_editing->initialize(acc);
        } else{
            addNewAccount(acc);
        }
        animateToUserPage();
    }
}

void PasswordManager::on_backFromExtrasButton_clicked()
{
    animateToUserPage();
}

void PasswordManager::on_backToUserPageButton_clicked()
{
    animateToUserPage();
}

void PasswordManager::on_extrasButton_clicked()
{
    animateToExtrasPage();
}

void PasswordManager::setMorePage()
{
    m_model.setMorePage();
    if(m_model.updateMorePage()){
        ui->registerButton->setChecked(m_model.isRegisterPageEnabled());
        enableRegisterPage(m_model.isRegisterPageEnabled());
    }
    ui->mainStackedWidget->setCurrentWidget(ui->morePage);
    ui->morePage->setFocus();
}

void PasswordManager::setLoginPage()
{
    m_model.setLoginPage();
    if(m_model.updateLoginPage()){
        ui->loginEdit->clear();
        ui->passwordEdit->clear();
        ui->loginEdit->setState(m_model.loginLoginState(), true);
        ui->passwordEdit->setState(m_model.loginPasswordState(), true);
    }
    ui->mainStackedWidget->setCurrentWidget(ui->loginPage);
    ui->loginEdit->setFocus();
}

void PasswordManager::setExtrasPage()
{
    m_model.setExtrasPage();
    ui->mainStackedWidget->setCurrentWidget(ui->extrasPage);
    ui->extrasPage->setFocus();
}

void PasswordManager::setAddPage()
{
    m_model.setAddPage();
    if(m_model.updateAddPage()){
        if(m_model.addPageState() == AddPageState::Add){
            ui->forLineEdit->clear();
            ui->loginLineEdit->clear();
            ui->passwordLineEdit->clear();
            ui->addAccountButton->setText(tr("Add"));
        } else if(m_model.addPageState() == AddPageState::Edit){
            ui->forLineEdit->setText(m_editing->title());
            ui->loginLineEdit->setText(m_editing->login());
            ui->passwordLineEdit->setText(m_editing->password());
            ui->addAccountButton->setText(tr("Apply"));
        }

        ui->passwordLineEdit->setState(m_model.addPasswordLineState(), true);
        ui->loginLineEdit->setState(m_model.addLoginLineState(), true);
        ui->forLineEdit->setState(m_model.addForLineState(), true);
    }
    ui->mainStackedWidget->setCurrentWidget(ui->addPage);
    ui->forLineEdit->setFocus();
}

void PasswordManager::setAboutPage()
{
    m_model.setAboutPage();
    ui->mainStackedWidget->setCurrentWidget(ui->aboutPage);
    ui->aboutPage->setFocus();
}

void PasswordManager::setUserPage()
{
    m_model.setUserPage();
    ui->mainStackedWidget->setCurrentWidget(ui->mainPage);
    ui->mainPage->setFocus();
}

void PasswordManager::animateToLoginPage()
{
    hideAnimation();
    m_hideAnimationFinish = [this]() { setLoginPage(); extendAnimation(ui->loginPage->baseSize()); };
}

void PasswordManager::animateToAddPage()
{
    hideAnimation();
    m_hideAnimationFinish = [this]() { setAddPage(); extendAnimation(ui->addPage->baseSize()); };
}

void PasswordManager::animateToExtrasPage()
{
    hideAnimation();
    m_hideAnimationFinish = [this]() { setExtrasPage(); extendAnimation(ui->extrasPage->baseSize()); };
}

void PasswordManager::animateToAboutPage()
{
    hideAnimation();
    m_hideAnimationFinish = [this]() { setAboutPage(); extendAnimation(ui->aboutPage->baseSize()); };
}

void PasswordManager::animateToUserPage()
{
    hideAnimation();
    m_hideAnimationFinish = [this]() { setUserPage(); extendAnimation(ui->mainPage->baseSize()); };
}

void PasswordManager::animateToMorePage()
{
    hideAnimation();
    m_hideAnimationFinish = [this]() { setMorePage(); extendAnimation(ui->morePage->baseSize()); };
}

void PasswordManager::hideAnimation()
{
    for(int i = 0;i<m_hideAnimation.animationCount(); ++i){
        QPropertyAnimation * an = static_cast<QPropertyAnimation*>(m_hideAnimation.animationAt(i));
        an->setStartValue(size());
        an->setEndValue(QSize(width(), 1));
        an->setDuration(m_hideAnimationDuration);
    }
    m_hideAnimation.start();
}

void PasswordManager::extendAnimation(const QSize &l_size)
{
    for(int i = 0;i<m_hideAnimation.animationCount(); ++i){
        QPropertyAnimation * an = static_cast<QPropertyAnimation*>(m_hideAnimation.animationAt(i));
        an->setStartValue(size());
        an->setEndValue(l_size);
        an->setDuration(m_hideAnimationDuration * 1.5);
    }
    m_hideAnimation.start();
}

void PasswordManager::resetRegisterPage(const bool& b)
{
    m_model.resetRegisterPage();
    enableRegisterPage(b);
    ui->registerLoginEdit->clear();
    ui->registerPasswordEdit->clear();
    ui->registerLoginEdit->setState(m_model.registerLoginState(), true);
    ui->registerPasswordEdit->setState(m_model.registerPasswordState(), true);
}

void PasswordManager::enableRegisterPage(const bool &b)
{
    ui->registerLoginEdit->setEnabled(b);
    ui->registerPasswordEdit->setEnabled(b);
    ui->createAccountButton->setEnabled(b);
}

void PasswordManager::loginWithUser(const Account &l_acc)
{
    if(!m_model.m_userManager.load(QApplication::applicationDirPath() + QString("/data/%1/data.pm").arg(l_acc.m_login))){
        QMessageBox::critical(this, tr("Error"), tr("Unable to load user data") + '(' + l_acc.m_login + ").", QMessageBox::Ok);
        return;
    }
    m_model.m_userManager.decrypt();
    m_model.m_userManager.setUser(l_acc.m_login);

    ui->scrollArea->setWidget(createScrollAreaWidget());
    for(auto& itr : m_model.m_userManager.container()){
        addNewAccount(*itr.second, true, itr.first);
    }
    animateToUserPage();
}

void PasswordManager::addNewAccount(const UserAccount &l_acc, const bool& l_onlyVisually, int l_id)
{
    if(!l_onlyVisually){
        l_id = m_model.m_userManager.addData(l_acc);
    }
    if(l_id == -1){
        QMessageBox::critical(this, tr("Error"), tr("Error when adding data: ") + l_acc.m_for, QMessageBox::Ok);
        return;
    }
    AccountWidget* tmp = new AccountWidget(ui->scrollArea, l_acc, &m_accWidgetManager);
    connect(tmp->titleWidget(), &AccountWidgetTitle::customContextMenuRequested, this, &PasswordManager::showContextMenu);
    ui->scrollArea->widget()->layout()->addWidget(tmp);
    m_widgets.emplace(l_id, tmp);
}

QWidget* PasswordManager::createScrollAreaWidget()
{
    QWidget* widget = new QWidget;
    QVBoxLayout* lay = new QVBoxLayout;
    lay->setAlignment(Qt::AlignTop);
    lay->setContentsMargins(5, 5, 5, 5);
    lay->setSpacing(5);
    widget->setLayout(lay);
    return widget;
}

void PasswordManager::createCustomContextMenu()
{
    m_customContextMenu.setStyleSheet("QMenu{ background: #3a3838; color: white; border: 1px solid black; }"
                                      "QMenu::item{ background-color: transparent; border: 1px solid transparent; padding: 3px 20px 3px 20px; }"
                                      "QMenu::item:disabled{ color: #a0a0a0; }"
                                      "QMenu::item:selected{ border-color: white; background: #565454; }");

    QAction* editAction = new QAction(tr("Edit"), &m_customContextMenu);
    connect(editAction, &QAction::triggered, this, &PasswordManager::editWidgetActionTriggered);
    m_customContextMenu.addAction(editAction);

    QAction* removeAction = new QAction(tr("Remove"), &m_customContextMenu);
    connect(removeAction, &QAction::triggered, this, &PasswordManager::removeWidgetActionTriggered);
    m_customContextMenu.addAction(removeAction);
}

void PasswordManager::removeWidgetActionTriggered()
{
    removeWidget(m_editing);
}

void PasswordManager::editWidgetActionTriggered()
{
    m_controller.editWidget();
    animateToAddPage();
}

void PasswordManager::removeWidget(AccountWidget* l_widget)
{
    int id = idOfWidget(l_widget);
    if(id == -1){
        QMessageBox::critical(this, tr("Error"), tr("Incorrect id of widget"), QMessageBox::Ok);
        return;
    }
    m_model.m_userManager.remove(id);
    ui->scrollArea->widget()->layout()->removeWidget(l_widget);
    m_widgets.erase(id);
    delete l_widget;
    l_widget = nullptr;
}

int PasswordManager::idOfWidget(AccountWidget *l_widget)
{
    for(auto& itr : m_widgets){
        if(itr.second == l_widget){
            return itr.first;
        }
    }
    return -1;
}

void PasswordManager::toggleRegisterWidgets()
{
    bool enabled = m_controller.toggleRegisterPage();

    if(enabled){ /// show
        extendAnimation(QSize(ui->morePage->baseSize().width(), ui->morePage->baseSize().height() + 130));
        resetRegisterPage(enabled);
        ui->loginInfo->setText(tr("At least %1 characters").arg(MINIMUM_LOGIN_LENGTH));
    } else { /// hide
        extendAnimation(ui->morePage->baseSize());
        enableRegisterPage(enabled);
    }
}

void PasswordManager::hideAnimationFinished()
{
    m_hideAnimationFinish();
    m_hideAnimationFinish = [](){};
    if(m_model.isResizingEnabled()){
        setMinimumSize(0, 0);
        setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    }
}

void PasswordManager::on_searchEdit_textChanged(const QString &arg1)
{
    for(auto& itr : m_widgets){
        if(itr.second->title().contains(arg1, Qt::CaseInsensitive) || arg1.isEmpty()){
            itr.second->setVisible(true);
        } else{
            itr.second->setVisible(false);
        }
    }
}

void PasswordManager::showContextMenu(const QPoint &l_point)
{
    for(auto& itr : m_widgets) {
        if(itr.second->underMouse()){
            m_editing = itr.second;
            break;
        }
    }
    m_customContextMenu.exec(m_editing->mapToGlobal(l_point));
}

void PasswordManager::on_exportButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Create file"), QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(),
                                               QStandardPaths::LocateDirectory).append(QString('/').append(m_model.m_userManager.user())),
                                               QString("Password Manager (*.pmn)"));
    if(path.isEmpty()){
        return;
    }

    CopyData cpy(&m_model);
    if(!cpy.Export(path)){
        QMessageBox::critical(this, tr("Error"), tr("An error occured when exporting data"), QMessageBox::Ok);
        return;
    }
}

void PasswordManager::on_importButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select file"), QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(),
                                               QStandardPaths::LocateDirectory).append(QString('/').append(m_model.m_userManager.user())),
                                               QString("Password Manager (*.pmn)"));
    if(path.isEmpty()){
        return;
    }

    CopyData cpy(&m_model);
    ImportError er = cpy.Import(path);
    if(er == ImportError::IncorrectFileType){
        QMessageBox::critical(this, tr("Error"), tr("Incorrect file type"), QMessageBox::Ok);
        return;
    } else if(er == ImportError::AccountExist){
        QMessageBox::critical(this, tr("Error"), tr("Account with that login already exists"), QMessageBox::Ok);
        return;
    } else{
        QMessageBox::information(this, tr("Success"), tr("Account successfully imported"), QMessageBox::Ok);
    }
}

void PasswordManager::on_deleteAccountButton_clicked()
{
    if(QMessageBox::warning(this, tr("Warning"), tr("Are you sure?\nThis action cannot be undone"), QMessageBox::Ok | QMessageBox::Cancel)
            == QMessageBox::Ok){
        m_model.m_accountManager.removeAccount(m_model.m_userManager.user());
        if(!m_model.m_userManager.removeUser()){
            QMessageBox::critical(this, tr("Error"), tr("Error when removing user directory"), QMessageBox::Ok);
        }
        ui->scrollArea->widget()->deleteLater();
        animateToLoginPage();
    }
}
