#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>


using namespace std;

string material[50];
char cont;
int i=0,support[50],shape[50],N;
double width_out[50],width_in[50],length[50],I[50],height_v[50],height[50],E[50],radius[50],k[50];
double A[50],Ix[50],Iy[50],Ib[50],Iyield[50],e[50],Y[50],c[50],N_axis[50],stress_cr[50], Pb[50],Py[50];
double r[50], f1[50], f2[50],RHS[50], R[50],SR[50],stress_max[50],safe_n[50],f[50],Pmax[50],vmax[50];

//user enter value
void input(){// a void function with no argument is used to receive all the input
    int a=1;
    cout<<"\nThe possible end conditions of the columns(supports)."<<endl;//choose type of end conditions, to determine the value of K.
    cout<<"1. Both are pinned ends"<<endl;
    cout<<"2. Both are fixed ends"<<endl;
    cout<<"3. Pinned and fixed ends"<<endl;
    cout<<"4. Fixed and free ends"<<endl;
    while(a==1){
        a=0;
        cout<<"\nEnter the end conditions (1,2,3 and 4 only): ";//User input should be either 1,2,3 or 4.
        cin>>support[i];

        switch(support[i]){//program will choose the specific value of K based on the end conditions entered by user.
            case 1:
                k[i]=1;
                break;
            case 2:
                k[i]=0.5;
                break;
            case 3:
                k[i]=0.7;
                break;
            case 4:
                k[i]=2;
                break;
            default:
                cout<<"The end conditions selected is invalid.";
                a=1;
                break;
        }//end switch
    }//end while

    cout<<"\nEnter the materials of column (wood, steel, aluminium, or more) :";//wood,steel, and aluminium are the existed files.
    cin>>material[i];

    ifstream properties;//read input file
    properties.open(material[i].c_str());//open the specific material file

    if (!properties.is_open()){//if the file is failed to be opened, then the program will choose this selection
        cout<<"File does not exist. A new material file is opening."<<endl;// if the material chosen doest not exist, a new file contained the details of the new will be opened.
        ofstream properties;//open a file in write mode
        properties.open(material[i].c_str());//the file of the material chosen by the
        cout<<"\nEnter the elastic modulus of the materials, E(GPa): ";
        cin>>E[i];//user input the value and store it in an array
        cout<<"Enter the yield stress of the column, σ(MPa): ";
        cin>>Y[i];
        properties << E[i]<<" "<<Y[i]<<endl;//write inputted data from array into a file.
        properties.close();// close the current file.
    }//end if
    else{//if the file of the material entered by the user is existed, then the program will choose this selection
        properties>>E[i]>>Y[i];// the file only contains the elastic modulus and the yield strength of the materials.
        properties.close();
    }//end else
    //the material properties output/input file is completed.
    cout<<"\nEnter the safety factor: ";
    cin>>safe_n[i];
    cout<<"Enter the length of the column(m): ";
    cin>>length[i];
    cout<<"Enter e(m): ";
    cin>>e[i];
    cout<<"Enter c(m): ";
    cin>>c[i];

    cout<<"\nThe possible shape of the columns are as following:"<<endl;//generally, there 4 type of shape
    cout<<"1. I-column"<<endl;
    cout<<"2. T-column"<<endl;
    cout<<"3. rectangle/square"<<endl;
    cout<<"4. circle"<<endl;

    while(a==0){//while-loop to allow user re-enter the shape if previous input was invalid
        a=1;
        cout<<"\nEnter the column shape (1,2,3 and 4 only): ";//1.I-column,2.T-column,3.rectangle,4.circle.
        cin>>shape[i];

        switch (shape[i]){//switch statement is used to select the specific calculation program of each shape
            case 1:
                cout<<"Enter the height of column(m): ";
                cin>>height[i];
                cout<<"Enter the height of vertical bar(m): ";
                cin>>height_v[i];
                cout<<"Enter the outer width of the column(m): ";
                cin>>width_out[i];
                cout<<"Enter the inner width of the column(m): ";
                cin>>width_in[i];
                //calculation of some fundamental varaible needed for Euler's buckling formula
                Ix[i]=((width_out[i]*pow(height[i],3))-((width_out[i]-width_in[i])*pow(height_v[i],3)))/12;
                Iy[i]=(height_v[i]*pow(width_in[i],3)+(height[i]-height_v[i])*pow(width_out[i],3))/12;
                A[i]=width_out[i]*height[i]-(height_v[i]*(width_out[i]-width_in[i]));
                cout<<"\nIx= "<<Ix[i]<<"m^4"<<endl;
                cout<<"Iy= "<<Iy[i]<<"m^4"<<endl;
                cout<<"Cross-sectional area= "<<A[i]<<"m^2"<<endl;
                break;

            case 2:
                cout<<"Enter the height of column(m): ";
                cin>>height[i];
                cout<<"Enter the height of vertical bar(m): ";
                cin>>height_v[i];
                cout<<"Enter the outer width of the column(m): ";
                cin>>width_out[i];
                cout<<"Enter the inner width of the column(m): ";
                cin>>width_in[i];
                //calculation of some fundamental parameters needed for Euler's buckling formula
                A[i]=width_out[i]*(height[i]-height_v[i])+(height_v[i]*width_in[i]);
                N_axis[i]=(width_out[i]*(height[i]-height_v[i])*(height_v[i]+(height[i]-height_v[i])/2)+pow(height_v[i],2)*width_in[i]/2)/A[i];
                Ix[i]=width_out[i]*pow(height[i]-height_v[i],3)/12+width_out[i]*(height[i]-height_v[i])*pow(height_v[i]+(height[i]-height_v[i])/2-N_axis[i],2)+width_in[i]*pow(height_v[i],3)/12+height_v[i]*width_in[i]*pow(N_axis[i]-height_v[i]/2,2);
                Iy[i]=(height_v[i]*pow(width_in[i],3)+(height[i]-height_v[i])*pow(width_out[i],3))/12;
                cout<<"NA= "<<N_axis[i]<<"m"<<endl;
                cout<<"\nIx= "<<Ix[i]<<"m^4"<<endl;
                cout<<"Iy= "<<Iy[i]<<"m^4"<<endl;
                cout<<"Cross-sectional area= "<<A[i]<<"m^2"<<endl;
                break;

            case 3:
                cout<<"Enter the height of column(m): ";
                cin>>height[i];
                cout<<"Enter the width of the column(m): ";
                cin>>width_out[i];
                //calculation of some fundamental parameters needed for Euler's buckling formula
                A[i]=height[i]*width_out[i];
                Ix[i]=width_out[i]*pow(height[i],3)/12;
                Iy[i]=height[i]*pow(width_out[i],3)/12;
                cout<<"\nIx= "<<Ix[i]<<"m^4"<<endl;
                cout<<"Iy= "<<Iy[i]<<"m^4"<<endl;
                cout<<"Cross-sectional area= "<<A[i]<<"m^2"<<endl;
                break;

            case 4:
                cout<<"Enter the radius of the rod(m): ";
                cin>>radius[i];
                //calculation of some fundamental parameters needed for Euler's buckling formula
                Ix[i]=M_PI_4*pow(radius[i],4);//M_PI_4 = PI/4
                Iy[i]=Ix[i];
                A[i]= M_PI*pow(radius[i],2);
                cout<<"\nI= "<<Ix[i]<<"m^4"<<endl;
                cout<<"Cross-sectional area= "<<A[i]<<"m^2"<<endl;
                break;

            default:
                cout<<"The column shape entered invalid.";
                a=0;
                break;
            }//end switch
    }//end while
    E[i]=E[i]*pow(10,9);//the unit of elastic modulus in Pa
    Y[i]=Y[i]*pow(10,6);// unit in Pa
    }// end function input
