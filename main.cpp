#include <iostream>
#include <stdlib.h>
#include <time.h>       //used for further randomizing the numbers we get
#include <vector>

#include "AudioFile.h"      //library used for reading and writing audio files

using namespace std;

vector<double> myConvolve(vector<double> a, vector<double> b){

    int i, j;
    int sizeA = a.size();       //getting the size from the first vector
    int sizeB = b.size();       //getting the size from the second vector
    int sizeC = sizeA + sizeB - 1;      //computing the size of the resulting vector, with the convolution

    vector <double> c(sizeC);       //creating the vector to store the convolution results
    int smallersize, biggersize;        //determination of bigger and smaller vector sizes, they are needed later on
    if(sizeA<sizeB){
        biggersize = sizeB;
        smallersize = sizeA;
    }
    else{
        biggersize = sizeA;
        smallersize = sizeB;
    }
    int count;      //counter for the number of summations added to compute a single cell in the convolution vector
    for(i=0; i<sizeC; i++){     //outside loop, for every one of the sizeC cells of the convolution vector
        c[i] = 0;       //initializing with 0
        count = 1;      //counter starts with 1, first summation is about to happen
        for(j=0; j<=i; j++){        //inside loop, for calculating c[i] result
            if(i-smallersize+count>biggersize-1 || count > smallersize){ //every cell of c vector is computed by maximum of smallersize
                break;      //if smallersize number of calculations is exceeded, loop stops
            }
            else {
                if(i >= smallersize){       //condition for later computations
                    c[i] += a[i- (i-smallersize+count)] * b[i-smallersize +count];
                    count++;        //increasing the operation counter
                }
                else{       //condition for first few loops, where a[0] and b[0] are both used
                    c[i] += a[i-j] * b[j];
                    count++;        //increasing the operation counter
                }
            }
        }
    }
    return c;       //function returns the resulting vector
}



void printConvolution(vector<double> c){    //function for printing a vector when needed
    int i, sizeC;
    sizeC = c.size();
    for(i=0; i<sizeC; i++){
        cout<< c[i]<<endl;
    }
}


int main()
{
    int N, i;

    vector<double> b(5);        //creating vector b
    //b = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};        //numbers used to check the correct operation of myConvolve function

    for(i=0; i<5; i++){     //filling up vector b as asked
        b[i]=0.2;
    }

    cout << "How many numbers would you like the vector A to have? "<<endl;  //message for user to know what to type
    cin >>  N;
    if(N<11){       //rejecting the number if it is smaller than 11
        cout<<"Vector size not big enough, sorry";
        return 0;
    }

    vector<double> a(N);        //creating vector a

    srand(time(0));
    for(i=0; i<N; i++){     //filling it up with random numbers between 0 and 1
        double x = rand()/static_cast <double> (RAND_MAX);
        a[i]=x;
    }

    //a = {2,4,6,8,10};     //numbers used to check the correct operation of myConvolve function
    cout<<"Part A of exercise"<<endl;
    vector<double> c = myConvolve(a, b);        //calling myConvolve for vectors a and b
    printConvolution(c);        //calling the printing function to print the result

    cout<<"Part B of exercise"<<endl;
    AudioFile<double> pinkNoise;        //reading the pinkNoise audio file
    pinkNoise.load ("../pink_noise.wav");
    vector<double> pnoise= pinkNoise.samples[0];        //creating vector with pinkNoise samples
    AudioFile<double> sampleAudio;      //reading the sampleAudio audio file
    sampleAudio.load ("../sample_audio.wav");
    vector<double> saudio = sampleAudio.samples[0];     //creating vector with sampleAudio samples

    AudioFile<double> pn_sa;        //creating an audiofile to write the result of pnoise and saudio
    pn_sa.samples[0] = myConvolve(pnoise, saudio);      //computing their convolution
    pn_sa.save("../pinkNoise_sampleAudio.wav");     //saving the result

    vector<double> whitenoise;      //creating vector to store whitenoise samples
    for(i = 0;i<pnoise.size(); i++){        //filling up vector(same size as pink noise) with random numbers between 0 and 1
        whitenoise[i] = rand()/static_cast <double> (RAND_MAX);
    }

    AudioFile<double> wn_sa;        //creating an audiofile to write the result of whitenoise and saudio
    wn_sa.samples[0] = myConvolve(whitenoise, saudio);      //computing their convolution
    wn_sa.save("../whiteNoise_sampleAudio.wav");        //saving the result


    return 0;
}
