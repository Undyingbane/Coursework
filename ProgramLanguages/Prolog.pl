%Definitions
m([eclipse, atom, euler, waring, jack, jimmy, chad]).
f([java, ruby, curie, sql, jill]).
family([eclipse, java, [ruby]).
family([euler, curie, [jill, jack]]).
family([waring, sql, jimmy, chad]).
%Rules
male(X) :- m(M), member(X,M).
female(X) :- f(F), member(X,F).
