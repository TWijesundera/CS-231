{-
    Thisara Wijesundera
    
    Purpose: To create a hangman program that cheats and prevets the user from winning
    Algorithm
        Take input from user. Sanitize the input and print out errors if there are any.
        Then limit the number of guesses the user has and capitalize the entire dictionary.
        Send the items to the playGame function which will initiate the hangman game.
        Get the user guess input and check if that character has already been guessed
            If they guess incorrectly and the character is not in the dictionary
                then decrement their guesses value
            Otherwise 
                Split the dictionary into a families checking if they contain that user input
        Pass the new items to the playGame function to restart the loop  
-}
import System.Exit
import System.Environment
import System.Directory
import System.IO
import Data.Char
import Data.List
import Data.Ord
import Control.Monad (when)


{-
    This function is used to play the evil hangman game
    First it prints out the word to guess. Then it prints
    out the number of guesses the user has with the letters
    the user has previously guessed.
    On each guess the program will change the family of words
    based on characters inputed by the user. This is looped several times
    Finally once the user runs out of guesses or the user 
    guesses the correct word, the game is over and a message is printed.
-}
playGame dictionary wordLength guesses debug guessedLetters wordToGuess pattern = do    
    if (length dictionary == 1) && not(elem '_' wordToGuess)
        then (putStrLn ("You win! The word was: " ++ head dictionary))
    else do
        if guesses == 0
            then do
                putStrLn ("You ran out of guesses. The word was: " ++ head dictionary)
        else do
            putStrLn (wordToGuess ++ "\n")
            putStrLn ("Guesses remaining: " ++ show guesses)
            putStrLn ("Letters guessed: " ++ guessedLetters)
            when debug (putStrLn $ "Number of words in the current family: " ++ show (length dictionary) ++ "\n") -- only prints if in debug mode: "-s"
            
            userInput <- hGetLine stdin
            let userInput' = if (userInput == []) then ' ' else (toUpper $ head userInput)
            -- Make sure the user input is and alphabetical character and not already guessed
            if (((not.isAlpha) userInput') || (elem userInput' guessedLetters))
                then do
                    putStrLn "\nAlready guessed. Try a different letter\n"
                    playGame dictionary wordLength guesses debug guessedLetters wordToGuess pattern
            else do
                -- Add the user guessed letter to the list of guessed letters
                let guessedLetters' = previouslyGuessesd userInput' guessedLetters ['A'..'Z']
                    checkDict = filter(\x -> userInput' `elem` x) dictionary
                if checkDict == []
                    then do
                        putStrLn "\nIncorrect Guess"
                        -- Decrement the amount of guesses based on a incorrect guess
                        playGame dictionary wordLength (guesses - 1) debug guessedLetters' wordToGuess pattern
                else do
                    putStrLn "Nice Guess!"
                    -- Create word families
                    let indiciesOfInput = getPatterns dictionary [userInput']
                        pattern = getLargestFamily indiciesOfInput
                        -- Create a new dictionary based on the user input
                        dictionary' = findFamily dictionary pattern [userInput']
                        -- Put the letter in the correct position of the word to guess
                        wordToGuess' = fillInWordToGuess wordToGuess userInput' pattern

                    --print dictionary'
                    --print indiciesOfInput
                    --print newFamilyPattern
                    
                    -- Keep allowing the user to guess with the current amount of guesses
                    playGame dictionary' wordLength (guesses-1) debug guessedLetters' wordToGuess' pattern


{-
    This is the main function of the program
    It is used to start the program and collect arguments from the console
    as well as sanatize input from the user
    Then it will send those variables to the playGame funtion which will start
    the evil hangman game
-}
main = do
    args <- getArgs
    -- Make sure there are enough arguments to start the game. 
        -- Otherwise print out a usage statment and exit
    if ((length args) > 4 || (length args) < 3)
        then do putStrLn "\nUsage: ./hangmanFinal dictionary_file word_length number_of_guesses [optional -s]\n"
                exitSuccess
    else
        return()
    
    dictionaryHandle <- openFile (getDictionary args) ReadMode
    dictionaryContents <- hGetContents dictionaryHandle
    -- Get each argument from the arguments passed to the program
    let dictionary = words dictionaryContents
        wordLength = getWordLength args
        guesses = getGuesses args
        debug = debugMode args
        -- Limit the amount of guesses a user can have
        guesses' = limitGuesses guesses
        -- Make the entire dictionary uppercase and filter it by the wordLength provided by the user
        dictionary' = map (map toUpper) (filter(\x -> length x == wordLength) dictionary)
        guessedLetters = []
        wordToGuess = replicate wordLength '_'
        pattern = []
    if dictionary' == []
        then do
            putStrLn "\nNo words in the dictionary with that length.\nTry again with a different word length"
            exitSuccess
    else do
        --print dictionary'
        --putStrLn wordLength
        --print guesses'
        --print debug
        playGame dictionary' wordLength guesses' debug guessedLetters wordToGuess pattern


-- Functions used to get each item from the arguments passed
getDictionary (dictionary:xs) = dictionary

getWordLength (x:wordLength:xs) = read wordLength :: Int

getGuesses (x:y:guesses:xs) = read guesses :: Int

debugMode args
   | (length args) == 4 && (last args) == "-s" = True
   | otherwise                                 = False

-- Limits the number of guesses that the user has
limitGuesses guesses
   | guesses < 5    = 5
   | guesses > 10   = 10
   | otherwise      = guesses

-- Put the userInput into the right place in the alphabet if it was guessed
-- Otherwise put in a blank character
previouslyGuessesd userInput guessedLetters [] = []
previouslyGuessesd userInput guessedLetters (x:xs)
    | (x == userInput) || (elem x guessedLetters) = x:(previouslyGuessesd userInput guessedLetters xs)
    | otherwise   = ' ':(previouslyGuessesd userInput guessedLetters xs)


getPatterns [] userInputCharacter = []
getPatterns (x:xs) userInputCharacter = (getWordPatterns x userInputCharacter) : getPatterns xs userInputCharacter

-- Find the indicies of where the userInputCharacter within the dictionaryWord
getWordPatterns dictionaryWord userInputCharacter = findIndices (`elem` userInputCharacter) dictionaryWord

-- Get the most common inidicies using maximumBy
getLargestFamily xs = head $ maximumBy (comparing length) $ group $ sort $ xs

findFamily [] wordPattern inputLetter = []
findFamily (x:xs) wordPattern inputLetter
   | (getWordPatterns x inputLetter) == wordPattern = x : findFamily xs wordPattern inputLetter
   | otherwise = findFamily xs wordPattern inputLetter

-- Fill in wordToGuess at the correct location in the word
fillInWordToGuess wordToGuess userInput []   = wordToGuess
fillInWordToGuess [] userInput familyPattern = []
fillInWordToGuess (x:xs) userInput pattern
   | (head pattern == 0)                   = userInput: fillInWordToGuess xs userInput (map (\x -> x - 1) (tail pattern))
   | otherwise                                   = x:fillInWordToGuess xs userInput (map (\x -> x - 1) pattern)