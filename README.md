# Intergrove project
Registration_List
Intergrove project

There are three types of registration allowed for the conference: standard registration, student registration and invited guests registration. The standard registration fee is the 
standard fee (stored in STANDARD_FEE). For those registered as students, the conference fee is half the price of the standard registration fee. For those registered as 
guests, the conference fee is 10% of the standard registration fee. An instance of RegistrationList contains a list of Registration*. 
A RegistrationList can return the number of attendees that are registered for the conference from an 
institution. Similarly, the total registration fees for a type (“Registration”, “StudentRegistration”, “GuestRegistration” or “All”) of registration.

The GUI displays the current list of registrations, including the respective registration fees, in the 
RegistrationList, using a table widget. The interface allows users to request and view the results of the various operations supported by the 
RegistrationList class.  

The user can save to file, using the standard file dialog, where the XML formatted registration list is saved. 

RegistrationListReader, can parse the XML formatted asset list using QXmlStreamReader, and creates and returns a list of Registration objects.  

A factory class (RegistrationFactory) using the Factory method design pattern, which creates and returns a Registration based on an appropriate string 
argument and other arguments. The RegistrationFactory is an implementation of the Singleton pattern.   
parts of the solution that were creating concrete Registration objects (based on the user input and 
RegistrationListReader) are replaced by appropriate code to utilise the RegistrationFactory. 

HOW TO RUN.
This project runs on QT Creator, the latest version can be used.
A Qt Widgets Application is the required template when configured as a new project.
The build system is CMake.
The kit selection is Desktop MinGW 64-bit

