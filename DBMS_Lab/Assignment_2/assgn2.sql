/*Table Creation*/
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

/*Populating Tables*/

INSERT INTO
Physician (EmployeeID, Name, Position, SSN)
VALUES
(1, 'Valerie', 'Medical Director', 1),
(2, 'Janet', 'Senior Resident', 2),
(3, 'Thomas', 'Head of Department', 3),
(4, 'Joseph', 'Head of Department', 4),
(5, 'George', 'Head of Department', 5),
(6, 'Sera', 'General Physician', 6);

INSERT INTO
Procedure (Code, Name, Cost)
VALUES
(1, 'bypass surgery', 450000),
(2, 'RTPCR test', 1400),
(3, 'Appendectomy', 30000),
(4, 'colonoscopy', 250000),
(5, 'Immunotherapy', 300000);

INSERT INTO
Nurse (EmployeeID, Name, Position, Registered, SSN)
VALUES
(6, 'Jaques', 'Registered Nurse', true, 7),
(7, 'Xavier', 'Nurse Practitioner', false, 8),
(8, 'Stella', 'ICU Registered Nurse', true, 9);

INSERT INTO
Medication (Code, Name, Brand, Description)
VALUES
(
1,
'remdesivir',
'Zydus Cadila',
'antiviral medicine that works against severe acute respiratory syndrome coronavirus 2 (SARS-CoV-2)'
),
(
2,
'prilosec',
'AstraZeneca',
'a prescription and over-the-counter medicine used to treat the symptoms of gastroesophageal reflux disease (GERD), gastric ulcers, and other conditions caused by excess stomach acid'
),
(
3,
'ibrutinib',
'Pharmacyclics LLC',
'a prescription medication used as an inhibitor of Bruton tyrosine kinase (BTK) used to treat patients with mantle cell lymphoma (MCL)'
);

INSERT INTO
Block (Floor, Code)
VALUES
(1, 1),
(1, 2),
(2, 1),
(3, 1);

INSERT INTO
Department (DepartmentID, Name, Head)
VALUES
(1, 'cardiology', 4),
(2, 'Gastroenterology', 5),
(3, 'Oncology', 3);

INSERT INTO
Patient (SSN, Name, Address, Phone, InsuranceID, PCP)
VALUES
(
9,
'Kobe',
'48 Finch Ave. Woodside, NY 11377',
'63045',
100,
1
),
(
10,
'Nancy',
'4 San Juan Ave. Grove City, OH 43123',
'45879',
101,
2
),
(
11,
'Iris',
'371 Locust Drive Glenview, IL 60025',
'87564',
102,
3
);

INSERT INTO
Affiliated_with (Physician, Department, PrimaryAffiliation)
VALUES
(1, 1, true),
(2, 2, true),
(3, 3, true),
(4, 1, true),
(5, 2, true),
(9, 2, true);

INSERT INTO
Trained_In (
Physician,
Treatment,
CertificationDate,
CertificationExpires
)
VALUES
(1, 1, '2020-01-03', '2024-01-03'),
(2, 4, '2020-11-13', '2023-01-13'),
(3, 5, '2020-10-07', '2024-04-07'),
(4, 1, '2021-04-20', '2025-04-20'),
(5, 1, '2021-01-17', '2026-01-17');

INSERT INTO
Room (Number, Type, BlockFloor, BlockCode, Unavailable)
VALUES
(121, 'ICU', 1, 1, false),
(122, 'Operation Theatre', 1, 2, false),
(123, 'Multibed Ward', 2, 1, false),
(124, 'Single Room', 2, 1, false),
(125, 'Single Room', 3, 1, true);

INSERT INTO
Stay (StayID, Patient, Room, Start, "End")
VALUES
(1, 10, 124, '2023-01-31', '2023-02-04'),
(
2,
11,
121,
'2023-02-06 14:30:00',
'2023-02-22 17:30:00'),
(3, 9, 124, '2023-02-10', '2023-02-13');

INSERT INTO
Undergoes (
Patient,
Procedure,
Stay,
Date,
Physician,
AssistingNurse
)
VALUES
(9, 1, 3, '2023-02-11', 1, 6),
(10, 4, 1, '2023-02-02', 2, NULL),
(11, 3, 2, '2023-02-06', 3, 8);

INSERT INTO
On_Call (Nurse, BlockFloor, BlockCode, Start, "End")
VALUES
(6, 2, 1, '2023-01-27', '2023-01-31'),
(7, 3, 1, '2023-01-30', '2023-02-01'),
(8, 2, 1, '2023-02-01', '2023-02-03'),
(7, 1, 1, '2023-02-05', '2023-02-08');

INSERT INTO
Appointment (
AppointmentID,
Patient,
PrepNurse,
Physician,
Start,
"End",
ExaminationRoom
)
VALUES
(
1,
9,
6,
1,
'2023-01-29 16:30:00',
'2023-01-29 17:30:00',
'Cardiovascular Exam Room'
),
(
2,
10,
NULL,
2,
'2023-01-31 19:00:00',
'2023-01-31 19:30:00',
'X-Ray Examination Room'
),
(
3,
11,
8,
3,
'2023-02-06 09:00:00',
'2023-02-06 10:15:00',
'Radiation Exam Room'
),
(
4,
9,
6,
4,
'2023-02-15 10:00:00',
'2023-02-15 11:00:00',
'Cardiovascular Exam Room'
);

