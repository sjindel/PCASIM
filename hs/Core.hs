import Data.List
import qualified Data.Sequence as S
import qualified Data.Foldable as F

type Cell = Int
type Prob = Double
type Rule = Int

data Inst = Inst {
      instProb :: Prob
      , instStep :: Integer
      , instConfig :: [Cell]
    } deriving (Eq)

instance Show Inst where
    show (Inst p s c) = "(" ++ (show p) ++ " : " ++ (show s) ++ ") " ++ (show c)

type Schedule = [Bool]

decToBin 0 = []
decToBin y = let (a,b) = quotRem y 2 in [b] ++ decToBin a

binToDec' n [] = 0
binToDec' n (x:xs) = x*(2^n) + (binToDec' (n+1) xs)

binToDec = binToDec' 0

dive :: Integral n => n -> (a -> [a]) -> ([a] -> [a]) -> [a] -> [a]
dive 0 _ _ i = i
dive n g f i = dive (n-1) g f $ f $ concat $ map g i

diveSeq :: Integral n => n -> (a -> S.Seq a) -> (S.Seq a -> S.Seq a) -> S.Seq a -> S.Seq a
diveSeq 0 _ _ i = i
diveSeq n g f i = diveSeq (n-1) g f $ f $ F.foldr (S.><) S.empty $ fmap g i

trace :: Integral n => n -> (a -> [a]) -> ([a] -> [a]) -> [a] -> [[a]]
trace 0 _ _ i = [i]
trace n g f i = i:(trace (n-1) g f $ f $ concat $ map g i)

toList (a,b,c) = [a,b,c]

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

generateSchedulesSeq :: Int -> S.Seq [Bool]
generateSchedulesSeq 0 = S.singleton []
generateSchedulesSeq n =  (fmap (False:) rest) S.>< (fmap (True:) rest)
    where rest = generateSchedulesSeq (n-1) :: S.Seq [Bool]

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

meldSeq :: Inst -> S.Seq Inst -> Inst
meldSeq i is = Inst probSum (instStep i) (instConfig i)
    where probSum = (instProb i) + (F.sum $ fmap instProb is)

combine :: [Inst] -> [Inst]
combine [] = []
combine (i:is) = (meld i same):(combine rest)
    where (same,rest) = partition (\x -> (instConfig x) == (instConfig i)) is

combineSeq :: S.Seq Inst -> S.Seq Inst
combineSeq is = case S.viewl is of
                  S.EmptyL -> S.empty
                  i S.:< is -> (meldSeq i same) S.<| (combineSeq rest)
                      where (same,rest) = S.partition (\x -> (instConfig x) == (instConfig i)) is

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

continueSeq :: Cell -> Prob -> Inst -> S.Seq Inst
continueSeq bg p inst = combineSeq $ fmap ((trimInst bg).(update bg p inst)) (generateSchedulesSeq len2)
    where len2 = (length $ instConfig inst) + 2
          trimInst c i = Inst (instProb i) (instStep i) (trim c $ instConfig i)

toPair :: Inst -> String
toPair (Inst p s c) = (show s) ++ "," ++ (show p)

--main = putStrLn $ concat $ intersperse "\n" $ map toPair $
--       concat $ trace 9 (continue 0 0.95) combine [(Inst 1 0 [1])]

--main = putStrLn $ concat $ intersperse "\n" $ map show $
--       concat $ trace 9 (continue 0 0.95) combine [(Inst 1 0 [1])]

main = putStrLn $ concat $ intersperse "\n" $ map show $
       concat $ F.toList $ trace 9 (continue 0 0.95) combine [(Inst 1 0 [1])]
