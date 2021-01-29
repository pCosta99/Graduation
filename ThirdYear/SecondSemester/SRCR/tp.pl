%--------------------------------- - - - - - - - - - -  -  -  -  -   -
% SIST. REPR. CONHECIMENTO E RACIOCINIO - MiEI/3

%--------------------------------- - - - - - - - - - -  -  -  -  -   -
% Programacao em logica 
% Trabalho prático 2

%--------------------------------- - - - - - - - - - -  -  -  -  -   -
% SICStus PROLOG: Declaracoes iniciais

:- set_prolog_flag( discontiguous_warnings,off ).
:- set_prolog_flag( single_var_warnings,off ).
:- set_prolog_flag( unknown,fail ).
:- set_prolog_flag(toplevel_print_options,
    [quoted(true), portrayed(true), max_depth(0)]).
:- consult(knowledge).

%--------------------------------- - - - - - - - - - -  -  -  -  -   -
%Trajeto entre dois pontos

% Path as depth first search
dfs(Origin,Destiny,[Origin|Path]):-
	dfs(Origin, Destiny, [Origin], Path).

dfs(Destiny, Destiny,_, []) :- !.

dfs(Origin, Destiny, Visited, [(Next,Op,C)|Path]):-
	connectedBiDir(Origin, Next, C, _),
	stop(Next,_,_,_,_,_,Op,_,_,_,_),
	\+memberchk(Next, Visited),
	dfs(Next, Destiny, [Next|Visited], Path).   

% Depth first but no extra info
dfsClean(Origin,Destiny,[Origin|Path]):-
	dfsClean(Origin, Destiny, [Origin], Path).

dfsClean(Destiny, Destiny,_, []) :- !.

dfsClean(Origin, Destiny, Visited, [Next|Path]):-
	connectedBiDir(Origin, Next, C, _),
	\+memberchk(Next, Visited),
	dfsClean(Next, Destiny, [Next|Visited], Path).  									  					 

% Depth first search with distance
dfsWithDist(Origin,Destiny,Distance,[Origin|Path]):-
	dfsWithDist(Origin, Destiny, [Origin], Distance, Path).

dfsWithDist(Destiny, Destiny,_, 0, []) :- !.

dfsWithDist(Origin, Destiny, Visited, Distance, [Next|Path]):-
	connectedBiDir(Origin, Next, _, D0),
	\+memberchk(Next, Visited),
	dfsWithDist(Next, Destiny, [Next|Visited], D1, Path),
	Distance is D0 + D1.

% Path as breadth first search : Test with 183 - 766
bfs(Origin, Destiny, Visited) :-
	bfs_aux([Origin], [], RevVisited, Destiny),
	remove_not_connected(RevVisited, Visited).
bfs_aux([Destiny|_], History, [Destiny|History], Destiny).
bfs_aux([Node|RestQ], History, RevVisited, Destiny) :-
	findall(NextNode, (connectedBiDir(Node,NextNode,_,_), \+ member(NextNode, History), \+ member(NextNode, RestQ)), Successors),
	append(RestQ, Successors, Queue),
	bfs_aux(Queue, [Node|History], RevVisited, Destiny).    

%Selecionar apenas algumas das operadoras de transporte para um determinado percurso
pathOps(Origin,Destiny,Ops,Path) :- 
	pathOpsAux(Origin,[Destiny],Ops,Path).
pathOpsAux(Origin,[Origin|P1],_,[Origin|P1]).
pathOpsAux(Origin,[Y|P1],Ops,Path) :- 	
	connectedBiDir(X,Y,C,_),
	stop(X,_,_,_,_,_,Operator,_,_,_,_),
	stop(Y,_,_,_,_,_,OperatorY,_,_,_,_),
	member(Operator,Ops),
	member(OperatorY,Ops),
	\+ memberchk(X,[Y|P1]), 
	pathOpsAux(Origin,[X,(Y,OperatorY,C)|P1],Ops,Path).	

%Excluir um ou mais operadores de transporte para o percurso							   												   						
pathWithoutOperators(Origin,Destiny, Operators, Path) :- 
	pathWithoutOperatorsAux(Origin,[Destiny],Operators,Path).
