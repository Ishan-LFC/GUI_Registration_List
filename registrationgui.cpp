#include "registrationgui.h"
#include "registrationtablemodel.h"
#include "registrationlist.h"
#include "studentregistration.h"
#include "guestregistration.h"
#include "registrationlistwriter.h"
#include "registrationlistreader.h"
#include "registrationfactory.h"

#include <QGridLayout>
#include <QTabWidget>
#include <QTableView>
#include <QAbstractTableModel>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QComboBox>
#include <QMessageBox>
#include <QCloseEvent>

const double Registration::STANDARD_FEE=100; //initialsing standard fee

RegistrationGui::RegistrationGui(QWidget *parent)
    : QWidget(parent),
      addRegistrationNameInput(new QLineEdit(parent)),
      affiliationInput(new QLineEdit(parent)),
      emailInput(new QLineEdit(parent)),
      addRegistrationTypeInput(new QComboBox(parent)),
      qualificationInput(new QLineEdit(parent)),
      categoryInput(new QLineEdit(parent)),
      registrationList(new RegistrationList),
      tableModel(new RegistrationTableModel(parent)),
      isRegisteredNameInput(new QLineEdit(parent)),
      totalsTypeInput(new QComboBox(parent)),
      addRegistrationButton(new QPushButton("&Book Reservation",parent)),
      isRegisteredButton(new QPushButton("&Check",parent)),
      totalFeeButton(new QPushButton("Calculate &Fees",this)),
      totalRegistrationsButton(new QPushButton("Count &Registrations",this)),
      saveXmlFileButton(new QPushButton("&Save XML file",this)),
      openXmlFileButton(new QPushButton("&Open XML file",this)),
      factorySingleton(RegistrationFactory::getInstance())
{
    setUpGui();
}

RegistrationGui::~RegistrationGui()
{
    delete factorySingleton;
}

void RegistrationGui::addRegistration()
{
//---creating the person variable
    Person attendee = Person(addRegistrationNameInput->text(), affiliationInput->text(), emailInput->text());
    QString additionalInfo = qualificationInput -> text().isEmpty() ? categoryInput -> text() : qualificationInput -> text();
    Registration *registration = factorySingleton->createRegistration(addRegistrationTypeInput -> currentText(), attendee, additionalInfo);

    if(registrationList  -> addRegistration(registration))
    {
        tableModel -> addRegistration(registration);

        QMessageBox warning;

        warning.setText("Registration Added succesfully");
        warning.setWindowTitle("Registration Added");
        warning.setStandardButtons(QMessageBox::Ok);
        warning.setDefaultButton(QMessageBox::Ok);
        warning.exec();
    }
    else
    {
        QMessageBox warning;

        warning.setText("This email is already registered with the same name.");
        warning.setInformativeText("User already Exists!");
        warning.setWindowTitle("Error!");
        warning.setStandardButtons(QMessageBox::Ok);
        warning.setDefaultButton(QMessageBox::Ok);
        warning.exec();
    }

    //clear inputs on the gui
    addRegistrationNameInput -> clear();
    affiliationInput         -> clear();
    emailInput               -> clear();
    qualificationInput       -> clear();
    categoryInput            -> clear();
    addRegistrationTypeInput -> setCurrentIndex(0);
}

//find whether the person by name is already registered
void RegistrationGui::isRegistered()
{
    QMessageBox info;

    if(registrationList ->isRegistered(isRegisteredNameInput -> text()))
    {
        info.setText("The person is registered for the conference");
        info.setInformativeText(isRegisteredNameInput -> text() + "was found in the registration list");
        info.setWindowTitle("Name found");
    }
    else {
        info.setText("The person is not registered for the conference");
        info.setInformativeText(isRegisteredNameInput -> text() + "was not found in the registration list/n" +
                                "You can register them in the registration tab");
        info.setWindowTitle("Name not found");
    }

    info.setStandardButtons(QMessageBox::Ok);
    info.setDefaultButton(QMessageBox::Ok);
    info.exec();

    isRegisteredNameInput -> clear(); //clear the name input
}

