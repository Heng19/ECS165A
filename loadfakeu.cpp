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

struct course{
  string CID;
  string Term;
  string Subj;
  string Crse;
  string Section;
  string Units;
  string Instructor;
  string Type;
  string Days;
  string Time;
  string Build;
  string Room;
};

struct student{
  string Term;
  string CID;
  string Seat;
  string SID;
  string Surname;
  string PrefName;
  string Level;
  string Units;
  string Class;
  string Major;
  string Grade;
  string Status;
  string Email;

};
class Data{
  public:
  vector<course> Courses;
  vector<student> Students;
};
Data add(Data d1, Data d2){

   d1.Courses.insert(d1.Courses.end(),d2.Courses.begin(),d2.Courses.end());
   d1.Students.insert(d1.Students.end(),d2.Students.begin(),d2.Students.end());
  return d1;
}
vector<string> split(string s, string delimiter) {
  //  cout << s << endl;
    int start = 0;
    int end;
    string token;
    vector<string> res;
    while ((end = s.find(delimiter, start)) != -1) {
        token = s.substr(start, end - start);
        start = end + delimiter.length();
        res.push_back(token);
    }
    res.push_back(s.substr(start));
    return res;
}

Data Read(const char* filename){
  ifstream file;
  file.open(filename);
  Data data;
  bool C1 = false;
  bool C2 = false;
  bool STUDENT = false;
  bool Same_CRM;
  string CRM;
  string Term;
  string value;
  string Instructor;
  string CID;
  course c1;
  student s1;
//  vector<course> Courses;
//  vector<student> Students;
  while ( file.good() )
  {
       getline ( file, value, '\n' );
       vector<string> strs = split(value, "\",\"");

       if(strs[0] == "\"CID"){
         C1 = true;
         continue;
       }
       if(strs[0] == "\"INSTRUCTOR(S)"){
         C2 = true;
         continue;
       }
       if(strs[0] == "\"SEAT"){
         STUDENT = true;
         continue;
       }
       if(C1 == true){
         strs[0].erase(strs[0].begin());
         strs[5].erase(strs[5].end()-1);
         strs[5].erase(strs[5].end()-1);
         for(int i = 0; i<= 5; i++){
           if(strs[0] == ""){strs[i] = "NULL";}
         }
         Term = strs[1];
         CID = strs[0];

         c1.CID = strs[0];
         c1.Term = strs[1];
         c1.Subj = strs[2];
         Same_CRM = CRM == strs[3];
         CRM = strs[3];
         c1.Crse = strs[3];
         c1.Section = strs[4];
         c1.Units = strs[5];

         C1 = false;
         continue;
         }
       if (C2 == true) {
         if(strs.size() == 1){
           C2 = false;
           continue;
         }
         else{
         strs[0].erase(strs[0].begin());
         strs[5].erase(strs[5].size()-1);
         strs[5].erase(strs[5].size()-1);
         if(strs[0] != "") {
           std::replace( strs[0].begin(), strs[0].end(), '\'', '\\');
           c1.Instructor = strs[0];
           Instructor = strs[0];
         }
         else{
           if(Same_CRM){c1.Instructor = Instructor;}
           else{Instructor = "";}
         }
         for(int i = 1; i <=5 ; i++){
           if(strs[i] == ""){strs[i] = "NULL";}
         }
         c1.Type = strs[1];
         c1.Days = strs[2];
         c1.Time = strs[3];
         c1.Build = strs[4];
         c1.Room = strs[5];
         data.Courses.insert(data.Courses.end(),c1);
         continue;
         }
       }
       if (STUDENT == true){
           if(strs.size() == 1){
             STUDENT = false;
             continue;
           }
           strs[0].erase(strs[0].begin());
           strs[10].erase(strs[10].end()-1);
           strs[10].erase(strs[10].end()-1);
           std::replace( strs[2].begin(), strs[2].end(), '\'', '\\');
           std::replace( strs[3].begin(), strs[3].end(), '\'', '\\');
           std::replace( strs[10].begin(), strs[10].end(), '\'', '\\');
           s1.Term = Term;
           s1.CID = CID;
           for(int i = 0; i <=10 ; i++){
             if(strs[i] == ""){strs[i] = "NULL";}
           }
           s1.Seat = strs[0];
           s1.SID = strs[1];
           s1.Surname = strs[2];
           s1.PrefName = strs[3];
           s1.Level = strs[4];
           s1.Units = strs[5];
           s1.Class = strs[6];
           s1.Major = strs[7];
           s1.Grade = strs[8];
           s1.Status = strs[9];
           s1.Email = strs[10];
           data.Students.insert(data.Students.end(),s1);
           c1.CID = "NULL";
           c1.Term = "NULL";
           c1.Subj = "NULL";
           c1.Crse = "NULL";
           c1.Section = "NULL";
           c1.Units = "NULL";
           c1.Type = "NULL";
           c1.Days = "NULL";
           c1.Time = "NULL";
           c1.Build = "NULL";
           c1.Room = "NULL";
         }
        }
        return data;
}
vector<string> open1(string path)       //opening any folder and saving all file-names in a vector<string>
{
    DIR    *dir;
    dirent *pdir;
    vector<string> files;
    dir = opendir(path.c_str());
    while (pdir = readdir(dir))
    {
        files.push_back(pdir->d_name);
    }
    return files;
}
int main(int argc, char* argv[]) {
    char path[256];
    string rel_path;

    Data data;
    vector<string> f ;
    fstream file;
    bool Default = false;
   if (argc < 2)
    {
      getcwd(path,sizeof(path));
      f = open1(path);
      Default = true;
    }
   else{
     f = open1(argv[1]);
   }
    string user_name = getenv("USER");
    try {
      string sql1;
      string sql2;
      string sql3;
      string sql4;

      connection C("dbname = FakeUData user = "+user_name);
      if (C.is_open()) {
         cout << "Opened database successfully: " << C.dbname() << endl;
      } else {
         cout << "Can't open database" << endl;
         return 1;
      }

      sql1 = "CREATE TABLE COURSES("  \
      "CID            CHAR(50)     NOT NULL," \
      "TERM           CHAR(50)    ," \
      "SUBJ           CHAR(50)     ," \
      "CRSE           CHAR(50)," \
      "SECTION        CHAR(50),"
      "UNITS          CHAR(50)," \
      "INSTRUCTOR           CHAR(50)    ," \
      "TYPE            CHAR(50)     ," \
      "DAYS        CHAR(50)," \
      "TIME         CHAR(50),"
      "BUILD         CHAR(50)    ," \
      "ROOM            CHAR(50) );";

      sql2 = "CREATE TABLE STUDENTS(" \
      "TERM           CHAR(50)      ,"\
      "CID            CHAR(50)     ," \
      "SEAT           CHAR(50)    ," \
      "SID           CHAR(50)     ," \
      "SURNAME           CHAR(50)," \
      "PREFNAME        CHAR(50),"
      "LEVEL         CHAR(50)," \
      "UNITS           INT    ," \
      "CLASS            CHAR(50)     ," \
      "MAJOR        CHAR(50)," \
      "GRADE         CHAR(50),"
      "STATUS         CHAR(50)    ," \
      "EMAIL            CHAR(50) );";

      work W(C);

      W.exec( sql1 );
      W.exec( sql2 );

      for (vector<string>::iterator s = f.begin(); s != f.end(); s++)
      {
        if (boost::ends_with(*s, ".csv")) {
            if(Default){
              rel_path = string(path) + '/' +string(*s);
              }
            else{
              rel_path = string(argv[1]) + '/' +string(*s);
              }

            int n = rel_path.length();
            char char_array[n+1];
            strcpy(char_array, rel_path.c_str());
            data = Read(char_array);

        sql3 = "INSERT INTO COURSES\
                  VALUES";
        for (int i = 0; i<= data.Courses.size()-2 ; i++){

            sql3 += "('" + data.Courses[i].CID + "','" + data.Courses[i].Term + "','" + data.Courses[i].Subj + "','" + data.Courses[i].Crse + "','"
                    +data.Courses[i].Section+ "'," + data.Courses[i].Units + ",'" + data.Courses[i].Instructor+ "','" + data.Courses[i].Type + "','"
                    + data.Courses[i].Days + "','"+ data.Courses[i].Time + "','" + data.Courses[i].Build + "','" + data.Courses[i].Room + "'),";
        }
        int i = data.Courses.size() - 1;
        sql3 += "('" + data.Courses[i].CID + "','" + data.Courses[i].Term + "','" + data.Courses[i].Subj + "','" + data.Courses[i].Crse + "','"
                  +data.Courses[i].Section+ "'," + data.Courses[i].Units + ",'" + data.Courses[i].Instructor+ "','" + data.Courses[i].Type + "','"
                  + data.Courses[i].Days + "','"+ data.Courses[i].Time + "','" + data.Courses[i].Build + "','" + data.Courses[i].Room + "');";

        sql4 = "INSERT INTO STUDENTS\
                VALUES";

      for (int i = 0; i<= data.Students.size()-2 ; i++){
            sql4 += "('" + data.Students[i].Term + "','"+ data.Students[i].CID + "','" + data.Students[i].Seat + "','" + data.Students[i].SID + "','" + data.Students[i].Surname + "','"
                    +data.Students[i].PrefName+ "','" + data.Students[i].Level + "'," + data.Students[i].Units+ ",'" + data.Students[i].Class + "','"
                    + data.Students[i].Major + "','"+ data.Students[i].Grade + "','" + data.Students[i].Status + "','" + data.Students[i].Email + "'),";
             }
        i = data.Students.size()-1;
        sql4  += "('" + data.Students[i].Term + "','" + data.Students[i].CID + "','" + data.Students[i].Seat + "','" + data.Students[i].SID + "','" + data.Students[i].Surname + "','"
                  +data.Students[i].PrefName+ "','" + data.Students[i].Level + "'," + data.Students[i].Units+ ",'" + data.Students[i].Class + "','"
                  +data.Students[i].Major + "','"+ data.Students[i].Grade + "','" + data.Students[i].Status + "','" + data.Students[i].Email + "');";
            W.exec( sql3 );
            W.exec( sql4 );
        }
      }

      W.commit();

      C.disconnect ();
   } catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return 1;
   }

}
