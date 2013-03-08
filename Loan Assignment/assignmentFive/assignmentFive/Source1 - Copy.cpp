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

//function prototypes
char error( char& x );
int printChoices();
int alignment( int n );
double numericCheckNum( char *numToString );
void printTable();
void openFile( double& inLoan, double& inAnnualRate, int& inTimeYears, int& inMonth);
void replaceFile( double& outLoan, double& outAnnualRate, int& outTimeYears, int& outMonth);
double payment( double rate, double loan, int months);
double roundMe( double x);
double addTotal( double y, double z);

int main()
{
	//beginning loop to keep program in constant run, variable exit is loop control
	cout <<"Welcome*************************************************************************\n";
	char exit = 'n';
	do{


	//declare&initialize variables
	int timeMonths = 0,		//number of monthly payments
		timeYears = 0,		//number of years for payment
		payNum = 0,			//payments
		currentMonth = 0,	//current month during the year
		yearlyPrint = 0,	//control for yearlyPrint
		choice = 0,			//control for switch statement
		spacing = 0;		//used for spacing in output format

	double	loan = 0,				//initial amount borrowed
			annualRate = 0,			//annual interest rate
			monthlyRate = 0,		//monthly interest rate
			monthlyPayment = 0,		//amount to pay monthly
			interest = 0,			//currentinterest to be paid
			balance = 0,			//currentBalance to be paid
			appliedPay = 0,			//amount of payment applied to principle
			yearlyInterest = 0,		//yearly totals
			yearlyPayment = 0,		//----------
			yearlyAppliedPay = 0,	//----------
			totalInterest = 0,		//grand totals
			totalPayment = 0,		//----------
			totalAppliedPay= 0;		//----------

	char	answerOpen = 'n',		//user input for openfile
			answerClose = 'n';		//user input for closefile

	char	charLoan[8],			//char arrays for temporary storage
			charRate[8],			//used for numericCheckNum for string manipulation
			charYears[8],
			charMonth[3];

	bool	switchControl = false;	//used for menu switch statement

	//input
		//loop for incorrect user input
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
				openFile( loan, annualRate, timeYears, currentMonth);
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
				error( answerOpen );

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
					error( answerClose );
		
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


		//calculate monthly payment, total payment, round monthly payment
		timeMonths= timeYears*12;
		monthlyRate= annualRate/12;
		monthlyPayment = payment( monthlyRate, loan, timeMonths );

		//initialize for while loop, check amounts, print table
		balance = loan;

		cout<<"Loan amount: "<<loan
			<<"\nAnnual Rate: "<<annualRate
			<<"\nTime in Years: "<<timeYears;
		printTable();


		//output
		while( timeMonths > 0){

			//monthly totals
			if( timeMonths != 1){	//run as long as its not the total payment
				interest = monthlyRate * balance;
				balance = balance - monthlyPayment + interest;
				appliedPay = monthlyPayment - interest;

				interest = roundMe( interest );
				//appliedPay = roundMe( appliedPay );
			}

			else{	//final month totals
				interest = monthlyRate * balance;
				monthlyPayment = balance + interest;
				appliedPay = monthlyPayment - interest;

				//appliedPay = roundMe( appliedPay );
				balance = balance - monthlyPayment + interest;
			}

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
	
		//program continue yes/no, reset values if yes
		cout<<"\nContinue?(y/n) ";
		cin>>exit;
		error( exit );

	} while( exit == 'y' );
	//end of do/while loop for beginning

	return 0;
}

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