//set a void function to create a text file, which can store input data entered by user
void input_file(){
    ofstream data;//open a file to store some of the desired input.
    data.open("input.txt");//named the text file as input.txt

    if(!data.is_open())//if the data file could not be opened.
    cout<<"Error while opening the file."<<endl;

    data<<"Support      Material      Elastic Modulus(GPa)    Yield Stress(MPa)   shape    Safety factor"<<endl;//only these inputs are stored
    for(int i=0;i<N;i++){
        data<<setw(3)<<support[i]<<setw(18)<<material[i]<<setw(17)<<E[i]/pow(10,9)<<setw(22)<<Y[i]/pow(10,6)<<setw(14)<<shape[i]
                <<setw(12)<<safe_n[i]<<endl;//the coding is separate into 2 lines.((each line should not more than 80 characters, good practice)
    }
    data.close();//the opened file is closed
}//end function input_file
//set a void function with arguments passed is used to calculate the buckling load
double buckling(double I){
    R[i]=sqrt(I/A[i]);
    SR[i]=k[i]*length[i]/R[i];
    cout<<"Slenderness ratio= "<<SR[i]<<endl;
    Pb[i]=(pow(M_PI,2)*E[i]*I)/pow((k[i]*length[i]),2);

    cout<<"\nBuckling:"<<endl;
    cout<<"The critical buckling load, P= "<<Pb[i]/1000<<"kN"<<endl;//divided by 1000, unit in kN
    cout<<"\nVerify the answer with Euler's formula."<<endl;
    stress_cr[i]=Pb[i]/A[i];

    if (stress_cr[i] <Y[i]){
        cout<<"The critical stress = "<<stress_cr[i]/1000000<<"MPa"<<" < "<<Y[i]/1000000<<"MPa"<<endl;//divided by 1M, unit in MPa
        cout<<"The critical buckling load is verified!"<<endl;
    }//end if

    else{
        cout<<"The critical stress = "<<stress_cr[i]/1000000<<"MPa"<<" > "<<Y[i]/1000000<<"MPa"<<endl;
        cout<<"The critical buckling load is not acceptable"<<endl;
    }return Pb[i];
}//end function buckling

