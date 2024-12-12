import Data.List.Split (splitOn)
import System.Environment (getArgs, getProgName)
import qualified Data.Map as Map

parse :: String -> [Int]
parse s = map read (splitOn " " s)

blink :: Int -> [Int]
blink 0 = [1]
blink n
    | even (length (show n)) = [read (take (length (show n) `div` 2) (show n)),
        read (drop (length (show n) `div` 2) (show n))]
    | otherwise = [n * 2024]


getStones :: Int -> [Int] -> [Int]
getStones 0 l = l
getStones maxBlinks l = getStones (maxBlinks - 1) (concatMap blink l)

getStonesCache :: Int -> [Int] -> Map.Map (Int, Int) Int -> (Int, Map.Map (Int, Int) Int)
getStonesCache _ [] c = (0,c)
getStonesCache 0 l c = (length l, c)
getStonesCache depth l cache
    | Map.member (depth, head l) cache
        = let (dataTail, cacheTail) = getStonesCache depth (tail l) cache in
            (cache Map.! (depth, head l) + dataTail, cacheTail)
    | head l == 0
        = let
            (result, newCache) = getStonesCache (depth - 1) [1] cache
            (dataTail, cacheTail) = getStonesCache depth (tail l) (Map.insert (depth, head l) result newCache)
        in
            (result + dataTail, cacheTail)
    | even (length (show (head l)))
        = let
            n = head l
            left = read (take (length (show n) `div` 2) (show n))
            right = read (drop (length (show n) `div` 2) (show n))
            (resultLeft, cacheLeft) = getStonesCache (depth - 1) [left] cache
            (resultRight, cacheRight) = getStonesCache (depth - 1) [right] cacheLeft
            (dataTail, cacheTail) = getStonesCache depth (tail l)
                (Map.insert (depth, head l) (resultLeft + resultRight) cacheRight)
        in
            (resultLeft + resultRight + dataTail, cacheTail)
    | otherwise
        = let
            (result, newCache) = getStonesCache (depth - 1) [head l * 2024] cache
            (dataTail, cacheTail) = getStonesCache depth (tail l) (Map.insert (depth, head l) result newCache)
        in
            (result + dataTail, cacheTail)


main :: IO ()
main = do
    args <- getArgs
    progName <- getProgName
    case args of
        [fileName] -> do
            contents <- readFile fileName
            let result1 = length (getStones 25 (parse contents))
            let result2 = fst $ getStonesCache 75 (parse contents) Map.empty
            putStrLn ("Stones after 25 blinks: " ++ show result1)
            putStrLn ("Stones after 75 blinks: " ++ show result2)
        _ -> putStrLn ("Usage: " ++ progName ++ " <input_file>")