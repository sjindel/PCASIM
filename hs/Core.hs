import Math.Polynomial
import Data.List

type Cell = Int
type Prob = Double

data Inst = Inst {
      instProb :: Prob
      , instStep :: Integer
      , instConfig :: [Cell]
    } deriving (Show, Eq)

type Schedule = [Bool]

decToBin x = reverse $ decToBin' x
    where
      decToBin' 0 = []
      decToBin' y = let (a,b) = quotRem y 2 in [b] ++ decToBin' a

dive :: Integral n => n -> (a -> [a]) -> [a] -> [a]
dive 0 _ i = i
dive n f i = dive (n-1) f $ concat $ map f i

trace :: Integral n => n -> (a -> [a]) -> [a] -> [[a]]
trace 0 _ i = [i]
trace n f i = i:(trace (n-1) f $ concat $ map f i)

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

triplize :: [Cell] -> [(Cell,Cell,Cell)]
triplize [] = []
triplize [x] = []
triplize [x,y] = []
triplize (x:y:z:s) = (x,y,z):(triplize (y:z:s))

update :: Cell -> Prob -> Inst -> Schedule -> Inst
update bg p inst sch = Inst ((probability p sch) * (instProb inst))
                     ((instStep inst) + 1) newConfig
    where
      config = instConfig inst
      eval u (p,c,n) = if u then evaluateRule (p,c,n) else c
      newConfig = zipWith eval sch (triplize (bg:bg:config ++ [bg,bg]))


generateSchedules 0 = [[]]
generateSchedules n =  (map (False:) rest) ++ (map (True:) rest)
    where rest = generateSchedules (n-1)

probability :: Prob -> Schedule -> Prob
probability p [] = 1
probability p (x:xs) = case x of
                         True -> p * (probability p xs)
                         False -> (1-p) * (probability p xs)

meld :: Inst -> [Inst] -> Inst
meld i is = Inst probSum (instStep i) (instConfig i)
    where probSum = (instProb i) + (sum $ map instProb is)

combine :: [Inst] -> [Inst]
combine [] = []
combine (i:is) = (meld i same):(combine rest)
    where (same,rest) = partition (\x -> (instConfig x) == (instConfig i)) is

expected insts = sum $ map (\i -> (instProb i)*
                                  (fromIntegral $ sum $ instConfig i)) insts

trimFront c [] = []
trimFront c (x:xs) = if (x == c) then (trimFront c xs) else (x:xs)
trimBack c l = reverse $ trimFront c $ reverse l
trim c l = trimFront c $ trimBack c l

continue :: Cell -> Prob -> Inst -> [Inst]
continue bg p inst = combine $ map ((trimInst bg).(update bg p inst)) (generateSchedules len2)
    where len2 = (length $ instConfig inst) + 2
          trimInst c i = Inst (instProb i) (instStep i) (trim c $ instConfig i)

main = return ()
