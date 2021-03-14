
/*
 Name / student number : Nikola Beerkens / s1041042
 Name / student number : Jokubas Tumelis / s1039146

 ********** ASSIGNMENT 5 *****************
*/

#include <iostream>
#include <fstream>          // for file I/O
#include <cassert>        // for assertion checking
#include <stdlib.h>

using namespace std;

enum Action {Encrypt, Decrypt} ;

int seed = 0 ;
void initialise_pseudo_random (int r)
{
//  pre-condition:
    assert (r > 0 && r <= 65536) ;
/*  post-condition:
    seed has value r.
*/
    seed = r ;
}

int next_pseudo_random_number ()
{
//  pre-condition:
    assert (seed > 0 && seed <= 65536) ;
/*  post-condition:
    result value > 0 and result value <= 65536 and result value != seed at entry of function
*/
    const int seed75 = seed * 75 ;
    int next = (seed75 & 65535) - (seed75 >> 16) ;
    if (next < 0)
        next += 65537 ;
    seed = next ;
    return next ;
}

char rotate_char (char a, int r, Action action)
{

    //Pre-condition:
    assert(r>0&&r<=65536);
    //Post-condition: 0=<a<32
    if (a<32)
        return a;
    if (a>=32&&action==Encrypt)
       return(a-32+(r%(128-32))+(128-32))%(128-32)+32;

    if (a>=32&&action==Decrypt)
        return(a-32-(r%(128-32))+(128-32))%(128-32)+32;
}

void test_rotate_char ()
{   int r;
	//  Pre-condition:
	assert(true);
	//  Post-condition:
	// encrypts and decrypts character, displays their ASCII values

	char a;
    cout<<"Enter a random number: ";
    cin>>r;
    cout<<"Enter a single character: ";
    cin>>a;
    while (r>0)
     {

        cout<<"character: '"<<a<<"'"<<"\n"<<"ASCII code: "<<int(a);

        cout<<"\n"<<"ASCII code of rotate_char: "<<static_cast<int>(rotate_char(a,r,Encrypt));


        cout<<"\n"<<"ASCII code of rotate_char (rotate_char (a, r, Encrypt), r, Decrypt): ";
        cout<<static_cast<int>(rotate_char (rotate_char (a, r, Encrypt), r, Decrypt));
        cin>>r;
     }
    if (r<0)
        exit(0);
}

bool open_input_and_output_file (ifstream& infile, ofstream& outfile)
{
	//  Pre-condition:
	assert(infile != outfile);
	//  Post-condition: The file names are stored in a variable. The file information is converted to a string.
	string intxt, outtxt;
	cout<<"Enter input file name: ";
	cin>>intxt;
	cout<<"Enter output file name: ";
	cin>>outtxt;
	assert(infile!=outfile);
	infile.open(intxt.c_str());
	outfile.open(outtxt.c_str()); //Converts to string
	if(infile&&outfile)
		return true;
	else{
		cout << "Input or output file could not be opened." << endl;
		return false;
	}
}

Action get_user_action ()
{// Pre-condition:
    assert ( true ) ;
/*  Post-condition:
    result is the Action that the user has indicated that the program should encrypt / decrypt
*/
    cout << "Do you want to encrypt the file? (y/n): " ;
    string answer ;
    cin  >> answer ;
    if (answer == "y")
        return Encrypt;
    else if (answer =="n")
        return Decrypt;
    else
        cout<<"This in not a correct answer."<<endl;
}

int initial_encryption_value ()
{// Pre-conditie:
    assert (true) ;
/*  Post-condition:
    result is a value between 0 and 65355 (both inclusive)
*/
    int initial_value = -1 ;
    while (initial_value < 0 || initial_value > 65535)
    {
        cout << "Please enter the initial coding value (greater or equal to 0 and less than 65536)" << endl ;
        cin  >> initial_value ;
    }
    return initial_value ;
}

void use_OTP (ifstream& infile, ofstream& outfile, Action action, int initial_value)
{
	//  Pre-condition:
	assert(infile.is_open() && outfile.is_open());
	//  Post-condition:	outputs the encrypted/decrypted character to the file
	char a;
    char outputchar;
  	initialise_pseudo_random (initial_value);
    while(infile)
    {
    	infile.get(a);
    	outputchar = rotate_char(a, next_pseudo_random_number(), action);
    	outfile << outputchar;
	}
}

int main()
{
    const Action ACTION = get_user_action() ;
    ifstream input_file ;
    ofstream output_file;
    if (!open_input_and_output_file (input_file,output_file))
    {
        cout << "Program aborted." << endl ;
        return -1 ;
    }
    const int INITIAL_VALUE = initial_encryption_value () ;

    use_OTP (input_file,output_file,ACTION,INITIAL_VALUE);

    input_file.clear () ;
    output_file.clear () ;
    input_file.close () ;
    output_file.close () ;
    if (!input_file || !output_file)
    {
        cout << "Not all files were closed succesfully. The output might be incorrect." << endl ;
        return -1 ;
    }
    return 0 ;
}
