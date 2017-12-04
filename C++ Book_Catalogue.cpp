#include <iostream>
#include <cstdlib>
#include <sstream> 

using namespace std;

class Book
{
	
public:
	
	enum bookCategory {novel, guidebook, children_book};
	string title;
	string author;
	bookCategory category;
	float price;
	string ISBN;
	unsigned int quantityInWarehouse;	
	unsigned int checkStock(); // function will return number of books in a warehouse
	bool sellBook(unsigned int quantity); // function will return true if we can sell the book 
	void changePrice(float newPrice);
	
};

void Book::changePrice(float newPrice)
{
	cout << "Book price \"" << title << "\". Old price: " << price;
	price = newPrice;
	cout << ". New price: " << price << "\n" ;
	return;
}

unsigned int Book::checkStock()
{
	return quantityInWarehouse;
}

bool Book::sellBook(unsigned int quantity)
{
	if (quantityInWarehouse < quantity)
	{
		cout << "There is not enough product in stock!\n";
		return false;
	}
	
	quantityInWarehouse -= quantity;
	
	cout << "Sold " << quantity << " titles of a book \"" << title << "\".\n";
	
	return true;
}

class BookCatalogue
{
	
private:
	
	unsigned int countBooks;
	unsigned int minimumLetter;
	static const unsigned int catalogueSize = 100;
	Book catalogue[catalogueSize];
	Book * pTitleCatalogue = &catalogue[0]; // set pointer on the first element of catalogue array
	Book * ArrayOfPointers[catalogueSize]; //array which keeps pointers
					
public:
	
	BookCatalogue();//constructor
	
	string & ISBNcompression(Book & copy);
	void sortISBN(int countBooks);
	void sortTitle(int countBooks);
	
	int searchByTheNumber(string ISBN);//fuction will return index in the catalogue where the book is kept

	int searchByTheTitle(string title);

	int addBook(Book copy); //will add new book and return index in the catalogue
		
	void showBookCatalogue();
	int categoryDiscount(Book::bookCategory category, unsigned int discount);
};

BookCatalogue::BookCatalogue()
{
	for(int i = 0; i < catalogueSize; i++) //fill array with pointers
	ArrayOfPointers[i] = &pTitleCatalogue[i];		
}

int BookCatalogue::searchByTheNumber(string ISBN)
{
	long long searchISBN; //string -> int
	istringstream iss(ISBN);
	iss >> searchISBN;
	
    int left = 0; 
    int right = countBooks - 1; 
	int middle = 0;
	
	while (true)
	{
		if (left > right)
		{
			cout << "There is no book with ISBN: " << searchISBN <<  endl;   
			break;
		}
		
		middle = (left + right)/2;
		
		long long intISBN;  //string -> int
		istringstream iss(catalogue[middle].ISBN);
		iss >> intISBN;
			
		if (intISBN == searchISBN)
		{
			cout << "ISBN " << searchISBN << " was found under index: " << middle << endl;
			break;
		} 	
		else if (intISBN < searchISBN)
		{
			left = middle + 1;			
		}
		else
		{
			right = middle - 1;
		}
			
	}
	return 0;  
}

int BookCatalogue::searchByTheTitle(string title)
{

    string searchTitle = title;
    
   	minimumLetter = 1; 
   	
    string partOftitle = searchTitle.substr(0,minimumLetter); 
		
	if(searchTitle.size()> partOftitle.size())
	{
		partOftitle = searchTitle;
		minimumLetter  = searchTitle.size();
	}
		
    int left = 0;
    int right = countBooks - 1; 
    int middle = 0;
    
	while(true)
	{
		if (left > right)
		{
			cout << "There is no books with phrase '" << searchTitle << "' in the title." <<  endl;     
			break;
		}
		
		middle = (left + right)/2;
	
		if(ArrayOfPointers[middle]-> title.substr(0,minimumLetter) == partOftitle)
		{
			cout << "Phrase '" << partOftitle << "'" << " was found in the title " <<  ArrayOfPointers[middle]->title << " under index: " << middle << endl;	
			
			while(true)
			{
				middle++;
				if(ArrayOfPointers[middle]->title.substr(0,minimumLetter) == partOftitle)	
					cout << "Phrase '" << partOftitle<< "'" << " was found in a title " <<  ArrayOfPointers[middle]-> title << " under index: " << middle << endl;	
				else
				{
					break;
				}
					
			}
					
			break;
		}
		else if (ArrayOfPointers[middle]-> title < partOftitle)
		{
			left = middle + 1;	
		}
		else
		{
			right = middle - 1;
		}
			
	}
    return 0;	 
}

