/*	
	Ben Yoder
	C++ AssignmentFour, Loan Payment
	Explanation: Take user input and print out the users monthly payments for a loan
	10/19/11
*/

#include<iostream>
#include<iomanip>
#include<cmath>
#include<fstream>
#include<string>
#include <cstring>
#include <sstream>
#include <cstdlib>

using namespace std;

//function templates
template <typename T> std::string to_string(T const& value);
template <typename T> void setZero(T &a, T &b, T &c, T &d, T &e, T &f, T &g, T &h, T &i, T &j,
								   T &k, T &l, T &m, T &n, T &o, T &p, T &q, T &r, T &s);

//function prototypes
char error( char& x );
int printChoices();
int alignment( int n );
double numericCheckNum( char *numToString );
void printTable();
void openFile( double& inLoan, double& inAnnualRate, int& inTimeYears, int& inMonth);
void replaceFile( double& outLoan, double& outAnnualRate, int& outTimeYears, int& outMonth);
void calcMonthlyValues(double& interest, double & balance, double& appliedPay, double& monthlyRate, 
		double& monthlyPayment, int timeMonths);
void calcYearlyValues( double& yearlyInterest, double& yearlyPayment, double& yearlyAppliedPay);
double payment( double rate, double loan, int months);
double roundMe( double x);
double addTotal( double y, double z);

