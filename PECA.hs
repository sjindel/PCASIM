import Data.Maybe
import Data.List
import System.Environment

type Cell = Int
type Prob = Double
type Rule = Int

data Inst = Inst {
      instProb :: Prob
      , instStep :: Integer
      , instConfig :: [Cell]
    } deriving (Eq, Show, Read)

prettyInst (Inst p s c) = "(" ++ show p ++ " : " ++ show s ++ ") " ++ show c

type Schedule = [Int]

decToBin 0 = []
decToBin y = let (a,b) = quotRem y 2 in b : decToBin a

binToDec' n [] = 0
binToDec' n (x:xs) = x*(2^n) + binToDec' (n+1) xs

binToDec = binToDec' 0

inject :: [a -> b] -> a -> [b]
inject [] _ = []
inject (f:fs) x = f x : inject fs x

explore :: (a -> b -> c) -> [a] -> [b] -> [c]
explore f a = concatMap (inject (map f a))

prod [] y = []
prod (x:xs) y = (map (\e -> (x,e)) y) ++ (prod xs y)

exploreReceipt :: (a -> b -> c) -> [a] -> [b] -> [(a,b,c)]
exploreReceipt f a = concatMap (inject (map (\a -> \b -> (a,b,f a b)) a))

trace :: Integral n => n -> (a -> [a]) -> ([a] -> [a]) -> [a] -> [[a]]
trace 0 _ _ i = [i]
trace n g f i = i : trace (n-1) g f (f $ concatMap g i)

printTransition (a,b,c) = a ++ " -> " ++ b ++ " [label = \"" ++ c ++ "\"];\n"

toList (a,b,c) = [a,b,c]

{- Rule 110 -}
evaluateRule :: (Cell,Cell,Cell) -> Cell
evaluateRule neighbors = case neighbors of
                           (0, 0, 0) -> 0
                           (0, 0, 1) -> 1
                           (0, 1, 0) -> 1
                           (0, 1, 1) -> 1
                           (1, 0, 0) -> 0
                           (1, 0, 1) -> 1
                           (1, 1, 0) -> 1
                           (1, 1, 1) -> 0

evalRule :: Int -> (Cell,Cell,Cell) -> Cell
evalRule rule (l,c,r) = fromJust $ lookup [l,c,r] (zip (binary 3) (decToBin rule ++ repeat 0))

binary 0 = [[]]
binary n =  map (0:) rest ++ map (1:) rest
    where rest = binary (n-1)

updateMask :: Int -> [Cell] -> Schedule -> [Cell]
updateMask rule state sch = map up $ zip3 [0..] state sch
    where l = length state
          up (n,c,u) = let prev = state!!( (n+l-1) `mod` l)
                           next = state!!( (n+1) `mod` l)
                       in
                         case u of
                           1 -> evalRule rule (prev,c,next)
                           0 -> c

triplize :: [Cell] -> [(Cell,Cell,Cell)]
triplize [] = []
triplize [x] = []
triplize [x,y] = []
triplize (x:y:z:s) = (x,y,z) : triplize (y:z:s)

update :: Cell -> Prob -> Inst -> Schedule -> Inst
update bg p inst sch = Inst (probability p sch * instProb inst)
                     (instStep inst + 1) newConfig
    where
      config = instConfig inst
      eval u (p,c,n) = case u of 1 -> evaluateRule (p,c,n)
                                 0 -> c
      newConfig = zipWith eval sch (triplize (bg:bg:config ++ [bg,bg]))

probability :: Prob -> Schedule -> Prob
probability p [] = 1
probability p (x:xs) = case x of
                         1 -> p * probability p xs
                         0 -> (1-p) * probability p xs

unite :: Inst -> Inst -> Inst
unite x y = Inst probSum (instStep x) (instConfig x)
    where probSum = instProb x + instProb y

insertUnique :: Inst -> [Inst] -> [Inst]
insertUnique x (i:is) = if instConfig x == instConfig i
                  then unite x i : is
                  else i : insertUnique x is
insertUnique x [] = [x]

combineInst' :: [Inst] -> [Inst] -> [Inst]
combineInst' cache (i:is) = foldl (flip insertUnique) cache is

combineInst = combineInst' []

expected insts = sum $ map (\i -> instProb i *
                                  fromIntegral (sum $ instConfig i)) insts

trimFront c [] = []
trimFront c (x:xs) = if x == c then trimFront c xs else x:xs
trimBack c l = reverse $ trimFront c $ reverse l
trim c l = trimFront c $ trimBack c l

progress :: Cell -> Prob -> [Cell] -> Inst -> [Inst]
progress bg up (prev:curr:next:cs) (Inst ip step cells) =
    let new = evaluateRule (prev,curr,next)
    in if new == curr then progress bg up (curr:next:cs) (Inst ip step (new:cells))
       else progress bg up (curr:next:cs) (Inst (ip*up) step (new:cells)) ++
            progress bg up (curr:next:cs) (Inst (ip*(1-up)) step (curr:cells))
progress _ _ _ (Inst p s c) = [Inst p s $ reverse c]

continue :: Cell -> Prob -> Inst -> [Inst]
continue bg up (Inst ip step conf) = combineInst $ map (trimInst bg) $
                                     progress bg up (bg:bg:conf ++ [bg,bg]) (Inst ip (step+1) [])
    where trimInst c (Inst ip step conf) = Inst ip step (trim c conf)


toPair :: Inst -> String
toPair (Inst p s c) = show s ++ "," ++ show p

superGroup f [] = []
superGroup f (x:xs) = let (same,others) = partition (f x) xs
                      in (x:same):(superGroup f others)

showGraph :: Int -> Int -> String
showGraph rule size = let transitions = concatMap printTransition $ map (foldl1 (\(a,b,c) -> (\(x,y,z) -> (a,b,c ++ " " ++ z)))) $ superGroup (\(x,y,z) -> (\(a,b,c) -> (a == x) && (b == y))) $ map (\(x,y,z) -> (concatMap show x, concatMap show z, concatMap show y)) $ exploreReceipt (updateMask rule) (binary size) (binary size)
                      in "digraph rule" ++ show rule ++ "_" ++ show size ++ " {\n" ++
                         transitions ++ "}\n"

main = do
  args <- getArgs
  let (rule:size:_) = args
  putStr $ showGraph (read rule) (read size)