pathWithoutOperatorsAux(Origin,[Origin|P1],_,[Origin|P1]).
pathWithoutOperatorsAux(Origin,[Y|P1],Operators,Path) :- 
	connectedBiDir(X,Y,C,_),
	stop(X,_,_,_,_,_,Operator,_,_,_,_),
	\+ member(Operator,Operators),
	\+ memberchk(X,[Y|P1]), 
	pathWithoutOperatorsAux(Origin,[X,Y,(Operator,C)|P1],Operators,Path).		

%Identificar quais as paragens com o maior número de carreiras num determinado percurso
most_tracks(Path, NTracks) :- 
	most_tracks_aux(Path, [], AccNTracks), 
	biggest_pairs(AccNTracks, NTracks).
most_tracks_aux([],[P|T1],[P|T1]).
most_tracks_aux([P|T1], Acc, NTracks) :- 
	stop(P,_,_,_,_,_,_,Tracks,_,_,_),
	length(Tracks, L),						  	   	   
	most_tracks_aux(T1,[(P,L)|Acc],NTracks).				   	 

biggest_pairs(L,R) :- maxList(L,K,R).

maxList([],(_,0),L).
maxList([(P1,P2)|Tail],(M1,M2),[(M1,M2)]) :- 
	maxList(Tail,(TM1,TM2),_), 
	P2 > TM2, 
	M1 is P1, 
	M2 is P2.
maxList([(P1,P2)|Tail],(M1,M2),L) :- 
	maxList(Tail,(TM1,TM2),L), 
	P2 < TM2, 
	M1 is TM1, 
	M2 is TM2.
maxList([(P1,P2)|Tail],(M1,M2),[H,(P1,P2)|L]) :- 
	maxList(Tail,(TM1,TM2),[H|L]), 
	P2 == TM2,
	M1 is TM1,
	M2 is TM2.

%Escolher o menor percurso (usando critério menor número de paragens)
shortest_in_stops(Origin,Destiny,Path) :- bfs(Origin, Destiny, Path),
	  								    \+ (bfs(Origin, Destiny, Path2),
	  								    	  length(Path, L1),
	  								    	  length(Path2, L2),
	  								    	  Path2 \= Path,
	  								     	  L2 =< L1).

%Escolher o percurso mais rápido (usando critério da distância)
fastest(Origin,Destiny,MinDist,Path) :- 
	dfsWithDist(Origin, Destiny, MinDist, Path),
	\+ (dfsWithDist(Origin, Destiny, LowerDist, Path2),
	Path2 \= Path,
	LowerDist =< MinDist).

%Escolher o percurso que passe apenas por abrigos com publicidade
withAdvertisingOnlyPath(Origin,Destiny, Path) :- 	
	withAdvertisingOnlyPathAux(Origin,[Destiny],Path).
withAdvertisingOnlyPathAux(Origin,[Origin|P1],[Origin|P1]).
withAdvertisingOnlyPathAux(Origin,[Y|P1],Path) :- 	
	connectedBiDir(X,Y,C,_),
 	stop(X,_,_,_,_,true,Operator,_,_,_,_),
	\+ memberchk(X,[Y|P1]), 
 	withAdvertisingOnlyPathAux(Origin,[X,Y,(Operator,C)|P1],Path).		

%Escolher o percurso que passe apenas por paragens abrigadas
shelteredPath(Origin,Destiny, Path) :- 	
	shelteredPathAux(Origin,[Destiny],Path).
shelteredPathAux(Origin,[Origin|P1],[Origin|P1]).
shelteredPathAux(Origin,[Y|P1],Path) :-
	connectedBiDir(X,Y,C,_),
	stop(X,_,_,_,'Fechado dos Lados',_,Operator,_,_,_,_),
	\+ memberchk(X,[Y|P1]), 
	shelteredPathAux(Origin,[X,Y,(Operator,C)|P1],Path).		

%Escolher um ou mais pontos intermédios por onde o percurso deverá passar
delete_1(Head, [Head|Tail], Tail).
delete_1(Item, [Head|Tail],[Head|New_Tail]) :- 
	delete_1(Item, Tail, New_Tail). 

