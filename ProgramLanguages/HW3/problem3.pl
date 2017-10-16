eightQueens(  )  :-
                               permuteEight( X ),
                               permuteEight( Y ),
                               checkSets( X, Y ).


permuteEight( List ) :- findall(X, permutation([1,2,3,4,5,6,7,8], X), List).

checkSets([], []).
checkSets([A,B,C,D,E,F,G,H | RestX], [A2,B2,C2,D2,E2,F2,G2,H2 | RestY] ) :-
  checkSet( [A,B,C,D,E,F,G,H], [A2, B2, C2, D2, E2, F2, G2, H2]),
  checkSets( RestX, RestY).


checkSet([], []).
checkSet([X | RestX], [Y | RestY]) :-
    checkSet(X, Y, RestX, RestY),
    checkSet(RestX, RestY).

checkSet(X, Y, [X2 | RestX], [Y2 | RestY]) :-
    Y2 - Y \== X2 - X,
    Y2 - Y \== X - X2,
    checkSet( X,Y, RestX, RestY).


%testUnique([X]).
%testUnique( [X|Y] ) :- \+member(X,Y), testUnique(Y).



%safeZone(_, []).
%safeZone( [ X ], [ Y ] ) :-       X \== X2,
%                                  Y \== Y2,
%                                  Y2 - Y \== X2 - X,
%                                  Y2- Y \== X - X2,
%                                  safeZone(X/Y, Rest).

%member(Item, [Item | T]).
%member(Item, [H | T]) :- member(Item, T).

%template([1/Y, 2/Y2, 3/Y3, 4/Y4, 5/Y5, 6/Y6, 7/Y7, 8/Y8]).