//set a void function with arguments passed is used to calculate the buckling load
double yielding(double I){
    RHS[i]={0.0};
    r[i]=sqrt(I/A[i]);
    f1[i]=e[i]*c[i]/(r[i]*r[i]);
    Py[i]=Y[i]*A[i];
    cout<<"\nYielding:"<<endl;
    cout<<"σ_max=P/A(1+ec/r²sec(KL/2r√P/EA))"<<endl;
    cout<<"σ_max = "<<Y[i]/1000000<<"MPa"<<endl;
    cout<<"\nBy trial and error (with +- 0.1% margin):"<<endl;//use iteration method to find the value P.
    //cout<<setw(15)<<"P(N)"<<setw(15)<<"RHS"<<endl; (this line is hidden)

    do{ //a do-while loop is used to perform an interation for sorting through data sets
        f2[i]=k[i]*length[i]/(2*r[i])*sqrt(Py[i]/(E[i]*A[i]));
        RHS[i]=Py[i]/A[i]*(1+f1[i]/cos(f2[i]));
        Py[i]*=0.9999;//the value of yielding load is reduced by 0.1% each time
        //cout<<setw(15)<<setprecision(3)<<Py[i]<<setw(15)<<RHS[i]<<endl;
        //tabulate the iteration process (not going to show in actual output, show in slide presentation only)

    }while (abs(RHS[i]-Y[i])>=(0.001*Y[i]));//0.01 indicates a 1% margin error from the actual yielding load.

    cout<<"P = "<<Py[i]<<"N = "<< Py[i]/1000<<"kN"<<endl;//divided by 1000, in unit kN
    return Py[i];
}//end function yielding

