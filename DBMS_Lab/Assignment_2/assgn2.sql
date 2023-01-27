create table Physician(
EmployeeID int NOT NULL PRIMARY KEY,
Name varchar(255),
Position varchar(255),
SSN int NOT NULL
);


create table Procedure(
Code int NOT NULL,
Name varchar(255),
Cost int NOT NULL,
primary key (Code)
);


create table Nurse(
EmployeeID int NOT NULL PRIMARY KEY,
Name varchar(255),
Position varchar(255),
Registered boolean NOT NULL,
SSN int NOT NULL,
);


create table Medication(
Code int NOT NULL PRIMARY KEY,
Name varchar(255),
Brand varchar(255),
Description varchar(255)
);


create table Block(
Floor int NOT NULL,
Code int NOT NULL,
primary key(Floor, Code)
);

create table Department(
DepartmentID int NOT NULL PRIMARY KEY,
Name varchar(255),
Head int NOT NULL REFERENCES Physician(EmployeeID)
);

create table Patient(
SSN int NOT NULL PRIMARY KEY,
Name varchar(255),
Address varchar(255),
Phone varchar(255),
InsuranceID int NOT NULL,
PCP int NOT NULL REFERENCES Physician(EmployeeID)
);

create table Affiliated_with(
Physician int NOT NULL REFERENCES Physician(EmployeeID),
Department int NOT NULL REFERENCES Department(DepartmentID),
PrimaryAffiliation boolean NOT NULL,
primary key(Physician, Department)
);

create table Trained_In(
Physician int NOT NULL REFERENCES Physician(EmployeeID),
Treatment int NOT NULL REFERENCES Procedure(Code),
CertificationDate TIMESTAMP NOT NULL,
CertificationExpires TIMESTAMP NOT NULL,
primary key(Physician, Treatment)
);


create table Room(
Number int NOT NULL PRIMARY KEY,
Type varchar(255),
BlockFloor int NOT NULL,
BlockCode int NOT NULL,
Unavailable boolean NOT NULL,
FOREIGN KEY (BlockFloor, BlockCode) REFERENCES Block(Floor, Code)
);


create table Stay(
StayID int NOT NULL PRIMARY KEY,
Patient int NOT NULL REFERENCES Patient(SSN),
Room int NOT NULL REFERENCES Room(Number),
Start TIMESTAMP NOT NULL,
"End" TIMESTAMP NOT NULL
);


create table Undergoes(
Patient int NOT NULL REFERENCES Patient(SSN),
Procedure int NOT NULL REFERENCES Procedure(Code),
Stay int NOT NULL REFERENCES Stay(StayID),
Date TIMESTAMP NOT NULL,
Physician int NOT NULL REFERENCES Physician(EmployeeID),
AssistingNurse int REFERENCES Nurse(EmployeeID)
);


create table On_Call(
Nurse int NOT NULL REFERENCES Nurse(EmployeeID),
BlockFloor int NOT NULL,
BlockCode int NOT NULL,
Start TIMESTAMP NOT NULL,
"End" TIMESTAMP NOT NULL,
FOREIGN KEY (BlockFloor, BlockCode) REFERENCES Block(Floor, Code),
primary key (Nurse, BlockFloor, BlockCode, Start, "End")
);


create table Appointment(
AppointmentID int NOT NULL PRIMARY KEY,
Patient int NOT NULL REFERENCES Patient(SSN),
PrepNurse int REFERENCES Nurse(EmployeeID),
Physician int NOT NULL REFERENCES Physician(EmployeeID),
Start TIMESTAMP NOT NULL,
"End" TIMESTAMP NOT NULL,
ExaminationRoom varchar(255)
);


create table Prescribes(
Physician int NOT NULL REFERENCES Physician(EmployeeID),
Patient int NOT NULL REFERENCES Patient(SSN),
Medication int NOT NULL REFERENCES Medication(Code),
Date TIMESTAMP NOT NULL,
Appointment int REFERENCES Appointment(AppointmentID),
Dose varchar(255),
primary key (Physician, Patient, Medication, Date)
);