INSERT INTO
Prescribes (
Physician,
Patient,
Medication,
Date,
Appointment,
Dose
)
VALUES
(9, 9, 1, '2023-02-10', 1, '100 mg'),
(2, 10, 2, '2023-01-31', NULL, '20 mg'),
(3, 11, 3, '2023-02-06', 3, '420 mg'),
(9, 10, 1, '2023-02-11', NULL, '100 mg');

/*Queries*/
--Query 1

SELECT p.name
FROM Physician as p
JOIN Trained_In as t on t.physician=p.EmployeeID
JOIN procedure as pr on pr.code=t.treatment
WHERE pr.name= 'bypass surgery';

--Query 2

SELECT p.name
FROM physician  p
JOIN trained_in t ON p.EmployeeID = t.physician
JOIN procedure pr ON t.treatment = pr.Code
JOIN affiliated_with a ON p.EmployeeID = a.physician
JOIN department d ON a.department = d.departmentID
WHERE pr.name = 'bypass surgery' AND d.name = 'cardiology';

--Query 3

SELECT n.name
FROM nurse n
JOIN Room as r on r.number=123
JOIN On_Call as oc on (oc.BlockCode, oc.BlockFloor)=(r.BlockCode, r.BlockFloor)
WHERE oc.Nurse=n.EmployeeID;

--Query 4

SELECT p.name, p.Address
FROM patient p
JOIN Medication as med on med.Name= 'remdesivir'
JOIN Prescribes as pres on pres.Medication=med.code and pres.patient=p.SSN;

--Query 5

SELECT p.name, p.InsuranceID
FROM Patient p
JOIN room as r on r.type = 'ICU'
JOIN Stay as st on st.room=r.number and st.patient=p.ssn
WHERE st."End"-st.Start >= interval '15 days';

--Query 6

SELECT n.name
FROM nurse n
JOIN Undergoes as ug on ug.AssistingNurse=n.EmployeeID
JOIN Procedure as pr on pr.Code=ug.Procedure
WHERE pr.name='bypass surgery';

--Query 7

SELECT n.name as Assisting_Nurse, n.Position, phys.name as Physician_Name
FROM nurse n
JOIN Undergoes as ug on ug.AssistingNurse=n.EmployeeID
JOIN Procedure as pr on pr.Code=ug.Procedure
JOIN Physician as phys on phys.EmployeeID=ug.Physician
WHERE pr.name='bypass surgery';

--Query 8

SELECT p.name
FROM physician p
JOIN Trained_in ti on ti.Physician=P.EmployeeID
JOIN undergoes ug on ug.physician=p.EmployeeID
WHERE ug.procedure != ti.treatment;

--Query 9

SELECT P.name
FROM Physician as P
JOIN Trained_In ti on ti.physician=p.EmployeeID
JOIN undergoes ug on ug.physician=p.EmployeeID and ug.Date>ti.CertificationExpires
WHERE ti.treatment=ug.procedure;

--Query 10


SELECT p.name as Physician_Name,
(
SELECT pr.name as Procedure
FROM Procedure as pr
WHERE Pr.code=ug.Procedure
),
ug.Date, (
SELECT pt.Name as Patient_Name
FROM Patient as pt
WHERE pt.SSN=ug.Patient
)
FROM physician as P
JOIN Trained_In ti on ti.physician=p.EmployeeID
JOIN undergoes ug on ug.physician=p.EmployeeID and ug.Date>ti.CertificationExpires
WHERE ti.treatment=ug.procedure;

--Query 11

WITH patient_procedure AS (
SELECT ug.patient, count(*) as count
FROM undergoes as ug
JOIN procedure ON ug.procedure = procedure.Code
WHERE cost > 5000
GROUP BY ug.patient
), patient_appointments AS (
SELECT appointment.patient, COUNT(*) AS appointment_count
FROM appointment
JOIN undergoes as ug ON appointment.patient = ug.patient
JOIN physician ON appointment.physician = physician.EmployeeID
JOIN affiliated_with ON appointment.physician = affiliated_with.physician
JOIN department ON affiliated_with.department = department.departmentid
WHERE department.name = 'cardiology'
GROUP BY appointment.patient
HAVING COUNT(*) >= 2
)
SELECT p.name AS patient_name, ph.name AS physician_name
FROM prescribes pr
JOIN patient p ON pr.patient = p.SSN
JOIN physician ph ON pr.physician = ph.EmployeeID
JOIN patient_procedure pp ON p.SSN = pp.patient
JOIN patient_appointments pa ON p.SSN = pa.patient
WHERE ph.EmployeeID NOT IN (
SELECT head
FROM department
);


--Query 12

WITH patient_medication AS (
SELECT medication, COUNT(*) AS total_prescriptions
FROM Prescribes
GROUP BY medication
)
SELECT m.name, m.brand
FROM patient_medication pm
JOIN medication m ON pm.medication = m.code
ORDER BY total_prescriptions
DESC LIMIT 1;