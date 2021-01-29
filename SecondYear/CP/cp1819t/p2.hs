import Cp
import List
import qualified Graphics.Gloss as G

-- (1) Datatype definition -----------------------------------------------------

type L2D = X Caixa Tipo 
data X a b = Unid a | Comp b (X a b) (X a b) deriving Show
type Caixa = ((Int,Int),(Texto, G.Color))
data Tipo = V | Vd | Ve | H | Ht | Hb deriving Show
type Texto = String
type Fig = [(Origem, Caixa)]
type Origem = (Float, Float)

teste :: L2D
teste = Comp Ht (Comp Vd (Comp Hb (Unid ((100,100),("A", col_blue))) (Unid ((100,100),("B", col_blue)))) (Comp Hb (Unid ((100,100),("C", col_blue))) (Unid ((100,100),("D", col_blue)))))
                (Comp V (Comp Ht (Unid ((100,100),("G", col_blue))) (Unid ((100,100),("H", col_blue)))) (Comp Hb (Unid ((100,100),("F", col_blue))) (Unid ((100,100),("E", col_blue)))))

inL2D :: Either a (b, (X a b,X a b)) -> X a b
inL2D = either Unid (tripleUncurry Comp)

tripleUncurry :: (a -> b -> c -> d) -> (a,(b,c)) -> d
tripleUncurry f (a,(b,c)) = f a b c

outL2D :: X a b -> Either a (b, (X a b,X a b))
outL2D (Unid a) = i1 a
outL2D (Comp b xl xr) = i2 (b, (xl, xr))

baseL2D f g = id -|- (f >< (g >< g))

recL2D f = baseL2D id f

cataL2D g = g . recL2D(cataL2D g) . outL2D

anaL2D g = inL2D . recL2D(anaL2D g) . g

hyloL2D f g = cataL2D f . anaL2D g

collectLeafs = cataL2D (either singl (conc . p2))

dimen :: X Caixa Tipo -> (Float, Float)
dimen = cataL2D (either ((fromIntegral >< fromIntegral) . p1) soma_dims)

soma_dims :: (Tipo,((Float, Float),(Float, Float))) -> (Float, Float)
soma_dims (t,((a,b),(c,d))) = (0,0)

calcOrigins :: (L2D,Origem) -> X (Caixa,Origem) ()
calcOrigins = hyloL2D conquer divide
{-
calcOrigins (Unid a, o) = Unid (a,o)
calcOrigins ((Comp t x1 x2), o) = (Comp () (calcOrigins (x1,o)) (calcOrigins (x2,soma_dims (t,(o,o)))))
-}
conquer :: Either (Caixa, Origem) (Tipo, (X (Caixa,Origem) (), X (Caixa,Origem) ())) -> X (Caixa,Origem) ()
conquer = inL2D . (id -|- aux)

aux :: (Tipo, (X (Caixa, Origem) (), X (Caixa, Origem) ())) -> ((), (X (Caixa, Origem) (), X (Caixa, Origem) ()))
aux (t, (Comp )) = 
aux (t, (Unid (c1,o1), Unid (c2,o2))) = ((), (Unid (c1, apply t c2 o2), Unid (c2, o2))) 
            where apply t ((a,b),(c,d)) (o1,o2) = soma_dims (t, ((fromIntegral a, fromIntegral b), (o1,o2)))

divide :: (L2D, Origem) -> Either (Caixa, Origem) (Tipo, ((L2D, Origem), (L2D, Origem)))
divide (Unid c, o) = i1 (c,o)
divide (Comp t x1 x2, o) = i2 (t, ((x1,o),(x2,o)))
{-
test :: (L2D, Origem) -> X (Caixa, Origem) ()
test ((Comp t (Unid x1) (Unid x2)), o) = Comp () (Unid a) (Unid b)
test ((Comp t (Unid x1) x2), o) = Comp t (Unid (x1,o)) (test (x2,o)) 
                where (a,b) = aux (t, (x1,x2), o)

aux :: (Tipo, (Caixa, Caixa), Origem) -> ((Caixa, Origem), (Caixa, Origem))
aux (t, (((a,b),(c,d)), ((a1,b1),(c1,d1))), o) = ((((a,b),(c,d)), soma_dims (t, ((fromIntegral a, fromIntegral b),(fromIntegral a1 + (p1 o), fromIntegral b1 + (p2 o))))), (((a1,b1),(c1,d1)), o))
-}
agrup_caixas :: X (Caixa, Origem) () -> Fig
agrup_caixas = cataL2D (either (singl . swap) (conc . p2))

mostra_caixas :: (L2D, Origem) -> IO ()
mostra_caixas = display . G.pictures . map parser . agrup_caixas . calcOrigins 
        where parser (o,((a,b),(c,d))) = crCaixa o (fromIntegral a) (fromIntegral b) c d

col_blue = G.azure
col_green = darkgreen

darkgreen = G.dark (G.dark G.green)

crCaixa :: Origem  -> Float -> Float -> String -> G.Color -> G.Picture
crCaixa (x,y) w h l c = G.Translate (x+(w/2)) (y+(h/2)) $  G.pictures [caixa, etiqueta] where
                    caixa = G.color c (G.rectangleSolid w h)
                    etiqueta = G.translate calc_trans_x calc_trans_y $ 
                             G.Scale calc_scale calc_scale $ G.color G.black $ G.Text l
                    calc_trans_x = (- ((fromIntegral (length l)) * calc_scale) / 2 )*base_shift_x
                    calc_trans_y = (- calc_scale / 2)*base_shift_y
                    calc_scale = bscale * (min h w) 
                    bscale = 1/700
                    base_shift_y = 100
                    base_shift_x = 64

display = G.display (G.InWindow "Problema 4" (400, 400) (40, 40)) G.white

ex1Caixas = G.display (G.InWindow "Problema 4" (400, 400) (40, 40)) G.white $ crCaixa (0, 0) 200 200 "Caixa azul" col_blue