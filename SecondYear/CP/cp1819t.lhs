\documentclass[a4paper]{article}
\usepackage[a4paper,left=3cm,right=2cm,top=2.5cm,bottom=2.5cm]{geometry}
\usepackage{palatino}
\usepackage[colorlinks=true,linkcolor=blue,citecolor=blue]{hyperref}
\usepackage{graphicx}
\usepackage{cp1819t}
\usepackage{subcaption}
\usepackage{adjustbox}
\usepackage{color}
%================= local x=====================================================%
\def\getGif#1{\includegraphics[width=0.3\textwidth]{cp1819t_media/#1.png}}
\let\uk=\emph
\def\aspas#1{``#1"}
%================= lhs2tex=====================================================%
%include polycode.fmt 
%format (div (x)(y)) = x "\div " y
%format succ = "\succ "
%format ==> = "\Longrightarrow "
%format map = "\map "
%format length = "\length "
%format fst = "\p1"
%format p1  = "\p1"
%format snd = "\p2"
%format p2  = "\p2"
%format Left = "i_1"
%format Right = "i_2"
%format i1 = "i_1"
%format i2 = "i_2"
%format >< = "\times"
%format >|<  = "\bowtie "
%format |-> = "\mapsto"
%format . = "\comp "
%format (kcomp (f)(g)) = f "\kcomp " g
%format -|- = "+"
%format conc = "\mathsf{conc}"
%format summation = "{\sum}"
%format (either (a) (b)) = "\alt{" a "}{" b "}"
%format (frac (a) (b)) = "\frac{" a "}{" b "}"
%format (uncurry f) = "\uncurry{" f "}"
%format (const f) = "\underline{" f "}"
%format TLTree = "\mathsf{TLTree}"
%format (lcbr (x)(y)) = "\begin{lcbr}" x "\\" y "\end{lcbr}"
%format (split (x) (y)) = "\conj{" x "}{" y "}"
%format (for (f) (i)) = "\for{" f "}\ {" i "}"
%format B_tree = "\mathsf{B}\mbox{-}\mathsf{tree} "
\def\ana#1{\mathopen{[\!(}#1\mathclose{)\!]}}
%format (cataA (f) (g)) = "\cata{" f "~" g "}_A"
%format (anaA (f) (g)) = "\ana{" f "~" g "}_A"
%format (cataB (f) (g)) = "\cata{" f "~" g "}_B"
%format (anaB (f) (g)) = "\ana{" f "~" g "}_B"
%format Either a b = a "+" b 
%format fmap = "\mathsf{fmap}"
%format NA   = "\textsc{na}"
%format NB   = "\textsc{nb}"
%format inT = "\mathsf{in}"
%format outT = "\mathsf{out}"
%format Null = "1"
%format (Prod (a) (b)) = a >< b
%format fF = "\fun F "
%format e1 = "e_1 "
%format e2 = "e_2 "
%format Dist = "\fun{Dist}"
%format IO = "\fun{IO}"
%format BTree = "\fun{BTree} "
%format LTree = "\mathsf{LTree}"
%format inNat = "\mathsf{in}"
%format (cataNat (g)) = "\cata{" g "}"
%format Nat0 = "\N_0"
%format muB = "\mu "
%format (frac (n)(m)) = "\frac{" n "}{" m "}"
%format (fac (n)) = "{" n "!}"
%format (underbrace (t) (p)) = "\underbrace{" t "}_{" p "}"
%format matrix = "matrix"
%format (bin (n) (k)) = "\Big(\vcenter{\xymatrix@R=1pt{" n "\\" k "}}\Big)"
%format `ominus` = "\mathbin{\ominus}"
%format % = "\mathbin{/}"
%format <-> = "{\,\leftrightarrow\,}"
%format <|> = "{\,\updownarrow\,}"
%format `minusNat`= "\mathbin{-}"
%format ==> = "\Rightarrow"
%format .==>. = "\Rightarrow"
%format .<==>. = "\Leftrightarrow"
%format .==. = "\equiv"
%format .<=. = "\leq"
%format .&&&. = "\wedge"
%format cdots = "\cdots "
%format pi = "\pi "

%---------------------------------------------------------------------------

\title{
       	    Cálculo de Programas
\\
       	Trabalho Prático
\\
       	MiEI+LCC --- 2018/19
}

\author{
       	\dium
\\
       	Universidade do Minho
}


\date\mydate