//calculate total fees for the given registration type
void RegistrationGui::totalFee()
{
    QMessageBox info;
    QString type = "All";

    if(totalsTypeInput -> currentText() == "Standard") type = "Registration";
    else if(totalsTypeInput -> currentText() != "All") type = totalsTypeInput -> currentText() + "Registration";

    info.setText("Total Fee: " + QString::number(registrationList -> totalFee(type),'f',2));
    info.setInformativeText("Total " + totalsTypeInput -> currentText() + " Registration Fees");
    info.setWindowTitle(totalsTypeInput -> currentText() + " Registration Total Fee" );
    info.setStandardButtons(QMessageBox::Ok);
    info.setDefaultButton(QMessageBox::Ok);
    info.exec();

    totalsTypeInput -> setCurrentIndex(0);
}

//count total registrations for the given type
void RegistrationGui::totalRegistrations()
{
    QMessageBox info;
    QString type = "All";

    if(totalsTypeInput -> currentText() == "Standard") type = "Registration";
    else if(totalsTypeInput -> currentText() != "All") type = totalsTypeInput -> currentText() + "Registration";

    info.setText("Total Registrations: " + QString::number(registrationList -> totalRegistrations(type)));
    info.setInformativeText("Total number of " + totalsTypeInput -> currentText() + " Registration");
    info.setWindowTitle("Total " + totalsTypeInput -> currentText() + " Registrations" );
    info.setStandardButtons(QMessageBox::Ok);
    info.setDefaultButton(QMessageBox::Ok);
    info.exec();

    totalsTypeInput -> setCurrentIndex(0);
}

void RegistrationGui::inputsChanged()
{
    //enable or disable add registration button depending on whether required fields are empty or not, white spaces are ignored
    addRegistrationButton -> setDisabled(addRegistrationNameInput -> text().remove(" ").isEmpty() ||
                                         affiliationInput -> text().remove(" ").isEmpty() ||
                                         emailInput -> text().remove(" ").isEmpty() ||
                                         (addRegistrationTypeInput -> currentText() == "Student" && qualificationInput -> text().remove(" ").isEmpty()) ||
                                         (addRegistrationTypeInput -> currentText() == "Guest" && categoryInput -> text().remove(" ").isEmpty()));

    //enable or disable is registered button depending on whether name input is empty or not, white spaces are ignored
    isRegisteredButton -> setDisabled(isRegisteredNameInput -> text().remove(" ").isEmpty());
}

void RegistrationGui::registrationTypeChanged()
{
    //enable or disable category and qualifications input depending on selected qualification type
    qualificationInput -> setEnabled(addRegistrationTypeInput -> currentText() == "Student");
    categoryInput      -> setEnabled(addRegistrationTypeInput -> currentText() == "Guest");

    //clear qualification and category inputs depending on regstration type
    if(qualificationInput -> isEnabled()) qualificationInput -> setFocus();
    else qualificationInput -> clear();

    if(categoryInput -> isEnabled()) categoryInput -> setFocus();
    else categoryInput -> clear();

    inputsChanged();
}

void RegistrationGui::saveRegistrationListToFile() const
{
    RegistrationListWriter writer;

    writer.writeToFIle(registrationList -> getRegistrationList());
}

void RegistrationGui::openRegistrationListFromFile()
{
    //get the registration list from file
    RegistrationListReader reader;
    QList<Registration *> list = reader.getRegistrationListFromFile();

    //add the registration to registration list if they do not already exist
    foreach(Registration *reg, list)
        if(registrationList->addRegistration(reg))
            tableModel ->addRegistration(reg);
}

