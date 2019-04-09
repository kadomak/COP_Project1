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

//
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
struct Address{ string streetAddress; string city; string state; string zipCode; }; //holds address data
struct Customer{ string customerNum; string customerName; double lineOfCredit; Address * corperateAddress; }; //holds customer data
struct Product{ int itemNo; string description; double price; }; //holds product data
struct Cart{Product item; int quantity; double total;}; // holds cart data
vector<Cart> ShoppingCart; //used to store items added to cart

//Function prototypes-explanation given in function definition
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


/**************************************************************
 *                 CustomerDataInput                          *
 * Called by: main function                                   *
 * Passed   : 4 arguments: a string, customer array, address  *
 *            vector, and int                                 *
 * Purpose  : Fills in customer array and address vector      *
 * Returns  : None                                            *
 **************************************************************/
void CustomerDataInput(string file, Customer CustList[],vector<Address>&AddList, int Length)
{
    
    //Temporary strings & vectors used in parsing
    vector<string> tempCust; 
    vector<string> tempAdd; 
    string str, tempSTR;
    int i =0;
    
    //Opening the file
    ifstream Filename;
    Filename.open(file.c_str()); //string is passed here
    
    if (Filename.is_open()) //if the file is open
    {
        while (getline(Filename,str)) //reads in entire line
        {
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
            i++; //increments to fill the array/vector
        }
        
        //Reading in addresses- fix this so only unique addresses are added after the customer array is filled
       vector<Address>::iterator ITERATOR = AddList.begin();
       for (int i = 0; i<Length; i++)
       { 
           CustList[i].corperateAddress = &(*(ITERATOR+i));      
       } 
         
       Filename.close(); //closes the file
    }
     
   else //ends the program if the file doesn't open
   {
       cout << "Error opening file. Terminating program." << endl;
       exit(0); 
   }
}

/**************************************************************
 *                 InventoryDataInput                         *
 * Called by: main function                                   *
 * Passed   : 3 arguments: a string, product array, and int   *
 * Purpose  : Fills in product array                          *
 * Returns  : None                                            *
 **************************************************************/
