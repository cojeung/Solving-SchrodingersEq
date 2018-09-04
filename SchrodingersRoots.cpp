#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <stdbool.h>

using namespace std;

/*
 * for linked list of root values
 */
class rootsListClass
{
  private:
      typedef struct rootNode
      {
        rootNode* next; // next pointer
      }* rNodePtr;

  public:
    int rootNum; // index of root
    double rootValue; // value of root

    void addNode(double data)
    {
      rNodePtr rN = new rootNode; // creates new

      rNodePtr head = NULL;
      rNodePtr curr = NULL;
      //rNodePtr temp = NULL;

      rN->next = NULL; // makes end of list

      rootValue = data; // makes that the rootValue
      rootNum++; // increments root number

      if (head) {
        curr = head;
        while(curr->next != NULL) {
          curr = curr->next;
        } //end while
        curr->next = rN; //point to new node
      } //end if
      else {
        head = rN;
      } //end else
    } //end AddNode

    double returnRootValue() {
      return rootValue;
    } //end returnRoot

    double returnRootNum() {
      return rootNum;
    } //end rootNum

}; //end class rootsListClass

/*
 *  Function used to check of sign of value changes, which indicates a root
 *  Returns true or false
 */
bool signChange( double y1_old, double y2_old, double y1_new, double y2_new)
{
  double y_old = y1_old - y2_old;
  double y_new = y1_new - y2_new;

  if ( (y_old > 0) && (y_new < 0) )  // from POS --> NEG
  {
    return true;
  } //end if

  return false;
}

/*
 *  Solving for the roots and writing them to corresponding files
 */
int main()
{
  /****************** DECLARING VARIABLES ********************/
    // Given Values
    double mE = 9.10938356e-31; // kg
    double hBar_J = 1.0545718e-34; // J * s

    // User Input Variables
    float L =  2 / 1e9; // length of well = 2 nm
    double mass = 0; //mass value----can type mE for mass of electron
    double U = 0; // potential

    double increment = 0;

    rootsListClass rootsList; // store roots

    ofstream myfile_rootNum;  // stores index of root
    myfile_rootNum.open("p1rn.txt");

    ofstream myfile_roots;  // stores values of root
    myfile_roots.open("p1root.txt");

    ofstream myfile_EVERYTHING; // stores every point of function to graph
    myfile_EVERYTHING.open("p1ugh.txt");

  /****************** GETTING INPUT ********************/

    cout << endl << "Please input the following values: " << endl;

    stringstream massInput; string mI; // to deal with input variable type

    int num;
    cout << "Potential (eV): U0 = " << endl;
    cout << "1) 2 eV \n2) 10 eV \n3) 10000 eV" << endl;
    cout << "Please select: ";
    cin >> num;

    if (num == 1) {
      U = 2;
      increment = 0.000001;
    }
    else if (num == 2) {
      U = 10;
      increment = 0.0001;
    }
    else if (num == 3) {
      U = 10000;
      increment = 0.1;
    }

    cout << "Solving for: U = " << U << " eV at well length = " << L << " nm"<< endl;

    cout << endl << endl;

  /****************** CONVERT VALUES ********************/

  //convert to U Joules
  U = U * (1.6e-19);

  //convert increments to Joules
  increment *= (1.6e-19);

  /****************** SETTING UP EQUATIONS ********************/

    // // Value of K term  k = sqrt ( [2 * mE * E] / hBar^2)
    // //double k = sqrt(  (2 * mE * E) / pow(hBar_eV,2) );
    //
    // // Value of alpha term a = sqrt ( 2m (U - E) / hBar^2 )
    // double a = sqrt( (2 * mass * (U - E)) / pow(hBar_eV, 2) );
    //
    // double y1 = 2 * ( 1 / tan(k * L) ); // cot = 1/tan
    // double y2 = ( k / a ) - ( a / k );

    double k = 0;
    double a = 0;

    double y1_new = 0;
    double y2_new = 0;
    double y1_old = 0;
    double y2_old = 0;

    mass = mE; //just to make it simpler

  /****************** GETTING ROOTS ********************/

  double E = 0;
  int countRoots = 0;
  for (double i = 0; i < U; i += increment) //goes until U
  {
    E = i;

    k = sqrt(  (2 * mass * E) / pow(hBar_J,2) );
    a = sqrt( (2 * mass * (U - E)) / pow(hBar_J, 2) );

    y1_new = 2 * ( 1 / tan(k * L) ); // cot = 1/tan
    y2_new = ( k / a ) - ( a / k );

    if (i > increment)
    {
      myfile_EVERYTHING << (y1_new - y2_new) << endl;
    }

    if ( (signChange( y1_old, y2_old, y1_new, y2_new) == true) // when sign changes
            || (y1_new - y2_new) == 0) // when it is 0
    {
      rootsList.addNode(E / (1.6e-19));
      countRoots++;
      //cout << E << setw(10);
      cout << "ROOT # " << rootsList.returnRootNum()
            << ": " << rootsList.returnRootValue() << endl;

      cout << "            k =" << k << "; \t a = " << a << endl;

      myfile_rootNum << rootsList.returnRootNum() << endl;
      myfile_roots << rootsList.returnRootValue() << endl;

    } //end if

    y1_old = y1_new;
    y2_old = y2_new;

  } //end for

  cout << endl << " NUM ROOTS:  " << countRoots << endl;

  myfile_rootNum.close();
  myfile_roots.close();

  return 0;
}
