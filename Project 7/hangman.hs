import System.IO
import System.Environment
import Data.List
import Data.Char


{-
import System.Environment
import System.IO
import Data.Maybe

usage = "./hangman dictionary length_of_word guesses"

testTooSmall args
  | length args < 3 = Nothing
  | otherwise      = Just args

testTooBig args
  | args == Nothing            = Nothing
  | length (fromJust args) > 4 = Nothing
  | otherwise                  = Just args

play args = do
  putStrLn $ show args


main = do
  argList <- getArgs
  let bigEnough = testTooSmall argList
  let smallEnough = testTooBig bigEnough
  if smallEnough == Nothing
     then putStrLn usage
     else play (fromJust smallEnough)

-}

-- Limits the number of guesses
limitGuesses guesses
    | guesses < 5       = 5
    | guesses > 10      = 10
    | otherwise             = guesses
        
getPattern guessed = map (\c -> if c `elem` guessed then c else '_' )

getUnknowns = sum . map (\x -> if x == '_' then 1 else 0)

findPattern newPattern ws = newPattern `elem` [ p | (_, p, _) <- ws ]
 
updateWords _ []                 = []
updateWords guessedChar ((x, known, _):xs) = (x, known', count') : updateWords guessedChar xs
    where known' = getPattern guessedChar x
          count' = getUnknowns known'
        
-- Checks whether or not there are still blanks remaining 
isSolved = not . elem '_'

getWord [] = error "This is an error"
getWord ws = (\(w,_,_) -> w) (head ws)

debugMode option
    | option == "-s" = True
    | otherwise    = False

userGuess guessedCharacters = do
    putStrLn "Pick a character"
    userGuessed <- getChar
    putStrLn ""
    let userGuessed' = [toUpper userGuessed]    
    return $ userGuessed' `union` guessedCharacters

removeNonPattern createdPattern = filter (\(_, x, _) -> x == createdPattern)

largestCount = maximum . map (\(_, _, count) -> count)

getNewPattern p ws = (matches, pattern)
    where maxWords = filter (\(_,_,x) -> x == maxLetters) ws
        maxLetters = largestCount ws
        -- possibly competing pattern; simply pick first one
        pattern  = head [ p | (_, p, _) <- maxWords]
        -- filter list to only include words with new pattern
        matches  = filter (\(_, p, _) -> p == pattern) maxWords

uiLoop guessedCharacters wordLengthList wordToGuess guesses
    | isSolved wordToGuess        = putStrLn $ "Good job!\n" ++ wordToGuess
    | (guesses == 0)              = putStrLn $ "Game over.\nThe word was (maybe) " ++ (getWord wordLengthList) ++ "." 
    | otherwise                   = do
            print("Guesses remaining: " ++ show guesses)
            print wordToGuess
            guessedCharacters' <- userGuess guessedCharacters
            print guessedCharacters'
            
            case guessedCharacters' == guessedCharacters of
                True  -> do print "Try again!"   -- illegal input
                            uiLoop guessedCharacters wordLengthList wordToGuess guesses  
                False -> do
                    let wordLengthList' = updateWords guessedCharacters' wordLengthList                    
                        -- is the old pattern still in wordList' ?
                        canTrick  = findPattern wordToGuess wordList'
                    --print wordLengthList'
                    
                    case canTrick of
                        -- old pattern in wordList:
                        True  -> do print "Incorrect"
                                    let removedPattern = removeNonPattern wordToGuess wordList' 
                                    uiLoop guessedCharacters' removedPattern wordToGuess (guesses - 1)
                        False -> do print "Correct Guess\n"
                                    let (removedPattern, createdPattern) = getNewPattern wordToGuess wordList'
                                    uiLoop guessedCharacters' removedPattern createdPattern guesses
            
main = do
    [dictionary, wordLength, guesses] <- getArgs
    -- Change the wordLength variable to type Int for further use
    let userLength = read wordLength :: Int
        guesses' = read guesses :: Int

    let userGuessesRemain = limitGuesses guesses'
    --let isDebug = debugMode debugOption
    -- Get the contents of the dictionary file
    dictionaryHandle <- openFile dictionary ReadMode 
    dictContents <- hGetContents dictionaryHandle
    let dict = words dictContents
    
    -- Filter the list by the length of word provided
    let wordLengthList = filter(\x -> length x == userLength) dict
        wordToGuess = replicate userLength '_'
        wordLengthList' = map (\x -> (x, wordToGuess, userLength)) wordLengthList

    -- Start the game
    uiLoop [] wordLengthList' wordToGuess userGuessesRemain




