zeros([], 0).
zeros([0 | T], Z) :- zeros(T, Z1), Z is Z1 + 1, !.
zeros([_ | T], Z) :- zeros(T, Z).
