import Data.List.Split (splitOn)
import System.Environment (getArgs, getProgName)

listToPair :: [a] -> (a, a)
listToPair [] = error "Empty list"
listToPair s = (head s, last s)

stringsToInts :: [(String, String)] -> [(Int, [Int])]
stringsToInts [] = []
stringsToInts ((x, y):xs) = (read x, map read (splitOn " " y)) : stringsToInts xs

parse :: String -> [(Int, [Int])]
parse s = stringsToInts (map (listToPair . splitOn ": ") (splitOn "\n" s))

possibleSolutions :: [Int] -> [Int]
possibleSolutions [] = []
possibleSolutions [x] = [x]
possibleSolutions (x:y:xs) = possibleSolutions ((x + y):xs) 
    ++ possibleSolutions ((x * y):xs)
    ++ possibleSolutions ((x `concatinateInts` y):xs) -- remove this line to get the answer for part 1

isValid :: Int -> [Int] -> Bool
isValid s l = s `elem` possibleSolutions l

concatinateInts :: Int -> Int -> Int
concatinateInts x y = read (show x ++ show y)

getValidCalibrations :: [(Int, [Int])] -> [(Int, [Int])]
getValidCalibrations = filter (uncurry isValid)

sumCalibrations :: [(Int, [Int])] -> Int
sumCalibrations = sum . map fst

main :: IO ()
main = do
    args <- getArgs
    progName <- getProgName
    case args of
        [fileName] -> do
            contents <- readFile fileName
            let result = sumCalibrations . getValidCalibrations . parse $ contents
            print result
        _ -> putStrLn ("Usage: " ++ progName ++ " <input_file>")

