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
struct Cart{Product item; int quantity; double total;};
vector<Cart> ShoppingCart;

//Function prototypes
vector<string> parse(string line, char delimiter);
void CustomerDataInput(string file, Customer CustList[],vector<Address>&AddList, int Length);
void InventoryDataInput(string file,Product ProdList[],int Length);
string generateOrderNum();
void selectionSort(Customer array[],vector<Address>&AddList, int Length);
int binarySearch(Customer array[], int low, int high, string key);
int linearSearch(Customer array[], int size, string name);
int linearSearchProduct(Product array[], int size, int name);
void creditChecker(Customer array[], int pos, Product Array[], int Size, vector<Cart>&shoppingList);
void orderSummary(string name, Customer array[],vector<Address>&AddList, int pos);
void menu(Customer array[],Product Array[], vector<Address>&vector,int &size);



int main()
{  
    //Variables
    string str = "";
    string customerData = "customers.dat";
    string inventoryData = "inventory.dat";
    int Length = 21; //size of the array
    
    //Creating the array and vectors
    Customer CustList[Length];
    vector<Address> AddList;
    Product ProdList[(Length-1)];
    
    //Opening files and reading in the data
    CustomerDataInput(customerData,CustList,AddList,Length);
    InventoryDataInput(inventoryData,ProdList,Length);
    
    //Sorting the data
    selectionSort(CustList, AddList, Length);
    
    //menu
    menu(CustList,ProdList,AddList,Length);     
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

//reads the file and fills the vector and array
void CustomerDataInput(string file, Customer CustList[],vector<Address>&AddList, int Length)
{
    //Temporary strings & vectors used in parsing
    vector<string> tempCust; 
    vector<string> tempAdd; 
    string str, tempSTR;
    
    //Opening the file
    ifstream Filename;
    Filename.open(file.c_str());
    
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
    }
    
       //Reading in addresses
       vector<Address>::iterator ITERATOR = AddList.begin();
       for (int i = 0; i<Length; i++)
       { 
           CustList[i].corperateAddress = &(*(ITERATOR+i));      
       } 
    
    Filename.close();
}