\makeindex
\newcommand{\rn}[1]{\textcolor{red}{#1}}
\begin{document}

\maketitle

\begin{center}\large
\begin{tabular}{ll}
\textbf{Grupo} nr. & 31
\\\hline
a85700 & Pedro Miguel Araújo Costa	
\\
a55872 & Joao Pedro Rodrigues Sepulveda Marques	
\\
a84010 & Ulisses Tiago Simões Araújo
\end{tabular}
\end{center}

\section{Preâmbulo}

A disciplina de \CP\ tem como objectivo principal ensinar
a progra\-mação de computadores como uma disciplina científica. Para isso
parte-se de um repertório de \emph{combinadores} que formam uma álgebra da
programação (conjunto de leis universais e seus corolários) e usam-se esses
combinadores para construir programas \emph{composicionalmente}, isto é,
agregando programas já existentes.
  
Na sequência pedagógica dos planos de estudo dos dois cursos que têm
esta disciplina, restringe-se a aplicação deste método à programação
funcional em \Haskell. Assim, o presente trabalho prático coloca os
alunos perante problemas concretos que deverão ser implementados em
\Haskell.  Há ainda um outro objectivo: o de ensinar a documentar
programas, validá-los, e a produzir textos técnico-científicos de
qualidade.

\section{Documentação} Para cumprir de forma integrada os objectivos
enunciados acima vamos recorrer a uma técnica de programa\-ção dita
``\litp{literária}'' \cite{Kn92}, cujo princípio base é o seguinte:
\begin{quote}\em Um programa e a sua documentação devem coincidir.
\end{quote} Por outras palavras, o código fonte e a documentação de um
programa deverão estar no mesmo ficheiro.

O ficheiro \texttt{cp1819t.pdf} que está a ler é já um exemplo de \litp{programação
literária}: foi gerado a partir do texto fonte \texttt{cp1819t.lhs}\footnote{O
suffixo `lhs' quer dizer \emph{\lhaskell{literate Haskell}}.} que encontrará
no \MaterialPedagogico\ desta disciplina descompactando o ficheiro \texttt{cp1819t.zip}
e executando
\begin{Verbatim}[fontsize=\small]
    $ lhs2TeX cp1819t.lhs > cp1819t.tex
    $ pdflatex cp1819t
\end{Verbatim}
em que \href{https://hackage.haskell.org/package/lhs2tex}{\texttt\LhsToTeX} é
um pre-processador que faz ``pretty printing''
de código Haskell em \Latex\ e que deve desde já instalar executando
\begin{Verbatim}[fontsize=\small]
    $ cabal install lhs2tex
\end{Verbatim}
Por outro lado, o mesmo ficheiro \texttt{cp1819t.lhs} é executável e contém
o ``kit'' básico, escrito em \Haskell, para realizar o trabalho. Basta executar
\begin{Verbatim}[fontsize=\small]
    $ ghci cp1819t.lhs
\end{Verbatim}

%if False
\begin{code}
{-# OPTIONS_GHC -XNPlusKPatterns #-}
{-# LANGUAGE GeneralizedNewtypeDeriving, DeriveDataTypeable, FlexibleInstances #-}
module Cp1819t where 
import Cp
import List  hiding (fac)
import Nat  
import Data.List hiding (find)
import Data.Typeable
import Data.Ratio
import Data.Bifunctor
import Data.Maybe
import Data.Matrix hiding ((!))
import Test.QuickCheck hiding ((><),choose)
import qualified Test.QuickCheck as QuickCheck
import System.Random  hiding (split)
import System.Process
import GHC.IO.Exception
import qualified Graphics.Gloss as G
import Control.Monad
import Control.Applicative hiding ((<|>))
import Data.Either
import Exp
\end{code}
%endif

\noindent Abra o ficheiro \texttt{cp1819t.lhs} no seu editor de texto preferido
e verifique que assim é: todo o texto que se encontra dentro do ambiente
\begin{quote}\small\tt
\verb!\begin{code}!
\\ ... \\
\verb!\end{code}!
\end{quote}
vai ser seleccionado pelo \GHCi\ para ser executado.

\section{Como realizar o trabalho}
Este trabalho teórico-prático deve ser realizado por grupos de três alunos.
Os detalhes da avaliação (datas para submissão do relatório e sua defesa
oral) são os que forem publicados na \cp{página da disciplina} na \emph{internet}.

Recomenda-se uma abordagem participativa dos membros do grupo
de trabalho por forma a poderem responder às questões que serão colocadas
na \emph{defesa oral} do relatório.

Em que consiste, então, o \emph{relatório} a que se refere o parágrafo anterior?
É a edição do texto que está a ser lido, preenchendo o anexo \ref{sec:resolucao}
com as respostas. O relatório deverá conter ainda a identificação dos membros
do grupo de trabalho, no local respectivo da folha de rosto.

Para gerar o PDF integral do relatório deve-se ainda correr os comando seguintes,
que actualizam a bibliografia (com \Bibtex) e o índice remissivo (com \Makeindex),
\begin{Verbatim}[fontsize=\small]
    $ bibtex cp1819t.aux
    $ makeindex cp1819t.idx
\end{Verbatim}
e recompilar o texto como acima se indicou. Dever-se-á ainda instalar o utilitário
\QuickCheck,
que ajuda a validar programas em \Haskell\ e a biblioteca \gloss{Gloss} para
geração de gráficos 2D:
\begin{Verbatim}[fontsize=\small]
    $ cabal install QuickCheck gloss
\end{Verbatim}
Para testar uma propriedade \QuickCheck~|prop|, basta invocá-la com o comando:
\begin{verbatim}
    > quickCheck prop
    +++ OK, passed 100 tests.
\end{verbatim}

Qualquer programador tem, na vida real, de ler e analisar (muito!) código
escrito por outros. No anexo \ref{sec:codigo} disponibiliza-se algum
código \Haskell\ relativo aos problemas que se seguem. Esse anexo deverá
ser consultado e analisado à medida que isso for necessário.

\Problema

Um compilador é um programa que traduz uma linguagem dita de
\emph{alto nível} numa linguagem (dita de \emph{baixo nível}) que
seja executável por uma máquina.
Por exemplo, o \gcc{GCC} compila C/C++ em código objecto que
corre numa variedade de arquitecturas.

Compiladores são normalmente programas complexos.
Constam essencialmente de duas partes:
o \emph{analisador sintático} que lê o texto de entrada
(o programa \emph{fonte} a compilar) e cria uma sua representação
interna, estruturada em árvore;
e o \emph{gerador de código} que converte essa representação interna
em código executável.
Note-se que tal representação intermédia pode ser usada para outros fins,
por exemplo,
para gerar uma listagem de qualidade (\uk{pretty print}) do programa fonte.

O projecto de compiladores é um assunto complexo que
será assunto de outras disciplinas.
Neste trabalho pretende-se apenas fazer uma introdução ao assunto,
mostrando como tais programas se podem construir funcionalmente à custa de
cata/ana/hilo-morfismos da linguagem em causa.

Para cumprirmos o nosso objectivo, a linguagem desta questão terá que ser, naturalmente,
muito simples: escolheu-se a das expressões aritméticas com inteiros,
\eg\ \( 1 + 2 \), \( 3 * (4 + 5) \) etc.
Como representação interna adopta-se o seguinte tipo polinomial, igualmente simples:
%
\begin{spec}
data Expr = Num Int | Bop Expr Op Expr 
data Op = Op String
\end{spec}

\begin{enumerate}
\item
Escreva as definições dos \{cata, ana e hilo\}-morfismos deste tipo de dados
segundo o método ensinado nesta disciplina (recorde módulos como \eg\ |BTree| etc).
\item
Como aplicação do módulo desenvolvido no ponto 1,
defina como \{cata, ana ou hilo\}-morfismo a função seguinte:
\begin{itemize}
\item |calcula :: Expr -> Int| que calcula o valor
de uma expressão;
\begin{propriedade}
O valor zero é um elemento neutro da adição.
\begin{code}
prop_neutro1 :: Expr -> Bool
prop_neutro1 = calcula . addZero .==. calcula where
            addZero e = Bop (Num 0) (Op "+") e
prop_neutro2 :: Expr -> Bool
prop_neutro2 = calcula . addZero .==. calcula where
            addZero e = Bop e (Op "+") (Num 0) 
\end{code}
\end{propriedade}  
\begin{propriedade}
As operações de soma e multiplicação são comutativas.
\begin{code}
prop_comuta = calcula . mirror .==. calcula where
            mirror = cataExpr (either Num g2)
            g2 = (uncurry (uncurry Bop)) . (swap >< id) . assocl . (id >< swap)
\end{code}
\end{propriedade}  
\end{itemize}
\item
Defina como \{cata, ana ou hilo\}-morfismos as funções
\begin{itemize}
\item |compile :: String -> Codigo| - trata-se do compilador propriamente
      dito. Deverá ser gerado código posfixo para uma máquina elementar
      de \pda{stack}. O tipo |Codigo| pode ser definido à escolha.
      Dão-se a seguir exemplos de comportamentos aceitáveis para esta
      função:
\begin{verbatim}
Tp4> compile "2+4"
["PUSH 2", "PUSH 4", "ADD"]
Tp4> compile "3*(2+4)"
["PUSH 3", "PUSH 2", "PUSH 4", "ADD", "MUL"]
Tp4> compile "(3*2)+4"
["PUSH 3", "PUSH 2", "MUL", "PUSH 4", "ADD"]
Tp4> 
\end{verbatim}
\item |show' :: Expr -> String| - gera a representação textual
      de uma |Expr| pode encarar-se como o \uk{pretty printer}
      associado ao nosso compilador
\begin{propriedade}
Em anexo, é fornecido o código da função |readExp|, que é ``inversa" da função |show'|,
tal como a propriedade seguinte descreve:
\begin{code}
prop_inv :: Expr -> Bool
prop_inv = p1 . head . readExp . show' .==. id
\end{code}
\end{propriedade}  
\end{itemize}
%% \item Generalize o tipo |Expr| de forma a admitir operadores
%% unários (\eg\ \(-5\)) e repita os exercícios dos pontos anteriores.
\end{enumerate}

\paragraph{Valorização}
Em anexo é apresentado código \Haskell\ que permite declarar
|Expr| como instância da classe |Read|. Neste contexto,  
|read| pode ser vista como o analisador
sintático do nosso minúsculo compilador de expressões aritméticas.

Analise o código apresentado, corra-o e escreva no seu relatório uma explicação
\textbf{breve} do seu funcionamento, que deverá saber defender aquando da
apresentação oral do relatório.

Exprima ainda o analisador sintático |readExp| como um anamorfismo.

\Problema

Pretende-se neste problema definir uma linguagem gráfica \aspas{brinquedo}
a duas dimensões (2D) capaz de especificar e desenhar agregações de
caixas que contêm informação textual.
Vamos designar essa linguagem por |L2D| e vamos defini-la como um tipo
 em \Haskell:
\begin{code}
type L2D = X Caixa Tipo 
\end{code}
onde |X| é a estrutura de dados
\begin{code}
data X a b = Unid a | Comp b (X a b) (X a b) deriving Show
\end{code}
e onde:
\begin{code}
type Caixa = ((Int,Int),(Texto,G.Color))
type Texto = String
\end{code}
Assim, cada caixa de texto é especificada pela sua largura, altura, o seu
texto e a sua côr.\footnote{Pode relacionar |Caixa| com as caixas de texto usadas
nos jornais ou com \uk{frames} da linguagem \Html\ usada na Internet.}
Por exemplo,
\begin{spec}
((200,200),("Caixa azul",col_blue))
\end{spec}
designa a caixa da esquerda da figura \ref{fig:L2D}.

O que a linguagem |L2D| faz é agregar tais caixas tipográficas
umas com as outras segundo padrões especificados por vários
\aspas{tipos}, a saber,
\begin{code}
data Tipo = V | Vd | Ve | H | Ht | Hb 
\end{code}
com o seguinte significado:
\begin{itemize}
\item[|V|] - agregação vertical alinhada ao centro
\item[|Vd|] - agregação vertical justificada à direita
\item[|Ve|] - agregação vertical justificada à esquerda
\item[|H|] - agregação horizontal alinhada ao centro
\item[|Hb|] - agregação horizontal alinhada pela base
\item[|Ht|] - agregação horizontal alinhada pelo topo
\end{itemize}
Como |L2D| instancia o parâmetro |b| de |X| com
|Tipo|, é fácil de ver que cada \aspas{frase} da linguagem
|L2D| é representada por uma árvore binária em que cada nó
indica qual o tipo de agregação a aplicar às suas duas sub-árvores.
%
Por exemplo, a frase
\begin{code}
ex2= Comp Hb  (Unid ((100,200),("A",col_blue)))
              (Unid ((50,50),("B",col_green)))
\end{code}
deverá corresponder à imagem da direita da figura \ref{fig:L2D}.
E poder-se-á ir tão longe quando a linguagem o permita. Por exemplo, pense na
estrutura da frase que representa o \uk{layout} da figura \ref{fig:L2D1}.

\begin{figure}
\centering
\begin{picture}(190.00,130.00)(-15,-15)
\put(00.00,0.00){$(0,0)$}
\put(80.00,50.00){$(200,200)$}
\put(20.00,-10.00){
	\includegraphics[width=70\unitlength]{cp1819t_media/ex3.png}
}
\end{picture}
%
\includegraphics[width=0.2\textwidth]{cp1819t_media/ex2.png}
%
\caption{Caixa simples e caixa composta.\label{fig:L2D}}
\end{figure}

\begin{figure}
\centering
\includegraphics[width=0.6\textwidth]{cp1819t_media/ex.png}
\caption{\uk{Layout} feito de várias caixas coloridas.\label{fig:L2D1}}
\end{figure}

É importante notar que cada ``caixa'' não dispõe informação relativa
ao seu posicionamento final na figura. De facto, é a posição relativa
que deve ocupar face às restantes caixas que irá determinar a sua
posição final. Este é um dos objectivos deste trabalho:
\emph{calcular o posicionamento absoluto de cada uma das caixas por forma a
respeitar as restrições impostas pelas diversas agregações}. Para isso vamos
considerar um tipo de dados que comporta a informação de todas as
caixas devidamente posicionadas (i.e. com a informação adicional da
origem onde a caixa deve ser colocada).

\begin{spec}
type Fig = [(Origem,Caixa)]
type Origem = (Float, Float)
\end{spec}
%
A informação mais relevante deste tipo é a referente à lista de
``caixas posicionadas'' (tipo |(Origem,Caixa)|). Regista-se aí a origem
da caixa que, com a informação da sua altura e comprimento, permite
definir todos os seus pontos (consideramos as caixas sempre paralelas
aos eixos). 

\begin{enumerate}
\item Forneça a definição da função |calc_origems|, que calcula as
coordenadas iniciais das caixas no plano:
\begin{spec}
calc_origems :: (L2D,Origem) -> X (Caixa,Origem) ()
\end{spec}
\item Forneça agora a definição da função |agrup_caixas|, que agrupa
todas as caixas e respectivas origens numa só lista:
\begin{spec}
agrup_caixas :: X (Caixa,Origem) () -> Fig
\end{spec}
\end{enumerate}

Um segundo problema neste projecto é \emph{descobrir como visualizar a
informação gráfica calculada por |desenho|}. A nossa estratégia para
superar o problema baseia-se na biblioteca \gloss{Gloss}, que permite a geração
de gráficos 2D. Para tal disponibiliza-se a função
\begin{spec}
crCaixa :: Origem  -> Float -> Float -> String -> G.Color -> G.Picture
\end{spec}
que cria um rectângulo com base numa coordenada, um valor para a largura, um valor
para a altura, um texto que irá servir de etiqueta, e a cor pretendida.
Disponibiliza-se também a função
\begin{spec}
display :: G.Picture -> IO ()
\end{spec}
que dado um valor do tipo |G.picture| abre uma janela com esse valor desenhado. O objectivo
final deste exercício é implementar então uma função 
\begin{spec}
mostra_caixas :: (L2D,Origem) -> IO ()
\end{spec}
que dada uma frase da linguagem |L2D| e coordenadas iniciais apresenta
o respectivo desenho no ecrã.
%
\textbf{Sugestão}: 
Use a função |G.pictures| disponibilizada na biblioteca \gloss{Gloss}.

\Problema

Nesta disciplina estudou-se como fazer \pd{programação dinâmica} por cálculo,
recorrendo à lei de recursividade mútua.\footnote{Lei (\ref{eq:fokkinga})
em \cite{Ol18}, página \pageref{eq:fokkinga}.}

Para o caso de funções sobre os números naturais (|Nat0|, com functor |fF
X = 1 + X|) é fácil derivar-se da lei que foi estudada uma
	\emph{regra de algibeira}
	\label{pg:regra}
que se pode ensinar a programadores que não tenham estudado
\cp{Cálculo de Programas}. Apresenta-se de seguida essa regra, tomando como exemplo o
cálculo do ciclo-\textsf{for} que implementa a função de Fibonacci, recordar
o sistema
\begin{spec}
fib 0 = 1
fib(n+1) = f n

f 0 = 1
f (n+1) = fib n + f n
\end{spec}
Obter-se-á de imediato
\begin{code}
fib' = p1 . for loop init where
   loop(fib,f)=(f,fib+f)
   init = (1,1)
\end{code}
usando as regras seguintes:
\begin{itemize}
\item	O corpo do ciclo |loop| terá tantos argumentos quanto o número de funções mutuamente recursivas.
\item	Para as variáveis escolhem-se os próprios nomes das funções, pela ordem
que se achar conveniente.\footnote{Podem obviamente usar-se outros símbolos, mas numa primeiraleitura
dá jeito usarem-se tais nomes.}
\item	Para os resultados vão-se buscar as expressões respectivas, retirando a variável |n|.
\item	Em |init| coleccionam-se os resultados dos casos de base das funções, pela mesma ordem.
\end{itemize}
Mais um exemplo, envolvendo polinómios no segundo grau a $x^2 + b x + c$ em |Nat0|.
Seguindo o método estudado nas aulas\footnote{Secção 3.17 de \cite{Ol18}.},
de $f\ x = a x^2 + b x + c$ derivam-se duas funções mutuamente recursivas:
\begin{spec}
f 0 = c
f (n+1) = f n + k n

k 0 = a + b
k(n+1) = k n + 2 a
\end{spec}
Seguindo a regra acima, calcula-se de imediato a seguinte implementação, em Haskell:
\begin{code}
f' a b c = p1 . for loop init where
  loop(f,k) = (f+k,k+2*a)
  init = (c,a+b) 
\end{code}

Qual é o assunto desta questão, então? Considerem fórmula que dá a série de Taylor da
função coseno:
\begin{eqnarray*}
	cos\ x = \sum_{i=0}^\infty \frac{(-1)^i}{(2i)!} x^{2i}
\end{eqnarray*}
Pretende-se o ciclo-\textsf{for} que implementa a função 
|cos' x n| que dá o valor dessa série tomando |i| até |n| inclusivé:
\begin{spec}
cos' x = cdots . for loop init where cdots
\end{spec}
%
\textbf{Sugestão}: Começar por estudar muito bem o processo de cálculo dado
no anexo \ref{sec:recmul} para o problema (semelhante) da função exponencial.

\begin{propriedade}
Testes de que |cos' x| calcula bem o coseno de |pi| e o coseno de |pi/2|:
\begin{code}
prop_cos1 n = n >= 10 ==> abs(cos pi - cos' pi n) < 0.001
prop_cos2 n = n >= 10 ==> abs(cos (pi/2) - cos' (pi/2) n) < 0.001
\end{code}
\end{propriedade}

\paragraph{Valorização} Transliterar |cos'| para a linguagem C; compilar
e testar o código. Conseguia, por intuição apenas, chegar a esta função?

\Problema

Pretende-se nesta questão desenvolver uma biblioteca de funções para
manipular \emph{sistemas de ficheiros} genéricos.
Um sistema de ficheiros será visto como uma associação de \emph{nomes}
a ficheiros ou \emph{directorias}.
Estas últimas serão vistas como sub-sistemas de ficheiros e assim
recursivamente.
Assumindo que |a| é o tipo dos identificadores dos ficheiros e
directorias, e que |b| é o tipo do conteúdo dos ficheiros,
podemos definir um tipo indutivo de dados para representar sistemas de
ficheiros da seguinte forma:
\begin{code}
data FS a b = FS [(a,Node a b)] deriving (Eq,Show)
data Node a b = File b | Dir (FS a b) deriving (Eq,Show)
\end{code}
Um caminho (\emph{path}) neste sistema de ficheiros pode ser representado pelo
seguinte tipo de dados:
\begin{code}
type Path a = [a]
\end{code}
Assumindo estes tipos de dados, o seguinte termo
\begin{spec}
FS [("f1", File "Ola"),
    ("d1", Dir (FS [("f2", File "Ole"),
                    ("f3", File "Ole")
                   ]))
   ]
\end{spec}
representará um sistema de ficheiros em cuja raíz temos um ficheiro chamado
|f1| com conteúdo |"Ola"| e uma directoria chamada |"d1"| constituída por dois
ficheiros, um chamado |"f2"| e outro chamado |"f3"|, ambos com conteúdo |"Ole"|.
%
Neste caso, tanto o tipo dos identificadores como o tipo do conteúdo dos
ficheiros é |String|. No caso geral, o conteúdo de um ficheiro é arbitrário:
pode ser um binário, um texto, uma colecção de dados, etc.

A definição das usuais funções |inFS| e |recFS| para este tipo é a seguinte:
\begin{code}
inFS = FS . map (id >< inNode) 
inNode = either File Dir

recFS f = baseFS id id f
\end{code}
Suponha que se pretende definir como um |catamorfismo| a função que
conta o número de ficheiros existentes num sistema de ficheiros. Uma
possível definição para esta função seria:
\begin{code}
conta :: FS a b -> Int
conta = cataFS (sum . map ((either (const 1) id) . snd))
\end{code}
O que é para fazer:
\begin{enumerate}
\item Definir as funções |outFS|, |baseFS|, |cataFS|, |anaFS| e |hyloFS|.

\item Apresentar, no relatório, o diagrama de |cataFS|.

\item Definir as seguintes funções para manipulação de sistemas de
  ficheiros usando, obrigatoriamente, catamorfismos, anamorfismos ou
  hilomorfismos:

  \begin{enumerate}
  \item Verificação da integridade do sistema de ficheiros (i.e. verificar
    que não existem identificadores repetidos dentro da mesma directoria). \\
    |check :: FS a b -> Bool|
  \begin{propriedade}
    A integridade de um sistema de ficheiros não depende da ordem em que os    
    últimos são listados na sua directoria:
\begin{code}
prop_check :: FS String String -> Bool
prop_check = check . (cataFS (inFS . reverse)).==. check
\end{code}
  \end{propriedade}  
  \item Recolha do conteúdo de todos os ficheiros num arquivo indexado pelo \emph{path}.\\
    |tar :: FS a b -> [(Path a, b)]|
  \begin{propriedade}
    O número de ficheiros no sistema deve ser igual ao número de ficheiros
    listados pela função |tar|.
\begin{code}
prop_tar :: FS String String -> Bool
prop_tar = length . tar .==. conta 
\end{code}
  \end{propriedade}  
  \item Transformação de um arquivo com o conteúdo dos ficheiros
    indexado pelo \emph{path} num sistema de ficheiros.\\
    |untar :: [(Path a, b)] -> FS a b| \\
  \textbf{Sugestão}: Use a função |joinDupDirs| para juntar directorias que estejam na mesma
  pasta e que possuam o mesmo identificador.
  \begin{propriedade}
    A composição |tar . untar| preserva o número de ficheiros no sistema.
\begin{code}
prop_untar :: [(Path String, String)] -> Property
prop_untar = validPaths .==>. ((length . tar . untar) .==. length)
validPaths :: [(Path String, String)] -> Bool
validPaths = (== 0) . length . (filter (\(a,_) -> length a == 0))
\end{code}
\end{propriedade}  
  \item Localização de todos os \emph{paths} onde existe um
    determinado ficheiro.\\
    |find :: a -> FS a b -> [Path a]|
  \begin{propriedade}
    A composição |tar . untar| preserva todos os ficheiros no sistema.
\begin{code}
prop_find :: String -> FS String String -> Bool
prop_find = curry $ 
      length . (uncurry find) .==. length . (uncurry find) . (id >< (untar . tar))
\end{code}
  \end{propriedade}  
  \item Criação de um novo ficheiro num determinado \emph{path}.\\
    |new :: Path a -> b -> FS a b -> FS a b|
  \begin{propriedade}
A adição de um ficheiro não existente no sistema não origina ficheiros duplicados.
\begin{code}
prop_new :: ((Path String,String), FS String String) -> Property
prop_new = ((validPath .&&&. notDup) .&&&. (check . p2)) .==>. 
      (checkFiles . (uncurry (uncurry new)))  where
      validPath = (/=0) . length . p1 . p1
      notDup = not . (uncurry elem) . (p1 >< ((fmap p1) . tar))
\end{code}
\textbf{Questão}: Supondo-se que no código acima se substitui a propriedade
|checkFiles| pela propriedade mais fraca |check|, será que a propriedade
|prop_new| ainda é válida? Justifique a sua resposta. 
\end{propriedade}
 
\begin{propriedade}
	A listagem de ficheiros logo após uma adição nunca poderá ser menor que a listagem
	de ficheiros antes dessa mesma adição.
\begin{code}
prop_new2 :: ((Path String,String), FS String String) -> Property
prop_new2 = validPath .==>. ((length . tar . p2) .<=. (length . tar . (uncurry (uncurry new)))) where
      validPath = (/=0) . length . p1 . p1
\end{code}
  \end{propriedade}  
  \item Duplicação de um ficheiro.\\
    |cp :: Path a -> Path a -> FS a b -> FS a b|
  \begin{propriedade}
    A listagem de ficheiros com um dado nome não diminui após uma duplicação.
\begin{code}
prop_cp :: ((Path String, Path String),  FS String String) -> Bool
prop_cp =   length . tar . p2 .<=. length . tar . (uncurry (uncurry cp))
\end{code}
  \end{propriedade}  
  \item Eliminação de um ficheiro.\\
    |rm :: Path a -> FS a b -> FS a b|

  \textbf{Sugestão}: Construir um anamorfismo |nav :: (Path a, FS a b) -> FS a b|
  que navegue por um sistema de ficheiros tendo como base o |path| dado como argumento.
    \begin{propriedade}
    Remover duas vezes o mesmo ficheiro tem o mesmo efeito que o remover apenas uma vez.
\begin{code}
prop_rm :: (Path String, FS String String) -> Bool
prop_rm = (uncurry rm ) . (split p1 (uncurry rm)) .==. (uncurry rm)
\end{code}
\end{propriedade}
\begin{propriedade}
Adicionar um ficheiro e de seguida remover o mesmo não origina novos ficheiros no sistema.
\begin{code}
prop_rm2 :: ((Path String,String), FS String String) -> Property
prop_rm2 = validPath  .==>. ((length . tar . (uncurry rm) . (split (p1. p1) (uncurry (uncurry new)))) 
         .<=. (length . tar . p2)) where
         validPath = (/=0) . length . p1 . p1
\end{code}
\end{propriedade}  
  \end{enumerate}
\end{enumerate}

\paragraph{Valorização} 

Definir uma função para visualizar em \graphviz{Graphviz}
a estrutura de um sistema de ficheiros. A Figura~\ref{ex_prob1}, por exemplo,
apresenta a estrutura de um sistema com precisamente dois ficheiros dentro
de uma directoria chamada |"d1"|. 

Para realizar este exercício será necessário apenas  escrever o anamorfismo
\begin{quote}
|cFS2Exp :: (a, FS a b) -> (Exp () a)| 
\end{quote}
que converte a estrutura de um sistema de ficheiros numa árvore de expressões
descrita em \href{http://wiki.di.uminho.pt/twiki/pub/Education/CP/MaterialPedagogico/Exp.hs}{Exp.hs}.
A função |dotFS| depois tratará de passar a estrutura do sistema de ficheiros para o visualizador.
\begin{figure}
\centering
\includegraphics[scale=0.5]{cp1819t_media/fs.png}
\caption{Exemplo de um sistema de ficheiros visualizado em \graphviz{Graphviz}.}
\label{ex_prob1}
\end{figure}

%----------------- Programa, bibliotecas e código auxiliar --------------------%

\newpage

\part*{Anexos}

\appendix

\section{Como exprimir cálculos e diagramas em LaTeX/lhs2tex}
Estudar o texto fonte deste trabalho para obter o efeito:\footnote{Exemplos tirados de \cite{Ol18}.} 
\begin{eqnarray*}
\start
	|id = split f g|
%
\just\equiv{ universal property }
%
        |lcbr(
		p1 . id = f
	)(
		p2 . id = g
	)|
%
\just\equiv{ identity }
%
        |lcbr(
		p1 = f
	)(
		p2 = g
	)|
\qed
\end{eqnarray*}

Os diagramas podem ser produzidos recorrendo à \emph{package} \LaTeX\ 
\href{https://ctan.org/pkg/xymatrix}{xymatrix}, por exemplo: 
\begin{eqnarray*}
\xymatrix@@C=2cm{
    |Nat0|
           \ar[d]_-{|cataNat g|}
&
    |1 + Nat0|
           \ar[d]^{|id + (cataNat g)|}dw
}
\end{eqnarray*}

\section{Programação dinâmica por recursividade múltipla}\label{sec:recmul}
Neste anexo dão-se os detalhes da resolução do Exercício \ref{ex:exp} dos apontamentos da
disciplina\footnote{Cf.\ \cite{Ol18}, página \pageref{ex:exp}.},
onde se pretende implementar um ciclo que implemente
o cálculo da aproximação até |i=n| da função exponencial $exp\ x = e^x$
via série de Taylor:
\begin{eqnarray}
	exp\ x 
& = &
	\sum_{i=0}^{\infty} \frac {x^i} {i!}
\end{eqnarray}
Seja $e\ x\ n = \sum_{i=0}^{n} \frac {x^i} {i!}$ a função que dá essa aproximação.
É fácil de ver que |e x 0 = 1| e que $|e x (n+1)| = |e x n| + \frac {x^{n+1}} {(n+1)!}$.
Se definirmos $|h x n| = \frac {x^{n+1}} {(n+1)!}$ teremos |e x| e |h x| em recursividade
mútua. Se repetirmos o processo para |h x n| etc obteremos no total três funções nessa mesma
situação:
\begin{spec}
e x 0 = 1
e x (n+1) = h x n + e x n

h x 0 = x
h x (n+1) = x/(s n) * h x n

s 0 = 2
s (n+1) = 1 + s n
\end{spec}
Segundo a \emph{regra de algibeira} descrita na página \ref{pg:regra} deste enunciado,
ter-se-á, de imediato:
\begin{code}
e' x = prj . for loop init where
     init = (1,x,2)
     loop(e,h,s)=(h+e,x/s*h,1+s)
     prj(e,h,s) = e
\end{code}

\section{Código fornecido}\label{sec:codigo}

\subsection*{Problema 1}
Tipos:
\begin{code}
data Expr = Num Int 
               | Bop Expr Op Expr  deriving  (Eq,Show)

data Op = Op String deriving (Eq,Show)

type Codigo = [String]
\end{code}
Functor de base:
\begin{code}
baseExpr f g = id -|- (f >< (g >< g))
\end{code}
Instâncias:
\begin{code}
instance Read Expr where
   readsPrec _ = readExp
\end{code}
Read para Exp's:
\begin{code}
readOp :: String -> [(Op,String)]
readOp input = do 
                 (x,y) <- lex input
                 return ((Op x),y)

readNum :: ReadS Expr
readNum  = (map (\ (x,y) -> ((Num x), y))).reads

readBinOp :: ReadS Expr
readBinOp = (map (\ ((x,(y,z)),t) -> ((Bop x y z),t))) .
                   ((readNum `ou` (pcurvos readExp))
                    `depois` (readOp `depois` readExp))

readExp :: ReadS Expr
readExp = readBinOp `ou` (
          readNum `ou` (
          pcurvos readExp))
\end{code}
Combinadores:
\begin{code}

depois :: (ReadS a) -> (ReadS b) -> ReadS (a,b)
depois _ _ [] = []
depois r1 r2 input = [((x,y),i2) | (x,i1) <- r1 input , 
                                   (y,i2) <- r2 i1]

readSeq :: (ReadS a) -> ReadS [a]
readSeq r input 
  = case (r input) of
    [] -> [([],input)]
    l -> concat (map continua l)
         where continua (a,i) = map (c a) (readSeq r i)
               c x (xs,i) = ((x:xs),i)                     

ou :: (ReadS a) -> (ReadS a) -> ReadS a
ou r1 r2 input = (r1 input) ++ (r2 input)

senao :: (ReadS a) -> (ReadS a) -> ReadS a
senao r1 r2 input = case (r1 input) of
                     [] -> r2 input
                     l  -> l

readConst :: String -> ReadS String
readConst c = (filter ((== c).fst)) . lex

pcurvos = parentesis '(' ')'
prectos = parentesis '[' ']'
chavetas = parentesis '{' '}'

parentesis :: Char -> Char -> (ReadS a) -> ReadS a
parentesis _ _ _ [] = []
parentesis ap pa r input 
  = do 
      ((_,(x,_)),c) <- ((readConst [ap]) `depois` (
                        r                `depois` (
                        readConst [pa])))           input
      return (x,c)
\end{code}

\subsection*{Problema 2}
Tipos:
\begin{code}
type Fig = [(Origem,Caixa)]
type Origem = (Float, Float)
\end{code}
``Helpers":
\begin{code}
col_blue = G.azure
col_green = darkgreen

darkgreen = G.dark (G.dark G.green)
\end{code}
Exemplos:
\begin{code}
ex1Caixas = G.display (G.InWindow "Problema 4" (400, 400) (40, 40)) G.white $
          crCaixa (0,0) 200 200 "Caixa azul" col_blue

ex2Caixas =  G.display (G.InWindow "Problema 4" (400, 400) (40, 40)) G.white $
          caixasAndOrigin2Pict ((Comp Hb bbox gbox),(0.0,0.0)) where
          bbox = Unid ((100,200),("A",col_blue))
          gbox = Unid ((50,50),("B",col_green))

ex3Caixas = G.display (G.InWindow "Problema 4" (400, 400) (40, 40)) G.white mtest where
          mtest = caixasAndOrigin2Pict $ (Comp Hb (Comp Ve bot top) (Comp Ve gbox2 ybox2), (0.0,0.0))
          bbox1 = Unid ((100,200),("A",col_blue))
          bbox2 = Unid ((150,200),("E",col_blue))
          gbox1 = Unid ((50,50),("B",col_green))
          gbox2 = Unid ((100,300),("F",col_green))
          rbox1 = Unid ((300,50),("C",G.red))
          rbox2 = Unid ((200,100),("G",G.red))
          wbox1 = Unid ((450,200),("",G.white))
          ybox1 = Unid ((100,200),("D",G.yellow))
          ybox2 = Unid ((100,300),("H",G.yellow))
          bot = Comp Hb wbox1 bbox2 
          top = (Comp Ve (Comp Hb bbox1 gbox1) (Comp Hb rbox1 (Comp H ybox1 rbox2)))

myL2D = G.display (G.InWindow "Problema 4" (400, 400) (40, 40)) G.white mtest where
   mtest = caixasAndOrigin2Pict $ ((Comp Ht (Comp Hb a b) c), (0.0,0.0))
   a = Unid ((300,600),("A", col_blue))
   b = Unid ((150,150),("B", col_green))  
   c = Unid ((200,100),("C", G.red))

\end{code}
A seguinte função cria uma caixa a partir dos seguintes parâmetros: origem,
largura, altura, etiqueta e côr de preenchimento.
\begin{code}
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
\end{code}
Função para visualizar resultados gráficos:
\begin{code}
display = G.display (G.InWindow "Problema 4" (400, 400) (40, 40)) G.white 
\end{code}

\subsection*{Problema 4}
Funções para gestão de sistemas de ficheiros:
\begin{code}
concatFS = inFS . (uncurry (++)) . (outFS >< outFS)
mkdir (x,y) = FS [(x, Dir y)]
mkfile (x,y) = FS [(x, File y)]

joinDupDirs :: (Eq a) => (FS a b) -> (FS a b)
joinDupDirs  = anaFS (prepOut . (id >< proc) . prepIn) where 
         prepIn = (id >< (map (id >< outFS))) . sls . (map distr) . outFS
         prepOut = (map undistr) . (uncurry (++)) . ((map i1) >< (map i2)) . (id >< (map (id >< inFS)))
         proc = concat . (map joinDup) . groupByName
         sls = split lefts rights

joinDup :: [(a,[b])] -> [(a,[b])]
joinDup = cataList (either nil g) where g = return . (split (p1 . p1) (concat . (map p2) . (uncurry (:))))

createFSfromFile :: (Path a, b) -> (FS a b)
createFSfromFile ([a],b) = mkfile(a,b)
createFSfromFile (a:as,b) = mkdir(a, createFSfromFile (as,b))
\end{code}
Funções auxiliares:
\begin{code}
checkFiles :: (Eq a) => FS a b -> Bool
checkFiles = cataFS ((uncurry (&&)) . (split f g)) where
           f = nr . (fmap p1) . lefts . (fmap distr)
           g = and . rights . (fmap p2)

groupByName :: (Eq a) => [(a,[b])] -> [[(a,[b])]]
groupByName = (groupBy (curry p)) where
            p = (uncurry (==)) . (p1 >< p1)

filterPath :: (Eq a) => Path a -> [(Path a, b)] -> [(Path a, b)]
filterPath = filter . (\p -> \(a,b) -> p == a)
\end{code}
Dados para testes:
\begin{itemize}
\item Sistema de ficheiros vazio:
\begin{code}
efs = FS []
\end{code}
\item Nível 0
\begin{code}
f1 = FS [("f1", File "hello world")]
f2 = FS [("f2", File "more content")]
f00 = concatFS (f1,f2)
f01 = concatFS (f1, mkdir("d1", efs))
f02 = mkdir("d1",efs)
\end{code}
\item Nível 1
\begin{code}
f10 = mkdir("d1", f00)
f11 = concatFS(mkdir("d1", f00), mkdir("d2", f00))
f12 = concatFS(mkdir("d1", f00), mkdir("d2", f01))
f13 = concatFS(mkdir("d1", f00), mkdir("d2", efs))
\end{code}
\item Nível 2
\begin{code}
f20 = mkdir("d1", f10)
f21 = mkdir("d1", f11)
f22 = mkdir("d1", f12)
f23 = mkdir("d1", f13)
f24 = concatFS(mkdir("d1",f10), mkdir("d2",f12))
\end{code}
\item Sistemas de ficheiros inválidos:
\begin{code}
ifs0 = concatFS (f1,f1)
ifs1 = concatFS (f1, mkdir("f1", efs))
ifs2 = mkdir("d1", ifs0)
ifs3 = mkdir("d1", ifs1)
ifs4 = concatFS(mkdir("d1",ifs1), mkdir("d2",f12))
ifs5 = concatFS(mkdir("d1",f1), mkdir("d1",f2))
ifs6 = mkdir("d1",ifs5)
ifs7 = concatFS(mkdir("d1",f02),mkdir("d1",f02))
\end{code}
\end{itemize}
Visualização em \graphviz{Graphviz}:
\begin{code}
dotFS :: FS String b -> IO ExitCode
dotFS = dotpict . bmap (const "") id . (cFS2Exp "root")
\end{code}
\def\omitted{``Automagically" generated:
\begin{code}
instance (Arbitrary a, Arbitrary b) => Arbitrary (FS a b) where
   arbitrary = sized genfs  where
             genfs 0 = liftM (inFS . (map (id >< (i1)))) QuickCheck.arbitrary
             genfs n = oneof [liftM (inFS . (map (id >< (i1)))) QuickCheck.arbitrary,
                     liftM (inFS . return . (id >< (i2))) (liftM2 (,) 
                     QuickCheck.arbitrary (genfs (n - 1))),
                     liftM3 genAux QuickCheck.arbitrary (genfs (n - 1)) (genfs (n - 1))]
             genAux a x y = inFS [(a, i2 x), (a, i2 y)]

instance Arbitrary Expr where
   arbitrary = (genExpr 10)  where
             genExpr 0 = liftM (inExpr . i1) QuickCheck.arbitrary
             genExpr n = oneof [liftM (inExpr . i1) QuickCheck.arbitrary,
                       liftM (inExpr . i2 . (uncurry genAux1)) 
                       $ liftM2 (,)  (genExpr (n-1)) (genExpr (n-1)),
                       liftM (inExpr . i2 . (uncurry genAux2)) 
                       $ liftM2 (,)  (genExpr (n-1)) (genExpr (n-1))
                     ]
             genAux1 x y = (Op "+",(x,y))
             genAux2 x y = (Op "*",(x,y))
\end{code}}

\subsection*{Outras funções auxiliares}
%----------------- Outras definições auxiliares -------------------------------------------%
Lógicas:
\begin{code}
infixr 0 .==>.
(.==>.) :: (Testable prop) => (a -> Bool) -> (a -> prop) -> a -> Property
p .==>. f = \a -> p a ==> f a

infixr 0 .<==>.
(.<==>.) :: (a -> Bool) -> (a -> Bool) -> a -> Property
p .<==>. f = \a -> (p a ==> property (f a)) .&&. (f a ==> property (p a))

infixr 4 .==.
(.==.) :: Eq b => (a -> b) -> (a -> b) -> (a -> Bool)
f .==. g = \a -> f a == g a

infixr 4 .<=.
(.<=.) :: Ord b => (a -> b) -> (a -> b) -> (a -> Bool)
f .<=. g = \a -> f a <= g a

infixr 4 .&&&.
(.&&&.) :: (a -> Bool) -> (a -> Bool) -> (a -> Bool)
f .&&&. g = \a -> ((f a) && (g a))
\end{code}
Compilação e execução dentro do interpretador:\footnote{Pode ser útil em testes
envolvendo \gloss{Gloss}. Nesse caso, o teste em causa deve fazer parte de uma função
|main|.}
\begin{code}

run = do { system "ghc cp1819t" ; system "./cp1819t" }
\end{code}

%----------------- Soluções dos alunos -----------------------------------------%

\section{Soluções dos alunos}\label{sec:resolucao}
Os alunos devem colocar neste anexo as suas soluções aos exercícios
propostos, de acordo com o "layout" que se fornece. Não podem ser
alterados os nomes ou tipos das funções dadas, mas pode ser adicionado texto e/ou 
outras funções auxiliares que sejam necessárias.

expn a b = a^b

%format (expn(a) (n)) = "{" a "}^{" n "}"
%format (anaNat (g)) = "\ana{" g "}"

\subsection*{Problema 1}

\begin{code}

opToMiddle = (swap >< id) . assocl

\end{code}
\begin{eqnarray*}
\xymatrix@@C=2cm{
  |Int| \ar[r]^-{|i1|} \ar[rd]_-{|f = Num|} &
  |Int + Op >< (expn Expr 2)| \ar[d]^{|inExpr = [f,g]|} &
  |Op >< expn Expr 2| \ar[l]_-{|i2|} \ar[ld]^--{|g = (uncurry (uncurry Bop)) . opToMiddle|}\\
  &|Expr|}
}
\end{eqnarray*}
\begin{code}

inExpr :: Either Int (Op,(Expr,Expr)) -> Expr
inExpr = either Num ((uncurry (uncurry Bop)) . opToMiddle)

outExpr :: Expr -> Either Int (Op,(Expr,Expr))
outExpr (Num n) = i1 n
outExpr (Bop e1 o e2) = i2 (o, (e1,e2))

recExpr f = baseExpr id f

cataExpr g = g . recExpr (cataExpr g) . outExpr

anaExpr g = inExpr . recExpr (anaExpr g) . g

hyloExpr f g = cataExpr f . anaExpr g

\end{code}
\begin{eqnarray*}
\xymatrix@@C=2cm{
    |Expr|
           \ar[d]_-{|calcula|}
&
    |Int + Op><(Expr><Expr)|
           \ar[d]^{|id + id >< (calcula >< calcula)|}
           \ar[l]_-{|inExpr|}
\\
     |Int|
&
     |Int + Op><(Int><Int)|
           \ar[l]^-{|[id,apply]|}
}
\end{eqnarray*}
\begin{code}

calcula :: Expr -> Int
calcula = cataExpr $ either id apply where 
        apply (Op "+",(e1,e2)) = e1 + e2
        apply (Op "*",(e1,e2)) = e1 * e2

\end{code}
\begin{eqnarray*}
\xymatrix@@C=2cm{
    |Expr|
           \ar[d]_-{|show'|}
&
    |Int + Op><(Expr><Expr)|
           \ar[d]^{|id + id >< (show' >< show')|}
           \ar[l]_-{|inExpr|}
\\
     |String|
&
     |Int + Op><(String><String)|
           \ar[l]^-{|[show,gather]|}
}
\end{eqnarray*}
\begin{code}

show' = cataExpr $ either show gather where 
      gather = conc . ((conc . (closeLeft >< outOp)) >< closeRight) .  opToMiddle 
      outOp (Op s) = " " ++ s ++ " "
      closeRight l = l ++ ")"
      closeLeft l = '(' : l

\end{code}
\begin{eqnarray*}
\xymatrix@@C=7cm{
    |String|
           \ar[d]_-{|anaNat divide|}
           \ar[r]_-{|divide|}
&
    |Int + Op><(String><String)|
           \ar[d]^{|id + id >< anaNat divide|}
\\
    |Expr|
           \ar[d]_-{|cataNat conquer|}
&
    |Int + Op><(Expr><Expr)|
           \ar[d]^{|id + id >< (cataNat conquer >< cataNat conquer)|}
           \ar[l]_-{|inExpr|}
\\
     |Codigo|
&
     |Int + Op><(Codigo><Codigo)|
           \ar[l]^-{|conquer = [singl . transform . show,conc . swap . (outOp >< conc)]|}
}
\end{eqnarray*}
\begin{code}

compile :: String -> Codigo
compile = hyloExpr conquer divide 

conquer = either (singl . transform . show) (conc . swap . (outOp >< conc))
    where outOp (Op s) = singl $ transform s

transform :: String -> String
transform "+" = "ADD"
transform "*" = "MUL" 
transform a = "PUSH " ++ a

\end{code}
\begin{eqnarray*}
\xymatrix@@C=7cm{
    |String|
           \ar[d]_-{|myReadExp = anaNat divide|}
           \ar[r]_-{|divide|}
&
    |Int + Op><(String><String)|
           \ar[d]^{|id + id >< anaNat divide|}
\\
    |Expr|
&
    |Int + Op><(Expr><Expr)|
           \ar[l]_-{|inExpr|}
}
\end{eqnarray*}
\begin{code}

myReadExp = anaExpr divide

divide :: String -> Either Int (Op, (String, String))
divide = (either (error "Invalid input") k) . outList where
   k = (cond areThereOps g f) . cons
   areThereOps = uncurry (||) . split (elem '+') (elem '*')
   f s = i1 (read s :: Int)
   g = i2 . getStrings  . p1 . head . readBinOp . removeOutermost
   removeOutermost = cond (uncurry (&&) . split ((== '(') . head) ((== ')') . last)) (tail . init) id
   getStrings (Bop e1 o e2) = (o,(show' e1, show' e2))
\end{code}

\subsection*{Problema 2}

\begin{eqnarray*}
\xymatrix@@C=2cm{
  |A| \ar[r]^-{|i1|} \ar[rd]_-{|f = Unid|} &
  |A + B >< (expn (X A B) 2)| \ar[d]^{|inL2D = [f,g]|} &
  |B >< expn (X A B) 2| \ar[l]_-{|i2|} \ar[ld]^--{|g = tripleUncurry Comp|}\\
  &|X A B|}
}
\end{eqnarray*}
\begin{code}

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

\end{code}
\begin{eqnarray*}
\xymatrix@@C=2cm{
    |L2D|
           \ar[d]_-{|collectLeafs = cataNat g|}
&
    |Caixa + Tipo >< (L2D >< L2D)|
           \ar[d]^{|id + id >< (collectLeafs >< collectLeafs)|}
           \ar[l]_-{|inL2D|}
\\
     |expn A (*)|
&
     |Caixa + Tipo >< (expn A (*) >< expn A (*))|
           \ar[l]^-{|g = [singl, conc . p2]|}
}
\end{eqnarray*}
\begin{code}

collectLeafs = cataL2D (either singl (conc . p2))

\end{code}
\begin{eqnarray*}
\xymatrix@@C=6cm{
    |L2D|
           \ar[d]_-{|dimen = cataNat g|}
&
    |Caixa + Tipo >< (L2D >< L2D)|
           \ar[d]^{|id + id >< (dimen >< dimen)|}
           \ar[l]_-{|inL2D|}
\\
     |expn Float 2|
&
     |Caixa + Tipo >< (expn Float 2><expn Float 2)|
           \ar[l]^-{|g = [(fromIntegral >< fromIntegral) . p1, uncurry soma_dimen]|}
}
\end{eqnarray*}
\begin{code}

dimen :: X Caixa Tipo -> (Float, Float)
dimen = cataL2D (either ((fromIntegral >< fromIntegral) . p1) (uncurry soma_dimen))

soma_dimen :: Tipo -> ((Float,Float),(Float,Float)) -> (Float, Float)
soma_dimen H ((x1,y1),(x2,y2)) = (x1+x2, y1+(y2-(y1/2)))
soma_dimen Hb ((x1,y1),(x2,y2)) = (x1+x2, max y1 y2)
soma_dimen Ht ((x1,y1),(x2,y2)) = (x1+x2, y1+y2)
soma_dimen V ((x1,y1),(x2,y2)) = (x1+(x2-(x1/2)), y1+y2)
soma_dimen Ve ((x1,y1),(x2,y2)) = (max x1 x2, y1+y2)
soma_dimen Vd ((x1,y1),(x2,y2)) = (x1+x2, y1+y2)

\end{code}

\begin{eqnarray*}
\xymatrix@@C=1cm{
    |L2D><Origem|
           \ar[d]_-{|calcOrigins = anaNat g|}
           \ar[r]_-{|g|}
&
    |(Caixa >< Origem) + 1 >< expn ((L2D><Origem)) 2|
           \ar[d]^{|id + id >< expn calcOrigins 2|}
\\
     |expn Float 2|
&
     |(Caixa >< Origem) + 1 >< (expn Float 2><expn Float 2)|
           \ar[l]^-{|inL2D|}
}
\end{eqnarray*}
\begin{eqnarray*}
\xymatrix@@C=3cm{
    |L2D><Origem|
           \ar[d]_-{|g|}
           \ar[r]^-{|outL2D><id|}
&
    |(Caixa + Tipo >< expn L2D 2) >< Origem|
           \ar[d]^-{|distl|}
\\
    |Caixa >< Origem + 1 >< (expn ((L2D><Origem)) 2)|
&
    |Caixa >< Origem + (Tipo >< expn L2D 2) >< Origem|
           \ar[l]^-{|id + <! . p1 . p1, uncurry (uncurry alpha)>|}
}
\end{eqnarray*}
\begin{code}

calcOrigins :: ((X Caixa Tipo),Origem) -> X (Caixa,Origem) ()
calcOrigins = anaL2D((id -|- k) . distl . (outL2D >< id)) where
   k = split ((!) . p1 . p1) (uncurry (uncurry alpha))
   alpha t (l1,l2) o = ((l1,o),(l2,calc t o $ dimen l1)) 

calc :: Tipo -> Origem -> (Float, Float) -> Origem
calc = curry $ curry fullCalc where
  fullCalc = uncurry calcAux . (split (p1 . p1) (split (p1 >< p1) (p2 >< p2) . (p2 >< id)))
  calcAux Ht = uncurry (+) >< uncurry (+)
  calcAux Hb = uncurry (+) >< p1
  calcAux H = uncurry (+) >< (uncurry (+) . (id >< (/2)))
  calcAux Vd = uncurry (+) >< uncurry (+)
  calcAux Ve = p1 >< uncurry (+)
  calcAux V = (uncurry (+) . (id >< (/2))) >< uncurry (+)


\end{code}
\begin{eqnarray*}
\xymatrix@@C=2cm{
    |L2D|
           \ar[d]_-{|agrup_caixas = cataNat g|}
&
    |Caixa + Tipo >< (expn L2D 2)|
           \ar[d]^{|id + id >< (expn agrup_caixas 2)|}
           \ar[l]_-{|inL2D|}
\\
     |expn A (*)|
&
     |Caixa + Tipo >< (expn A (*) >< expn A (*))|
           \ar[l]^-{|g = [singl . swap, conc . p2]|}
}
\end{eqnarray*}
\begin{code}

agrup_caixas :: X (Caixa,Origem) () -> [(Origem,Caixa)]
agrup_caixas = cataL2D (either (singl . swap) (conc . p2))

\end{code}
\begin{eqnarray*}
\xymatrix@@C=2cm{
    |L2D><Origem|
           \ar[d]^-{|caixasAndOrigin2Pict|}
           \ar[r]^-{|calcOrigins|}
&
    |X (Caixa><Origem) 1|
           \ar[r]^-{|agrup_caixas|}
&
    |expn (Origem >< Caixa) 2|
           \ar[d]^-{|expn ((id >< caixaToFloat)) (*)|}
\\
    |G.Picture|
&
    |expn (G.Picture) (*)|
          \ar[l]^---{|G.Pictures|}
&
    |expn (Origem ><((expn Float 2)><(Texto><G.Color)))|
          \ar[l]^-{|expn (parseCrCaixa) (*)|}
}
\end{eqnarray*}
\begin{code}

caixasAndOrigin2Pict = G.pictures . map (parseCrCaixa . (id >< caixaToFloat)) . agrup_caixas . calcOrigins where
   caixaToFloat ((x,y),(t, c)) = ((fromIntegral x, fromIntegral y),(t, c)) 
   parseCrCaixa (o,((x,y),(t, c))) = crCaixa o x y t c

mostra_caixas :: (L2D,Origem) -> IO()
mostra_caixas = display . caixasAndOrigin2Pict

\end{code}

\subsection*{Problema 3}
Solução:

\begin{code}
cos' x = prj . for loop init where
   init = (1, negate (x^2/2), 12, 18)
   loop (c, f, g, k) = (f + c, (negate ((x^2) / g)) * f, k + g, 8 + k)
   prj (c, f, g, k) = c
\end{code}

Solução em C:

\begin{spec}
double myCos(double x, int n){
  double c=1,g=12,k=18;double f=(-(pow(x,2)/2));int i;
  for ((i=0; i<n+1; i++)) {c+=f;f*=(-(pow(x,2)/g));g+=k;k+=8;}
  return c;
}
\end{spec}

Na minha opinião não é uma resposta fácil de obter apenas intuitivamente mas creio que após descobrir a possibilidade de o fazer,
torna-se um método relativamente simples de abordar problemas do género.
\subsection*{Problema 4}
Triologia ``ana-cata-hilo":
\begin{code}
outFS (FS l) = map (id >< outNode) l
outNode (File b) = i1 b
outNode (Dir d) = i2 d

baseFS f g h = map (f >< (g -|- h))

\end{code}
\begin{eqnarray*}
\xymatrix@@C=2cm{
    |FS A B|
           \ar[d]_-{|k = cataNat g|}
&
    |expn ((A >< (B + FS A B))) (*)|
           \ar[d]^{|expn ((id >< id + k)) (*)|}
           \ar[l]_-{|inFS|}
\\
     |C|
&
     |expn ((A >< (B + C))) (*)|
           \ar[l]^-{|g|}
}
\end{eqnarray*}
\begin{code}
cataFS :: ([(a, Either b c)] -> c) -> FS a b -> c
cataFS g = g . recFS (cataFS g) . outFS

anaFS :: (c -> [(a, Either b c)]) -> c -> FS a b
anaFS g = inFS . recFS (anaFS g) . g

hyloFS g h = cataFS g . anaFS h
\end{code}
Outras funções pedidas:
\begin{eqnarray*}
\xymatrix@@C=2cm{
    |FS A B|
           \ar[d]_-{|check = cataNat g|}
&
    |expn ((A >< (B + FS A B))) (*)|
           \ar[d]^{|expn ((id >< id + check)) (*)|}
           \ar[l]_-{|inFS|}
\\
     |Bool|
&
     |expn ((A >< (B + Bool))) (*)|
           \ar[l]^-{|g|}
}
\end{eqnarray*}
\begin{code}
check :: (Eq a) => FS a b -> Bool
check = cataFS $ (uncurry (&&) . nrPlusAllTrue . adapt) where
   adapt = map (id >< either (const True) id)

\end{code}
\begin{eqnarray*}
\xymatrix@@C=2cm{
    |expn ((A><Bool)) (*)|
           \ar[d]_-{|nrPlusAllTrue|}
&
    |1 + (A><Bool)><(expn ((A><Bool)) (*))|
           \ar[d]^{|expn ((id + id >< nrPlusAllTrue)) (*)|}
           \ar[l]_-{|inList|}
\\
     |expn (A) (*) >< (Bool >< Bool)|
&
     |1 + (A >< Bool) >< (expn (A) (*) >< (Bool >< Bool))|
           \ar[l]^-{|g = [init, loop]|}
}
\end{eqnarray*}
\begin{code}

nrPlusAllTrue :: (Eq a) => [(a,Bool)] -> (Bool, Bool)
nrPlusAllTrue = p2 . (cataList $ either init loop) where
   init = const ([], (True, True))
   loop = split (cons . (p1 >< p1)) (split h (uncurry (&&) . (p2 >< (p2 . p2))))
   h = uncurry (&&) . split (not . uncurry elem . (p1 >< p1)) (p1 . p2 . p2)

\end{code}
\begin{eqnarray*}
\xymatrix@@C=2cm{
    |FS A B|
           \ar[d]_-{|tar = cataNat g|}
&
    |expn ((A >< (B + FS A B))) (*)|
           \ar[d]^{|expn ((id >< id + tar)) (*)|}
           \ar[l]_-{|inFS|}
\\
     |expn ((Path A >< B)) (*)|
&
     |expn ((A >< (B + (expn ((Path A >< B)) (*))))) (*)|
           \ar[l]^-{|g|}
}
\end{eqnarray*}

\begin{eqnarray*}
\xymatrix@@C=2cm{
    |expn ((A >< (B + (expn ((Path A >< B)) (*))))) (*)|
           \ar[d]^-{|g|}
           \ar[r]^-{|expn (distr) (*)|}
&
    |expn ((A >< B + A >< (expn ((Path A >< B)) (*)))) (*)|
           \ar[d]^-{|expn ([singl . (singl >< id), uncurry insert]) (*)|}
\\
    |expn ((Path A >< B)) (*)|
&
    |expn (expn ((Path A >< B)) (*)) (*)|
           \ar[l]^-{|concat|}
}
\end{eqnarray*}
\begin{code}

tar :: FS a b -> [(Path a, b)]
tar = cataFS g where
   g = concat . map ((either (singl . (singl >< id)) (uncurry insert)) . distr)
   insert v = map ((:) v >< id)

\end{code}
\begin{eqnarray*}
\xymatrix@@C=1cm{
    |expn ((Path A >< B)) (*)|
           \ar[d]_-{|untar = anaNat untarAux|}
           \ar[r]_-{|untarAux|}
&
    |expn ((A >< (B + (expn ((Path A >< B)) (*))))) (*)|
           \ar[d]^{|expn ((id >< id + untar)) (*)|}
\\
     |FS A B|
&
     |expn ((A >< (B + FS A B))) (*)|
           \ar[l]^-{|inFS|}
}
\end{eqnarray*}
\begin{code}

untar :: (Eq a) => [(Path a, b)] -> FS a b
untar = joinDupDirs . anaFS superUntarAux

untarAux :: (Eq a) => [(Path a, b)] -> [(a, Either b ([(Path a, b)]))]
untarAux [] = []
untarAux (([], file):t) = untarAux t
untarAux (([x], file):t) = (x, i1 file) : untarAux t
untarAux l@((p:ps,file):t) = (p, i2 $ removeHeads a) : untarAux b where
   (a,b) = partition (uncurry (&&) . split ((>1) . length) ((==) p . head) . p1) l
   removeHeads = map $ (tail >< id)

superUntarAux :: (Eq a) => [(Path a, b)] -> [(a, Either b ([(Path a, b)]))]
superUntarAux = anaList $ cond ((== 0) . length) (i1 . (!)) f where
   f = cond ((>1) . length . p1 . head) h g
   g = cond ((==0) . length . p1 . head) undefined (i2 . (split ((head >< i1) . head) tail))
   h = i2 . split (id >< (i2 . removeHeads . p1)) (p2 . p2) . split storeP ((uncurry break) . split storeP id)
   break p = partition (uncurry (&&) . split ((>1) . length) ((==) p . head) . p1)
   removeHeads = map $ (tail >< id)
   storeP = head . p1 . head

\end{code}
\begin{eqnarray*}
\xymatrix@@C=2cm{
    |FS A B|
           \ar[d]_-{|find = cataNat g|}
&
    |expn ((A >< (B + FS A B))) (*)|
           \ar[d]^{|expn ((id >< id + find)) (*)|}
           \ar[l]_-{|inFS|}
\\
     |expn ((Path A)) (*)|
&
     |expn ((A >< (B + (expn ((Path A)) (*))))) (*)|
           \ar[l]^-{|g|}
}
\end{eqnarray*}
\begin{eqnarray*}
\xymatrix@@C=2cm{
    |expn ((A >< (B + (expn ((Path A)) (*))))) (*)|
           \ar[d]^-{|g|}
           \ar[r]^-{|expn (distr) (*)|}
&
    |expn ((A >< B + A >< (expn ((Path A)) (*)))) (*)|
           \ar[d]^-{|expn ([comparePaths, uncurry gatherPaths]) (*)|}
\\
    |expn ((Path A)) (*)|
&
    |expn (expn ((Path A)) (*)) (*)|
           \ar[l]^-{|concat|}
}
\end{eqnarray*}
\begin{code}

find :: (Eq a) => a -> FS a b -> [Path a]
find path = cataFS g where
   g = concat . map ((either comparePaths (uncurry gatherPaths)) . distr)
   comparePaths = singl . (cond ((==) path) singl nil) . p1
   gatherPaths stuff = map $ cond ((== 0) . length) id ((:) stuff)

\end{code}
\begin{eqnarray*}
\xymatrix@@C=1cm{
    |(Path A >< B)><FS A B|
           \ar[d]_-{|f = anaNat g|}
           \ar[r]_-{|g|}
&
    |expn ((A >< (B + ((Path A >< B)><FS A B)))) (*)|
           \ar[d]^{|expn ((id >< id + f)) (*)|}
\\
     |FS A B|
&
     |expn ((A >< (B + FS A B))) (*)|
           \ar[l]^-{|inFS|}
}
\end{eqnarray*}
\begin{code}

new :: (Eq a) => Path a -> b -> FS a b -> FS a b
new = curry $ curry $ f where
   f = anaFS (cond p (remodel . shapeFS) (shapeFS . split ((tail >< id) . p1) (concatFS . (createFSfromFile >< id))))
   remodel = map $ undistr . (id -|- func) . distr
   shapeFS = auxJoin . swap . (id >< outFS)
   p = uncurry (||) . split ((/= 1) . length . p1 . p1) ((/= 0) . length . uncurry filterPath . (p1 >< tar))
   func = cond ((==) 0 . length . p1 . p1 . p2) id (cond (uncurry (==) . (id >< (head . p1 . p1))) tailPath nilPath)
   tailPath = (id >< ((tail >< id) >< id))
   nilPath = (id >< ((nil >< id) >< id))
\end{code}

Caso se substitua a propriedade checkFiles pela propriedade check, a verificação |prop_new| deixa de ser válida. 
Isto deve-se, unicamente, ao facto de a propriedade checkFiles não verificar se existem ficheiros 
e diretorias com o mesmo nome, o que, por sinal,  é considerado um teste válido. 
No entanto, a propriedade check faz essa verificação pelo que quando tentamos inserir um ficheiro numa 
determinada diretoria onde se encontra uma outra diretoria com nome semelhante ao do ficheiro, a |prop_new| falhará.

\begin{code}
cp :: (Eq a) => Path a -> Path a -> FS a b -> FS a b
cp = curry $ curry $ cond p f p2 where
   applyF = uncurry filterPath . (p1 >< tar)
   p = (/=) 0 . length . applyF 
   f = (uncurry (uncurry new)) . split g p2
   g = split (p2 . p1) (p2 . head . applyF)


\end{code}
\begin{eqnarray*}
\xymatrix@@C=1cm{
    |Path A><FS A B|
           \ar[d]_-{|rm = curry (anaNat g|}
           \ar[r]_-{|g|}
&
    |expn ((A >< (B + (Path A><FS A B)))) (*)|
           \ar[d]^{|expn ((id >< id + rm)) (*)|}
\\
     |FS A B|
&
     |expn ((A >< (B + FS A B))) (*)|
           \ar[l]^-{|inFS|}
}
\end{eqnarray*}
\begin{code}

rm :: (Eq a) => Path a -> FS a b -> FS a b
rm = curry $ anaFS $ cond p (pattern id id) (cond ((/= 1) . length . p1) g k) where
   p = (== 0) . length . p1
   pattern f g = g . auxJoin . swap . f . (id >< outFS) 
   g = pattern id (map (undistr . (id -|- h) . distr))
   k = pattern (split (tail . p1) (uncurry filterEqual)) id
   h = cond (uncurry (==) . (id >< (head . p1))) (id >< (tail >< id)) (id >< (nil >< id))
   filterEqual (p:ps) = filter ((/= p) . p1)

\end{code}
\begin{eqnarray*}
\xymatrix@@C=1cm{
    |Path A><FS A B|
           \ar[d]_-{|nav = anaNat (p -> f,g)|}
           \ar[r]_-{|p -> f,g|}
&
    |expn ((A >< (B + (Path A><FS A B)))) (*)|
           \ar[d]^{|expn ((id >< id + f)) (*)|}
\\
     |FS A B|
&
     |expn ((A >< (B + FS A B))) (*)|
           \ar[l]^-{|inFS|}
}
\end{eqnarray*}
\begin{code}

nav :: (Eq a) => (Path a, FS a b) -> FS a b
nav = anaFS $ cond p f g where
   p = (== 0) . length . p1
   f = auxJoin . swap . (id >< outFS)
   g = map (undistr . (id -|- h) . distr) . auxJoin . swap . (id >< outFS)
   h = cond (uncurry (==) . (id >< (head . p1))) (id >< (tail >< id)) (id >< (nil >< id))

\end{code}
\begin{eqnarray*}
\xymatrix@@C=1cm{
    |expn ((A >< (B + C))) (*) >< D|
           \ar[d]_-{|auxJoin = anaNat g|}
           \ar[r]_-{|g|}
&
    |1 + (A >< (B + (D><C)))><(expn ((A >< (B + C))) (*) >< D)|
           \ar[d]^{|id + id >< auxJoin|}
\\
     |expn ((A >< (B + (D><C)))) (*)|
&
     |1 + ((A >< (B + (D><C)))><expn ((A >< (B + (D><C)))) (*))|
           \ar[l]^-{|inList|}
}
\end{eqnarray*}
\begin{code}

auxJoin :: ([(a, Either b c)],d) -> [(a, Either b (d,c))]
auxJoin = anaList ((id -|- (aux >< id)) . (p1 -|- toPair) . distl . (outList >< id)) where
   aux = (split (p1 . p1) ((p1 -|- swap) . distl . (p2 >< id)))
   toPair = split (p1 >< id) (p2 >< id)

auxJoinCata :: ([(a, Either b c)],d) -> [(a, Either b (d,c))]
auxJoinCata = uncurry aux . swap
   where aux d = map $ g d
         g d = split p1 ((p1 -|- swap) . distl . split p2 (const d))

\end{code}
\begin{eqnarray*}
\xymatrix@@C=1cm{
    |A><FS A B|
           \ar[d]_-{|cFS2Exp = anaNat aux|}
           \ar[r]_-{|aux|}
&
    |1 + A >< expn ((A><FS A B)) (*)|
           \ar[d]^{|(id + id >< cFS2Exp)|}
\\
     |Exp 1 A|
&
     |1 + A >< expn ((Exp 1 A)) (*)|
           \ar[l]^-{|inExp|}
}
\end{eqnarray*}
\begin{code}

cFS2Exp :: a -> FS a b -> Exp () a
cFS2Exp = curry aux where
   aux = anaExp $ i2 . (id >< (map ((either (id >< (const $ FS [])) id) . distr) . outFS))
\end{code}

%----------------- Fim do anexo com soluções dos alunos ------------------------%

%----------------- Índice remissivo (exige makeindex) -------------------------%

\printindex

%----------------- Bibliografia (exige bibtex) --------------------------------%

\bibliographystyle{plain}
\bibliography{cp1819t}

%----------------- Fim do documento -------------------------------------------%
\end{document}
