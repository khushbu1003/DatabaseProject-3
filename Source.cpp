//***************************************************************************
//*      Name: Khushbu Shah                                                 *
//*      Project 2 CSC 30500                                                *
//*      DataBase Management                                                *
//*      Date : 11 - 29 - 2020                                              *
//*                                                                         *
//*                                                                         *
//*                                                                         *
//*                                                                         *
//*                                                                         *
//***************************************************************************



#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>

#include <mysql/jdbc.h> 

#include <conio.h>

using namespace std;

// reads in a password without echoing it to the screen
string myget_passwd()
{
	string passwd;

	for (;;)
	{
		char ch = 0;
		while (ch == 0)   // Ugh. Microsoft not following standards on getch, so need to keep reading until a new char is typed. 
			ch = _getch();           // get char WITHIOUT echo! (should really be getch(), but MS is not following standrads again.)
		if (ch == 13 || ch == 10) // if done ...
			break;           //  stop reading chars!
		cout << '*';  // dump * to screen
		passwd += ch;   // addd char to password
	}
	cin.sync(); // flush anything else in the buffer (remaining newline)
	cout << endl;  // simulate the enter that the user pressed

	return passwd;
}

int main()
{


	// strings for mysql hostname, userid, ...
	string db_host;
	string db_user;
	string db_password;
	string db_name;

	// set up the client (this machine) to use mysql
	cout << "initializing client DB subsystem ..."; cout.flush();
	//mysql_init(&mysql);
	sql::Driver* driver = sql::mysql::get_driver_instance();
	cout << "Done!" << endl;

	// get user credentials and mysql server info
	cout << "Enter MySQL database hostname (or IP adress):";
	cin >> db_host;

	cout << "Enter MySQL database username:";
	cin >> db_user;

	cout << "Enter MySQL database password:";
	db_password = myget_passwd();

	// could also prompt for this, if desired
	//db_name = db_user;
	cout << "Enter a MySQL Database Name: ";
	cin >> db_name;       //same as db_user in my case


	try
	{
		// go out and connect to the mysql server
		cout << "Connecting to remote DB ..."; cout.flush();
		sql::Connection* conn = driver->connect(db_host, db_user, db_password);
		conn->setSchema(db_name);

		//conn = mysql_real_connect(&mysql,
			//db_host.c_str(), db_user.c_str(), db_password.c_str(), db_user.c_str(),
			//0, 0, 0); // last three are usually 0's


		cout << "DB connection established" << endl;


		cout << "Creating table if need to be.." << endl;


		sql::Statement* stmt = conn->createStatement();
		sql::ResultSet* res;
		//this will hold the string creating tables
		int TableStatus;

		//create table for courses
		string Courses = "create table if not exists Courses (CoursePrefix varchar(100) NOT NULL, code int NOT NULL PRIMARY KEY,"
			"CourseName varchar(100) NOT NULL, CreditHr int NOT NULL);";

		TableStatus = stmt->execute(Courses.c_str()); //this creates table courses into SQL server

		//create tables for grades
		string Grades = "create table if not exists Grades(CourseGrades char(2) NOT NULL, GradVal double NOT NULL);";
		TableStatus = stmt->execute(Grades.c_str());   //creates table Grades into the server

		//create table for semester
		string Semester = "create table if not exists Semester (semestercode varchar(10) NOT NULL, semesteryear int NOT NULL PRIMARY KEY,"
			"semestername varchar(10) NOT NULL);";
		TableStatus = stmt->execute(Semester.c_str());    //creates table Semester into the sql server

		//create table for students
		string Students = "create table if not exists Students (Lname varchar(100) NOT NULL , Fname varchar(100) NOT NULL , PhoneNumber varchar(100) NOT NULL PRIMARY KEY);";
		TableStatus = stmt->execute(Students.c_str());    //creates table Student into the server

		//create table for Transcript
		string Transcript = "create table if not exists \Transcript (\LastName char(100) NOT NULL, \FirstName char(100) NOT NULL, \CoursePrefix char(3) NOT NULL,"
			"code int NOT NULL ,CourseGrades char(2) NOT NULL, semestercode char(4) NOT NULL);";
               /*PRIMARY KEY(LastName, FirstName, CoursePrefix)
                CONSTRAINT fk_firstname FOREIGN KEY(FirstName) \
                   REFERENCES Student(Lname) ON DELETE CASCADE ON UPDATE CASCADE,\
                  CONSTRAINT fk_lastname FOREIGN KEY(LastName) \
				 REFERENCES Student(Lname) ON DELETE CASCADE ON UPDATE CASCADE,\
                  CONSTRAINT fk_courseprefix FOREIGN KEY(CoursePrefix) \
				 REFERENCES Courses(CoursePrefix) ON DELETE CASCADE ON UPDATE CASCADE)*/
		     TableStatus = stmt->execute(Transcript.c_str());     //creates table transcript into the Server


		cout << "Done Creating" << endl;
		//you need to create a table that your PROGRAM NEEDS

		
		// repeats until user quits with q

		char command;   //holds user command
		char type;     //holds user command type
		



		cout << ">>>";
		do {
			// prompt for command

			cin >> command;
			switch (command)
			{
				// user wants to add 
			case 'a':
				cout << ">>>" << "Enter Type:" << endl;
				cin >> type;
				if (type == 'c')    //to add courses
				{
					string CoursePrefix;           // string for course prefix like CSC
					string CourseName;             //Course Title like computer system
					int code, CreditHr;           //30500 and credit hour

					cin >> CoursePrefix >> code >> CourseName >> CreditHr;   //input declared variables

					cout << CoursePrefix << " " << code << " " << CourseName << " " << CreditHr << " " << endl;   //output declared variables

					// this string holds the inserting courses into the table
					string InsertValue = "INSERT INTO Courses (CoursePrefix, code, CourseName, CreditHr) VALUES(\"";
					InsertValue += CoursePrefix + "\", \"";
					InsertValue += to_string(code) + "\",\"";
					InsertValue += CourseName + "\", \"";
					InsertValue += to_string(CreditHr) + "\");";



					cout << InsertValue << endl;  //to check

					stmt->execute(InsertValue);   //execute those values to the table in sql
				}

				else if (type == 'g')
				{ // adds grades

					string CourseGrades;             //declaring grade type A,B etc
					double GradeValue;          //Declaring value according to grades. user can manually do that

					cin >> CourseGrades >> GradeValue;   //user input these var
					cout << CourseGrades << " " << GradeValue << " " << endl;

					 //this string variable holds the inserting value that user enter
					string InsertValue = "INSERT INTO Grades (CourseGrades, GradVal ) VALUES(\"";
					InsertValue += CourseGrades + "\", \"";
					InsertValue += to_string(GradeValue) + "\");";


					cout << InsertValue << endl;  //to check

					stmt->execute(InsertValue);    //sending values to the sql server

				}

				else if (type == 'm')
				{ // adds grades

					string semesterCode;        //F86, Sp89
					int year;                   //Declare year
					string term;                //spring , summer etc


					cin >> semesterCode >> year >> term;             //inputting semester code, year and term
					cout << semesterCode << " " << year << " " << term << " " << endl;

					//this string variable holds the inserting values that user enter for semester
					string InsertValue = "INSERT INTO Semester (semestercode, semesteryear , semestername) VALUES(\"";
					InsertValue += semesterCode + "\", \"";
					InsertValue += to_string(year) + "\", \"";
					InsertValue += term + "\");";

					cout << InsertValue << endl;  //to check

					stmt->execute(InsertValue);    //sending values to the table in sql server


				}

				else if (type == 's')
				{ // adds student

					string FirstName;      //holds firstname
					string LastName;       //holds lastname
					string PhoneNumber;    //holds phone number

					cin >> LastName >> FirstName >> PhoneNumber;

					cout << LastName << " " << FirstName << " " << PhoneNumber << " " << endl;

					//this string will hold student information user enters
					string InsertValue = "INSERT INTO Students (Lname, Fname, PhoneNumber) VALUES(\"";
					InsertValue += LastName + "\", \"";
					InsertValue += FirstName + "\", \"";
					InsertValue += PhoneNumber + "\");";

					cout << InsertValue << endl;  //to check

					stmt->execute(InsertValue);     //sending values to the table

				}

				//if user enter t to add transcript
				else if (type == 't')
				{ // adds grades

					string lname;      //holds lastname
					string fname;      //holds firstname
					string CoursePrefix;        //F86,sp89
					int CourseCode;      //35500 etc
					string grade;      //holds grade such B, A
					string TermCode;   //holds term code , sp20 etc

					//reading in all values 
					cin >> lname >> fname >> CoursePrefix >> CourseCode >> grade >> TermCode;
					cout << lname << " " << " " << fname << " " << CoursePrefix << " " << CourseCode << " " << grade << " " << TermCode << " " << endl;

					//this string will hold the transcript values nad will keep adding to the table
					string InsertValue = "INSERT INTO Transcript (LastName, FirstName, CoursePrefix,code, CourseGrades, semestercode) VALUES(\"";
					InsertValue += fname + "\", \"";
					InsertValue += lname + "\", \"";
					InsertValue += CoursePrefix + "\" , \"";
					InsertValue += to_string(CourseCode) + "\",\"";
					InsertValue += grade + "\", \"";
					InsertValue += TermCode + "\");";

					cout << InsertValue << endl;  //to check

					stmt->execute(InsertValue);   //sends value to the server

				}
				break;

				//*************************For Listing*********************************************
			    case 'l':       //l command for listing
				cin >> type;    //user will input the command type
				if (type == 'c')     //'c' to get the list of courses
				{
					cout << "Sending Query ... "<<endl; cout.flush();  //sending queries to the table
					sql::Statement* stmt = conn->createStatement();
					sql::ResultSet* res = stmt->executeQuery("SELECT *from Courses");  //executing the values from courses 
																						//table that inserted into add command into the server
					//checking by getting output on the screen
					while (res->next())
					{
						cout << res->getString("CoursePrefix") << " " << res->getString("code") << " " << res->getString("CourseName") << " " <<
							res->getString("CreditHr") << " " << endl;   //go look for attribute
					}

					

				}

				else if (type == 'g')  //command for listing grades
				 {
					cout << "Sending Query ... " << endl; cout.flush();
					sql::Statement* stmt = conn->createStatement();  
					sql::ResultSet* res = stmt->executeQuery("SELECT *from Grades");   //executing the values from Grades
																						//table that inserted into add command into the server

					while (res->next())
					{
						cout << res->getString("CourseGrades") << " " << res->getString("code") << " " << endl;
					}



				}

				else if(type == 'm')   //command for listing semesters
				{
					cout << "Sending Query ... "<<endl; cout.flush();
					sql::Statement* stmt = conn->createStatement();
					sql::ResultSet* res = stmt->executeQuery("SELECT *from Semester");    //executing the values from semester
																						 //table that inserted into add command into the server

					while (res->next())
					{
						cout << res->getString("semestercode") << " " << res->getString("semesteryear") << " " << res->getString("semestername") << " " << endl;

					}



				}
				else if(type == 's')   //command for listing students 
				{
					cout << "Sending Query ... "<<endl; cout.flush();
					sql::Statement* stmt = conn->createStatement();
					sql::ResultSet* res = stmt->executeQuery("SELECT *from Students"); //executing the values from Students
																						//table that inserted into add command into the server

					while (res->next())
					{
						cout << res->getString("LName") << " " << res->getString("FName") << " " << res->getString("PhoneNumber") << " " << endl;

					}



				}

	
				else if (type == 't')     //command for listing transcripts
				{
					string firstname;     //holds firstname
					string lastname;      //holds lastname
					cout << "Enter Firstname and LastName" << endl;
					cin >> firstname >> lastname;
					cout << "Sending Query ... "<<endl; cout.flush();
					sql::Statement* stmt = conn->createStatement();
					//this string will match the last and first name that user entered and print corresponding result
					string ListTranscript = "SELECT Students.Lname, Students.Fname, CoursePrefix, code, CourseName,CreditHr,CourseGrades,GradVal \
                                              FROM Students,Courses,Grades  \
                                              WHERE(((Courses.CoursePrefix) = CoursePrefix) AND ((Courses.code)= code) AND ((Courses.CourseName) = Coursename))\
                                               ORDER BY Semester.semesteryear, Semester.semestername;";

					//then print the GPA from grades table and count average
					string CalculateGPA = "SELECT AVG(GradeVal) FROM Grades;";

					cout << CalculateGPA<<endl;   //to check
					cout << ListTranscript << endl;

					sql::ResultSet* res = stmt->executeQuery(ListTranscript);  //send the values to the server 

					 stmt->executeQuery(CalculateGPA);    //send calculation to the server


			        //to check
					while (res->next())
					{
						cout << res->getString("LastName") << " " << res->getString("FirstName") << " " << res->getString("CoursePrefix") << " " << res->getInt("code") << res->getString("CourseGrades") <<
									res->getString("semestercode") << endl;

					}

				}
				
				case 'q':      //when user enter q, it will quit the program
					break;

				case 'd':   //for deleting the record
				{
					
					
					string _Lname, _Fname;   //holds the lastname and firstname
						
					cout << "Enter Last and Firstname that you want to delete from the Query" << endl;
					// reading user's input
					cin >> _Lname;      //reading in lastname
					cin >> _Fname;      //reading in firstname
					
					// Build delete query that will delete records from chosen student 
					string DeleteValue = "DELETE FROM Transcript WHERE ( LastName= \"";
					DeleteValue += _Lname + "\" AND FirstName = \"";
					DeleteValue += _Fname + "\" AND  Transcript.CoursePrefix \"";
					                        "\" AND  Transcript.code  \"" 
										   "\" AND Transcript.CourseName \"" "\");";

					sql::ResultSet* res = stmt->executeQuery(DeleteValue);     //sending values to the server
					
				}

				
			}return 0;

		} while (command != 'q');
	}
	catch (sql::SQLException sqle)    //catch any errors program have 
	{
		cout << "Exception in SQL: " << sqle.what() << endl;
	}


}