void InventoryDataInput(string file,Product ProdList[],int Length)
{
    //Temporary vectors & strings used in parsing
    string str;
    vector<string> tempProd;
    int i = 0;
    
    //Opening the file
    ifstream Filename;
    Filename.open(file.c_str()); //string is passed here
    
    if (Filename.is_open()) // if the file is open
    {
        while(getline(Filename,str)) //reads in the data
        {
             tempProd = parse(str, ','); //vector of line parsed by the ,
        
            //indexing the vector to fill in the array of structs
            ProdList[i].itemNo = atoi(tempProd[0].c_str());  // converting the type from string to int
            ProdList[i].description = tempProd[1]; 
            ProdList[i].price = atof(tempProd[2].c_str());  //converting the type 
            i++;
        }
        
        //Closing the file
        Filename.close();
    }
    
    else //closes the program if the file doesn't open
    {
        cout << "Error opening file. Terminating program.";
        exit(0);
    }    
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

/**************************************************************
 *                 selectionSort                              *
 * Called by: main function                                   *
 * Passed   : 3 arguments: customer array, address vector     *
 *            and int                                         *
 * Purpose  : sorts the customer array by number              *
 * Returns  : None                                            *
 **************************************************************/
void selectionSort(Customer array[],vector<Address>&AddList, int Length)
{
    int start, minIndex; 
    double minCredit;
    string minVal, minName, minStreet, minCity, minState, minZip;
    Address * minAddress;
    
    for (start =0; start <(Length-1); start++) //iterates through the array
    {
        //begins indexing at the beginning
        ////setting the minimum values at the beginning of the array
        minIndex = start; 
        minVal = array[start].customerNum; 
        minName =array[start].customerName;
        minCredit = array[start].lineOfCredit;
        minAddress = array[start].corperateAddress;
        
        minStreet = AddList[start].streetAddress; //using pointers to access Addresses
        minCity = AddList[start].city;
        minState = AddList[start].state;
        minZip = AddList[start].zipCode; 
        
        
        for (int index = (start+1); index <Length; index++) //comparing two values
        {
            if (array[index].customerNum < minVal) //determining the lesser value 
            {
                //setting the new minimums to the lesser value index
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
        
        //swapping the values
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




/**************************************************************
 *                 binarySearch                               *
 * Called by: menu function                                   *
 * Passed   : 4 arguments: customer array, low int, high int, *
 *            and string (ie search item)                     *
 * Purpose  : To find a customer by number                    *
 * Returns  : Integer                                         *
 **************************************************************/
int binarySearch(Customer array[], int low, int high, string key) 
{
    while (low <= high) //iterates through array
    { 
	    int mid = low + (high - low) / 2; //determines midpoint
	  
	        // Check if middle value is equal to key 
	        if (array[mid].customerNum == key) 
	            return mid; //if value is found, returns position it is found in
	  
	        // If middle value is less than key, search right half
	        if (array[mid].customerNum < key) 
	            low = mid + 1; //new low value
	  
	        // Search left half
	        else
	            high = mid - 1; //new high value 
     }
    
    return -1; //if item not found
} 

/**************************************************************
 *                linearSearch                                *
 * Called by: menu function                                   *
 * Passed   : 3 arguments: customer array, size int,          *
 *            and string (ie search item)                     *
 * Purpose  : To find a customer by name                      *
 * Returns  : Integer                                         *
 **************************************************************/
int linearSearch(Customer array[], int size, string name)
{
    int index = 0; //begins search at beginning
    int position = -1; //if not found
    bool found = false; //initially not found
    
    while (index < size && !found) //iterates through array if not found and position is less than size
    {
        if (array[index].customerName == name) //when found
        {
            found = true;
            position = index;
        }
        index++; //if not found increment index
    } 
    
    return position;
}

/**************************************************************
 *                linearSearchProduct                         *
 * Called by: menu function                                   *
 * Passed   : 3 arguments: product array, size int,           *
 *            and string (ie search item)                     *
 * Purpose  : To find a product by name                       *
 * Returns  : Integer                                         *
 **************************************************************/
int linearSearchProduct(Product array[], int size, int name)
{
    int index = 0; //begins search at beginning
    int position = -1; //if not found
    bool found = false; //initially not found
    
    while (index < size && !found) //iterates through array if not found and position is less than size
    {
        if (array[index].itemNo == name) //when found
        {
            found = true;
            position = index;
        }
        index++; //if not found increment index
    } 
    
    return position;
}

/**************************************************************
 *                creditChecker                               *
 * Called by: menu function                                   *
 * Passed   : 5 arguments: customer and product array,        *
 *            size int, position int, and cart vector         *
 * Purpose  : To determine if there is credit                 *
 * Returns  : None                                            *
 **************************************************************/
void creditChecker(Customer array[], int pos, Product Array[], int Size, vector<Cart>&shoppingList)
{
    //variables
    int searchItem, amount;
    int i=0, qty;
    double sum;
    int increment = 0;
    
    //prompt- determines how many items to add to receipt
    cout << "How many items do you want to enter? ";
    cin >> amount;
    
    //cannot have an empty recepit; error checking
    while (amount <= 0)
    {
        cout << "Cannot enter less than zero items. Please enter a valid number: ";
        cin >> amount;
    }
    
    //this loop allows the user to continually add items to their order
    do
    {
        cout << "Enter the item number: ";
        cin >> searchItem;
        int Found = linearSearchProduct(Array,Size,searchItem); //determines if item is in product array
    
        while (Found == -1) //if item isn't in array; error checking
        {
            cout << "Invalid item number. Please enter a valid number: "; //allows user to correct their mistake
            cin >> searchItem;
            Found = linearSearchProduct(Array,Size,searchItem);    //determins if item is in product array 
        }
    
        cout << "Enter the quantity: "; //enters quantity
        cin >> qty;
        
        while (qty < 0) //cannot have quantity less than zero; error checking
        {
            cout << "Quantity cannot be less than zero. Enter a valid quantity: "; //allows user to correct their mistake
            cin >> qty;
        }
        
        
        sum = (qty * Array[Found].price); //calculates the sum which is quantity * price
        double remainder = array[pos].lineOfCredit-sum; //calculates remainder which is credit - sum
        
        //only adds item to the cart if there is credit left
        if (remainder >= 0)
        {
            shoppingList.push_back(Cart()); //pushes back the entries to fill the vector of structs
            shoppingList[i].item = Array[Found];
            shoppingList[i].quantity = qty;
            shoppingList[i].total = sum;
            i++; //increments vector 
        }
        
        else //if there is no more credit remaining, terminates the loop
        {
          cout << "Quantity exceeded credit. Item was not added to cart"<<endl;
          break;
        } 
        
        increment++; //increments to repeat the loop
        
    } while (increment <amount); //does this the desired amount of items added
}

/**************************************************************
 *                orderSummary                                *
 * Called by: menu function                                   *
 * Passed   : 4 arguments: string, customer array,            *
 *            address vector,  position int,                  *
 * Purpose  : Writes receipt to a file                        *
 * Returns  : None                                            *
 **************************************************************/
void orderSummary(string name, Customer array[],vector<Address>&AddList, int pos)
{
    //opening the file to write
    ofstream FileHandle;
    string orderNum = generateOrderNum(); //calls the generate order number function to create a random number
    FileHandle.open(orderNum.c_str()); //creates the file
    
    double cost, remaining; //variables used in displaying the remaining credit and total cost of items
    
    //this block of code formats the customer information
    FileHandle<< string(65,'-') << endl << "B2B Shopping Cart" << endl << string(65,'-') << endl;
    FileHandle << "\nOrder Number: " << orderNum << "\nAssociate: " << name << endl;
    FileHandle << "Customer Number: " << array[pos].customerNum << "\nCustomer: " << array[pos].customerName<<endl;
    FileHandle << "Address: " << AddList[pos].streetAddress << " |\n" << AddList[pos].city << "," << AddList[pos].state
    << AddList[pos].zipCode << endl << endl << endl;
    
    //this creates the first row of the table
    FileHandle << string(65,'-') << endl << setw(20) << left << "Item No" << setw(25) << left<< "Description" << setw(10)
        << left << "Qty" << setw(5) << "Total" << endl << string(65,'-') << endl;
    
    //prints the number of items
    for (int i = 0; i < ShoppingCart.size(); i++) 
    {
      FileHandle << setw(15) << left << ShoppingCart[i].item.itemNo << setw(30) << left << ShoppingCart[i].item.description << left << setw(10)
        << ShoppingCart[i].quantity << fixed << setprecision(2)<< "$" << ShoppingCart[i].total << endl;
        
        cost += ShoppingCart[i].total; //the total cost is the sum of each item
    }
    
    remaining = array[pos].lineOfCredit-cost; //determines customer line of credit remaining by subtracting intial credit and cost
    
    FileHandle << string(65,'-') << endl << setw(55) << left << "Total "<< fixed << setprecision(2)<< "$" << cost << endl << string(65,'-') << endl;
    FileHandle << setw(55) << left << "Remaining Credit " << fixed << setprecision(2) << "$"<< remaining<< endl;
    
}

/**************************************************************
 *                menu                                        *
 * Called by: main function                                   *
 * Passed   : 4 arguments: customer array, product array      *
 *            address vector,  size int,                      *
 * Purpose  : Writes receipt to a file                        *
 * Returns  : None                                            *
 **************************************************************/
void menu(Customer array[],Product Array[], vector<Address>&vector,int &size)
{
    //variables to enter
    char response;
    string nameSearch, userName, numSearch;
    int found;
    
    //prompt
    cout << string(65,'-') << endl;
    cout << "Welcome to the B2B Shopping Cart! " << endl << endl;
    cout << "This program allows a buyer to select items to add to their cart." << endl;
    cout << "A customer can be found by their ID number or their name. \nBe careful not to exceed the alotted credit." <<
    endl << "Please enter your name to begin." << endl << string(65,'-') << endl;
    cout << "Your name: ";
    getline(cin, userName); //gets the user's name
    cout << "\nHello " << userName << ". If you are searching by name enter 'A'. \nEnter 'B' to search by number." << endl;
    cout << "A or B? : ";
    cin >> response;
    cin.ignore();
    
    //Error checking if they enter neither A nor B
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
        found = linearSearch(array,size, nameSearch); //calls linear search
        
        while (found == -1) //if not found allows user to fix their mistake
        {
            cout << "Invalid input. Please enter a valid customer name: ";
            getline(cin,nameSearch); 
            found = linearSearch(array,size, nameSearch); //calls linear search to make sure error is corrected
        } 
    }
    
    //searching by number
    else
    {
        cout << "Enter the customer number: ";
        cin >> numSearch;
        found = binarySearch(array,0,size,numSearch); //calls binary search to find the customer by number
        
        while (found == -1) //if not found, gives the user opportunity to correct their mistake
        {
           cout << "Invalid input. Please enter a valid customer number: ";
           cin >> numSearch;
           found = binarySearch(array,0,size,numSearch); //cals binary search to make sure error is corrected
        }
    }
    
    //calls the creditChecker to determine if there is credit left
    creditChecker(array,found,Array,(size-1),ShoppingCart);
  
    //calls the orderSummary function to print the receipt
    orderSummary(userName,array,vector,found);   
}
