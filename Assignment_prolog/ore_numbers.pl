isOreNum(HM):-
	Ceil is ceiling(HM),
	Diff is HM - Ceil,
	AbsDiff is abs(Diff),
	AbsDiff =< 0.0000001.
isOreNum(HM):-
	Floor is floor(HM),
	Diff is HM - Floor,
	AbsDiff is abs(Diff),
	AbsDiff =< 0.0000001.


harmonicMean(X, N, InvSum, Count, HM):-
	N = 1, !,
	InvSum1 is InvSum + 1 / N + 1 / X,
	Count1 is Count + 2,
	HM is Count1 / InvSum1.
harmonicMean(X, N, InvSum, Count, HM) :-
	Z is X mod N,
	Z \= 0, !,
	N1 is N-1,
	harmonicMean(X, N1, InvSum, Count, HM).	
harmonicMean(X, N, InvSum, Count, HM) :-
	Z is X mod N,
	Z == 0, !,
	N1 is N-1,
	Count1 is Count + 2,
	Fraction1 is 1 / N,
	InvSum1 is InvSum + Fraction1,
	N2 is X / N,
	Fraction2 is 1 / N2,
	InvSum2 is InvSum1 + Fraction2,
	harmonicMean(X, N1, InvSum2, Count1, HM).

initSumCount(A, Asq, InvSum, Count) :-
	Asqrt is sqrt(A),
	Asqfloor is floor(Asqrt),
	Z is Asqfloor * Asqfloor,
	Z == A, !,
	Asq is Asqfloor - 1,
	InvSum is 1 / Asqfloor,
	Count is 1.
initSumCount(A, Asq, InvSum, Count) :-
	Asqrt is sqrt(A),
	Asqfloor is floor(Asqrt),
	Asq is Asqfloor,
	InvSum = 0,
	Count = 0.

oreNums(A, B, _) :-
	A >= B, !.
oreNums(1, B, Step) :-
	thread_self(TID),
	write("Thread "),
	write(TID),
	write(" found num "),
	writeln(1),
	A1 is 1 + Step,
	oreNums(A1, B, Step).
oreNums(A, B, Step) :-
	initSumCount(A, Asq, InvSum, Count),
	harmonicMean(A, Asq, InvSum, Count, HM),
	isOreNum(HM), !,
	thread_self(TID),
	write("Thread "),
	write(TID),
	write(" found num "),
	writeln(A),
	A1 is A + Step,
	oreNums(A1, B, Step).
oreNums(A, B, Step) :-
	A1 is A + Step,
	oreNums(A1, B, Step).

oreThreads(A, B, NrThreads) :-
	Step = NrThreads,
	oreThreadCreator(A, B, NrThreads, NrThreads, Step, []).

oreThreadCreator(_, _, _, 0, _, TList) :-
	oreThreadJoiner(TList).
oreThreadCreator(A, B, NrThreads, N, Step, TList) :-
	A1 is A + N - 1,
	thread_create(oreNums(A1, B, Step), Tid, []),
	N1 is N - 1,
	oreThreadCreator(A, B, NrThreads, N1, Step, [Tid|TList]).

oreThreadJoiner([]).
oreThreadJoiner([HTh|TTh]) :-
	thread_join(HTh),
	oreThreadJoiner(TTh).
