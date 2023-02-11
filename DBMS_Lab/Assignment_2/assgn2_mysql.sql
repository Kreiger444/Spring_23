/*table creation*/
create table physician(
employeeid int not null primary key,
name varchar(255),
position varchar(255),
ssn int not null
);


create table procedures(
code int not null,
name varchar(255),
cost int not null,
primary key (code)
);


create table nurse(
employeeid int not null primary key,
name varchar(255),
position varchar(255),
registered boolean not null,
ssn int not null
);


create table medication(
code int not null primary key,
name varchar(255),
brand varchar(255),
description varchar(255)
);


create table block(
floor int not null,
code int not null,
primary key(floor, code)
);

create table department(
departmentid int not null primary key,
name varchar(255),
head int not null references physician(employeeid)
);

create table patient(
ssn int not null primary key,
name varchar(255),
address varchar(255),
phone varchar(255),
insuranceid int not null,
pcp int not null references physician(employeeid)
);

create table affiliated_with(
physician int not null references physician(employeeid),
department int not null references department(departmentid),
primaryaffiliation boolean not null,
primary key(physician, department)
);

create table trained_in(
physician int not null references physician(employeeid),
treatment int not null references procedures(code),
certificationdate timestamp not null,
certificationexpires timestamp not null,
primary key(physician, treatment)
);


create table room(
number int not null primary key,
type varchar(255),
blockfloor int not null,
blockcode int not null,
unavailable boolean not null,
foreign key (blockfloor, blockcode) references block(floor, code)
);


create table stay(
stayid int not null primary key,
patient int not null references patient(ssn),
room int not null references room(number),
start timestamp not null,
end timestamp not null
);


create table undergoes(
patient int not null references patient(ssn),
procedures int not null references procedures(code),
stay int not null references stay(stayid),
date timestamp not null,
physician int not null references physician(employeeid),
assistingnurse int references nurse(employeeid)
);


create table on_call(
nurse int not null references nurse(employeeid),
blockfloor int not null,
blockcode int not null,
start timestamp not null,
end timestamp not null,
foreign key (blockfloor, blockcode) references block(floor, code),
primary key (nurse, blockfloor, blockcode, start, end)
);


create table appointment(
appointmentid int not null primary key,
patient int not null references patient(ssn),
prepnurse int references nurse(employeeid),
physician int not null references physician(employeeid),
start timestamp not null,
end timestamp not null,
examinationroom varchar(255)
);


create table prescribes(
physician int not null references physician(employeeid),
patient int not null references patient(ssn),
medication int not null references medication(code),
date timestamp not null,
appointment int references appointment(appointmentid),
dose varchar(255),
primary key (physician, patient, medication, date)
);

/*populating tables*/

insert into
physician (employeeid, name, position, ssn)
values
(1, 'valerie', 'medical director', 1),
(2, 'janet', 'senior resident', 2),
(3, 'thomas', 'head of department', 3),
(4, 'joseph', 'head of department', 4),
(5, 'george', 'head of department', 5),
(6, 'sera', 'general physician', 6);

insert into
procedures (code, name, cost)
values
(1, 'bypass surgery', 450000),
(2, 'rtpcr test', 1400),
(3, 'appendectomy', 30000),
(4, 'colonoscopy', 250000),
(5, 'immunotherapy', 300000);

insert into
nurse (employeeid, name, position, registered, ssn)
values
(6, 'jaques', 'registered nurse', 1, 7),
(7, 'xavier', 'nurse practitioner', 0, 8),
(8, 'stella', 'icu registered nurse', 1, 9);

insert into
medication (code, name, brand, description)
values
(
1,
'remdesivir',
'zydus cadila',
'antiviral medicine that works against severe acute respiratory syndrome coronavirus 2 (sars-cov-2)'
),
(
2,
'prilosec',
'astrazeneca',
'a prescription and over-the-counter medicine used to treat the symptoms of gastroesophageal reflux disease (gerd), gastric ulcers, and other conditions caused by excess stomach acid'
),
(
3,
'ibrutinib',
'pharmacyclics llc',
'a prescription medication used as an inhibitor of bruton tyrosine kinase (btk) used to treat patients with mantle cell lymphoma (mcl)'
);

insert into
block (floor, code)
values
(1, 1),
(1, 2),
(2, 1),
(3, 1);

insert into
department (departmentid, name, head)
values
(1, 'cardiology', 4),
(2, 'gastroenterology', 5),
(3, 'oncology', 3);

insert into
patient (ssn, name, address, phone, insuranceid, pcp)
values
(
9,
'kobe',
'48 finch ave. woodside, ny 11377',
'63045',
100,
1
),
(
10,
'nancy',
'4 san juan ave. grove city, oh 43123',
'45879',
101,
2
),
(
11,
'iris',
'371 locust drive glenview, il 60025',
'87564',
102,
3
);

insert into
affiliated_with (physician, department, primaryaffiliation)
values
(1, 1, 1),
(2, 2, 1),
(3, 3, 1),
(4, 1, 1),
(5, 2, 1),
(9, 2, 1);

insert into
trained_in (
physician,
treatment,
certificationdate,
certificationexpires
)
values
(1, 1, '2020-01-03', '2024-01-03'),
(2, 4, '2020-11-13', '2023-01-13'),
(3, 5, '2020-10-07', '2024-04-07'),
(4, 1, '2021-04-20', '2025-04-20'),
(5, 1, '2021-01-17', '2026-01-17');

insert into
room (number, type, blockfloor, blockcode, unavailable)
values
(121, 'icu', 1, 1, 0),
(122, 'operation theatre', 1, 2, 0),
(123, 'multibed ward', 2, 1, 0),
(124, 'single room', 2, 1, 0),
(125, 'single room', 3, 1, 1);

