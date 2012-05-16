import PECA
import Control.Parallel.MPI.Simple
import Control.Monad
import Data.List
import System.IO

divide 0 l = []
divide n [] = replicate n []
divide n l = (:) head (divide (n-1) rest)
              where (head,rest) = splitAt (ceiling $ (fromIntegral $ length l) / (fromIntegral n)) l

processInstances = do
  -- putStrLn "Slave: waiting for instances..."
  (msg :: String, _status) <- recv commWorld 0 unitTag
  -- putStrLn $ "Slave: instances received: " ++ msg
  case msg of
    "done" -> return ()
    _ -> do
      -- putStrLn "Slave: creating new instances..."
      let newInstances = show $ concat $ map (continue 0 0.95) $ read msg
      -- putStrLn $ "Slave: sending instances: " ++ newInstances
      send commWorld 0 unitTag newInstances
      -- putStrLn "Slave: sent instances."
      processInstances

--distributeInstances size initial = do
--  -- putStrLn "Master: distributing instances..."
--  mapM (\(r,m) -> send commWorld (toRank r) unitTag $ show m) $ zip [1..] $ divide (size-1) initial
--  -- putStrLn "Master: instances sent."
--  -- putStrLn "Master: waiting for instances..."
--  incoming <- mapM (\r -> recv commWorld (toRank r) unitTag) [1..size-1]
--  -- putStrLn "Master: instances received."
--  print incoming
--  let instLists = read $ map fst incoming :: [Inst]
--  print instLists

collectInstances size = mapM (\r -> recv commWorld (toRank r) unitTag) [1..(size-1)]
--collectInstances size = sequence $ replicate (size-1) $ recv commWorld anySource unitTag

distributeInstances size 0 init = do
  mapM (\r -> send commWorld (toRank r) unitTag "done") [1..(size-1)]
  return [init]
distributeInstances size n init = do
  putStrLn $ "On step " ++ (show n) ++ " with " ++ (show $ length init) ++ " instances."
  mapM (\(r,m) -> send commWorld r unitTag $ show m) $ zip [1..] $ divide (size-1) init
  -- putStrLn "Master: waiting..."
  incoming <- collectInstances (size)
  let msgs = map fst incoming :: [String]
  -- print msgs
  let insts = map read msgs :: [[Inst]]
  let gen_lists = concat insts
  putStrLn $ "Received " ++ (show $ length gen_lists) ++ " messages."
  let gen = combine gen_lists
  rest <- distributeInstances size (n-1) gen
  return (init:rest)

main = mpiWorld $ \size rank ->
       if size < 2 then putStrLn "At least 2 processes necessary."
       else case rank of
              0 -> do
                results <- distributeInstances size 11 [Inst 1 0 [1]]
                print $ map length results
                let output = concat $ intersperse "\n" $ map prettyInst $ concat results
                writeFile "PECAMPI_OUT" output
              _ -> processInstances
