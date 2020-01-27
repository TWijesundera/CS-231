{-
    Thisara Wijesundera
    CS-231

    Purpose
        The purpose of this program is to take a file for input and 
        compare it with a dictionary file. Then the program will print
        whether or not the word was spelled correctly or not.
    
    Algorithm
        Take from the command line input, the input file, the dictionary file
            and the output file. 
        Get the contents of the file input and the dictionary
            and assign them to variables that split them up into words
        Sort the file input removing any duplicate words
        Compare the words from the file input to the files in the dictionary
        Print to the output file if the file input words are spelled correctly
            or incorrectly.
-}
import System.IO
import System.Environment
import Data.Char 
import Data.List 

{-
    The purpose of this function is to proccess the input file,
    dictionary file, and the output file. Here is where the program
    prints to the output file.
-}
processInput copiedList dictList outFileHandle = do
    let result = compareStrings copiedList dictList
    
    hPutStrLn outFileHandle $ unlines $ map printToOutFile result
    return()

-- Function to print out the result tuple
printToOutFile (string, x) = x ++ "-------" ++ string

{- 
    This function is used to compare the word from the input file list
    and the dictionary list. If the word from the input file is less than
    the dictionary then the word is incorrect and the next item in the list
    is evaluated. If its greater than, then it will check the next item
    in the dictionary list. Finally, if they are equal then the next word in
    each file is compared and a tuple is created.
-}
compareStrings (x:xs) [] = ("INCORRECT", x) : compareStrings xs []
compareStrings [] _ = []
compareStrings (x:xs) (y:ys)
    | map toLower x == map toLower y      = ("CORRECT", x) : compareStrings xs ys
    | map toLower x < map toLower y       = ("INCORRECT", x) : compareStrings xs (y:ys)
    | otherwise   = compareStrings (x:xs) ys

-- Quicksort function that will sort words in a list and remove and dupliactes
quicksort[] = []    
quicksort (x:xs) = 
    let smallerSorted = quicksort [a | a <- xs, lessThan a x]  
        biggerSorted = quicksort [a | a <- xs, lessThan x a]  
    in  smallerSorted ++ [x] ++ biggerSorted  

lessThan x y = map toLower x < map toLower y

{-
    This is the main function of the program. This will take files from the
    comannd line. Open those files for reading or writing based on their function.
    Get the contents from each file and sort the input file. Then send the files
    to processInput for proceessing and printing. Finally main will close all
    the files and exit.
-}
main = do
    -- Get the files from command line arguments
    [inFile, dictionary, outFile] <- getArgs
    
    -- Open files for read and write
    inFileHandle <- openFile inFile ReadMode
    dictionaryHandle <- openFile dictionary ReadMode
    outFileHandle <- openFile outFile WriteMode
    
    contents <- hGetContents inFileHandle
    dictContents <- hGetContents dictionaryHandle
    
    -- Get the contents of the fileInput and sort it removing non alphabetical
        -- characters and duplicates 
    let copiedList = quicksort $ words $ map (\x -> if isAlpha x then x else ' ') contents
    let dictList = words dictContents
    processInput copiedList dictList outFileHandle
    
    -- Close all the files associated to the program
    hClose inFileHandle
    hClose dictionaryHandle
    hClose outFileHandle