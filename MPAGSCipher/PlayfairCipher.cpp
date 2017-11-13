#include<string>
#include<PlayfairCipher.hpp>
#include<iostream>
#include<algorithm>
#include<utility>
#include<map>

PlayfairCipher::PlayfairCipher(const std::string& key): key_{key}
{
	setKey(key_);
}

void PlayfairCipher::setKey(const std::string& key)
{
	//store the original key
	key_ = key;
	// Append the alphabet
	std::string alphabetStr_{alphabet_.begin(),alphabet_.end()};
	key_.append(alphabetStr_);
	// Make sure the key is upper case
	std::transform(key_.begin(), key_.end(), key_.begin(), ::toupper);
	
	// Remove non-alpha characters
	auto iterNonAlpha = std::remove_if(key_.begin(), key_.end(), [] (char in) {return !isalpha(in);});
	key_.erase(iterNonAlpha, key_.end());

	// Change J -> I
	std::transform(key_.begin(), key_.end(), key_.begin(), [] (char in) {if(in == 'J'){return 'I';} else{return in;}});
	
	// Remove duplicated letters
	// Return true if alreadySeen contains the relevant character, appends alreadySeen
	std::string alreadySeen{""};
	auto iterDuplicates = std::remove_if(key_.begin(), key_.end(), [&] (char in) {if(alreadySeen.find(in)!=std::string::npos){return true;}else{alreadySeen+=in; return false;}});
	key_.erase(iterDuplicates, key_.end());
	// Store the coords of each letter

	std::map<char, std::pair<size_t, size_t>> mapLetter2Coords;
        std::map<std::pair<size_t, size_t>, char> mapCoords2Letter;

	size_t x{0},y{0};
	std::pair<size_t,size_t> elem{x,y};
	
	for(size_t i = 0; i < key_.length() ; i++)
	{
		elem = std::make_pair(x,y);
		mapLetter2Coords.insert(std::make_pair(key_[i], elem));
		mapCoords2Letter.insert(std::make_pair(elem, key_[i]));
		x++;

		if(x==5)
		{
			x=0;
			y++;
		}
		if(y==5)
		{
			break;
		}
	
	}

	// Store the playfair cipher key map
	
	mapLetter2Coords_ = mapLetter2Coords;
	mapCoords2Letter_ = mapCoords2Letter;
		
}

const std::string PlayfairCipher::applyCipher(const std::string& inputText, const CipherMode cipherMode)
{
	std::string input = inputText;

	// Change J->I
	switch(cipherMode)
	{
		case CipherMode::Encrypt:
			std::cout << "Encrypting..." << std::endl;
		case CipherMode::Decrypt:
			std::cout << "Warning: decryption not yet implemented, encrypting instead." << std::endl;
	}
	
	std::transform(input.begin(), input.end(), input.begin(), [] (char in) {if(in == 'J'){return 'I';} else{return in;}});
	// If repeated chars in a digraph add an X or Q if XX
	const std::string x = "X";

	for(size_t i = 1; i < input.length(); i++)
	{
		if(input[i] == input[i-1])
		{
			input.insert(i, x);
		}
	}

	// if the size of input is odd, add a trailing Z

	if(input.length()%2 != 0)
	{
		input+="Z";
	}
	
	// Loop over the input in Digraphs

	std::pair<size_t,size_t> firstCoord, secondCoord;
	size_t store;

	for(size_t i = 0; i < input.length(); i+=2)
	{
		firstCoord = mapLetter2Coords_.find(input[i])->second;
		secondCoord = mapLetter2Coords_.find(input[i+1])->second;

	//	- Apply the rules to these coords to get `new' coords

		if(firstCoord.first != secondCoord.first && firstCoord.second != secondCoord.second)
		{
			store = firstCoord.first;
			firstCoord.first = secondCoord.first;
			secondCoord.first = store;
		}
		else if(firstCoord.first == secondCoord.first)
		{
			firstCoord.second = (firstCoord.second + 1)%5;
			secondCoord.second = (secondCoord.second + 1)%5;
		}
		else if(firstCoord.second == secondCoord.second)
		{
			firstCoord.first = (firstCoord.first + 1)%5;
			secondCoord.first = (secondCoord.first + 1)%5;
		}
	// 	- Find the letter associated with the new coords

		input[i] = mapCoords2Letter_.find(firstCoord)->second;	
		input[i+1] = mapCoords2Letter_.find(secondCoord)->second;
	}

	// return the text
	std::cout << "Playfair cipher applied!" << std::endl;
	const std::string output = input;
	return output;

}
