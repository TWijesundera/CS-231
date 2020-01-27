{-
    Thisara Wijesundera
    05/17/2019
    CS-231

    Purpose
        The purpose of this module is to tell whether a meeting
        will be held by the chairman of the commitee based on
        how many people arrive within the time limit and if that
        number is greater than the minimum amount of people needed
        to hold the meeting

    Algorithm
        Take input and convert to a list of Int
        Get the first number in the list and save to variable
        Get second number in list and save to variable
        Recursively count the number of commmitte members that
            arrived before the amount of time the chairmen will wait
        The end of the holdMeeting function will create a tuple with
            the number of members to arrive before the timeout and a
            boolean value if the meeting was held or not
-}

module HoldMeeting where

holdMeeting :: String -> (Int, Bool)
parseInput :: String -> [Int]
counter :: Int -> [Int] -> Int

holdMeeting str = 
    let inputArray = parseInput str
    in scanList inputArray -- pass integer list to scanList function

-- Split the string up into an integer list
parseInput st = map read $ words st :: [Int]

-- Base cases in case only a list of 1 or list of nothing was passed to function
scanList [] = (0, False)
scanList [x] = (0, False)

scanList (x:xs) =
    let timeout = head xs
        firstInput = x
        count = counter timeout (tail xs)
    in (count, count >= firstInput)

-- Base cases
counter timeout [] = 0
counter timeout [x]
    | x <= timeout    = 1
    | otherwise   = 0

-- Recursively count members that arrived before the timeout
counter timeout (x:xs)
    | x <= timeout    = 1 + counter timeout xs
    | otherwise   = counter timeout xs
