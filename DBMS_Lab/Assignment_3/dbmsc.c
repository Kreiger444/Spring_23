#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

int main(){

    MYSQL *connection;
    MYSQL_RES *answer;
    MYSQL_ROW row;

    char *server= "10.5.18.70";
    char *username= "20CS10075";
    char *password= "20CS10075";
    char *database= "20CS10075";
    int port= 3306;

    char *queries[13];

    queries[0]="select p.name from physician as p join trained_in as t on t.physician=p.employeeid join procedures as pr on pr.code=t.treatment where pr.name= 'bypass surgery';";
    queries[1]="select p.name from physician  p join trained_in t on p.employeeid = t.physician join procedures pr on t.treatment = pr.code join affiliated_with a on p.employeeid = a.physician join department d on a.department = d.departmentid where pr.name = 'bypass surgery' and d.name = 'cardiology';";
    queries[2]="select n.name from nurse n join room as r on r.number=123 join on_call as oc on (oc.blockcode, oc.blockfloor)=(r.blockcode, r.blockfloor) where oc.nurse=n.employeeid;";
    queries[3]="select p.name, p.address from patient p join medication as med on med.name= 'remdesivir' join prescribes as pres on pres.medication=med.code and pres.patient=p.ssn;";
    queries[4]="select p.name, p.insuranceid from patient p join room as r on r.type = 'icu' join stay as st on st.room=r.number and st.patient=p.ssn where timestampdiff(DAY,st.end, st.start) >=  15;";
    queries[5]="select n.name from nurse n join undergoes as ug on ug.assistingnurse=n.employeeid join procedures as pr on pr.code=ug.procedures where pr.name='bypass surgery';";
    queries[6]="select n.name as assisting_nurse, n.position, phys.name as physician_name from nurse n join undergoes as ug on ug.assistingnurse=n.employeeid join procedures as pr on pr.code=ug.procedures join physician as phys on phys.employeeid=ug.physician where pr.name='bypass surgery';";
    queries[7]="select p.name from physician p join trained_in ti on ti.physician=p.employeeid join undergoes ug on ug.physician=p.employeeid where ug.procedures != ti.treatment;";
    queries[8]="select p.name from physician as p join trained_in ti on ti.physician=p.employeeid join undergoes ug on ug.physician=p.employeeid and ug.date>ti.certificationexpires where ti.treatment=ug.procedures;";
    queries[9]="select p.name as physician_name, ( select pr.name as procedures from procedures as pr where pr.code=ug.procedures ) as `procedure`, ug.date, ( select pt.name as patient_name from patient as pt where pt.ssn=ug.patient ) as finger from physician as p join trained_in ti on ti.physician=p.employeeid join undergoes ug on ug.physician=p.employeeid and ug.date>ti.certificationexpires where ti.treatment=ug.procedures;";
    queries[10]="with patient_procedures as ( select ug.patient, count(*) as count from undergoes as ug join procedures on ug.procedures = procedures.code where cost > 5000 group by ug.patient ), patient_appointments as ( select appointment.patient, count(*) as appointment_count from appointment join undergoes as ug on appointment.patient = ug.patient join physician on appointment.physician = physician.employeeid join affiliated_with on appointment.physician = affiliated_with.physician join department on affiliated_with.department = department.departmentid where department.name = 'cardiology' group by appointment.patient having count(*) >= 2 ) select p.name as patient_name, ph.name as physician_name from prescribes pr join patient p on pr.patient = p.ssn join physician ph on p.pcp = ph.employeeid join patient_procedures pp on p.ssn = pp.patient join patient_appointments pa on p.ssn = pa.patient where ph.employeeid not in ( select head from department );";
    queries[11]="with patient_medication as ( select medication, count(*) as total_prescriptions from prescribes group by medication ) select m.name, m.brand from patient_medication pm join medication m on pm.medication = m.code order by total_prescriptions desc limit 1;";
    queries[12]="select p.name from physician as p join trained_in as t on t.physician=p.employeeid join procedures as pr on pr.code=t.treatment where pr.name= '";

//establishing connection
    connection=mysql_init(NULL);
    
    MYSQL *status=mysql_real_connect(connection, server, username, password, database, port, NULL, 0);
    if(status==NULL){
        perror("Connection Failed");
        exit(0);
    }
    for(;;){
        int querynum;
        int columns;
        int rows;
        //taking input for query number from user
        printf("Enter a Query number from 1-13 or enter -1 to exit: ");
        scanf("%d", &querynum);
        printf("\n");
        //exit condition
        if(querynum==-1){
            break;
        }
        //invalid query number given
        else if(querynum<1 || querynum>13){
            printf("Invalid Query number\n");
            continue;
        }
        
        else{
            //query number 13 --- must take input from user as procedure
            if(querynum==13){
                char procedure[50];
                char *newquery;
                //taking input from user and appending '; to make it a proper sql query
                printf("Enter the procedure's name: ");
                getchar();
                fgets(procedure, 50, stdin);
                procedure[strlen(procedure)-1]='\0';

                strcat(procedure, "';");
                int newlen=strlen(procedure)+strlen(queries[12]);

                newquery=(char*)malloc((newlen+1)*sizeof(char));

                strcpy(newquery, queries[12]);
                strcat(newquery, procedure);

                newquery[strlen(newquery)-1]='\0';
                //checking if the query is correct or not
                int flag=mysql_query(connection, newquery);
                if(flag!=0){
                    perror("could not run query");
                    exit(0);
                }
                free(newquery);
            }
            else{
                //checking if the query is correct or not
                int flag= mysql_query(connection, queries[querynum-1]);
                if(flag!=0){
                    perror("could not run query");
                    exit(0);
                }
            }
        }
        //storing result in answer
        answer=mysql_store_result(connection);

        columns=mysql_num_fields(answer);
        MYSQL_FIELD *fields=mysql_fetch_fields(answer);
        rows=mysql_num_rows(answer);
        //prints empty set if there are no matching entries
        if(rows==0){
            printf("Empty set\n");
            continue;
        }
        //calculating max lengths of each field to format the table
        int *lengths=(int*)malloc(columns*sizeof(int));
        for(int i=0; i<columns; i++){
            lengths[i]=0;
        }
        for(int i=0; i<columns; i++){
            int temp=strlen(fields[i].name);
            if(temp>lengths[i]){
                lengths[i]=temp;
            }
        }
        //creating a new array to store rows
        MYSQL_ROW *newrow;
        newrow=(MYSQL_ROW*)malloc(rows*sizeof(MYSQL_ROW));
        int j=0;
        while((row=mysql_fetch_row(answer))!=NULL){
            newrow[j]=row;
            j++;
            for(int i=0; i<columns; i++){
                int temp=strlen(row[i]);
                if(temp>lengths[i]){
                    lengths[i]=temp;
                }
            }
        }
        //printing into the table
        printf("||");
        for(int i=0; i< columns; i++){
            printf(" %-*s ||", lengths[i], fields[i].name);

        }
        printf("\n");
        //
        for(int i=0; i<rows; i++){
            printf("||");
            for(int j=0; j<columns; j++){
                printf(" %-*s ||", lengths[j], newrow[i][j]);
            }
            printf("\n");
        }
        free(lengths);
    }

    mysql_close(connection);
    return 0;
}