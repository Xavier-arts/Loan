/*	
	Ben Yoder
	C++ AssignmentSix, Loan Payment
	Explanation: Take user input and print out the users monthly payments for a loan
	10/27/11
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

//function prototypes
char error( char& x );
int printChoices();
int alignment( int n );
double payment( double rate, double loan, int months);
double roundMe( double x);
double numericCheckNum( char *numToString );
void printTable();
void calcMonthlyValues(double& interest, double & balance, double& appliedPay, double& monthlyRate, 
			double& monthlyPayment, int timeMonths);
void calcTotalValues( double& yearlyInterest, double& yearlyPayment, double& yearlyAppliedPay,
			double& interest, double& monthlyPayment, double& appliedPay, double& totalInterest,
			double& totalPayment, double& totalAppliedPay, int& yearlyPrint, int& currentMonth,
			int timeMonths);
void printOutputTable( int spacing, int currentMonth,int &currentYear , int timeMonths, double monthlyPayment, double interest,
					double appliedPay, double balance, double yearlyPayment, double yearlyInterest,
					double yearlyAppliedPay, double totalPayment, double totalInterest, double totalAppliedPay, const char *month,
					int& yearlyPrint);
void setZero(double& loan,double& annualRate,int& timeYears,int& currentMonth, double& yearlyInterest,
			double& yearlyPayment,double& yearlyAppliedPay,double& totalInterest,double& totalPayment,
			double& totalAppliedPay, int& yearlyPrint);

int main()
{
	//declare&initialize variables, reset variables if program loops
	int timeMonths = 0,		//number of monthly payments
		timeYears = 0,		//number of years for payment
		currentMonth = 0,	//current month during the year
		currentYear = 0,	//current year
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
			answerClose = 'n',		//user input for closefile
			remain = 'y',			//loop control for output
			run = 'y',				//loop control for menu
			runOverwrite = 'y';			//loop control for menu overwrite

	char	charLoan[8],			//char arrays for temporary storage
			charRate[8],			//used for numericCheckNum for string manipulation
			charYears[8],
			charMonth[5],
			charYear[5];
	
	const char *month[13] = { "aaa", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
							"Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	bool	switchControl = false;	//used for menu switch statement

	ifstream inFile;
	ofstream outFile;
	
	//beginning loop to keep program in constant run, variable exit is loop control *menu*
	cout <<"Welcome*************************************************************************\n";
	do{
		//reset variables
		setZero(loan, annualRate, timeYears, currentMonth, yearlyInterest, yearlyPayment, yearlyAppliedPay, 
						totalInterest, totalPayment, totalAppliedPay, yearlyPrint);
		remain = 'y', run = 'y', runOverwrite = 'y';
		switchControl = false;

	//input
		//loop for incorrect user input in menu, loops menu if user input fails
		do{
			choice = printChoices();
			switch(choice){
			//************************************
			//openPreviousFile
			case 1:
				switchControl = true;
				cout<<"Open File\n";
				inFile.open("file.txt", ios::in);
				inFile>>loan>>annualRate>>timeYears>>currentMonth>>currentYear;

				if ( !inFile ) {
					cerr << "File could not be opened.\n";
					switchControl = false;
				}


			break;

			//************************************
			//overwrite file
			case 2:
				switchControl = true;
				cout<<"Overwrite File\n";
				outFile.open("file.txt", ios::out);
				if ( !outFile ) {
						cerr << "\nFile could not be opened.\n";	
						runOverwrite = 'n';
						switchControl = false;
				}

				//continue overwrite till user responds n
				while( runOverwrite == 'y' ){
					cout<<"Enter amount borrowed: ";
					cin>>charLoan;
					loan = numericCheckNum( charLoan );

					cout<<"Enter annual interest rate(decimal format no sign): ";
					cin>>charRate;
					annualRate = numericCheckNum( charRate );

					cout<<"Enter number of years: ";
					cin>>charYears;
					timeYears = numericCheckNum( charYears );

					cout<<"Choose your first month: ";
					cin>>charMonth;
					currentMonth = numericCheckNum( charMonth );

					cout<<"Choose your year: ";
					cin>>charYear;
					currentYear = numericCheckNum( charYear );

					outFile<<loan<<" "<<annualRate<<" "<<timeYears<<" "<<currentMonth<<" "<<currentYear<<"\n";

					cout<<"\nContinue writing to file?(y/n) ";
					cin>>runOverwrite;
					error( runOverwrite );
				}

				//close file to save, reopen for output
				outFile.close();
				inFile.open("D:\\amort.txt", ios::in);
				inFile>>loan>>annualRate>>timeYears>>currentMonth>>currentYear;
				remain = 'n';

				break;

			//************************************
			//debug
			case 3:
				switchControl = true;
				cout<<"Enter amount borrowed: ";
				cin>>charLoan;
				loan = numericCheckNum( charLoan );

				cout<<"Enter annual interest rate(decimal format no sign): ";
				cin>>charRate;
				annualRate = numericCheckNum( charRate );

				cout<<"Enter number of years: ";
				cin>>charYears;
				timeYears = numericCheckNum( charYears );

				cout<<"Choose your first month: ";
				cin>>charMonth;
				currentMonth = numericCheckNum( charMonth );

				inFile.open("D:\\amort.txt", ios::in);
				break;

			//************************************
			//default - incorrect choice rerun switch statement
			default:
				cerr << "Incorrect Choice\n\n";
				switchControl = false;
			}//end switch for menu
		}while( switchControl == false ); //loop menu if incorrect choice

		while( !inFile.eof() && remain == 'y' ){
			//calculate monthly payment, total payment, round monthly payment
			timeMonths= timeYears*12;
			monthlyRate= annualRate/12;
			monthlyPayment = payment( monthlyRate, loan, timeMonths );

			//initialize for while loop, check amounts, print table
			balance = loan;

			cout<<"Loan amount: "<<loan
				<<"\nAnnual Rate: "<<annualRate
				<<"\nTime in Years: "<<timeYears
				<<"\nBeggining Month, Year: "<<month[currentMonth]<<", "<<currentYear;
			printTable();

			//output
			while( timeMonths > 0){

				//calculate monthly amounts
				calcMonthlyValues( interest, balance, appliedPay, monthlyRate, 
						monthlyPayment, timeMonths);

				//calculate yearly and total amounts, reset currentMonth, set YearlyPrint
				calcTotalValues( yearlyInterest, yearlyPayment, yearlyAppliedPay,
						interest, monthlyPayment, appliedPay, totalInterest, totalPayment, 
						totalAppliedPay, yearlyPrint, currentMonth, timeMonths);

				//print output table
				printOutputTable( spacing, currentMonth, currentYear, timeMonths, monthlyPayment, interest,
					appliedPay, balance, yearlyPayment, yearlyInterest, yearlyAppliedPay, 
					totalPayment, totalInterest, totalAppliedPay, month[currentMonth],
					yearlyPrint);

				//controls
				currentMonth++;				//cycle months
				timeMonths = timeMonths--;	//track of payments using number of Months
			}//end of while loop for loan payment output
	
			//program continue yes/no, reset values if yes
			cout<<"\nContinue?(y/n) ";
			cin>>remain;
			error( remain );
			

			if( remain == 'y' ){
				setZero(loan, annualRate, timeYears, currentMonth, yearlyInterest, yearlyPayment, yearlyAppliedPay, 
						totalInterest, totalPayment, totalAppliedPay, yearlyPrint);

				if(!inFile.eof()){
					inFile>>loan>>annualRate>>timeYears>>currentMonth>>currentYear;
				}
				if(inFile.eof()){
					cerr << "End of File, kicking to menu\n\n";
				}
			}

		}//end of while( inFile )

		inFile.close();
		cout<<"\nReturn to Menu?(y/n) ";
		cin>>run;
		error( run );
			
	} while( run == 'y' );
	//end of do/while loop for *menu* beginning 

	return 0;
}

//calculates each payment per month and assigns the value to each variable
void calcMonthlyValues(double& interest, double & balance, double& appliedPay, double& monthlyRate, 
				double& monthlyPayment, int timeMonths)
{
	//monthly totals
	if( timeMonths != 1){	//run as long as its not the total payment
			
		interest = monthlyRate * balance;
		interest = roundMe( interest );
		balance = balance - monthlyPayment + interest;
		appliedPay = monthlyPayment - interest;
	}

	else{	//final month totals
		interest = monthlyRate * balance;
		monthlyPayment = balance + interest;
		appliedPay = monthlyPayment - interest;

		balance = balance - monthlyPayment + interest;
	}
}

//calculates yearly and total values
void calcTotalValues( double& yearlyInterest, double& yearlyPayment, double& yearlyAppliedPay,
			double& interest, double& monthlyPayment, double& appliedPay, double& totalInterest,
			double& totalPayment, double& totalAppliedPay, int& yearlyPrint, int& currentMonth,
			int timeMonths)
{
	//yearly totals, add until year reset then print set yearlyPrint check
			if( currentMonth <= 12 ){
				yearlyInterest = yearlyInterest + interest;
				yearlyPayment = yearlyPayment + monthlyPayment;
				yearlyAppliedPay = yearlyAppliedPay + appliedPay;

				//add totals for the year on the last month
				if( currentMonth == 12 || timeMonths == 1 ){
					totalInterest = totalInterest + yearlyInterest;
					totalPayment = totalPayment + yearlyPayment;
					totalAppliedPay = totalAppliedPay + yearlyAppliedPay;
					yearlyPrint++;
				}
			}

			//resets so current month is 1 and yearly amounts equal first month
			if( currentMonth == 13 ){
				currentMonth = currentMonth - 12;
				yearlyInterest = interest;
				yearlyPayment = monthlyPayment;
				yearlyAppliedPay = appliedPay;
			}
}

//print monthly,yearly, and total
void printOutputTable( int spacing, int currentMonth,int &currentYear, int timeMonths, double monthlyPayment, double interest,
					double appliedPay, double balance, double yearlyPayment, double yearlyInterest,
					double yearlyAppliedPay, double totalPayment, double totalInterest, double totalAppliedPay, const char *month,
					int& yearlyPrint)
{
			//print to screen
				//rounds numbers before printing to sreen
				cout<<setprecision(2)<<setiosflags( ios::fixed | ios::showpoint )
					<< month[0] << month[1] << month[2] << ",  "<<currentYear<< setw(13) <<monthlyPayment<< setw(16) 
					<<interest<< setw(17) <<appliedPay << setw(19) <<balance<<endl;

				//print yearly totals if yearlyPrint clicks
				if(yearlyPrint == 1){
					cout<< "\n" << "Yearly Totals" << setw(10) <<yearlyPayment<< setw(16) <<yearlyInterest<< setw(17) <<yearlyAppliedPay
						<< "\n\n";
					yearlyPrint--;
					currentYear++;
				}

				//print final totals if final month
				//set all values to zero for next loan
				if( timeMonths == 1){
					cout<<"Final Totals" << setw(11) << totalPayment << setw(16) << totalInterest << setw(17) << totalAppliedPay
						<<"\n********************************************************************************";
				}
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
	int len = 0,		//length of string
		ind2 = 0,		//error indicator
		decCounter = 0;	//flags number of decimals in number
	double	ind = 0,	//work digit
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
					cin>>string;
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

	while( n != 1 )
	{
		x = toupper(x);
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
	char check[8];

	cout<<"Menu" << endl
		<<"1 - Open Previous File" << endl
		<<"2 - Overwrite File" << endl
		<<"3 - Debug"<< endl
		<<"Choose how to run the program by picking a number: ";
	cin>>check;
	x = numericCheckNum(check);
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

//resets variables to zero
void setZero(double& loan,double& annualRate,int& timeYears,int& currentMonth,double& yearlyInterest,
			double& yearlyPayment,double& yearlyAppliedPay,double& totalInterest,double& totalPayment,
			double& totalAppliedPay,int& yearlyPrint)
{
	loan = 0; annualRate = 0; timeYears = 0; currentMonth = 0; 
	yearlyInterest = 0;	yearlyPayment = 0; yearlyAppliedPay = 0; 
	totalInterest = 0; totalPayment = 0; totalAppliedPay = 0;
	yearlyPrint = 0;
}