int BookCatalogue::addBook(Book copy)
{

	ISBNcompression(copy);	

	unsigned int bookCounter = 0; //adding book to catalogue 
		
	while (catalogue[bookCounter].ISBN != "" || bookCounter >= catalogueSize - 1)
	{	
		bookCounter++; // we want to put the book on the last element of the array
		
		if(bookCounter > catalogueSize - 1) //if there is no place in the catalogue
		{
			cout << "There is no place in the catalogue." << endl;
			
			return -1;
		}
		if(catalogue[bookCounter].title == copy.title)
		{
			cout << "There is allready book with this title in the catalogue." << endl;
			
			return -1;
		}				
	}

	catalogue[bookCounter] = copy;
	
	countBooks = 0;
	
	for(int i = 0; i < catalogueSize; i++)
	{
		if(catalogue[i].ISBN !="")
			countBooks++;
	}
	
	sortISBN(countBooks);
	
	sortTitle(countBooks);
	
	cout << "Books in a warehouse: " << countBooks << endl;
	
	return countBooks;
}

string & BookCatalogue::ISBNcompression(Book& copy)
{
	
	for(int i = 0; i<=copy.ISBN.size(); i++)
	{
		if(copy.ISBN[i]=='-')
		{
			for(int j = i; j<copy.ISBN.size(); j++) 
			copy.ISBN[j] = copy.ISBN[j+1];	
			i-=1;
		}
	}
	
	return copy.ISBN; 
}

void BookCatalogue::sortISBN(int countBooks)
{
	unsigned int counter = countBooks;

	while(counter--)
	{
		for(int k = 0; k < countBooks - 1; k++)
		{	
			if(catalogue[k].ISBN>catalogue[k+1].ISBN)
				swap(catalogue[k], catalogue[k+1] );
		}	
	}
	
	return;
}

void BookCatalogue::sortTitle(int countBooks)
{ 
	unsigned int counter = countBooks;
	
	while(counter--)
	{
		for(int k = 0; k < countBooks - 1; k++)
		{			
			if(ArrayOfPointers[k]-> title > ArrayOfPointers[k+1]-> title)
				swap(ArrayOfPointers[k], ArrayOfPointers[k+1]);
		}	
	}

	return;
}

void BookCatalogue::showBookCatalogue()
{
	ios_base::fmtflags orig = //set format #.##
	cout.setf(ios_base::fixed, ios_base::floatfield);
	std::streamsize prec = cout.precision(2);
	
	char *categoryType[] =
	{
	    "novel",
	    "guidebook",
	    "children_book"
	};
	
	char Version;
	cout << endl << "Show book catalogue. Choose version: [S]hort or [L]ong?" << endl;
	
	while(cin >> Version)
	{
		cout << endl << "Catalogue content:" << endl;
		
		if(Version == 'L' || Version == 'l' )
		{
			for(int i = 0; catalogue[i].ISBN != ""; i++)
			{
				cout << "Title: " << catalogue[i].title<< endl;
				cout << "Author: " << catalogue[i].author << endl;		
				cout << "Category: " << categoryType[catalogue[i].category] << endl;
				cout << "Price: " << catalogue[i].price << endl;
				cout << "ISBN: " << catalogue[i].ISBN << endl << endl;	
			}
			
			break;
		}
		else if(Version == 'S' || Version == 's')
		{
			for(int i = 0; catalogue[i].ISBN != ""; i++)
			{
				cout << "Title: " << catalogue[i].title << endl;
				cout << "Author: " << catalogue[i].author << endl;		
				cout << "Price: " << catalogue[i].price << endl << endl;
			}
			break;
		}
		else
		{
			cout << "'S' or 'L'" << endl;
		}
					
	}
	
	cout.setf(orig, ios_base::floatfield);	//get back to the previous format
	cout.precision(prec);
	
	return;
}

