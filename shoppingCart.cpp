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
#include <iomanip>
using namespace std;

//Creating structures
struct Address{ string streetAddress; string city; string state; string zipCode; };
struct Customer{ string customerNum; string customerName; double lineOfCredit; Address * corperateAddress; };
struct Product{ int itemNo; string description; double price; };

//Function prototypes
vector<string> parse(string line, char delimiter);
string generateOrderNum();
void selectionSort(Customer array[], Address *aPtr, int Length);
void orderSummary();
void menu();


int main()
{  
    string str, tempSTR; //used to read in the data
    int Length = 21; //size of the array
    
    //Creating the array and vectors
    Customer CustList[Length];
    Address Addresses[Length];
    CustList->corperateAddress = Addresses; //creating a pointer to the array of address structs
    Product ProdList[(Length-1)];
    vector<string> tempCust; //temporary vector used in parsing the customer data
    vector<string> tempProd; //temprorary vector used in parsing the inventory data
    vector<string> tempAdd; //temporary vector used in parsing the address data
    vector<Address> AddList;
    
    
    //Opening the file
    ifstream Filename;
    Filename.open("customers.dat");
    
    //Reading in the customer data
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
        
        Addresses[i].streetAddress = AddList[i].streetAddress; //filling array with data
        Addresses[i].city= AddList[i].city; 
        Addresses[i].state = AddList[i].state;
        Addresses[i].zipCode =AddList[i].zipCode;
    }
    Filename.close(); //closing the file
    
    //Opening the inventory file
    Filename.open("inventory.dat");
    
    //Reading in the inventory data
    for (int i= 0; i < (Length-1); i++)
    {
        getline(Filename,str); //reads in entire line
        tempProd = parse(str, ','); //vector of line parsed by the ,
        
        //indexing the vector to fill in the array of structs
        ProdList[i].itemNo = atoi(tempProd[0].c_str());  // converting the type from string to int
        ProdList[i].description = tempProd[1]; 
        ProdList[i].price = atof(tempProd[2].c_str());  //converting the type from string to double           
    }
    Filename.close(); //closing the file
    
    //selection sorts by numerical ascending order for the address & customer arrays
    selectionSort(CustList, Addresses,Length);
    
    
    //checks to see if results are entered or sorted correctly
    /*for (int i = 0; i< Length; i++)
    {  
        cout << CustList[i].customerNum << " " << CustList[i].customerName << " " << CustList[i].lineOfCredit << " "<< Addresses[i].streetAddress << " " <<
            Addresses[i].city << " " << Addresses[i].state << " " << Addresses[i].zipCode << endl; 
    } */ 
    
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


void selectionSort(Customer array[], Address *aPtr, int Length) //fix this
{
    int start, minIndex;
    double minCredit;
    string minVal, minName, minStreet, minCity, minState, minZip;
    
    for (start =0; start <(Length-1); start++)
    {
        minIndex = start;
        minVal = array[start].customerNum;
        minName =array[start].customerName;
        minCredit = array[start].lineOfCredit;
        
        minStreet = aPtr[start].streetAddress; //using pointers to access Addresses
        minCity = aPtr[start].city;
        minState = aPtr[start].state;
        minZip = aPtr[start].zipCode; 
        
        
        for (int index = (start+1); index <Length; index++)
        {
            if (array[index].customerNum < minVal)
            {
                minVal = array[index].customerNum;
                minName = array[index].customerName;
                minCredit= array[index].lineOfCredit;
                
                minStreet = aPtr[index].streetAddress;
                minCity = aPtr[index].city;
                minState = aPtr[index].state;
                minZip = aPtr[index].zipCode; 
                
                minIndex = index;
            }
        }
        
        array[minIndex].customerNum = array[start].customerNum;
        array[minIndex].customerName = array[start].customerName;
        array[minIndex].lineOfCredit = array[start].lineOfCredit;
        aPtr[minIndex].streetAddress = aPtr[start].streetAddress;
        aPtr[minIndex].city = aPtr[start].city;
        aPtr[minIndex].state = aPtr[start].state;
        aPtr[minIndex].zipCode = aPtr[start].zipCode;
        
        array[start].customerNum = minVal;
        array[start].customerName = minName;
        array[start].lineOfCredit = minCredit; 
        aPtr[start].streetAddress = minStreet;
        aPtr[start].city = minCity;
        aPtr[start].state = minState;
        aPtr[start].zipCode = minZip;
    }
    
}

void orderSummary()
{
    //opening the file to write
    ofstream FileHandle;
    string orderNum = generateOrderNum();
    FileHandle.open(orderNum.c_str());
    
    //FileHandle << "This is the result of the order. Order number: " << orderNum;
    
    FileHandle<< string(60,'-') << endl << "B2B Shopping Cart" << endl << string(60,'-') << endl;
    FileHandle << "\nOrder Number: " << orderNum << "\nAssociate: " << endl;
    //fill in the remainding
    //
    
    FileHandle << string(60,'-') << endl << setw(20) << left << "Item No" << setw(20) << left<< "Description" << setw(20)
        << left << "Qty" << setw(10) << left << "Total" << endl << string(60,'-');
    
    
   /* for (int i = 0; i < num; i++) //prints the number of items
    {
      cout << setw(20) << left << //insertItem << setw(10) << left << insertItem << setw(4) << insertItem<< endl;  
    }*/
    
}


void menu()
{
    //variables to enter
    char response;
    string nameSearch, userName;
    int numSearch;
    
    //prompt
    cout << "Welcome to the B2B Shopping Cart! " << endl << "Please enter your name: ";
    getline(cin, userName);
    cout << "Hello " << userName << ". If you are searching by name enter 'A' or if you are searching by number enter 'B'" << endl;
    cout << "A or B? : ";
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
    
    //appropriate functions to check credit
    orderSummary();
    
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


