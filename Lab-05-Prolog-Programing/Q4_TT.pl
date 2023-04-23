intersect([], _, []).
intersect([X|R], Y, [X|Z]) :- member(X, Y), !, intersect(R, Y, Z).
intersect([_|R], Y, Z) :- intersect(R, Y, Z).