int BookCatalogue::categoryDiscount(Book::bookCategory category, unsigned int discount) //will return the number of books with a discount
{
	
	ios_base::fmtflags orig =    // set format #.##
	cout.setf(ios_base::fixed, ios_base::floatfield);
	std::streamsize prec = cout.precision(2);
	
	unsigned int booksWithDiscount = 0;
	float floatDiscount = discount;
	
	char *categoryType[] =
	{
	    "novel",
	    "guidebook",
	    "children_book"
	};
		
	for(int i = 0; i <= countBooks; i++) //searching of the book from the set category
	{
		if(catalogue[i].category == category)
		{
			catalogue[i].Book::price = catalogue[i].Book::price - (catalogue[i].Book::price*(floatDiscount/100));
			
			booksWithDiscount++;
		}
	}
	
	cout.setf(orig, ios_base::floatfield); 
	cout.precision(prec);
	
	return booksWithDiscount;
}

int main()
{
	
	BookCatalogue cheapBookLibrary;
	
					
	Book add = { "Zamien chemie na jedzenie", "Julita Bator", Book::guidebook, 28.99, "9788324020454", 2 };					
	
	cheapBookLibrary.addBook(add);	
	
	add = { "Alkoholik", "Glowacz Ewelina", Book::guidebook, 29.90, "9788362329960", 2 };
	
	cheapBookLibrary.addBook(add);				
				
	add =	{ "Doctor Sleep", "Stephen King", Book::novel, 29.90, "9788378396185", 2 };
	
	cheapBookLibrary.addBook(add);
	
	Book add1 = { "The Complete Tales of Winnie-the-Pooh", "A. A. Milne", Book::children_book, 39.90, "978-83-246-7538-8", 1 };
	
	cheapBookLibrary.addBook(add1);
	
	Book add2 = { "Shawshank Redemption", "Stephen King", Book::novel, 32.49, "9788308049051", 2 };
	
	cheapBookLibrary.addBook(add2);
										  																										  

	cout << endl;// test search by the number
	string ISBN = "9788362329960";
	cheapBookLibrary.searchByTheNumber(ISBN);
	cout << endl;
	ISBN = "9788324020454";
	cheapBookLibrary.searchByTheNumber(ISBN);
	cout << endl;
	ISBN = "9788378396185";
	cheapBookLibrary.searchByTheNumber(ISBN);
	cout << endl;
	ISBN = "9788324675340";
	cheapBookLibrary.searchByTheNumber(ISBN);
	cout << endl;
	ISBN = "9788324675388";
	cheapBookLibrary.searchByTheNumber(ISBN);
	
			
	cout << endl<<endl; //test search by the title
	
	string searchTitle = "Shaw";
	
	cheapBookLibrary.searchByTheTitle(searchTitle);
	
	cout << endl;
	searchTitle = "Barszcz ukrainski";
	
	cheapBookLibrary.searchByTheTitle(searchTitle);
	
	cout << endl;
	searchTitle = "Doctor Sleep";
	
	cheapBookLibrary.searchByTheTitle(searchTitle);
	
	cout << endl;
	searchTitle = "The Man in the Brown Suit";
	
	cheapBookLibrary.searchByTheTitle(searchTitle);
	
	
	cout << endl << endl;// test discount 
	Book::bookCategory category;
	category = Book::guidebook;//set category
	unsigned int discount = 50;//discount percentage 
	cout << "Number of books with a discount: " << cheapBookLibrary.categoryDiscount(category, discount);
	
	cheapBookLibrary.showBookCatalogue();
	
	return 0;
} 