void RegistrationGui::setUpGui()
{
    int labelSizes = 100, inputSizes = 350, buttonSizes = 150;

    //creating the main layout, operations at the top with tabs and table at the bottom
    QGridLayout *mainLayout            = new QGridLayout(this);
    QTabWidget  *operationsWidget      = new QTabWidget(this);
    QTableView  *registrationListTable = new QTableView(this);

    mainLayout -> addWidget(operationsWidget,0,0);
    mainLayout -> addWidget(registrationListTable,1,0);

    this -> setLayout(mainLayout);

    //creating the table Widget
    registrationListTable -> setModel(tableModel);
    registrationListTable -> setShowGrid(true);
    registrationListTable -> setGridStyle(Qt::SolidLine);
    registrationListTable -> setMinimumWidth(850);
    registrationListTable -> horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    registrationListTable -> setEditTriggers(QAbstractItemView::NoEditTriggers);

    //creating the operation Tabs
    QWidget *addRegistrationTab = new QWidget(this);
    QWidget *isRegisteredTab    = new QWidget(this);
    QWidget *totalsTab          = new QWidget(this);
    QWidget *XmlFileTab         = new QWidget(this);

    operationsWidget -> addTab(addRegistrationTab,"&Add Registrations");
    operationsWidget -> addTab(isRegisteredTab,"&Find Registration");
    operationsWidget -> addTab(totalsTab,"&Totals");
    operationsWidget -> addTab(XmlFileTab, "&XML Files");

//---Add registration Tab

    //creating layout of add registration Tab (info at top, inputs in the middle and add button at bottom)
    QGridLayout *addRegistrationTabLayout    = new QGridLayout(addRegistrationTab);
    QLabel      *registrationTabInfoLabel    = new QLabel("Add Conference Registration", this);
    QWidget     *addRegistrationTabContainer = new QWidget(this);
    QSpacerItem *addRegistrationSpacer       = new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Minimum);

    addRegistrationTabLayout -> addWidget(registrationTabInfoLabel,0,0,Qt::AlignCenter);
    addRegistrationTabLayout -> addWidget(addRegistrationTabContainer,1,0);
    addRegistrationTabLayout -> addWidget(addRegistrationButton,2,0,Qt::AlignCenter);
    addRegistrationTabLayout -> addItem(addRegistrationSpacer,0,1);

    addRegistrationTab -> setLayout(addRegistrationTabLayout);

    //creating Add registartion tab inputs and labels
    QGridLayout *addRegistrationTabInputsLayout = new QGridLayout(addRegistrationTabContainer);

    QLabel *addRegistrationNameLabel = new QLabel("&Name: ",this);
    QLabel *affiliationLabel         = new QLabel("&Affiliation: ",this);
    QLabel *emailLabel               = new QLabel("&Email: ",this);
    QLabel *addRegistrationTypeLabel = new QLabel("&Registration Type: ",this);
    QLabel *qualificationLabel       = new QLabel("&Qualification",this);
    QLabel *categoryLabel            = new QLabel("&Category",this);

    addRegistrationNameLabel -> setBuddy(addRegistrationNameInput);
    affiliationLabel         -> setBuddy(affiliationInput);
    emailLabel               -> setBuddy(emailInput);
    addRegistrationTypeLabel -> setBuddy(addRegistrationTypeInput);
    qualificationLabel       -> setBuddy(qualificationInput);
    categoryLabel            -> setBuddy(categoryInput);

    addRegistrationTabInputsLayout -> addWidget(addRegistrationNameLabel,0,0);
    addRegistrationTabInputsLayout -> addWidget(addRegistrationNameInput,0,1);
    addRegistrationTabInputsLayout -> addWidget(affiliationLabel,1,0);
    addRegistrationTabInputsLayout -> addWidget(affiliationInput,1,1);
    addRegistrationTabInputsLayout -> addWidget(emailLabel,2,0);
    addRegistrationTabInputsLayout -> addWidget(emailInput,2,1);
    addRegistrationTabInputsLayout -> addWidget(addRegistrationTypeLabel,3,0);
    addRegistrationTabInputsLayout -> addWidget(addRegistrationTypeInput,3,1);
    addRegistrationTabInputsLayout -> addWidget(qualificationLabel,4,0);
    addRegistrationTabInputsLayout -> addWidget(qualificationInput,4,1);
    addRegistrationTabInputsLayout -> addWidget(categoryLabel,5,0);
    addRegistrationTabInputsLayout -> addWidget(categoryInput,5,1);

    addRegistrationTypeInput -> addItem("Standard");
    addRegistrationTypeInput -> addItem("Student");
    addRegistrationTypeInput -> addItem("Guest");

    addRegistrationButton -> setEnabled(false);
    qualificationInput    -> setEnabled(false);
    categoryInput         -> setEnabled(false);

    addRegistrationNameInput    -> setFocus();
    addRegistrationButton       -> setFixedWidth(buttonSizes);
    addRegistrationNameLabel    -> setFixedWidth(labelSizes);
    addRegistrationNameInput    -> setFixedWidth(inputSizes);
    addRegistrationTabContainer -> setLayout(addRegistrationTabInputsLayout);

    connect(addRegistrationButton, &QPushButton::clicked, this, &RegistrationGui::addRegistration);
    connect(addRegistrationTypeInput, &QComboBox::currentIndexChanged, this, &RegistrationGui::registrationTypeChanged);
    connect(addRegistrationNameInput, &QLineEdit::textChanged, this, &RegistrationGui::inputsChanged);
    connect(affiliationInput, &QLineEdit::textChanged, this, &RegistrationGui::inputsChanged);
    connect(emailInput, &QLineEdit::textChanged, this, &RegistrationGui::inputsChanged);
    connect(qualificationInput, &QLineEdit::textChanged, this, &RegistrationGui::inputsChanged);
    connect(categoryInput, &QLineEdit::textChanged, this, &RegistrationGui::inputsChanged);

