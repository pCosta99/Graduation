import Data.List.Split (splitOn, splitWhen)
import List
import Cp
import Data.Map as M hiding (map, drop)
import Data.Maybe

type Coords = (Float, Float)

data Node = Node { 
    gid :: String, 
    coords :: Coords,
    conservationState :: String, 
    shelterType :: String, 
    withAdvertising :: String,
    operator :: String,
    nodeTracks :: String, 
    streetCode :: String, 
    streetName :: String, 
    parish :: String 
}

instance Show Node where
    show (Node g c cs st wa o nt sc stn p) = "node(" ++ g ++ "," ++ (show $ p1 c) ++ "," ++ (show $ p2 c) ++ "," ++ cs ++ "," ++ st ++ "," ++ wa ++ "," ++ o ++ ",[" ++ nt ++ "]," ++ sc ++ "," ++ stn ++ "," ++ p ++ ")." 

data Connection = Connection { 
    source_gid :: String, 
    destiny_gid :: String, 
    track :: String, 
    distance :: Float
}

instance Show Connection where
    show (Connection sg dg t d) = "connected(" ++ sg ++ "," ++ dg ++ "," ++ t ++ "," ++ show d ++ ")."

-- creates a pair of gid and the actual node from a list of strings
nodeFromStringList l = ((l!!0) , Node (l!!0) (readFloat $ (l!!1), readFloat $ (l!!2)) (l!!3) (l!!4) (l!!5) (l!!6) (l!!7) (l!!8) (l!!9) (l!!10)) where
    readFloat s = if length s > 0 then read s :: Float else 0

connectionFromStringPair :: (String,String) -> Map String Node -> Connection
connectionFromStringPair (l1, l2) m = Connection (array_l1!!0) (array_l2!!0) (array_l1!!7) (addCoords $ getCoords m) where
    array_l1 = splitOn ";" l1
    array_l2 = splitOn ";" l2
    getCoords = (coords >< coords) . (fromJust >< fromJust) . Cp.split (M.lookup (array_l1 !! 0)) (M.lookup (array_l2 !! 0))
    addCoords ((lat1,lon1),(lat2,lon2)) = sqrt((lat1 - lat2)^2 + (lon1 - lon2)^2)

pairUpList :: [a] -> [(a,a)]
pairUpList = anaList g where
    g [] = i1 ()
    g [x] = i1 ()
    g (x:y:xs) = i2 $ ((x,y),(y:xs))

linesIntoNodeMap = fromList . fmap (nodeFromStringList . splitOn ";") . drop 1 . lines

linesIntoConnectionList nodes = concat . map (map ((\(x,y) -> connectionFromStringPair (x,y) nodes)) . pairUpList) . map (drop 1) . splitWhen (\x -> length x == 0) . splitOn "\n"

nodes = do
    -- Parse the nodes first
    let nodeFile = "paragem_autocarros_oeiras_processado_4.csv"
    allNodes <- readFile nodeFile
    -- split them by lines and drop the header, then split them by the csv spearator and make them into a (gid, node) map
    sequence_ (map (putStrLn . show . p2) $ toList $ linesIntoNodeMap allNodes)

connections = do 
    -- Proceed to parse the connections and nodes
    let files = ("list.csv", "paragem_autocarros_oeiras_processado_4.csv")
    allConnections <- readFile $ p1 files
    allNodes <- readFile $ p2 files
    -- split them by line and the split into the several tracks, dropping the headers after. then transform each list into the connections present there
    let connections = linesIntoConnectionList (linesIntoNodeMap allNodes) allConnections
    sequence_ (map (putStrLn . show) connections)