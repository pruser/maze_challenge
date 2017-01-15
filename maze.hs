module Main where
import Data.List
import Data.Maybe 

free = 0
wall = 1
start = 2
finish = 3

findPosition' n m y = case m of
  x:xs -> 
    case elemIndex n x of 
      Just x -> Just (y, x)
      Nothing -> findPosition' n xs (y+1) 
  _ -> Nothing               

findPosition n m = do
  findPosition' n m 0

get :: Int -> [a] -> Maybe a
get 0 (x:xs) = Just x
get n [] = Nothing
get n (x:xs) = get (n-1) xs

get2 (y,x) m = get y m >>= get x 

move (a,b) (y,x) = (y+a,x+b) 

up = move (-1,0) 
down = move (1,0) 
left = move (0,-1) 
right = move (0,1) 

orM m1 m2 = case m1 of
  Nothing -> m2
  _ -> m1 

solve' m path point = do
  case point `elem` path of
    True -> Nothing
    False -> do
      let path' = [point] ++ path
      case get2 point m of
        Nothing -> Nothing
        Just x | x == finish -> Just path'
        Just x | x == wall -> Nothing
        Just _ -> do 
          let slv = solve' m path'
          (slv $ left point) `orM` 
            (slv $ down point) `orM`
            (slv $ right point)`orM` 
            (slv $ up point) `orM` Nothing

solve m = do
  findPosition start m >>= solve' m []

main = do
  let m = [[1,2,0,0],[1,0,1,0],[1,0,3,1]]
  print $ solve m