//---Is registered Tab

    //creating layout of is registered Tab (info at top, inputs in the middle and add button at bottom)
    QGridLayout *isRegisteredTabLayout    = new QGridLayout(isRegisteredTab);
    QLabel      *isRegisteredTabInfoLabel = new QLabel("Check whether a person has already registered", this);
    QWidget     *isRegisteredTabContainer = new QWidget(this);
    QSpacerItem *isRegisteredHSpacer      = new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Minimum);
    QSpacerItem *isRegisteredVSpacer      = new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Expanding);

    isRegisteredTabLayout -> addWidget(isRegisteredTabInfoLabel,0,0,Qt::AlignCenter);
    isRegisteredTabLayout -> addWidget(isRegisteredTabContainer,1,0);
    isRegisteredTabLayout -> addWidget(isRegisteredButton,3,0,Qt::AlignCenter);
    isRegisteredTabLayout -> addItem(isRegisteredHSpacer,0,1);
    isRegisteredTabLayout -> addItem(isRegisteredVSpacer,2,0);

    //creating is registered tab inputs and labels
    QGridLayout *isRegisteredTabInputsLayout = new QGridLayout(isRegisteredTabContainer);
    QLabel      *isRegisteredNameLabel       = new QLabel("&Name: ",this);

    isRegisteredTabInputsLayout -> addWidget(isRegisteredNameLabel,0,0);
    isRegisteredTabInputsLayout -> addWidget(isRegisteredNameInput,0,1);

    isRegisteredNameLabel -> setBuddy(isRegisteredNameInput);
    isRegisteredButton    -> setFixedWidth(buttonSizes);
    isRegisteredNameLabel -> setFixedWidth(labelSizes);
    isRegisteredNameInput -> setFixedWidth(inputSizes);
    isRegisteredButton    -> setEnabled(false);
    isRegisteredTab       -> setLayout(isRegisteredTabLayout);

    connect(isRegisteredNameInput, &QLineEdit::textChanged, this, &RegistrationGui::inputsChanged);
    connect(isRegisteredButton, &QPushButton::clicked, this, &RegistrationGui::isRegistered);

