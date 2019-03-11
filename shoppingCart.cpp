/**************************************************************
 *
 * GROUP #2
 * Group Members: Kowe Kadoma
                  Nana Ama Marfo
                  Trevor Taylor
                  Kierra Howard
                  Vanessa Chery
                  Kenneth Brown
 * Date Completed:
 * Filename: shoppingCart.cpp
 * 
 **************************************************************/

#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

//Creating structures
struct Address{ string streetAddress; string city; string state; string zipCode; };
struct Customer{ string customerNum; string customerName; double lineOfCredit; Address * corperateAddress; };
struct Product{ int itemNo; string description; double price; };

//Function prototypes
vector<string> parse(string line, char delimiter);
string generateOrderNum();
void selectionSort(Customer array[], int Length);
void menu();


int main()
{  
    string str, tempSTR; //used to read in the data
    int Length = 21; //size of the array
    
    //Creating the array and vectors
    Customer CustList[Length];
    vector<string> tempCust; //temporary vector used in parsing the customer data
    vector<string> tempAdd; //temporary vector used in parsing the address data
    vector<Address> AddList;
    
    //Opening the file
    ifstream Filename;
    Filename.open("customers.dat");
    
    //Reading in the data
    for (int i= 0; i < Length; i++)
    {
        getline(Filename,str); //reads in entire line
        tempCust = parse(str, '|'); //vector of line parsed by the |
        CustList[i].customerNum = tempCust[0];  //indexing the vector to fill in the array of structs
        CustList[i].customerName = tempCust[1]; 
        CustList[i].lineOfCredit = atof(tempCust[2].c_str());  //converting the type from string to double
        tempSTR = tempCust[3];
        
        tempAdd = parse(tempSTR, ','); //vector of addresses parsed by the ,
        AddList.push_back(Address()); //pushes back the entries to fill the vector of structs
        AddList[i].streetAddress = tempAdd[0]; 
        AddList[i].city = tempAdd[1]; 
        AddList[i].state = tempAdd[2];
        AddList[i].zipCode= tempAdd[3];       
    }
    
    Filename.close(); //closing the file
    
    //selection sorts by numerical ascending order
    //selectionSort(CustList, Length);
    
    /*for (int i = 0; i<Length; i++)
    {
        cout << CustList[i].customerNum << " " << CustList[i].customerName << endl; // " " << CustList[i].lineOfCredit << " " <<AddList[i].streetAddress << " " <<
            //AddList[i].city << " " << AddList[i].state << " " << AddList[i].zipCode << endl; 
    }  */ //testing to see if the vector and array are filled 
    
    menu();
    return 0;
}


//Function definitions

/**************************************************************
 *                             parse                          *
 * Called by: <fill in>                                       *
 * Passed   : 2 arguments: a string, a character that is the  *
 *            delimiter                                       *
 * Purpose  : Split a given string by its delimiter           *
 * Returns  : String vector                                   *
 **************************************************************/
vector<string> parse(string line, char delimiter){
    
    vector<string> parsedLine;
    istringstream stringStream( line ); //create string stream from the line
    //iterate through the stream
    while (stringStream)
    {
        string s;
        //uses getline to get the values from the stream before the delimiting character
        if (getline( stringStream, s, delimiter))
          parsedLine.push_back(s);
    }
    return parsedLine;
}

/**************************************************************
 *                  generateOrderNum                          *
 * Called by: <fill in>                                       *                                       *
 * Purpose  : Generate an order number based on epoch         *
 * Returns  : String order number                             *
 **************************************************************/
string generateOrderNum(){
    
    time_t now = time(0); //current time object
    
    string oNum;
    stringstream strstream;
    
    strstream << now; //read the date/time in epoch into the string stream
    strstream >> oNum; //output a string from the string stream
    return oNum;
}


void selectionSort(Customer array[], int Length) //fix this
{
    int start, minIndex;
    string minVal;
    
    for (start =0; start <(Length-1); start++)
    {
        minIndex = start;
        minVal = array[start].customerNum;
        
        for (int index = (start+1); index <Length; index++)
        {
            if (array[index].customerNum < minVal)
            {
                minVal = array[index].customerNum;
                minIndex = index;
            }
        }
        
        array[minIndex].customerNum = array[start].customerNum;
        array[start].customerNum = minVal;
    }
    
}


void menu()
{
    //variables to enter
    char response;
    string nameSearch;
    int numSearch;
    
    //prompt
    cout << "Welcome to the B2B Shopping Cart! " << endl << "A) Search for customer by name" << endl << "B) Search for customer by number" <<
        endl <<"Enter A or B: ";
    cin >> response;
    
    //error checking
    while (toupper(response) != 'A' && toupper(response) != 'B')
    {
        cout << "Invalid response. Please enter either A or B: ";
        cin >> response;
    }
    
    if (toupper(response) == 'A')
    {
        cout << "Enter the customer name: ";
        cin >> nameSearch;
        //call linear search
    }
    
    else
    {
        cout << "Enter the customer number: ";
        cin >> numSearch;
        //call binary search
    }
    
    //function to enter info into the inventory structure
    //appropriate functions to check credit
    //print out results
    
    cout << "Would you like to search for another customer? Enter Y (yes) or N (no): ";
    cin >> response;
    while (toupper(response) != 'Y' && toupper(response) != 'N')
    {
        cout << "Invalid response. Please enter either Y or N: ";
        cin >> response;
    }
    
    if (toupper(response) == 'Y')
        menu();
    
    else
        exit(0);   
}