//set a void function without arguments and return value is used to perform the calculation based on different conditions
void buck_yield_cal(){

    if(Ix[i]>Iy[i]){  //the section with smaller value of I will undergoes buckling and larger I will undergoes yielding
        cout<<"Buckling about y-y axis:"<<endl;
        buckling(Iy[i]);//the function named buckling is called (call a function within a function)
        yielding(Ix[i]);//the function named yielding is called
        Ib[i]=Iy[i];
        Iyield[i]=Ix[i];
    }//end if
    else if (Ix[i]<Iy[i]){
        cout<<"Buckling about x-x axis:"<<endl;
        buckling(Ix[i]);
        yielding(Iy[i]);
        Ib[i]=Ix[i];
        Iyield[i]=Iy[i];
    }//end else if
    else{  //In this conditions, the value of Ix and Iy are the same,therefore, both yielding and buckling use the same T
        buckling(Ix[i]);
        yielding(Ix[i]);
        Ib[i]=Ix[i];
        Iyield[i]=Ix[i];
    }//end else

    if(Pb[i]<Py[i])//smaller load is chosen.
    {
        cout<<"\nThe column will undergoes buckling first."<<endl;
        Pmax[i]=Pb[i]/safe_n[i];
        I[i]=Ib[i];
    }//end if
    else
    {
        cout<<"\nThe column will undergoes yielding first."<<endl;
        Pmax[i]=Py[i]/safe_n[i];
        I[i]=Iyield[i];
    }//end else
    stress_max[i]=Pmax[i]/A[i];//maximum stress can be applied to the load to prevent yielding or buckling
    cout<<"\nMaximum allowable load P that can be applied to the column= "<<Pmax[i]/1000<<"kN"<<endl;
    cout<<"Maximum allowable stress σ that can be applied to the column= "<<stress_max[i]/1000000<<"MPa"<<endl;
}//end function buck_yield cal
//set a void function called deflection() without arguments is used to calculate the deflection of column
void deflection(){ 
    f[i]=sqrt(Pmax[i]/(E[i]*I[i]))*k[i]*length[i]/2;
    vmax[i]=e[i]*(1.0/cos(f[i])-1);//maximum deflection
    cout<<"\nMaximum deflection:\n";
    cout<<"vmax= "<<vmax[i]<<"m ="<<vmax[i]*1000<<"mm";
    cout<<"\n"<<setfill('-')<<setw(85)<<"\n\n\n";
}//end function deflection
//set a void function called output to print the output table and store the output table in text file
void output(){
    ofstream data;//open file to store some of the desired input.
    data.open("output.txt");
    if(!data.is_open())//if the data file could not be opened.
    cout<<"Error in creating a new file."<<endl;

    data<<"Buckling load (kN)    Critical Stress(MPa)    Yielding Load (kN)    Maximum load (kN)    Maximum stress (Mpa)    Deflection (mm)"<<endl;//only these inputs are stored
    for(int i=0;i<N;i++){
        data<<setprecision(3)<<fixed;//fix to 3 decimal places
        data<<setw(12)<<Pb[i]/1000<<setw(25)<<stress_cr[i]/1000000<<setw(20)<<Py[i]/1000<<setw(21)<<Pmax[i]/1000<<setw(22)<<stress_max[i]/1000000<<setw(25)<<vmax[i]*1000<<endl;
    }//end for
    data.close();//close the opened file

    ifstream data1;
    data1.open("output.txt");// read the output file and display it in the console.
    char s=data1.get();
    while(!data1.eof())//end of file function is used.All data will be read from the file.
    {
        cout<<s;//print all the characters, one by one.
        s=data1.get();// read all the characters including white space
    }//end while
    data1.close();//close the opened file.
}//end function output
//set a void function called termination, to allow user continue/terminate the program after N times.
void termination(){
    cout<<"You have run this program for "<<N<<"times."<<endl;
    cout<<"Do you wish to continue the program? (y or n): ";
            cin>>cont;
            cout<<"\n\n";
                if (cont=='y'){
                   N++;
                }//end if
                else{ 
                    output();//call the void function output()
                    exit(0);
                }//end else 
}//end function termination

int main()
{
    cout<<"This program is used to determine the buckling load, yiedling load and the deflection of a coloumn."<<endl;
    cout<<"how many times you would like to run this program? (max:50): ";//maximum 50 times
    cin>>N;
    while(1){
        while(i<N){// program will run N times
            input();//call the void function input()
            buck_yield_cal();//call the void function buck_yield_cal()
            deflection();//call the void function deflection()
            i++;//counter+1
        }//end while(i<N)
            input_file();//call the void function input_file()
            termination();//call the void function termination()
    }//end while(1), infinity loop
}//end main()

