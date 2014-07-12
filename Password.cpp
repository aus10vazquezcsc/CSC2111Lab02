#include "Password.h"

#include "ListArrayIterator.h"
using CSC2110::ListArrayIterator;

#include "ReadFile.h"
using CSC2110::ReadFile;

#include "Text.h"
using CSC2110::String;

#include <iostream>
using namespace std;

Password::Password()
{
    viable_words = new ListArray<String>();
	all_words = new ListArray<String>();
}

Password::~Password()
{
    ListArrayIterator<String>* iter = viable_words->iterator();
    while(iter->hasNext())
    {
        String* i = iter->next();
        delete i;
    }
	ListArrayIterator<String>* iters = all_words->iterator();
    while(iters->hasNext())
    {
        String* i = iters->next();
        delete i;
    }
    delete[] viable_words;
	delete[] all_words;
}

void Password::addWord(String* word)
{ 
    all_words->add(word);
}

void Password::guess(int try_password, int num_matches)
{
 //longest method
    int counters = 0;
	
    ListArrayIterator<String>* iter = all_words->iterator();
    while(iter->hasNext())
    {
        String* i = iter->next();
	
	    if(getNumMatches(i,all_words->get(try_password)) == num_matches){
	        viable_words->add(i);
	}	
    }
	
}

int Password::getNumberOfPasswordsLeft()
{
    return (all_words->size() - viable_words->size());
}

int Password::getNumMatches(String* curr_word, String* word_guess)
{
    int curr_word_size = curr_word->length();
	int word_guess_size = word_guess->length();
	int char_matches = 0;
	
    if(curr_word->compare(word_guess) == 0) return curr_word_size; 
    
	else
	    for(int x = 0; x < curr_word_size; x++){
		    for(int y = 0; y < word_guess_size; y++){
			
		        if(curr_word->charAt(x) == word_guess->charAt(y))
			        char_matches++;
			}		
		}
	
    return char_matches;
}

void Password::displayViableWords()
{
    for(int x = 0; x < viable_words->size() - 1; x++){
	    //cout << endl << viable_words->get(x)->displayString();
	}
}

String* Password::getOriginalWord(int index)
{
     return all_words->get(index-1);
}

int Password::bestGuess()
{
   int best_guess_index = -1;
   int best_num_eliminated = -1;
   int num_viable_passwords = getNumberOfPasswordsLeft();

   //loop over ALL words, even if they have been eliminated as the password
   int count = 1;
   ListArrayIterator<String>* all_iter = all_words->iterator();
   while(all_iter->hasNext())
   {
      String* original_word = all_iter->next();

      //loop over only those words that could still be the password
      //count up the number of matches between a possible password and a word in the original list
      int* count_num_matches = new int[len + 1];

      for (int i = 0; i < len; i++) 
      {
         count_num_matches[i] = 0;
      }

      ListArrayIterator<String>* viable_iter = viable_words->iterator();
      while(viable_iter->hasNext())
      {
         String* viable_word = viable_iter->next();
         int num_matches = getNumMatches(viable_word, original_word);
         count_num_matches[num_matches]++;
      }
      delete viable_iter;

      //find the largest number in the count_num_matches array
      //the largest number indicates the guess that will generate the most eliminations
      int most_num_matches = 0;
      for (int j = 0; j < len; j++) 
      {
         int curr_num_matches = count_num_matches[j];
         if (curr_num_matches > most_num_matches)
         {
            most_num_matches = curr_num_matches;
         }
      }

      //compute the fewest that can possibly be eliminated by guessing the current word (original list)
      int num_eliminated = num_viable_passwords - most_num_matches;

      //select the word to guess that maximizes the minimum number of eliminations (minimax)
      if (num_eliminated > best_num_eliminated)
      {
         best_num_eliminated = num_eliminated;
         best_guess_index = count;
      }
      
      count++;
      delete[] count_num_matches;
   }

   delete all_iter;
   return best_guess_index;  //return a 1-based index into the all_words list of words (careful)
}