int main()
{
	ifstream iFile;
	//opens file D:\amort.txt
	iFile.open("D:\\amort.txt");

	//declare variables
			int timeMonths,		//number of monthly payments
				timeYears,		//number of years for payment
				payNum,			//payments
				currentMonth,	//current month during the year
				yearlyPrint,	//control for yearlyPrint
				choice,			//control for switch statement
				spacing;		//used for spacing in output format
			double	loan,				//initial amount borrowed
					annualRate,			//annual interest rate
					monthlyRate,		//monthly interest rate
					monthlyPayment,		//amount to pay monthly
					interest,			//currentinterest to be paid
					balance,			//currentBalance to be paid
					appliedPay,			//amount of payment applied to principle
					yearlyInterest,		//yearly totals
					yearlyPayment,		//----------
					yearlyAppliedPay,	//----------
					totalInterest,		//grand totals
					totalPayment,		//----------
					totalAppliedPay;		//----------
			char	answerOpen,		//user input for openfile
					answerClose;		//user input for closefile
			char	charLoan[8],			//char arrays for temporary storage
					charRate[8],			//used for numericCheckNum for string manipulation
					charYears[8],
					charMonth[3];
			bool	switchControl = false;	//used for menu switch statement

	//beginning loop to keep program in constant run, variable exit is loop control *menu*
	cout <<"Welcome*************************************************************************\n";
	char exit = 'n',		//exit
		 skipMenu = 'n';	//reset variables, skip menu to continue reading from file
	do{	//reset program, back to menu
			
			//initialize variables, reset variables if program loops
			setZero(loan, annualRate, monthlyRate, monthlyPayment, interest, balance, appliedPay, yearlyInterest,
					yearlyPayment, yearlyAppliedPay, totalInterest, totalPayment, totalAppliedPay, timeMonths, timeYears,
					payNum, currentMonth, yearlyPrint, choice, spacing);

			answerOpen = 'n',		//user input for openfile
			answerClose = 'n';		//user input for closefile

			charLoan[8],			//char arrays for temporary storage
			charRate[8],			//used for numericCheckNum for string manipulation
			charYears[8],
			charMonth[3];

			switchControl = false;	//used for menu switch statement
		do{ //continue reading from file

			//input
		
			if( skipMenu == 'n'){
		
			//loop for incorrect user input in menu, loops menu if user input fails
			do{
				choice = printChoices();
				switch(choice){
				//************************************
				//quickrun
				case 1:
					switchControl = true;
					cout<<"Enter amount borrowed: ";
					while(loan == 0){
						cin>>charLoan;
						loan = numericCheckNum( charLoan );
					}

					cout<<"Enter annual interest rate(decimal format no sign): ";
					while(annualRate == 0){
						cin>>charRate;
						annualRate = numericCheckNum( charRate );
					}

					cout<<"Enter number of years: ";
					while(timeYears == 0){
						cin>>charYears;
						timeYears = numericCheckNum( charYears );
					}

					cout<<"Choose your first month: ";
					while(currentMonth == 0){
						cin>>charMonth;
						currentMonth = numericCheckNum( charMonth );
					}
					break;

				//************************************
				//openPreviousFile
				case 2:
					switchControl = true;
					iFile>>loan>>annualRate>>timeYears>>currentMonth;

					if ( !iFile ) {
						cerr << "File could not be opened.\n";	//close program if error
						switchControl = false;
					}

				break;

				//************************************
				//overwrite file
				case 3:
					switchControl = true;
					cout<<"Enter amount borrowed: ";
					while(loan == 0){
						cin>>charLoan;
						loan = numericCheckNum( charLoan );
					}

					cout<<"Enter annual interest rate(decimal format no sign): ";
					while(annualRate == 0){
						cin>>charRate;
						annualRate = numericCheckNum( charRate );
					}

					cout<<"Enter number of years: ";
					while(timeYears == 0){
						cin>>charYears;
						timeYears = numericCheckNum( charYears );
					}

					cout<<"Choose your first month: ";
					while(currentMonth == 0){
						cin>>charMonth;
						currentMonth = numericCheckNum( charMonth );
					}

					replaceFile( loan, annualRate, timeYears, currentMonth);
					break;

				//************************************
				//normal run, goes through all choices
				case 4:
					switchControl = true;
					cout<<"Would you like to open the file?(y/n)";
					cin>>answerOpen;
					//error( answerOpen );

					//opens file D:\amort.txt to set three variables
					if ( answerOpen == 'y'){
						openFile( loan, annualRate, timeYears, currentMonth);
					}
					//continue with input
					else {	
						cout<<"Enter amount borrowed: ";
						while(loan == 0){
							cin>>charLoan;
							loan = numericCheckNum( charLoan );
						}

						cout<<"Enter annual interest rate(decimal format no sign): ";
						while(annualRate == 0){
							cin>>charRate;
							annualRate = numericCheckNum( charRate );
						}

						cout<<"Enter number of years: ";
						while(timeYears == 0){
							cin>>charYears;
							timeYears = numericCheckNum( charYears );
						}

						cout<<"Choose your first month: ";
						while(currentMonth == 0){
							cin>>charMonth;
							currentMonth = numericCheckNum( charMonth );
						}

						cout<<"Would you like to overwrite the file?(y/n)";
						cin>>answerClose;
						//error( answerClose );
		
						//overwrites file if user says yes
						if ( answerClose == 'y'){
							replaceFile( loan, annualRate, timeYears, currentMonth);
						}
					}
					break;

				//************************************
				//default - incorrect choice rerun switch statement
				default:
					cerr << "Incorrect Choice\n\n";
					switchControl = false;
					break;
				}//end switch
			}while( switchControl == false );
			}//skip menu if statement

			if( skipMenu == 'y')
				iFile>>loan>>annualRate>>timeYears>>currentMonth;

				//calculate monthly payment, total payment, round monthly payment
				timeMonths = timeYears*12;
				monthlyRate = annualRate/12;
				monthlyPayment = payment( monthlyRate, loan, timeMonths );

				//initialize for while loop, check amounts, print table
				balance = loan;

				cout<<"Loan amount: "<<loan
					<<"\nAnnual Rate: "<<annualRate
					<<"\nTime in Years: "<<timeYears;
				printTable();


				//output
				while( timeMonths > 0){

					calcMonthlyValues( interest, balance, appliedPay, monthlyRate, 
									monthlyPayment, timeMonths);

					//yearly totals, add until year reset then print set yearlyPrint check
					if( currentMonth <= 12 ){
						yearlyInterest = addTotal( yearlyInterest, interest);
						yearlyPayment = addTotal( yearlyPayment, monthlyPayment);
						yearlyAppliedPay = addTotal( yearlyAppliedPay, appliedPay);

						//add totals for the year on the last month
						if( currentMonth == 12 || timeMonths == 1 ){
							totalInterest = addTotal( totalInterest, yearlyInterest);
							totalPayment = addTotal( totalPayment, yearlyPayment);
							totalAppliedPay = addTotal( totalAppliedPay, yearlyAppliedPay);
							yearlyPrint++;
						}
					}

					//resets so current month is 1 and yearly amounts are 0
					if( currentMonth == 13 ){
						currentMonth = currentMonth - 12;
						yearlyInterest = addTotal( 0, interest);
						yearlyPayment = addTotal( 0, monthlyPayment);
						yearlyAppliedPay = addTotal( 0, appliedPay);
					}

					//rounds numbers before printing to sreen
					spacing = alignment( currentMonth );
					cout<<setprecision(2)<<setiosflags( ios::fixed | ios::showpoint )
						<< currentMonth << setw(spacing) <<monthlyPayment<< setw(16) <<interest<< setw(17) <<appliedPay
						<< setw(19) <<balance<<endl;

					if(yearlyPrint == 1){
						cout<< "\n" << "Yearly Totals" << setw(10) <<yearlyPayment<< setw(16) <<yearlyInterest<< setw(17) <<yearlyAppliedPay
							<< "\n\n";
						yearlyPrint--;
					}

					if( timeMonths == 1){
						cout<<"Final Totals" << setw(11) << totalPayment << setw(16) << totalInterest << setw(17) << totalAppliedPay
							<<"\n********************************************************************************";
					}

					currentMonth++;
					timeMonths = timeMonths--;	//control
				}//end of while loop for output
			


				//continue reading from file
				cout<<"\nContinue?(y/n) ";
				cin>>skipMenu;
				error( skipMenu );
			} while( skipMenu == 'y' );
		//program continue yes/no, reset values if yes
		if( exit == 'n'){
			iFile.close();
			iFile.open("D:\\amort.txt");

			cout<<"\nDo you wish to go back to the menu?(y/n) ";
			cin>>exit;
			error( exit );
			cout<<"\n";
		}

	} while( exit == 'y' );
	//end of do/while loop for *menu* beginning 

	return 0;
}

