import java.sql.*;
import java.util.Scanner;
import java.util.HashMap;
public class Main {
//storing database access details
    static final String JDBC_DRIVER = "com.mysql.cj.jdbc.Driver";
    static final String DB_URL = "jdbc:mysql://10.5.18.70:3306/20CS10075";

    static final String USER = "20CS10075";
    static final String PASS = "20CS10075";

    public static void main(String[] args) {
        Connection conn;
        Statement stmt;
        //creating queries
        HashMap<Integer, String> map = new HashMap<>();
        map.put(1, "select p.name from physician as p join trained_in as t on t.physician=p.employeeid join procedures as pr on pr.code=t.treatment where pr.name= 'bypass surgery';");
        map.put(2, "select p.name from physician  p join trained_in t on p.employeeid = t.physician join procedures pr on t.treatment = pr.code join affiliated_with a on p.employeeid = a.physician join department d on a.department = d.departmentid where pr.name = 'bypass surgery' and d.name = 'cardiology';");
        map.put(3, "select n.name from nurse n join room as r on r.number=123 join on_call as oc on (oc.blockcode, oc.blockfloor)=(r.blockcode, r.blockfloor) where oc.nurse=n.employeeid;");
        map.put(4, "select p.name, p.address from patient p join medication as med on med.name= 'remdesivir' join prescribes as pres on pres.medication=med.code and pres.patient=p.ssn;");
        map.put(5, "select p.name, p.insuranceid from patient p join room as r on r.type = 'icu' join stay as st on st.room=r.number and st.patient=p.ssn where timestampdiff(DAY,st.end, st.start) >=  15;");
        map.put(6, "select n.name from nurse n join undergoes as ug on ug.assistingnurse=n.employeeid join procedures as pr on pr.code=ug.procedures where pr.name='bypass surgery';");
        map.put(7, "select n.name as assisting_nurse, n.position, phys.name as physician_name from nurse n join undergoes as ug on ug.assistingnurse=n.employeeid join procedures as pr on pr.code=ug.procedures join physician as phys on phys.employeeid=ug.physician where pr.name='bypass surgery';");
        map.put(8, "select p.name from physician p join trained_in ti on ti.physician=p.employeeid join undergoes ug on ug.physician=p.employeeid where ug.procedures != ti.treatment;");
        map.put(9, "select p.name from physician as p join trained_in ti on ti.physician=p.employeeid join undergoes ug on ug.physician=p.employeeid and ug.date>ti.certificationexpires where ti.treatment=ug.procedures;");
        map.put(10, "select p.name as physician_name, ( select pr.name as procedures from procedures as pr where pr.code=ug.procedures ) as `procedure`, ug.date, ( select pt.name as patient_name from patient as pt where pt.ssn=ug.patient ) as finger from physician as p join trained_in ti on ti.physician=p.employeeid join undergoes ug on ug.physician=p.employeeid and ug.date>ti.certificationexpires where ti.treatment=ug.procedures;");
        map.put(11, "with patient_procedures as ( select ug.patient, count(*) as count from undergoes as ug join procedures on ug.procedures = procedures.code where cost > 5000 group by ug.patient ), patient_appointments as ( select appointment.patient, count(*) as appointment_count from appointment join undergoes as ug on appointment.patient = ug.patient join physician on appointment.physician = physician.employeeid join affiliated_with on appointment.physician = affiliated_with.physician join department on affiliated_with.department = department.departmentid where department.name = 'cardiology' group by appointment.patient having count(*) >= 2 ) select p.name as patient_name, ph.name as physician_name from prescribes pr join patient p on pr.patient = p.ssn join physician ph on p.pcp = ph.employeeid join patient_procedures pp on p.ssn = pp.patient join patient_appointments pa on p.ssn = pa.patient where ph.employeeid not in ( select head from department );");
        map.put(12, "with patient_medication as ( select medication, count(*) as total_prescriptions from prescribes group by medication ) select m.name, m.brand from patient_medication pm join medication m on pm.medication = m.code order by total_prescriptions desc limit 1;");
        map.put(13, "select p.name from physician as p join trained_in as t on t.physician=p.employeeid join procedures as pr on pr.code=t.treatment where pr.name= '%s';");

        try {
            //establishing connection
            Class.forName(JDBC_DRIVER);
            conn = DriverManager.getConnection(DB_URL, USER, PASS);
            stmt = conn.createStatement();
            while (true) {
                //taking input from the user
                Scanner scan = new Scanner(System.in);
                System.out.print("Enter the query number or enter -1 to exit: ");
                int querynum = scan.nextInt();
                //break condition
                if(querynum==-1) {
                    break;
                }
                //setting string query from the map
                String query=map.get(querynum);
                //handling query 13 and adding the entered procedure to the query
                if(querynum==13){
                    Scanner scanner= new Scanner(System.in);
                    System.out.print("Enter the procedure name: ");
                    String procedure =scanner.nextLine();
                    query=String.format(query,procedure);
                }

                //executing the query
                ResultSet rs = stmt.executeQuery(query);
                //retrieving data after execution of the query
                ResultSetMetaData rsmd = rs.getMetaData();
                int columnsNumber = rsmd.getColumnCount();

                if(rs.next()){
                    //printing the column names
                    for (int i = 1; i <= columnsNumber; i++) {
                        if (i > 1) System.out.print(" || ");
                        System.out.print(rsmd.getColumnLabel(i));

                    }
                    System.out.print("\n");
                    //printing the first row
                    for (int i = 1; i <= columnsNumber; i++) {
                        if (i > 1) System.out.print(" || ");
                        String columnValue = rs.getString(i);
                        System.out.print(columnValue);

                    }

                }
                //if there is no first row, it prints empty table
                else{
                    System.out.print("Empty table");
                }
                System.out.print("\n");
                //printing further rows
                while (rs.next()) {
                    for (int i = 1; i <= columnsNumber; i++) {
                        if (i > 1) System.out.print(" || ");
                        String columnValue = rs.getString(i);
                        System.out.print(columnValue);

                    }

                    System.out.print("\n");

                }
                rs.close();

            }
            //closing connection
            stmt.close();
            conn.close();
        }
        //handling exceptions
        catch (SQLException se) {
                se.printStackTrace();
            } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        }

    }

}