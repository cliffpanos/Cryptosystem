//
//  decryptor.cpp
//  Decryptor — ONE-WAY DECRYPTION SYSTEM using built-in text; the password is the only input
//
//  Created by Clifford Panos on 7/25/16.
//  Copyright © 2016 panOS. All rights reserved.
//
//  Decrypt a set of built-in text using a password as the only input
//  Uses simple one-character-to-one-character substitution keys
//
//  However, letter-frequency analysis is (somewhat) prevented because more than one key is used to
//  encrypt the characters in the text, and the use of the different keys is dependent on the length
//  of the password. Thus, the cipher text is severely more convoluted to those who do not know the pw.
//

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <iostream>
using namespace std;


string keyGen(string password) {
    //Generate the initial key using the password
    string stringOfAllCharacters = "&*()-_+=|ABCDEFGHIJK'~,.?<>LMNOPQRSTUVWXYZ/:;[]{}abcdefghijklmnop!@#$%^qrstuvwxyz0123456789";
    stringOfAllCharacters.push_back('"');
    //stringOfAllCharacters replaced the space character with a tilde here to prevent user input error
    string key = password; //Places the password at the beginning of the key
    bool isTheCharacterAlreadyInThePassword;
    for (unsigned int i=0; i <= stringOfAllCharacters.length() - 1; i++) {
        isTheCharacterAlreadyInThePassword = false;
        for (unsigned int j=0; j <= password.length() - 1; j++) {
            if (stringOfAllCharacters[i] == password[j])
                isTheCharacterAlreadyInThePassword = true;
        }
        if (isTheCharacterAlreadyInThePassword == false)
            key += stringOfAllCharacters[i];
    }
    return key;
}

string extraKeyGen(string password, string initialKey, int keyNum) {
    //Generate the second, third, fourth, and/or fifth keys to be used
    //These extra keys are used to make letter-frequency analysis nearly impossible
    
    string newKey = initialKey; //This initialKey will now be modified to produce the newKey
    string sectionOfPasswordToBeTranslated = password.substr((2*keyNum - 2), int(password.length() - 1));
    newKey.insert(int(newKey.length() - sectionOfPasswordToBeTranslated.length()), sectionOfPasswordToBeTranslated);
    newKey.erase((2*keyNum - 2),sectionOfPasswordToBeTranslated.length());
    if (keyNum % 2 == 0 && password.length() % 2 == 0)
        newKey = newKey.substr(45, newKey.length() - 1) + newKey.substr(0,45);
    if (keyNum % 2 == 1 && password.length() % 2 == 1)
        newKey = newKey.substr(30, 20) + newKey.substr(0,30) + newKey.substr(75, newKey.length() - 1) + newKey.substr(50,25);
    //cout << "newKey " << keyNum << " is: " << '"' << newKey << '"' << endl;
    return newKey;
}

int main() {
    string START = "YES";
    string text = "<k";
    text += '"';
    text += "n;saj#{ijYaOD";
    cout << text << endl;
    
    do {
        
        string input, password;
        string output = "";
        string stringOfAcceptablePasswordCharacters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz?!@#$%^&*(){}[]<>:;/._-=+";
        string stringOfAllCharacters = "' ,.?<>/:;[]{}!@#$%^&*()-_+=|ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        stringOfAllCharacters.push_back('"');
        
        cout << "Enter the password to decrypt the message built into this program." << endl;
        getline(cin, input);
        
        //Remove spaces (' ') between the three arguments
        string temporaryProcessedInput = "";
        for (unsigned int b = 0; b < input.length(); b++) {
            if (input[b] != ' ')
                temporaryProcessedInput += input[b];
        }
        password = temporaryProcessedInput; //This new input (now the password) now has no spaces

        //DELETES DUPLICATE CHARACTERS IN THE PASSWORD; i.e. "hashbrowns has two 's' and two 'h'"
        string duplicatePassword = ""; //This string will be the duplicate of the password and will be used to delete repeats
        duplicatePassword += password[0]; //The first character of password is added to this no matter what
        bool duplicate = false;			//This bool stores whether or not the character is duplicated
        for (int f=0; f < password.length(); f++) {    //This will actually duplicate possibleSolutions into duplicateList
            for (int g=0; g <= duplicatePassword.length() - 1; g++) {
                if (password[f] == duplicatePassword[g])
                    duplicate = true;
            }
            if (duplicate == false)  //If the number makes it through the whole loop without being found to be a repeat
                duplicatePassword += password[f];
            else duplicate = false;
        }
        password = duplicatePassword;
        
        
        //Generate between 1 and 5 keys depending on the length of the password
        int numberOfKeysToUse = 0;
        string key2, key3, key4, key5;
        if (password.length() == 1 || password.length() == 2) numberOfKeysToUse = 1;
        if (password.length() == 3 || password.length() == 4) numberOfKeysToUse = 2;
        if (password.length() == 5 || password.length() == 6) numberOfKeysToUse = 3;
        if (password.length() == 7 || password.length() == 8) numberOfKeysToUse = 4;
        if (password.length() >= 9) numberOfKeysToUse = 5;
        if (password.length() >= 9 && password.length() % 2 == 0) numberOfKeysToUse = 4;
        if (password.length() >= 9 && password.length() % 3 == 0) numberOfKeysToUse = 3;
        
        string initialKey = keyGen(password); //Generates the initialKey
        if (numberOfKeysToUse > 1) { //Produces the extra keys if the password is long enough
            for (unsigned int c=2; c <= numberOfKeysToUse; c++) {
                if (c == 2) key2 = extraKeyGen(password, initialKey, 2);
                if (c == 3) key3 = extraKeyGen(password, initialKey, 3);
                if (c == 4) key4 = extraKeyGen(password, initialKey, 4);
                if (c == 5) key5 = extraKeyGen(password, initialKey, 5);
            }
        }
 //-----
        //Decrypt function
        //The loops below will decrypt the message using the initialKey and any extraKeys
        for (unsigned int r = 0; r <= text.length() - 1; r+=numberOfKeysToUse) {
            if (numberOfKeysToUse > 0 && ((r+0) <= (text.length() - 1)))
                output += stringOfAllCharacters[initialKey.find(text[r])];
            if (numberOfKeysToUse > 1 && ((r+1) <= (text.length() - 1)))
                output += stringOfAllCharacters[key2.find(text[r+1])];
            if (numberOfKeysToUse > 2 && ((r+2) <= (text.length() - 1)))
                output += stringOfAllCharacters[key3.find(text[r+2])];
            if (numberOfKeysToUse > 3 && ((r+3) <= (text.length() - 1)))
                output += stringOfAllCharacters[key4.find(text[r+3])];
            if (numberOfKeysToUse > 4 && ((r+4) <= (text.length() - 1)))
                output += stringOfAllCharacters[key5.find(text[r+4])];
        }
        cout << endl << "Originally encrypted text characters that were unacceptable have been decrypted as '?' signs.";
        cout << endl << "The decrypted text is: " << endl << endl << output << endl << endl;
        START = "no";
    
    } while (START == "yes"); //Ends the do-while loop
    
    return 0; //Ends int main()
}
