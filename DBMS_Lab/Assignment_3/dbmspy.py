import mysql.connector
from prettytable import PrettyTable



parameters = {
    'database': '20CS10075',
    'user': '20CS10075',
    'password': '20CS10075',
    'host': '10.5.18.70',
    'port': 3306
}

connection = mysql.connector.connect(**parameters)
curs=connection.cursor()
Queries = {
    1: '''select p.name
        from physician as p
        join trained_in as t on t.physician=p.employeeid
        join procedures as pr on pr.code=t.treatment
        where pr.name= 'bypass surgery';''',
    2: '''select p.name
        from physician  p
        join trained_in t on p.employeeid = t.physician
        join procedures pr on t.treatment = pr.code
        join affiliated_with a on p.employeeid = a.physician
        join department d on a.department = d.departmentid
        where pr.name = 'bypass surgery' and d.name = 'cardiology';''',
    3: '''select n.name
        from nurse n
        join room as r on r.number=123
        join on_call as oc on (oc.blockcode, oc.blockfloor)=(r.blockcode, r.blockfloor)
        where oc.nurse=n.employeeid;''',
    4: '''select p.name, p.address
        from patient p
        join medication as med on med.name= 'remdesivir'
        join prescribes as pres on pres.medication=med.code and pres.patient=p.ssn;''',
    5: '''select p.name, p.insuranceid
        from patient p
        join room as r on r.type = 'icu'
        join stay as st on st.room=r.number and st.patient=p.ssn
        where timestampdiff(DAY,st.end, st.start) >=  15;''',
    6: '''select n.name
        from nurse n
        join undergoes as ug on ug.assistingnurse=n.employeeid
        join procedures as pr on pr.code=ug.procedures
        where pr.name='bypass surgery';''',
    7: '''select n.name as assisting_nurse, n.position, phys.name as physician_name
        from nurse n
        join undergoes as ug on ug.assistingnurse=n.employeeid
        join procedures as pr on pr.code=ug.procedures
        join physician as phys on phys.employeeid=ug.physician
        where pr.name='bypass surgery';''',
    8: '''select p.name
        from physician p
        join trained_in ti on ti.physician=p.employeeid
        join undergoes ug on ug.physician=p.employeeid
        where ug.procedures != ti.treatment;''',
    9: '''select p.name
        from physician as p
        join trained_in ti on ti.physician=p.employeeid
        join undergoes ug on ug.physician=p.employeeid and ug.date>ti.certificationexpires
        where ti.treatment=ug.procedures;''',
    10: '''select p.name as physician_name,
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
        where ti.treatment=ug.procedures;''',
    11: '''with patient_procedures as (
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
        );''',
    12: '''with patient_medication as (
        select medication, count(*) as total_prescriptions
        from prescribes
        group by medication
        )
        select m.name, m.brand
        from patient_medication pm
        join medication m on pm.medication = m.code
        order by total_prescriptions
        desc limit 1;''',
    13: '''select p.name
        from physician as p
        join trained_in as t on t.physician=p.employeeid
        join procedures as pr on pr.code=t.treatment
        where pr.name= '{}';'''
}

while True:
    try:
        querynum = int(input("Enter a Query number or enter -1 to exit: "))
    except ValueError:
        print("Invalid input", end='\n')
        continue
    if querynum == -1:
        break
    elif querynum > 13 or querynum < 1:
        print("Invalid Query number", end='\n')
        continue
    elif querynum==13:
        procedure= input("Enter Procedure: ")
        Query=Queries[querynum].format(procedure.lower())
    else:
        Query=Queries[querynum]

    curs.execute(Query)
    column=[]
    for sample in curs.description:
        column.append(sample[0])
    
    table=PrettyTable(column)
    rows=curs.fetchall()   
    for flag in rows:
        table.add_row(flag)
    print(table) 
curs.close()
connection.close()