//set to zero
template <typename T> void setZero(T &a, T &b, T &c, T &d, T &e, T &f, T &g, T &h, T &i, T &j,
								   T &k, T &l, T &m, T &n, T &o, T &p, T &q, T &r, T &s){
	a = 0;	
	b = 0;
	c = 0;
	d = 0;
	e = 0;
	f = 0;
	g = 0;
	h = 0;
	i = 0;
	j = 0;
	k = 0;
	l = 0;
	m = 0;
	n = 0;
	o = 0;
	p = 0;
	q = 0;
	r = 0;
	s = 0;
}

//calculates each payment per month and assigns the value to each variable
void calcMonthlyValues(double& interest, double & balance, double& appliedPay, double& monthlyRate, 
				double& monthlyPayment, int timeMonths)
{
	//monthly totals
	if( timeMonths != 1){	//run as long as its not the total payment
			
		interest = monthlyRate * balance;
		balance = balance - monthlyPayment + interest;
		appliedPay = monthlyPayment - interest;

		interest = roundMe( interest );
	}

	else{	//final month totals
		interest = monthlyRate * balance;
		monthlyPayment = balance + interest;
		appliedPay = monthlyPayment - interest;

		balance = balance - monthlyPayment + interest;
	}
}

/*void calcYearlyValues( double& yearlyInterest, double& yearlyPayment, double& yearlyAppliedPay)
{
	yearlyInterest = addTotal( yearlyInterest, interest);
	yearlyPayment = addTotal( yearlyPayment, monthlyPayment);
	yearlyAppliedPay = addTotal( yearlyAppliedPay, appliedPay);
}*/

//checks if string is an numeric value
//allows for doubles with one '.', greater than one flag triggers
//converts string to double value, returns double
double numericCheckNum( char *numToString )
{
	//assigns pointer to string, creates *char large enough for str
	//copies string to newly created *char
	string str = numToString;
	char * string = new char [str.size()+1];
	strcpy (string, str.c_str());
	int len = 0,	//length of string
		ind2 = 0,	//error indicator
		decCounter = 0;	//flags number of decimals in number
	double	ind = 0,		//work digit
			amt = 0;	//storage for string conversion

		while( ind == 0 )
		{
			if(ind2 == 1)
				ind2 = 0;
			len = strlen(string);
				
			//checks if string is an numeric value
				for ( int i = 0; i<len; i++)
				{
					//ind2 is an error indicator
					if( string[i] < '0')
						ind2 = 1;
					else if (string[i] > '9')
						ind2 = 1;
					if( string[i] == '.')
					{
						decCounter++;
						ind2 = 0;
					}
				}//end of for loop

				//check flags
				if(decCounter > 1)
					ind2 = 1;
				if(ind2 == 0)
					ind = 1;
				else if(ind2 == 1)
				{
					//flag reset
					decCounter = 0;
					//retry with new input
					cout<<"Invalid numeric data, please reenter: ";
					return 0;
				}
		}//end of while loop

		//converts string to double value
		amt = atof( string );
		return amt;
}