//---Totals Tab
    QGridLayout *totalsTabLayout    = new QGridLayout(totalsTab);
    QLabel      *totalsTabInfoLabel = new QLabel("Calculate total fees or number of registrations per registration type", this);
    QWidget     *totalsTabContainer = new QWidget(this);
    QSpacerItem *totalsHSpacer      = new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Minimum);
    QSpacerItem *totalsVSpacer      = new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Expanding);

    totalsTabLayout -> addWidget(totalsTabInfoLabel,0,0,Qt::AlignCenter);
    totalsTabLayout -> addWidget(totalsTabContainer,1,0);
    totalsTabLayout -> addWidget(totalFeeButton,3,0,Qt::AlignCenter);
    totalsTabLayout -> addWidget(totalRegistrationsButton,4,0,Qt::AlignCenter);
    totalsTabLayout -> addItem(totalsHSpacer,0,1);
    totalsTabLayout -> addItem(totalsVSpacer,2,0);

    //creating totals tab inputs and labels
    QGridLayout *totalsTabInputsLayout = new QGridLayout(totalsTabContainer);
    QLabel      *totalsTypeLabel       = new QLabel("&Registration Type: ",this);

    totalsTabInputsLayout -> addWidget(totalsTypeLabel,0,0);
    totalsTabInputsLayout -> addWidget(totalsTypeInput,0,1);

    totalsTypeInput -> addItem("All");
    totalsTypeInput -> addItem("Standard");
    totalsTypeInput -> addItem("Student");
    totalsTypeInput -> addItem("Guest");

    totalRegistrationsButton -> setFixedWidth(buttonSizes);
    totalFeeButton  -> setFixedWidth(buttonSizes);
    totalsTypeLabel -> setFixedWidth(labelSizes);
    totalsTypeInput -> setFixedWidth(inputSizes);
    totalsTypeLabel -> setBuddy(totalsTypeInput);
    totalsTab       -> setLayout(totalsTabLayout);

    connect(totalFeeButton, &QPushButton::clicked, this, &RegistrationGui::totalFee);
    connect(totalRegistrationsButton, &QPushButton::clicked, this, &RegistrationGui::totalRegistrations);

//---Save/Open Xml FIle Tab
    QGridLayout *XmlFileTabLayout    = new QGridLayout(XmlFileTab);
    QLabel      *XmlFIleTabInfoLabel = new QLabel("Save and load the registration list to or from an XML formatted file", this);
    QSpacerItem *XmlFIleTabHSpacer1  = new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Minimum);
    QSpacerItem *XmlFIleTabHSpacer2  = new QSpacerItem(labelSizes + inputSizes + 25,1,QSizePolicy::Minimum,QSizePolicy::Minimum);
    QSpacerItem *XmlFIleTabVSpacer   = new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Expanding);

    XmlFileTabLayout -> addWidget(XmlFIleTabInfoLabel,0,0,Qt::AlignCenter);
    XmlFileTabLayout -> addItem(XmlFIleTabHSpacer1,0,1);
    XmlFileTabLayout -> addItem(XmlFIleTabHSpacer2,1,0);
    XmlFileTabLayout -> addItem(XmlFIleTabVSpacer,2,0);
    XmlFileTabLayout -> addWidget(openXmlFileButton,3,0,Qt::AlignCenter);
    XmlFileTabLayout -> addWidget(saveXmlFileButton,4,0,Qt::AlignCenter);

    openXmlFileButton -> setFixedWidth(buttonSizes);
    saveXmlFileButton -> setFixedWidth(buttonSizes);
    XmlFileTab        -> setLayout(XmlFileTabLayout);

    connect(saveXmlFileButton, &QPushButton::clicked, this, &RegistrationGui::saveRegistrationListToFile);
    connect(openXmlFileButton, &QPushButton::clicked, this, &RegistrationGui::openRegistrationListFromFile);
}


void RegistrationGui::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::warning(this, "Exit Registration App", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}
