#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        if(k.empty()){return 0;}
        unsigned long long w[5];
        for(int i = 0; i < 5; i++){w[i] = 0;}
        int len = k.length();

        if(len < 6){
            len = len - 1;

            //less than six load into w4, multiply by rValue and return
            for(int i = 0; len > 0; len--, i++){
                w[4] += (pow(36, len) * letterDigitToNumber(k[i]));
            }
            w[4] += letterDigitToNumber(k[k.length()-1]);

            return (w[4] * rValues[4]);
        }else{
            //6 or more
            //grab the first group of six (back six)
            int groupCount = len/6;
            int remainder = len%6;
            int wIdx = 4;

            for(int i = 1; i < groupCount+1; i++){
                int startSub = (int)(len - (6*i));
               std::string group = k.substr(startSub, 6);

                for(int j = 5; j >= 0 ; j--){
                    w[wIdx] += (pow(36, j) * letterDigitToNumber(group[5-j]));
                }

                wIdx = wIdx - 1;
            }

            //deal with remainder
            if(remainder > 0){
                std::string remain = k.substr(0, remainder);
                remainder = remainder - 1;
                for(int i = remainder; i >= 0; i--){
                    w[wIdx] += (pow(36, i) * letterDigitToNumber(remain[remainder-i]));
                }
            }

            //add up resulting number
            unsigned long long res = 0;
            for(int i = 0; i < 5; i++){
                res += (rValues[i] * w[i]);
            }

            return res;

        }

    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        if(std::isdigit(letter)){
            return ((letter-48)+26);
        }else{
            letter = (char)std::tolower(letter);
            return(letter-97);
        }
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