//return char y/n for if statement using y/n
char error( char& x )
{
	int n = 0;

	x = toupper(x);

	while( n != 1 )
	{
		if ( x == 'Y' ){
			n = 1;
		}
		else if ( x == 'N' ){
			n = 1;
		}
		else {
			cout<<"Error\nEnter in (y/n): ";
			cin>>x;
		}
	}

	x = tolower(x);

	return x;
}

//used in proper output format, aligns all numbers on output screen
int alignment( int n )
{
	int x;

	if ( n > 9 )
		x = 21;
	else x = 22;

	return x;
}

//adds variable by monthly payment for yearly total
double addTotal( double y, double z)
{
	y = y + z;
	return y;
}

//rounds the number to the 2nd decimal point
double roundMe( double x )
{
	x = x + .005;
	x *= 100;
	x = int(x);
	x = x/100;

	return x;
}

//print choices for switch statements, use int for selection
int printChoices()
{
	int x;

	cout<<"1 - Quick Run" << endl
		<<"2 - Open Previous File" << endl
		<<"3 - Overwrite File" << endl
		<<"4 - Normal Run" << endl
		<<"Choose how to run the program by picking a number: ";
	cin>>x;
	cout<<"\n\n";

	return x;
}

//print table
void printTable()
{
	cout<<"\n********************************************************************************"
		<< "Payment" << setw(17) << "Payment" << setw(17) << "Interest" << setw(19) << "Applied to" << setw(19) <<"New Balance";
	cout<< "\nNumber"  << setw(17) << "Amount"  << setw(16) << "Amount" << setw(20) << "principal" << setw(16) << "Amount "
		<<"\n--------------------------------------------------------------------------------";
	return;
}

//calculate monthly payment & round monthly payment. Then returns monthly Payment.
//needs a monthlyRate, total loan amount, and time in months
double payment( double rate, double loan, int months )
{
	double	payment,	//calculate monthlyPayment
			calcA = 0, calcB = 0;	//use for monthlyPayment calculations

	calcA= 1 + rate;
	calcB= pow( calcA, months);
	calcA= calcB - 1;
	calcA= calcA / calcB;
	calcA= calcA / rate;
	payment= loan / calcA;

	payment = payment + .005;
	payment *= 100;
	payment = int(payment);
	payment = payment/100;

	return payment;
}

//replace file data, replaces file data with the values stored in variables with a space between each variable
//needs a loan/annualRate/time/currentMonth in months variable to replace in textfile
void replaceFile( double& outLoan, double& outAnnualRate, int& outTimeYears, int& outMonth)
{
	char error;
	ofstream oFile;
	oFile.open("D:\\amort.txt", ios::out);
	oFile<<outLoan<<" "<<outAnnualRate<<" "<<outTimeYears<<" "<<outMonth;

		if ( !oFile ) {
			cerr << "File could not be opened.\nPress any key to continue...";	//close program if error
			cin>>error;
			exit(1);
		}

	oFile.close();
	//cout<<"\nreplaceFile testing 1,2,3... running\n";
	return;
}

//opens file data, and replaces variable values with the file data, one space must be between each variable in the file to properly run
//needs a loan/annualRate/time/currentMonth in months variable to fill with data from a text file
void openFile( double& inLoan, double& inAnnualRate, int& inTimeYears, int& inMonth)
{
	char error;
	//opens file D:\amort.txt to set three variables
	ifstream iFile;
	iFile.open("D:\\amort.txt");
	iFile>>inLoan>>inAnnualRate>>inTimeYears>>inMonth;

	if ( !iFile ) {
		cerr << "File could not be opened.\nPress any key to continue...";	//close program if error
		cin>>error;
		exit(1);
	}

	iFile.close();
	//cout<<"\nopenFile testing 1,2,3... running\n";
	return;
}

/* Notes
	-use if calcValues is not supposed to be used for the final payment
	//monthly totals
			if( timeMonths != 1){	//run as long as its not the total payment
				calcValues( interest, balance, appliedPay, monthlyRate, 
							monthlyPayment, timeMonths);
			}

			else{	//final month totals
				interest = monthlyRate * balance;
				monthlyPayment = balance + interest;
				appliedPay = monthlyPayment - interest;

				balance = balance - monthlyPayment + interest;
			}

	//openFile( loan, annualRate, timeYears, currentMonth);

	-string manipulations reading off of file by using 
		stringstream, allows use of input/output buffers
	string values;
	stringstream ss(stringstream::in | stringstream::out);
	getLine(ifile, ss) or getLine(ifile, values)
	ss>>loan>>annualRate>>timeYears>>currentMonth;
*/




