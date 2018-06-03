#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <pqxx/pqxx>
#include <algorithm>
#include <dirent.h>
#include <cstdlib>
#include <unistd.h>
#include <boost/algorithm/string/predicate.hpp>

using namespace std;
using namespace pqxx;

int main(int argc, char const *argv[]) {
  char proNum;
  string user_name = getenv("USER");
  connection C("dbname = FakeUData user = "+user_name);
  result r;
  if (C.is_open()) {
     cout << "Opened database successfully: " << C.dbname() << endl;
  } else {
     cout << "Can't open database" << endl;
     return 1;
  }
  work W(C);
  string sql1;
  string sql2;
  string sql3;
  string sql4;
  string sql5;
  string sql6;
  string sql7;
  string sql8;
  while(proNum != 'x'){
    cout << "Problem Number(a-h, x for exit): ";
    cin >> proNum;
  if(proNum == 'a'){
  // P2.A
  for(int i = 1; i <= 20;i ++){
    sql1 = "SELECT T1.TERM, ROUND(NUM*1.0/TOTAL*1.0,3) AS UNIT_"+ to_string(i)+ " INTO UNIT" + to_string(i) + " FROM (SELECT TERM, COUNT(DISTINCT SID) AS TOTAL FROM STUDENTS WHERE MAJOR LIKE '%ABC%' OR MAJOR LIKE '%DEF%' GROUP BY TERM) T1 LEFT OUTER JOIN (SELECT TERM, COUNT(*) AS NUM FROM( SELECT TERM,SID FROM STUDENTS WHERE MAJOR LIKE '%ABC%' OR MAJOR LIKE '%DEF%'GROUP BY TERM,SID HAVING SUM(UNITS) = " + to_string(i) +") T2 GROUP BY TERM ) T3 ON T1.TERM = T3.TERM;";
    W.exec(sql1);
  }
  sql1 = "SELECT UNIT1.TERM";
  for(int i = 1; i<= 20 ; i++){
    sql1 += ",UNIT_" + to_string(i);
  }
  sql1 += " FROM(UNIT1";

  for(int i = 2; i <= 20;i++){
    sql1 += " FULL OUTER JOIN UNIT" + to_string(i)+ " ON UNIT1.TERM = UNIT" + to_string(i)+".TERM ";
  }
  sql1 += ");";
  r = W.exec(sql1);

  for (int rownum = 0; rownum < r.size(); ++rownum) {
        const result::tuple row = r[rownum];
        for (int colnum = 0; colnum < row.size(); ++colnum) {
            const result::field myField = row[colnum];
            if(strlen(myField.c_str()) == 0){std::cout << "      ";}
            else{std::cout << myField.c_str() << ' ';}
        }
        std::cout << std::endl;
    }
  }


   else if (proNum == 'b'){
    //P2.B

    sql2 = "WITH GPA_TABLE AS( "\
           "SELECT INSTRUCTOR, ROUND(TOTAL/NUM,4) AS AVAG "\
           "FROM (SELECT INSTRUCTOR, SUM(case "\
                            "when GRADE ='A+' then 4.0 "\
                            "when GRADE ='A' then 4.0 "\
                            "when GRADE = 'A-' then 3.7 "\
                            "when GRADE ='B+' then 3.3 "\
                            "when GRADE ='B' then 3.0 "\
                            "when GRADE ='B-' then 2.7 "\
                            "when GRADE ='C+' then 2.3 "\
                            "when GRADE ='C' then 2 "\
                            "when GRADE ='C-' then 1.7 "\
                            "when GRADE ='D+' then 1.3 "\
                            "when GRADE ='D' then 1 "\
                            "when GRADE ='D-' then 0.7 "\
                            "else 0.0"\
                   "end "\
                   ") AS TOTAL, "\
                   "SUM (case "\
                   "when GRADE ='A+' then 1.0 "\
                   "when GRADE ='A' then 1.0 "\
                   "when GRADE ='A-'then 1.0 "\
                   "when GRADE ='B+' then 1.0 "\
                   "when GRADE ='B' then 1.0 "\
                   "when GRADE ='B-' then 1.0 "\
                   "when GRADE ='C+' then 1.0 "\
                   "when GRADE ='C' then 1.0 "\
                   "when GRADE ='C-' then 1.0 "\
                   "when GRADE ='D+' then 1.0 "\
                   "when GRADE ='D' then 1.0 "\
                   "when GRADE ='D-' then 1.0 "\
                   "else 0.0 "\
                   "end "\
                   ") AS NUM "\

    "FROM COURSES,STUDENTS "\
    "WHERE COURSES.CID = STUDENTS.CID "\
    "GROUP BY INSTRUCTOR) T1 "\
    "WHERE T1.NUM <> 0) "\
    "SELECT INSTRUCTOR, AVAG "\
    "FROM GPA_TABLE "\
    "WHERE AVAG = (SELECT MAX(AVAG) FROM GPA_TABLE)  "\
    "UNION "\
    "SELECT INSTRUCTOR, AVAG "\
    "FROM GPA_TABLE "\
    "WHERE AVAG = (SELECT MIN(AVAG) FROM GPA_TABLE);";


    r = W.exec(sql2);

    for (int rownum = 0; rownum < r.size(); ++rownum) {
          const result::tuple row = r[rownum];
          for (int colnum = 0; colnum < row.size(); ++colnum) {
              const result::field myField = row[colnum];
              if(strlen(myField.c_str()) == 0){std::cout << "      ";}
              else{std::cout << myField.c_str() << ' ';}
          }
          std::cout << std::endl;
      }
    }
   else if(proNum == 'c'){
// P2.C
   for(int i = 1;i <= 20;i++){
   sql3 = "WITH GPA_TABLE AS( " \
   "SELECT TERM, ROUND(AVG(AG),4) AS AG_GRADE "\
   "FROM( " \
   "SELECT TERM,SID,ROUND(SUM(case "\
  "                       when GRADE ='A+' then 4.0*UNITS "\
    "                        when GRADE ='A' then 4.0*UNITS "\
      "                      when GRADE = 'A-' then 3.7*UNITS "\
        "                    when GRADE ='B+' then 3.3*UNITS " \
          "                  when GRADE ='B' then 3.0*UNITS " \
            "              when GRADE ='B-' then 2.7*UNITS " \
            "                when GRADE ='C+' then 2.3*UNITS "\
            "                when GRADE ='C' then 2*UNITS " \
              "              when GRADE ='C-' then 1.7*UNITS "\
                "            when GRADE ='D+' then 1.3*UNITS " \
                  "          when GRADE ='D' then 1*UNITS " \
                    "        when GRADE ='D-' then 0.7*UNITS "\
                      "      else 0.0 " \
                  " end " \
                  " )/ " \
                  " SUM (case "\
                  " when GRADE ='A+' then 1.0*UNITS " \
                  " when GRADE ='A' then 1.0*UNITS " \
                  " when GRADE ='A-'then 1.0*UNITS " \
                  " when GRADE ='B+' then 1.0*UNITS " \
                  " when GRADE ='B' then 1.0*UNITS " \
                  " when GRADE ='B-' then 1.0*UNITS " \
                  "             when GRADE ='C' then 1.0*UNITS " \
            "       when GRADE ='C+' then 1.0*UNITS " \
      "             when GRADE ='C-' then 1.0*UNITS " \
      "             when GRADE ='D+' then 1.0*UNITS " \
            "             when GRADE ='D' then 1.0*UNITS " \
      "             when GRADE ='D-' then 1.0*UNITS " \
      "             else 0.0 " \
      "             end " \
      "             ),4) AS AG " \
   " FROM STUDENTS " \
   " WHERE MAJOR LIKE '%ABC%' " \
   " OR MAJOR LIKE '%DEF%' " \
   " GROUP BY TERM,SID " \
   " HAVING SUM(UNITS) = " +to_string(i) +" " \
   " AND SUM (case " \
   " when GRADE ='A+' then 1.0*UNITS " \
   " when GRADE ='A' then 1.0*UNITS " \
   " when GRADE ='A-'then 1.0*UNITS " \
   " when GRADE ='B+' then 1.0*UNITS " \
   " when GRADE ='B' then 1.0*UNITS " \
   " when GRADE ='B-' then 1.0*UNITS " \
   " when GRADE ='C+' then 1.0*UNITS " \
   " when GRADE ='C' then 1.0*UNITS " \
   " when GRADE ='C-' then 1.0*UNITS " \
   " when GRADE ='D+' then 1.0*UNITS " \
   " when GRADE ='D' then 1.0*UNITS " \
   " when GRADE ='D-' then 1.0*UNITS " \
   " else 0.0 " \
   " end " \
   " ) <> 0) T1 " \
   " GROUP BY TERM) " \
   " SELECT T4.TERM AS TERM, AG_GRADE AS AG_GRADE" +to_string(i) +" INTO GPA" + to_string(i) + " " \
   " FROM GPA_TABLE " \
   " RIGHT OUTER JOIN " \
   " (SELECT DISTINCT TERM FROM STUDENTS)T4 "\
   " ON GPA_TABLE.TERM = T4.TERM "\
   " ORDER BY T4.TERM;" ;
    W.exec(sql3);
  }


sql3 = "SELECT GPA1.TERM";
for(int i = 1; i<= 20 ; i++){
  sql3 += ",AG_GRADE" + to_string(i);
}
sql3 += " FROM(GPA1";

for(int i = 2; i <= 20;i++){
  sql3 += " FULL OUTER JOIN GPA" + to_string(i)+ " ON GPA1.TERM = GPA" + to_string(i)+".TERM ";
}
sql3 += ");";
r = W.exec(sql3);

for (int rownum = 0; rownum < r.size(); ++rownum) {
      const result::tuple row = r[rownum];
      for (int colnum = 0; colnum < row.size(); ++colnum) {
          const result::field myField = row[colnum];
          if(strlen(myField.c_str()) == 0){std::cout << "       ";}
          else{std::cout << myField.c_str() << ' ';}
      }
      std::cout << std::endl;
  }
}

  else if(proNum == 'd'){
//p2.D

sql4 = "WITH RATIO_TABLE AS( "\
       "WITH PASS_TABLE AS  "\
       "(SELECT SUBJ,CRSE, SUM (CASE "\
        "           when GRADE = 'F' then 0 "\
      "             when GRADE = 'NP' then 0 "\
      "             when GRADE = 'NS' then 0 "\
      "             when GRADE = 'F' then 0 "\
      "             when GRADE = 'U' then 0.0 "\
      "             when GRADE ='A+' then 1.0 "\
      "             when GRADE ='A' then 1.0 "\
      "             when GRADE ='A-'then 1.0 "\
      "             when GRADE ='B+' then 1.0 "\
      "             when GRADE ='B' then 1.0 "\
      "             when GRADE ='B-' then 1.0 "\
      "             when GRADE ='C+' then 1.0 "\
      "             when GRADE ='C' then 1.0 "\
      "             when GRADE ='C-' then 1.0 "\
      "             when GRADE ='D+' then 1.0 "\
      "             when GRADE ='D' then 1.0 "\
      "             when GRADE ='D-' then 1.0 "\
      "             when GRADE ='P' then 1.0 "\
            "             when GRADE = 'S' then 1.0 "\
      "             else 0.0 "\
      "             end "\
      "             ) AS PASS_NUM, "\
      "             SUM (CASE "\
      "                               when GRADE = 'F' then 1.0 "\
      "                               when GRADE = 'NP' then 1.0 "\
      "                               when GRADE = 'NS' then 1.0 "\
      "                               when GRADE = 'F' then 1.0 "\
                        "             when GRADE = 'U' then 0.0 "\
      "                               when GRADE ='A+' then 1.0 "\
      "                               when GRADE ='A' then 1.0 "\
      "                               when GRADE ='A-'then 1.0 "\
      "                               when GRADE ='B+' then 1.0 "\
      "                               when GRADE ='B' then 1.0 "\
      "                               when GRADE ='B-' then 1.0 "\
      "                               when GRADE ='C+' then 1.0 "\
      "                               when GRADE ='C' then 1.0 "\
      "                               when GRADE ='C-' then 1.0 "\
      "                               when GRADE ='D+' then 1.0 "\
      "                               when GRADE ='D' then 1.0 "\
      "                               when GRADE ='D-' then 1.0 "\
      "                               when GRADE ='P' then 1.0 "\
                        "             when GRADE = 'S' then 1.0 "\
      "                               else 0.0 "\
      "                               end "\
      "                               ) AS TOTAL_NUM "\
    "  FROM (SELECT COURSES.SUBJ,COURSES.CRSE, STUDENTS.GRADE "\
    "               FROM COURSES INNER JOIN STUDENTS "\
    "               ON COURSES.CID = STUDENTS.CID)T1 "\
  "    GROUP BY SUBJ,CRSE "\
"   HAVING                   SUM (CASE "\
"                                       when GRADE = 'F' then 1.0 "\
"                                       when GRADE = 'NP' then 1.0 "\
"                                       when GRADE = 'NS' then 1.0 "\
"                                       when GRADE = 'F' then 1.0 "\
                          "             when GRADE = 'U' then 0.0 "\
"                                       when GRADE ='A+' then 1.0 "\
"                                       when GRADE ='A' then 1.0 "\
"                                       when GRADE ='A-'then 1.0 "\
"                                       when GRADE ='B+' then 1.0 "\
"                                       when GRADE ='B' then 1.0 "\
"                                       when GRADE ='B-' then 1.0 "\
"                                       when GRADE ='C+' then 1.0 "\
"                                       when GRADE ='C' then 1.0 "\
"                                       when GRADE ='C-' then 1.0 "\
"                                       when GRADE ='D+' then 1.0 "\
"                                       when GRADE ='D' then 1.0 "\
"                                       when GRADE ='D-' then 1.0 "\
"                                       when GRADE ='P' then 1.0 "\
                          "             when GRADE = 'S' then 1.0 "\
"                                       else 0.0 "\
"                                       end "\
"                                       )  <> 0 "\
                                      " ORDER BY PASS_NUM) "\
                                        "SELECT SUBJ,CRSE, ROUND(PASS_NUM/TOTAL_NUM,4) AS PASSING_RATE "\
                                        "FROM PASS_TABLE "\
                                        "WHERE TOTAL_NUM <>0 "\
                                        "ORDER BY PASS_NUM/TOTAL_NUM) "\
                                        "SELECT SUBJ,CRSE, PASSING_RATE "\
                                        "FROM RATIO_TABLE "\
                                        "WHERE PASSING_RATE = (SELECT MAX(PASSING_RATE) FROM RATIO_TABLE) "\
                                        "UNION "\
                                        "SELECT SUBJ,CRSE, PASSING_RATE "\
                                        "FROM RATIO_TABLE "\
                                        "WHERE PASSING_RATE = (SELECT MIN(PASSING_RATE) FROM RATIO_TABLE) "\
                                        "ORDER BY PASSING_RATE;";
    r = W.exec(sql4);
    for (int rownum = 0; rownum < r.size(); ++rownum) {
          const result::tuple row = r[rownum];
          for (int colnum = 0; colnum < row.size(); ++colnum) {
              const result::field myField = row[colnum];
              if(strlen(myField.c_str()) == 0){std::cout << "      ";}
              else{std::cout << myField.c_str() << ' ';}
          }
          std::cout << std::endl;
      }
    }
//P2.E
      else if (proNum == 'e'){
      sql5 = "WITH CR_TABLE AS ( "\
      "SELECT DISTINCT INSTRUCTOR,CID,TERM,SUBJ,CRSE,SECTION,DAYS,TIME,BUILD,ROOM "\
      "FROM COURSES "\
      "NATURAL JOIN "\
      "( "\
      "SELECT TERM, DAYS, TIME,BUILD, ROOM "\
      "FROM COURSES "\
      "WHERE UNITS IS NOT NULL "\
      "AND (TYPE LIKE '%Lecture%' OR TYPE LIKE '%Laboratory%') "\
      "GROUP BY TERM,SECTION, DAYS, TIME,BUILD, ROOM "\
      "HAVING COUNT(*) > 1 "\
      "AND DAYS NOT LIKE '%NULL%' "\
      "AND TIME NOT LIKE '%NULL%' "\
      "AND BUILD NOT LIKE '%NULL%' "\
      "AND ROOM NOT LIKE '%NULL%') T2 "\
      "WHERE SECTION LIKE '%1%' "\
      "ORDER BY INSTRUCTOR,TERM,CRSE,SUBJ) "\
      "SELECT DISTINCT SUBJ,CRSE,TERM,INSTRUCTOR "\
      "FROM CR_TABLE "\
      "ORDER BY TERM,INSTRUCTOR;";
      r = W.exec(sql5);
      for (int rownum = 0; rownum < r.size(); ++rownum) {
            const result::tuple row = r[rownum];
            for (int colnum = 0; colnum < row.size(); ++colnum) {
                const result::field myField = row[colnum];
                if(strlen(myField.c_str()) == 0){std::cout << "      ";}
                else{std::cout << myField.c_str() << ' ';}
            }
            std::cout << std::endl;
          }
}


// P2.F
else if(proNum == 'f'){
    cout << "For ABC" << endl;
    sql6 = "WITH AVG_TABLE AS( "\
    "WITH MAJOR_TABLE AS( "\
    "SELECT MAJOR, "\
    "SUM(case "\
    "                        when GRADE ='A+' then 4.0 "\
    "                        when GRADE ='A' then 4.0 "\
    "                        when GRADE = 'A-' then 3.7 "\
    "                        when GRADE ='B+' then 3.3 "\
    "                        when GRADE ='B' then 3.0 "\
    "                        when GRADE ='B-' then 2.7 "\
    "                        when GRADE ='C+' then 2.3 "\
    "                        when GRADE ='C' then 2 "\
    "                        when GRADE ='C-' then 1.7 "\
    "                        when GRADE ='D+' then 1.3 "\
    "                        when GRADE ='D' then 1 "\
    "                        when GRADE ='D-' then 0.7 "\
    "                        else 0.0 "\
    "                        end) AS TOTAL, "\
    "SUM (case "\
    "                        when GRADE ='A+' then 1.0 "\
    "                        when GRADE ='A' then 1.0 "\
    "                        when GRADE ='A-'then 1.0 "\
    "                        when GRADE ='B+' then 1.0 "\
    "                        when GRADE ='B' then 1.0 "\
    "                        when GRADE ='B-' then 1.0 "\
    "                        when GRADE ='C+' then 1.0 "\
    "                        when GRADE ='C' then 1.0 "\
    "                        when GRADE ='C-' then 1.0 "\
    "                        when GRADE ='D+' then 1.0 "\
    "                        when GRADE ='D' then 1.0 "\
    "                        when GRADE ='D-' then 1.0 "\
    "                        else 0.0 "\
    "                        end) AS NUM "\
    "                FROM "\
    "STUDENTS  "\
    "WHERE CID IN (SELECT CID FROM COURSES WHERE SUBJ LIKE '%ABC%') "\
    "GROUP BY MAJOR) "\
    "SELECT MAJOR, TOTAL/NUM AS AVG_GPA "\
    "FROM MAJOR_TABLE "\
    "WHERE NUM <> 0 "\
    "ORDER BY AVG_GPA) "\
    "SELECT MAJOR,AVG_GPA "\
    "FROM AVG_TABLE "\
    "WHERE AVG_GPA = (SELECT MAX(AVG_GPA) FROM AVG_TABLE) "\
    "UNION "\
    "SELECT MAJOR,AVG_GPA "\
    "FROM AVG_TABLE "\
    "WHERE AVG_GPA = (SELECT MIN(AVG_GPA) FROM AVG_TABLE) "\
    "ORDER BY AVG_GPA";
    r = W.exec(sql6);
    for (int rownum = 0; rownum < r.size(); ++rownum) {
          const result::tuple row = r[rownum];
          for (int colnum = 0; colnum < row.size(); ++colnum) {
              const result::field myField = row[colnum];
              if(strlen(myField.c_str()) == 0){std::cout << "      ";}
              else{std::cout << myField.c_str() << ' ';}
          }
          std::cout << std::endl;
        }
    cout << "For DEF " << endl;
    sql6 = "WITH AVG_TABLE AS( "\
        "WITH MAJOR_TABLE AS( "\
        "SELECT MAJOR, "\
        "SUM(case "\
        "                        when GRADE ='A+' then 4.0 "\
        "                        when GRADE ='A' then 4.0 "\
        "                        when GRADE = 'A-' then 3.7 "\
        "                        when GRADE ='B+' then 3.3 "\
        "                        when GRADE ='B' then 3.0 "\
        "                        when GRADE ='B-' then 2.7 "\
        "                        when GRADE ='C+' then 2.3 "\
        "                        when GRADE ='C' then 2 "\
        "                        when GRADE ='C-' then 1.7 "\
        "                        when GRADE ='D+' then 1.3 "\
        "                        when GRADE ='D' then 1 "\
        "                        when GRADE ='D-' then 0.7 "\
        "                        else 0.0 "\
        "                        end) AS TOTAL, "\
        "SUM (case "\
        "                        when GRADE ='A+' then 1.0 "\
        "                        when GRADE ='A' then 1.0 "\
        "                        when GRADE ='A-'then 1.0 "\
        "                        when GRADE ='B+' then 1.0 "\
        "                        when GRADE ='B' then 1.0 "\
        "                        when GRADE ='B-' then 1.0 "\
        "                        when GRADE ='C+' then 1.0 "\
        "                        when GRADE ='C' then 1.0 "\
        "                        when GRADE ='C-' then 1.0 "\
        "                        when GRADE ='D+' then 1.0 "\
        "                        when GRADE ='D' then 1.0 "\
        "                        when GRADE ='D-' then 1.0 "\
        "                        else 0.0 "\
        "                        end) AS NUM "\
        "                FROM "\
        "STUDENTS  "\
        "WHERE CID IN (SELECT CID FROM COURSES WHERE SUBJ LIKE '%DEF%') "\
        "GROUP BY MAJOR) "\
        "SELECT MAJOR, TOTAL/NUM AS AVG_GPA "\
        "FROM MAJOR_TABLE "\
        "WHERE NUM <> 0 "\
        "ORDER BY AVG_GPA) "\
        "SELECT MAJOR,AVG_GPA "\
        "FROM AVG_TABLE "\
        "WHERE AVG_GPA = (SELECT MAX(AVG_GPA) FROM AVG_TABLE) "\
        "UNION "\
        "SELECT MAJOR,AVG_GPA "\
        "FROM AVG_TABLE "\
        "WHERE AVG_GPA = (SELECT MIN(AVG_GPA) FROM AVG_TABLE) "\
        "ORDER BY AVG_GPA";
        r = W.exec(sql6);
        for (int rownum = 0; rownum < r.size(); ++rownum) {
              const result::tuple row = r[rownum];
              for (int colnum = 0; colnum < row.size(); ++colnum) {
                  const result::field myField = row[colnum];
                  if(strlen(myField.c_str()) == 0){std::cout << "      ";}
                  else{std::cout << myField.c_str() << ' ';}
              }
              std::cout << std::endl;
            }
          }
    //P2.G
    else if(proNum == 'g'){
    sql7 = "WITH TRANS_TABLE AS ( "\
    "WITH TMP AS ( "\
    "SELECT DISTINCT T1.SID AS SID, T1.MAJOR AS OLD_M, "\
    "T1.TERM AS OLD_T,T2.MAJOR AS NEW_M,T2.TERM AS NEW_T "\
    "FROM STUDENTS AS T1 "\
    "INNER JOIN "\
    "STUDENTS AS T2 "\
    "ON T1.SID = T2.SID "\
    "AND T1.MAJOR <> T2.MAJOR "\
    "AND T1.MAJOR NOT LIKE '%ABC%' "\
    "AND T2.MAJOR LIKE '%ABC%' "\
    "AND T1.TERM <= T2.TERM) "\
    "SELECT DISTINCT SID,OLD_M,NEW_M "\
    "FROM TMP t1 "\
    "WHERE NOT EXISTS(  "\
    "SELECT * FROM TMP t2 "\
    "WHERE t1.SID = t2.SID "\
    "AND t1.OLD_M = t2.OLD_M "\
    "AND t1.OLD_T > t2.OLD_T )) "\
    "SELECT OLD_M AS MAJOR, COUNT(*), COUNT(*)*1.0/(SELECT COUNT(*) FROM TRANS_TABLE) AS PER "\
    "FROM TRANS_TABLE "\
    "GROUP BY OLD_M "\
    "ORDER BY COUNT(*) DESC "\
    "LIMIT 5;";
    r = W.exec(sql7);
    for (int rownum = 0; rownum < r.size(); ++rownum) {
          const result::tuple row = r[rownum];
          for (int colnum = 0; colnum < row.size(); ++colnum) {
                const result::field myField = row[colnum];
                if(strlen(myField.c_str()) == 0){std::cout << "      ";}
                else{std::cout << myField.c_str() << ' ';}
                }
              std::cout << std::endl;
            }
    }
    else if(proNum == 'h'){
    sql8 = "WITH TRANS_TABLE AS ( "\
      "WITH TMP AS ( "\
      "SELECT DISTINCT T1.SID AS SID, T1.MAJOR AS OLD_M, "\
      "T1.TERM AS OLD_T,T2.MAJOR AS NEW_M,T2.TERM AS NEW_T "\
      "FROM STUDENTS AS T1 "\
      "INNER JOIN "\
      "STUDENTS AS T2 "\
      "ON T1.SID = T2.SID "\
      "AND T1.MAJOR <> T2.MAJOR "\
      "AND T1.MAJOR LIKE '%ABC%' "\
      "AND T2.MAJOR NOT LIKE '%ABC%' "\
      "AND T1.TERM < T2.TERM) "\
      "SELECT DISTINCT SID,OLD_M,NEW_M "\
      "FROM TMP t1 "\
      "WHERE NOT EXISTS(  "\
      "SELECT * FROM TMP t2 "\
      "WHERE t1.SID = t2.SID "\
      "AND t1.NEW_M = t2.NEW_M "\
      "AND t1.NEW_T > t2.NEW_T )) "\
      "SELECT NEW_M AS MAJOR, COUNT(*), COUNT(*)*1.0/(SELECT COUNT(*) FROM TRANS_TABLE) AS PER "\
      "FROM TRANS_TABLE "\
      "GROUP BY NEW_M "\
      "ORDER BY COUNT(*) DESC "\
      "LIMIT 5;";
    r = W.exec(sql8);
    for (int rownum = 0; rownum < r.size(); ++rownum) {
          const result::tuple row = r[rownum];
          for (int colnum = 0; colnum < row.size(); ++colnum) {
                const result::field myField = row[colnum];
                if(strlen(myField.c_str()) == 0){std::cout << "      ";}
                else{std::cout << myField.c_str() << ' ';}
                }
              std::cout << std::endl;
    }
  }
  else if(proNum == 'x'){cout << "End. " << endl;}
  else{
    cout << "Not Valid Num." << endl;
  }
}


    C.disconnect();

  return 0;
}
