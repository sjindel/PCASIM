import Data.List
import System.Environment
import Math.Polynomial

type Cell = Int
type Prob = Poly Double
type Rule = Int

data Inst = Inst {
      instProb :: Prob
      , instStep :: Integer
      , instConfig :: [Cell]
    } deriving (Eq, Show)

prettyInst (Inst p s c) = "(" ++ (show p) ++ " : " ++ (show s) ++ ") " ++ (show c)

type Schedule = [Bool]

decToBin 0 = []
decToBin y = let (a,b) = quotRem y 2 in [b] ++ decToBin a

binToDec' n [] = 0
binToDec' n (x:xs) = x*(2^n) + (binToDec' (n+1) xs)

binToDec = binToDec' 0


trace :: Integral n => n -> (a -> [a]) -> ([a] -> [a]) -> [a] -> [[a]]
trace 0 _ _ i = [i]
trace n g f i = i:(trace (n-1) g f $ f $ concat $ map g i)

toList (a,b,c) = [a,b,c]

{- Rule 14 -}
evaluateRule :: (Cell,Cell,Cell) -> Cell
evaluateRule neighbors = case neighbors of
                           (0, 0, 0) -> 0
                           (0, 0, 1) -> 1
                           (0, 1, 0) -> 1
                           (0, 1, 1) -> 1
                           (1, 0, 0) -> 0
                           (1, 0, 1) -> 0
                           (1, 1, 0) -> 0
                           (1, 1, 1) -> 0


{- Rule 98
evaluateRule :: (Cell,Cell,Cell) -> Cell
evaluateRule neighbors = case neighbors of
                           (0, 0, 0) -> 0
                           (0, 0, 1) -> 1
                           (0, 1, 0) -> 0
                           (0, 1, 1) -> 0
                           (1, 0, 0) -> 0
                           (1, 0, 1) -> 1
                           (1, 1, 0) -> 1
                           (1, 1, 1) -> 0
 -}
triplize :: [Cell] -> [(Cell,Cell,Cell)]
triplize [] = []
triplize [x] = []
triplize [x,y] = []
triplize (x:y:z:s) = (x,y,z):(triplize (y:z:s))

update :: Cell -> Prob -> Inst -> Schedule -> Inst
update bg p inst sch = Inst ((probability p sch) `multPoly` (instProb inst))
                     ((instStep inst) + 1) newConfig
    where
      config = instConfig inst
      eval u (p,c,n) = if u then evaluateRule (p,c,n) else c
      newConfig = zipWith eval sch (triplize (bg:bg:config ++ [bg,bg]))

flipPoly :: (Num a, Eq a) => Poly a -> Poly a
flipPoly p = one `addPoly` (negatePoly p)

probability :: Prob -> Schedule -> Prob
probability p [] = one
probability p (x:xs) = case x of
                         True -> p `multPoly` (probability p xs)
                         False -> (flipPoly p) `multPoly` (probability p xs)

unite :: Inst -> Inst -> Inst
unite x y = Inst probSum (instStep x) (instConfig x)
    where probSum = (instProb x) `addPoly` (instProb y)

insertUnique :: Inst -> [Inst] -> [Inst]
insertUnique x (i:is) = if ((instConfig x) == (instConfig i))
                  then (unite x i):is
                  else i:(insertUnique x is)
insertUnique x [] = [x]

combine' :: [Inst] -> [Inst] -> [Inst]
combine' cache [] = cache
combine' cache (i:is) = combine' (insertUnique i cache) is

combine = combine' []

expected insts = sumPolys $ map (\i -> scalePoly (fromIntegral $ sum $
                                                 instConfig i) (instProb i)) insts

trimFront c [] = []
trimFront c (x:xs) = if (x == c) then (trimFront c xs) else (x:xs)
trimBack c l = reverse $ trimFront c $ reverse l
trim c l = trimFront c $ trimBack c l

progress :: Cell -> Prob -> [Cell] -> Inst -> [Inst]
progress bg up (prev:curr:next:cs) (Inst ip step cells) =
    let new = evaluateRule (prev,curr,next)
    in if (new == curr) then progress bg up (curr:next:cs) (Inst ip step (new:cells))
       else (progress bg up (curr:next:cs) (Inst (ip `multPoly` up) step (new:cells))) ++
            (progress bg up (curr:next:cs) (Inst (ip `multPoly` (flipPoly up)) step (curr:cells)))
progress _ _ _ (Inst p s c) = [Inst p s $ reverse c]

continue :: Cell -> Prob -> Inst -> [Inst]
continue bg up (Inst ip step conf) = combine $ map (trimInst bg) $
                                     progress bg up (bg:bg:conf ++ [bg,bg]) (Inst ip (step+1) [])
    where trimInst c (Inst ip step conf) = Inst ip step (trim c conf)


toPair :: Inst -> String
toPair (Inst p s c) = (show s) ++ "," ++ (show p)

--main = print $ map length $ trace 9 (continue 0 0.95) combine [(Inst 1 0 [1])]

main = do
  args <- getArgs
  let tr = trace (read $ head args) (continue 0 x) combine [(Inst one 0 [1,1])]
  let expr = map expected tr
  mapM_ (\p -> print $ map (\x -> evalPoly x p) expr) [0,0.01..1]

--main = putStrLn $ concat $ intersperse "\n" $ map show $ F.toList
--       $ F.foldr (S.><) S.empty $ traceSeq 9 (continueSeq 0 0.95) combineSeq
--       $ S.singleton (Inst 1 0 [1])
