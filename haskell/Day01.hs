module Day01 where

import Data.Char
import qualified Data.Map as Map

findFirstDigit :: [Char] -> Char
findFirstDigit [] = error "no digit"
findFirstDigit (h:r) =
    if isDigit h
    then h
    else findFirstDigit r

findLastDigit :: [Char] -> Char
findLastDigit = findFirstDigit . reverse

calibrationValue :: [Char] -> Int
calibrationValue line = 10 * digitToInt (findFirstDigit line) + digitToInt (findLastDigit line)

sumCalibrationValues :: [[Char]] -> Int
sumCalibrationValues = foldl (+) 0 . map calibrationValue

digitMap = Map.fromList [ (((: []) . intToDigit) x, x) | x <- [0..9]]

main = do
    content <- readFile "input/day-01"
    let l = lines content
    print $ "Sum - part 1: " <> show (sumCalibrationValues (lines content))
