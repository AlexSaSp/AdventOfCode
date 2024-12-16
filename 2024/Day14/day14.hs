import Data.List.Split (splitOn)
import System.Environment (getArgs, getProgName)

stringListToIntTuple :: [String] -> (Int, Int)
stringListToIntTuple l = (read (head l), read (last l))

listToTuple :: [a] -> (a, a)
listToTuple [x, y] = (x, y)

parsePositionVelocity :: String -> (Int, Int)
parsePositionVelocity s = stringListToIntTuple (splitOn "," (drop 2 s))

parseLines :: [String] -> [((Int, Int), (Int, Int))]
parseLines = map (listToTuple . map parsePositionVelocity . splitOn " ")

parse :: String -> [((Int, Int), (Int, Int))]
parse s = parseLines (splitOn "\n" s)

move :: Int -> ((Int, Int), (Int, Int)) -> (Int, Int)
move n ((x, y), (dx, dy)) = (mod (x + n * dx) 101, mod (y + n * dy) 103)

splitQuadrants :: [(Int, Int)] -> (Int, Int, Int, Int)
splitQuadrants [] = (0, 0, 0, 0)
splitQuadrants ((x, y):xs)
    | x < 50 && y < 51 = (1 + a, b, c, d)
    | x > 50 && y < 51 = (a, 1 + b, c, d)
    | x < 50 && y > 51 = (a, b, 1 + c, d)
    | x > 50 && y > 51 = (a, b, c, 1 + d)
    | otherwise = (a, b, c, d)
    where (a, b, c, d) = splitQuadrants xs

getSafetyFactor :: (Int, Int, Int, Int) -> Int
getSafetyFactor (a, b, c, d) = a * b * c * d

visualize :: [(Int, Int)] -> String
visualize positions = unlines [[if (x, y) `elem` positions then '#' else '.' | x <- [0..100]] | y <- [0..102]]

getNSafetyFactors :: Int -> [((Int, Int), (Int, Int))] -> [Int]
getNSafetyFactors 10402 pos = []
getNSafetyFactors n pos = getSafetyFactor (splitQuadrants (map (move n) pos)) : getNSafetyFactors (n + 1) pos

findPosInList :: Int -> [Int] -> Int
findPosInList _ [] = 0
findPosInList n (x:xs)
    | n == x = 0
    | otherwise = 1 + findPosInList n xs


main :: IO ()
main = do
    args <- getArgs
    progName <- getProgName
    case args of
        [fileName, n] -> do
            contents <- readFile fileName
            let positions = map (move (read n)) (parse contents)
            let quadrants = splitQuadrants positions
            let result = show $ getSafetyFactor quadrants
            let safetyFactors = getNSafetyFactors 0 (parse contents)
            let tree = minimum safetyFactors
            putStrLn ("Safety factor: " ++ result ++ " ") -- ++ "\n" ++ visualize positions)
            putStrLn ("Lowest: " ++ show tree ++ "seconds: " ++ show (findPosInList tree safetyFactors))
        _ -> putStrLn ("Usage: " ++ progName ++ " <input_file>")