//reading in and filling in product array
void InventoryDataInput(string file,Product ProdList[],int Length)
{
    //Temporary vectors & strings used in parsing
    string str;
    vector<string> tempProd;
    
    
    //Opening the file
    ifstream Filename;
    Filename.open(file.c_str());
    
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
    
    //Closing the file
    Filename.close();
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

//selection sort used arrange array and vector in ascending order
void selectionSort(Customer array[],vector<Address>&AddList, int Length)
{
    int start, minIndex;
    double minCredit;
    string minVal, minName, minStreet, minCity, minState, minZip;
    Address * minAddress;
    
    for (start =0; start <(Length-1); start++)
    {
        minIndex = start;
        minVal = array[start].customerNum;
        minName =array[start].customerName;
        minCredit = array[start].lineOfCredit;
        minAddress = array[start].corperateAddress;
        
        minStreet = AddList[start].streetAddress; //using pointers to access Addresses
        minCity = AddList[start].city;
        minState = AddList[start].state;
        minZip = AddList[start].zipCode; 
        
        
        for (int index = (start+1); index <Length; index++)
        {
            if (array[index].customerNum < minVal)
            {
                minVal = array[index].customerNum;
                minName = array[index].customerName;
                minCredit= array[index].lineOfCredit;
                minAddress = array[index].corperateAddress;
                
                minStreet = AddList[index].streetAddress;
                minCity = AddList[index].city;
                minState = AddList[index].state;
                minZip = AddList[index].zipCode; 
                
                minIndex = index;
            }
        }
        
        array[minIndex].customerNum = array[start].customerNum;
        array[minIndex].customerName = array[start].customerName;
        array[minIndex].lineOfCredit = array[start].lineOfCredit;
        array[minIndex].corperateAddress = array[start].corperateAddress;
        AddList[minIndex].streetAddress = AddList[start].streetAddress;
        AddList[minIndex].city = AddList[start].city;
        AddList[minIndex].state = AddList[start].state;
        AddList[minIndex].zipCode = AddList[start].zipCode;
        
        array[start].customerNum = minVal;
        array[start].customerName = minName;
        array[start].lineOfCredit = minCredit; 
        array[start].corperateAddress = minAddress;
        AddList[start].streetAddress = minStreet;
        AddList[start].city = minCity;
        AddList[start].state = minState;
        AddList[start].zipCode = minZip;
    }  
}

//binary search used finding customer number
int binarySearch(Customer array[], int low, int high, string key) 
{
    while (low <= high) 
    { 
	    int mid = low + (high - low) / 2; 
	  
	        // Check if searchVAl is equal to mid 
	        if (array[mid].customerNum == key) 
	            return mid; 
	  
	        // If searchVal greater, ignore left half 
	        if (array[mid].customerNum < key) 
	            low = mid + 1; 
	  
	        // If searchVal is smaller, ignore right half 
	        else
	            high = mid - 1; 
     }
    
    return -1;
} 

//linear search used in finding customer name
int linearSearch(Customer array[], int size, string name)
{
    int index = 0;
    int position = -1;
    bool found = false;
    
    while (index < size && !found)
    {
        if (array[index].customerName == name)
        {
            found = true;
            position = index;
        }
        index++;
    }
    
    return position;
}

//linear search used to find items in inventory/product array
int linearSearchProduct(Product array[], int size, int name)
{
    int index = 0;
    int position = -1;
    bool found = false;
    
    while (index < size && !found)
    {
        if (array[index].itemNo == name)
        {
            found = true;
            position = index;
        }
        index++;
    }
    
    return position;
}

//used to add items to cart and check credit
void creditChecker(Customer array[], int pos, Product Array[], int Size, vector<Cart>&shoppingList)
{
    //variables
    int searchItem;
    char response = 'y';
    int i=0, qty;
    double sum;
    
    while (toupper(response) == 'Y')
    {
        cout << "Enter the item number: ";
        cin >> searchItem;
        int Found = linearSearchProduct(Array,Size,searchItem); 
    
        while (Found == -1)
        {
            cout << "Invalid item number. Please enter a valid number: ";
            cin >> searchItem;
            Found = linearSearchProduct(Array,Size,searchItem);     
        }
    
        cout << "Enter the quantity: ";
        cin >> qty;
        while (qty < 0)
        {
            cout << "quantity cannot be less than zero. Enter a valid quantity: ";
            cin >> qty;
        }
        
        
        sum = (qty * Array[Found].price);
        double remainder = array[pos].lineOfCredit-sum;
        
        //only adds item to the cart if there is credit left
        if (remainder >= 0)
        {
            shoppingList.push_back(Cart()); //pushes back the entries to fill the vector of structs
            shoppingList[i].item = Array[Found];
            shoppingList[i].quantity = qty;
            shoppingList[i].total = sum;
            i++; 
            cout << "Enter another item? Enter 'Y' to add another item or 'N' to quit and print summary: ";
            cin >> response;
            while (toupper(response)!= 'Y' && toupper(response) != 'N')
            {
                cout << "Invalid reponse. Please enter 'Y' or 'N': ";
                cin >> response;
            }
        }
        
        else
        {
          cout << "Quantity exceeded credit. Item was not added to cart"<<endl;
          break;
        }
            
    }

}

//used to print receipt
void orderSummary(string name, Customer array[],vector<Address>&AddList, int pos)
{
    //opening the file to write
    ofstream FileHandle;
    string orderNum = generateOrderNum();
    FileHandle.open(orderNum.c_str());
    
    double cost, remaining;
    
    FileHandle<< string(60,'-') << endl << "B2B Shopping Cart" << endl << string(60,'-') << endl;
    FileHandle << "\nOrder Number: " << orderNum << "\nAssociate: " << name << endl;
    FileHandle << "Customer Number: " << array[pos].customerNum << "\nCustomer: " << array[pos].customerName<<endl;
    FileHandle << "Address: " << AddList[pos].streetAddress << " |\n" << AddList[pos].city << "," << AddList[pos].state
    << AddList[pos].zipCode << endl << endl << endl;
    
    FileHandle << string(60,'-') << endl << setw(20) << left << "Item No" << setw(20) << left<< "Description" << setw(15)
        << left << "Qty" << setw(5) << "Total" << endl << string(60,'-') << endl;
    
    
    for (int i = 0; i < ShoppingCart.size(); i++) //prints the number of items
    {
      FileHandle << setw(20) << left << ShoppingCart[i].item.itemNo << " " << ShoppingCart[i].item.description <<
          " " << ShoppingCart[i].quantity << " " << ShoppingCart[i].total << endl;
        
        cost += ShoppingCart[i].total;
    }
    
    remaining = array[pos].lineOfCredit-cost;
    
    
    FileHandle << string(60,'-') << endl << setw(20) << left << "Total "<<cost << string(60,'-') << endl;
    FileHandle << setw(20) << left << "Remaining Credit " <<remaining<< endl;
    
}

//menu used to find items, fill cart, etc..
void menu(Customer array[],Product Array[], vector<Address>&vector,int &size)
{
    //variables to enter
    char response;
    string nameSearch, userName, numSearch;
    int found;
    
    //prompt-issue with entering name during second iteration
    cout << "Welcome to the B2B Shopping Cart! " << endl << "Please enter your name: ";
    getline(cin, userName);
    cout << "Hello " << userName << ". If you are searching by name enter 'A' or if you are searching by number enter 'B'" << endl;
    cout << "A or B? : ";
    cin >> response;
    cin.ignore();
    
    //Error checking
    while (toupper(response) != 'A' && toupper(response) != 'B')
    {
        cout << "Invalid response. Please enter either A or B: ";
        cin >> response;
        cin.ignore();
    }
    
    //searching by name
    if (toupper(response) == 'A')
    {
        cout << "Enter the customer name: ";
        getline(cin,nameSearch); 
        found = linearSearch(array,size, nameSearch);
        
        while (found == -1)
        {
            cout << "Invalid input. Please enter a valid customer name: ";
            getline(cin,nameSearch); 
            found = linearSearch(array,size, nameSearch); 
        } 
    }
    
    //searching by number
    else
    {
        cout << "Enter the customer number: ";
        cin >> numSearch;
        found = binarySearch(array,0,size,numSearch);
        
        while (found == -1)
        {
           cout << "Invalid input. Please enter a valid customer number: ";
           cin >> numSearch;
           found = binarySearch(array,0,size,numSearch);
        }
    }
    
    //appropriate functions to check credit
    creditChecker(array,found,Array,(size-1),ShoppingCart);
  
    //printing final result
    orderSummary(userName,array,vector,found);
    
    cout << "Would you like to search for another customer? Enter Y (yes) or N (no): ";
    cin >> response;
    cin.ignore();
    while (toupper(response) != 'Y' && toupper(response) != 'N')
    {
        cout << "Invalid response. Please enter either Y or N: ";
        cin >> response;
        cin.ignore();
    }
    
    if (toupper(response) == 'Y')
        menu(array,Array,vector,size);
    
    else
        exit(0);   
}