insert into
stay (stayid, patient, room, start, end)
values
(1, 10, 124, '2023-01-31', '2023-02-04'),
(
2,
11,
121,
'2023-02-06 14:30:00',
'2023-02-22 17:30:00'),
(3, 9, 124, '2023-02-10', '2023-02-13');

insert into
undergoes (
patient,
procedures,
stay,
date,
physician,
assistingnurse
)
values
(9, 1, 3, '2023-02-11', 1, 6),
(10, 4, 1, '2023-02-02', 2, null),
(11, 3, 2, '2023-02-06', 3, 8);

insert into
on_call (nurse, blockfloor, blockcode, start, end)
values
(6, 2, 1, '2023-01-27', '2023-01-31'),
(7, 3, 1, '2023-01-30', '2023-02-01'),
(8, 2, 1, '2023-02-01', '2023-02-03'),
(7, 1, 1, '2023-02-05', '2023-02-08');

insert into
appointment (
appointmentid,
patient,
prepnurse,
physician,
start,
end,
examinationroom
)
values
(
1,
9,
6,
1,
'2023-01-29 16:30:00',
'2023-01-29 17:30:00',
'cardiovascular exam room'
),
(
2,
10,
null,
2,
'2023-01-31 19:00:00',
'2023-01-31 19:30:00',
'x-ray examination room'
),
(
3,
11,
8,
3,
'2023-02-06 09:00:00',
'2023-02-06 10:15:00',
'radiation exam room'
),
(
4,
9,
6,
4,
'2023-02-15 10:00:00',
'2023-02-15 11:00:00',
'cardiovascular exam room'
);

insert into
prescribes (
physician,
patient,
medication,
date,
appointment,
dose
)
values
(9, 9, 1, '2023-02-10', 1, '100 mg'),
(2, 10, 2, '2023-01-31', null, '20 mg'),
(3, 11, 3, '2023-02-06', 3, '420 mg'),
(9, 10, 1, '2023-02-11', null, '100 mg');

/*queries*/
--query 1

select p.name
from physician as p
join trained_in as t on t.physician=p.employeeid
join procedures as pr on pr.code=t.treatment
where pr.name= 'bypass surgery';

--query 2

select p.name
from physician  p
join trained_in t on p.employeeid = t.physician
join procedures pr on t.treatment = pr.code
join affiliated_with a on p.employeeid = a.physician
join department d on a.department = d.departmentid
where pr.name = 'bypass surgery' and d.name = 'cardiology';

--query 3

select n.name
from nurse n
join room as r on r.number=123
join on_call as oc on (oc.blockcode, oc.blockfloor)=(r.blockcode, r.blockfloor)
where oc.nurse=n.employeeid;

--query 4

select p.name, p.address
from patient p
join medication as med on med.name= 'remdesivir'
join prescribes as pres on pres.medication=med.code and pres.patient=p.ssn;

--query 5

select p.name, p.insuranceid
from patient p
join room as r on r.type = 'icu'
join stay as st on st.room=r.number and st.patient=p.ssn
where timestampdiff(DAY,st.end, st.start) >=  15;

--query 6

select n.name
from nurse n
join undergoes as ug on ug.assistingnurse=n.employeeid
join procedures as pr on pr.code=ug.procedures
where pr.name='bypass surgery';

--query 7

select n.name as assisting_nurse, n.position, phys.name as physician_name
from nurse n
join undergoes as ug on ug.assistingnurse=n.employeeid
join procedures as pr on pr.code=ug.procedures
join physician as phys on phys.employeeid=ug.physician
where pr.name='bypass surgery';

--query 8

select p.name
from physician p
join trained_in ti on ti.physician=p.employeeid
join undergoes ug on ug.physician=p.employeeid
where ug.procedures != ti.treatment;

--query 9

select p.name
from physician as p
join trained_in ti on ti.physician=p.employeeid
join undergoes ug on ug.physician=p.employeeid and ug.date>ti.certificationexpires
where ti.treatment=ug.procedures;

--query 10


select p.name as physician_name,
(
select pr.name as procedures
from procedures as pr
where pr.code=ug.procedures
) as `procedure`,
ug.date, (
select pt.name as patient_name
from patient as pt
where pt.ssn=ug.patient
) as finger
from physician as p
join trained_in ti on ti.physician=p.employeeid
join undergoes ug on ug.physician=p.employeeid and ug.date>ti.certificationexpires
where ti.treatment=ug.procedures;

--query 11

with patient_procedures as (
select ug.patient, count(*) as count
from undergoes as ug
join procedures on ug.procedures = procedures.code
where cost > 5000
group by ug.patient
), patient_appointments as (
select appointment.patient, count(*) as appointment_count
from appointment
join undergoes as ug on appointment.patient = ug.patient
join physician on appointment.physician = physician.employeeid
join affiliated_with on appointment.physician = affiliated_with.physician
join department on affiliated_with.department = department.departmentid
where department.name = 'cardiology'
group by appointment.patient
having count(*) >= 2
)
select p.name as patient_name, ph.name as physician_name
from prescribes pr
join patient p on pr.patient = p.ssn
join physician ph on p.pcp = ph.employeeid
join patient_procedures pp on p.ssn = pp.patient
join patient_appointments pa on p.ssn = pa.patient
where ph.employeeid not in (
select head
from department
);


--query 12

with patient_medication as (
select medication, count(*) as total_prescriptions
from prescribes
group by medication
)
select m.name, m.brand
from patient_medication pm
join medication m on pm.medication = m.code
order by total_prescriptions
desc limit 1;