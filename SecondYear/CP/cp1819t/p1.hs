import Cp
import Data.Char
import List

-- (1) Datatype definition -----------------------------------------------------

data Expr = Num Int | Bop Expr Op Expr deriving (Eq, Show)
data Op = Op String deriving (Eq, Show)
type Codigo = [String]

teste :: Expr
teste = Bop (Bop (Num 4) (Op "-") (Num 2)) (Op "+") (Num 19)

inExpr = either Num ((uncurry $ uncurry Bop) . (swap >< id) . assocl)

outExpr :: Expr -> Either Int (Op, (Expr, Expr))
outExpr (Num n) = i1 n
outExpr (Bop e1 o e2) = i2 (o, (e1, e2))

baseExpr f g = id -|- (f >< (g >< g))

-- (2) Ana + cata + hylo -------------------------------------------------------

recExpr f = baseExpr id f

cataExpr g = g . (recExpr (cataExpr g)) . outExpr

anaExpr g = inExpr . (recExpr (anaExpr g)) . g

hyloExpr f g = cataExpr f . anaExpr g

-- (4) Examples ----------------------------------------------------------------

calcula :: Expr -> Int
calcula = cataExpr (either id math)

math :: (Op, (Int, Int)) -> Int
math (Op "+", (a, b)) = a + b
math (Op "-", (a, b)) = a - b
math (Op "*", (a, b)) = a * b

compile :: String -> Codigo
compile = hyloExpr conquer divide

myReadExp :: String -> Expr
myReadExp = anaExpr divide

conquer = either (singl . transform . show) (conc . swap . (outOp >< conc))
    where outOp (Op s) = singl $ transform s

transform :: String -> String
transform "+" = "ADD"
transform "*" = "MUL"
transform a = "PUSH " ++ a

divide :: String -> Either Int (Op, (String, String))
divide s = if (length x /= length s && length y /= length s) then i2 (o, (t1, t2)) else i1 (read s :: Int) where
    f = filter (\x -> if (x /= '(' && x /= ')') then True else False)
    (x,y) =  (f >< f) $ mySpan s
    t1 = x
    o = Op (singl $ head y)
    t2 = tail y

mySpan :: String -> (String, String)
mySpan s = splitAt (improvedSpan s 0) s

improvedSpan :: String -> Int -> Int
improvedSpan [] i = (i-1)
improvedSpan [x] i = (i-1)
improvedSpan (c:c1:s) i | c == '+' && c1 == '(' = i
                        | c == '*' && c1 == '(' = i
                        | c == ')' && c1 == '+' = i
                        | c == ')' && c1 == '*' = i
                        | otherwise = improvedSpan (c1:s) (i+1)