withIntermediate(Origin,Destiny, Intermediate, Path) :- 
	withIntermediateAux(Origin,[Destiny],Intermediate,Path).
withIntermediateAux(Origin,[Origin|P1],[],[Origin|P1]).
withIntermediateAux(Origin,[Y|P1],Intermediate,Path) :- 
	connectedBiDir(X,Y,C,_),
	stop(X,_,_,_,_,_,Operator,_,_,_,_),
	member(X,Intermediate),
	delete_1(X,Intermediate, New),
	\+ memberchk(X,[Y|P1]), 
	withIntermediateAux(Origin,[X,Y,(Operator,C)|P1],New,Path).		
withIntermediateAux(Origin,[Y|P1],Intermediate,Path) :- 
	connectedBiDir(X,Y,C,_),
	stop(X,_,_,_,_,_,Operator,_,_,_,_),
	\+member(X,Intermediate),
	\+ memberchk(X,[Y|P1]), 
	withIntermediateAux(Origin,[X,Y,(Operator,C)|P1],Intermediate,Path).	

% Pesquisa informada para o caminho mais lento
greedy(Origin, Destiny, Visited) :- 
	assert(goal(Destiny)),
	greedy3([Origin], [], RevVisited),
	remove_not_connected(RevVisited, Visited),
	retract(goal(Destiny)).

greedy3([Node|RestQ], History, RevVisited) :-
	\+ goal(Node),
	findall(NextNode, (connectedBiDir(Node, NextNode, _, _), \+ member(NextNode, History), \+ member(NextNode, RestQ)),	Successors),
	addPriorityQ(RestQ, Successors, PriorityQ),
	greedy3(PriorityQ, [Node | History], RevVisited).
greedy3([Node|_], History, [Node | History]).

addPriorityQ(L1, Q1, Q2) :- 
	append(L1, Q1, L2),
	bubblesort(L2, Q2).

heuristic(Node1, Node2) :-
	goal(Destiny),
	estima(Node1, Destiny, D1),
	estima(Node2, Destiny, D2),
	D1 < D2.

bubblesort(List, Sorted):-
	swap(List, List1), !,
	bubblesort(List1,Sorted).
bubblesort(Sorted, Sorted).

swap([X, Y | Rest], [Y, X | Rest]):-
	heuristic(X,Y).
swap([Z | Rest],[Z | Rest1]):-
	swap(Rest, Rest1).

%------------------------------------------------------------------------------------------- 
% Helper functions

% Reverse a list
reverse(L,R) :- reverse_aux(L,R,[]).
reverse_aux([],Z,Z).
reverse_aux([H|T],Z,Acc) :- reverse_aux(T,Z,[H|Acc]).

% Estimates the distance between two nodes -------------------------------------------------------
estima(Nodo,Destino,Dist) :- 
	stop(Nodo,LatX,LonX,_,_,_,_,_,_,_,_),
	stop(Destino,LatY,LonY,_,_,_,_,_,_,_,_),
	Lat is (LatX - LatY) * (LatX - LatY), Lon is (LonX - LonY) * (LonX - LonY), Dist is sqrt(Lat+Lon).

%---------------------------------------------------------------------------
cena_estranha([], Acc, Acc).
cena_estranha([Elem], Acc, Result) :- cena_estranha([], [Elem|Acc], Result).
cena_estranha([Node1,Node2|Rest], Acc, Result) :- 
	\+ connectedBiDir(Node1,Node2,_,_),
	cena_estranha([Node1|Rest], Acc, Result).
cena_estranha([Node1,Node2|Rest], Acc, Result) :-
	connectedBiDir(Node1,Node2,_,_),
	cena_estranha([Node2|Rest], [Node1|Acc], Result).

remove_not_connected(List, Result) :- cena_estranha(List, [], Result).

% Calculates the distance traveled in a certain path --------------------------------
distance([Elem],0).
distance([Node,Next|Path], D) :- 
	estima(Node,Next,Dist),	
	distance([Next|Path],D0),
	D is Dist + D0.

% Checks if a connection happens in either direction
connectedBiDir(Node,Next,C,Dist) :- connected(Node,Next,C,Dist).
connectedBiDir(Node,Next,C,Dist) :- connected(Next,Node,C,Dist).	