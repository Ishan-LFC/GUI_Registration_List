#ifndef REGISTRATIONGUI_H
#define REGISTRATIONGUI_H

#include <QWidget>

class QGridLayout;
class QTabWidget;
class QTableView;
class QAbstractTableModel;
class QHeaderView;
class RegistrationTableModel;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpacerItem;
class QComboBox;
class Person;
class Registration;
class StudentRegistration;
class GuestRegistration;
class RegistrationList;
class RegistrationListWriter;
class RegistrationListWriter;
class RegistrationFactory;
class QCloseEvent;

class RegistrationGui : public QWidget
{
    Q_OBJECT

public:
    RegistrationGui(QWidget *parent = nullptr);
    ~RegistrationGui();

private slots:
    void addRegistration();
    void isRegistered();
    void totalFee();
    void totalRegistrations();
    void inputsChanged();
    void registrationTypeChanged();
    void saveRegistrationListToFile() const;
    void openRegistrationListFromFile();

private:
    void setUpGui();
    void closeEvent(QCloseEvent *event);

    QLineEdit *addRegistrationNameInput;
    QLineEdit *affiliationInput;
    QLineEdit *emailInput;
    QComboBox *addRegistrationTypeInput;
    QLineEdit *qualificationInput;
    QLineEdit *categoryInput;
    RegistrationList *registrationList;
    RegistrationTableModel *tableModel;
    QLineEdit *isRegisteredNameInput;
    QComboBox *totalsTypeInput;
    QPushButton *addRegistrationButton;
    QPushButton *isRegisteredButton;
    QPushButton *totalFeeButton;
    QPushButton *totalRegistrationsButton;
    QPushButton *saveXmlFileButton;
    QPushButton *openXmlFileButton;
    RegistrationFactory *factorySingleton;
};
#endif // REGISTRATIONGUI_H
