import Data.List.Split (splitOn)
import System.Environment (getArgs, getProgName)
import Data.Bits (xor)

parse :: String -> [Integer]
parse = map read . splitOn "\n"

nextSecret :: Integer -> Integer
nextSecret s =
    let result1 = (s `xor` (s * 64)) `mod` 16777216
        result2 = (result1 `xor` (result1 `div` 32)) `mod` 16777216
    in
        result2 `xor` (result2 * 2048) `mod` 16777216

calculateSecret :: Integer -> Integer -> Integer
calculateSecret depth s
    | depth == 0 = s
    | otherwise = calculateSecret (depth - 1) (nextSecret s)

getDifferences :: [Integer] -> [Integer]
getDifferences [] = []
getDifferences [x] = []
getDifferences (x:y:xs) = (y - x) : getDifferences (y:xs)

main :: IO ()
main = do
    args <- getArgs
    progName <- getProgName
    case args of
        [fileName, n] -> do
            contents <- readFile fileName
            let result = map (calculateSecret (read n)) (parse contents)
            let sumResult = sum result
            putStrLn (n ++ "th secret: " ++ show sumResult)
        _ -> putStrLn ("Usage: " ++ progName ++ " <input_file> <depth>")