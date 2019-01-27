/** Kabuki Toolkit @version 0.x
@file    /kabuki/f2sdk/ai/include/agent.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#ifndef KABUKI_AIML_AGENT_H
#define KABUKI_AIML_AGENT_H

//#include "module_config.h"

namespace kabuki { namespace ai {

// The comments below have code taken from the book Artificial Intelligence
// (3rd Edition) and we're porting the code into our toolkit.

// # Section 1

// ## Chapter 2 - Intelligent Agents

/** Invoked for each new percept and returns an action each time.
    Figure 2.7 The TABLE-DRIVEN-AGENT program is invoked for each new percept and returns an action each time. It retains the complete percept sequence in memory.

    Figure 2.2 A vacuum-cleaner world with just two locations.
    Percept sequence Action
    [A, Clean] Right
    [A, Dirty] Suck
    [B, Clean] Left
    [B, Dirty] Suck
    [A, Clean], [A, Clean] Right
    [A, Clean], [A, Dirty] Suck
    ...
    ...
    [A, Clean], [A, Clean], [A, Clean] Right
    [A, Clean], [A, Clean], [A, Dirty] Suck
    ...
    ...

    Figure 2.3 Partial tabulation of a simple agent function for the vacuum-cleaner world
    shown in Figure 2.2.

    @group TableDrivenAgent

    @return An Action
    @const percepts A sequence, initially empty
    @const table, a table of actions, indexed by percept sequences, initially fully specified.

    @code
    append percept to the end of percepts
    action <- LOOKUP (percepts, table)
    return action
    @endcode
*/

/** Figure 2.8 The agent program for a simple reflex agent in the two-state vacuum environment. This program implements the agent function tabulated in Figure 2.3.
    function REFLEX-VACUUM-AGENT([location,status]) returns an action

    @code
    if (status == Dirty then return Suck
    else if location = A then return Right
    else if location = B then return Left
    @endcode
*/

/** Figure 2.12 A model-based reflex agent. It keeps track of the current state of the world, using an internal model. It then chooses an action in the same way as the reflex agent.
    function SIMPLE-REFLEX-AGENT(percept ) returns an action
persistent: rules, a set of condition–action rules
    @code
    state←INTERPRET-INPUT(percept )
    rule←RULE-MATCH(state, rules)
    action ←rule.ACTION
    return action
    @endcode
*/

// 3. Problem-Solving Agents

/**

    @code
    @endcode
*/

/**
    Figure 3.1 A simple problem-solving agent. It first formulates a goal and a problem, searches for a sequence of actions that would solve the problem, and then executes the actions one at a time. When this is complete, it formulates another goal and starts over.
    @code
    function SIMPLE-PROBLEM-SOLVING-AGENT(percept ) returns an action
    @const seq, an action sequence, initially empty
    @const state, some description of the current world state
    @const goal , a goal, initially null
    @const problem, a problem formulation

    state←UPDATE-STATE(state, percept )
    if seq is empty then
        goal ←FORMULATE-GOAL(state)
        problem ←FORMULATE-PROBLEM(state, goal )
        seq ←SEARCH(problem)
        if seq = failure then return a null action
    action ←FIRST(seq)
    seq ←REST(seq)
    return action
    @endcode
*/


/**
    Figure 3.7 An informal description of the general tree-search and graph-search algorithms. The parts of GRAPH-SEARCH marked in bold italic are the additions needed to handle repeated states.
    @code
    function TREE-SEARCH(problem) returns a solution, or failure
    initialize the frontier using the initial state of problem

    loop do
    if the frontier is empty then return failure
        choose a leaf node and remove it from the frontier
        if the node contains a goal state then return the corresponding solution
        expand the chosen node, adding the resulting nodes to the frontier

    function GRAPH-SEARCH(problem) returns a solution, or failure
    initialize the frontier using the initial state of problem
    initialize the explored set to be empty

    loop do
        if the frontier is empty then return failure
        choose a leaf node and remove it from the frontier
        if the node contains a goal state then return the corresponding solution
        add the node to the explored set
        expand the chosen node, adding the resulting nodes to the frontier
            only if not in the frontier or explored set
    @endcode
*/

/**

    @code
    function CHILD-NODE(problem, parent , action) returns a node
    return a node with
        STATE = problem.RESULT(parent.STATE, action),
        PARENT = parent, ACTION = action,
        PATH-COST = parent.PATH-COST + problem.STEP-COST(parent.STATE, action)
    @endcode
*/

/** Figure 3.11 Breadth-first search on a graph.
    @code
    function BREADTH-FIRST-SEARCH(problem) returns a solution, or failure

    node ←a node with STATE = problem.INITIAL-STATE, PATH-COST = 0

    if problem.GOAL-TEST(node.STATE) then return SOLUTION(node)
    frontier ←a FIFO queue with node as the only element
    explored ←an empty set

    loop do
        if EMPTY?( frontier) then return failure
        node←POP( frontier ) // chooses the shallowest node in frontier
        add node.STATE to explored
        for each action in problem.ACTIONS(node.STATE) do
            child ←CHILD-NODE(problem, node, action)
            if child .STATE is not in explored or frontier then
                if problem.GOAL-TEST(child .STATE) then return SOLUTION(child )
                frontier ←INSERT(child , frontier )
    @endcode
*/

/**

    @code
    function UNIFORM-COST-SEARCH(problem) returns a solution, or failure

    node ←a node with STATE = problem.INITIAL-STATE, PATH-COST = 0
    frontier ←a priority queue ordered by PATH-COST, with node as the only element
    explored ←an empty set

    loop do
        if EMPTY?( frontier) then return failure
        node←POP( frontier ) // chooses the lowest-cost node in frontier
        if problem.GOAL-TEST(node.STATE) then return SOLUTION(node)
        add node.STATE to explored
        for each action in problem.ACTIONS(node.STATE) do
            child ←CHILD-NODE(problem, node, action)
            if child .STATE is not in explored or frontier then
                frontier ←INSERT(child , frontier )
            else if child .STATE is in frontier with higher PATH-COST then
                replace that frontier node with child
    @endcode
*/

/** Figure 3.17 A recursive implementation of depth-limited tree search.

    @code
    function DEPTH-LIMITED-SEARCH(problem, limit ) returns a solution, or failure/cutoff
        return RECURSIVE-DLS(MAKE-NODE(problem.INITIAL-STATE), problem, limit )

    function RECURSIVE-DLS(node, problem, limit ) returns a solution, or failure/cutoff
        if problem.GOAL-TEST(node.STATE) then return SOLUTION(node)
        else if limit = 0 then return cutoff
        else
            cutoff occurred?←false
            for each action in problem.ACTIONS(node.STATE) do
                child ←CHILD-NODE(problem, node, action)
                result ←RECURSIVE-DLS(child , problem, limit − 1)
                if result = cutoff then cutoff occurred?←true
                else if result = failure then return result
            if cutoff occurred? then return cutoff else return failure
    @endcode
*/

/** Figure 3.18 The iterative deepening search algorithm, which repeatedly applies depth-limited search with increasing limits. It terminates when a solution is found or if the depth-limited search returns failure, meaning that no solution exists.

    @code
    function ITERATIVE-DEEPENING-SEARCH(problem) returns a solution, or failure
    for depth = 0 to ∞ do
        result ←DEPTH-LIMITED-SEARCH(problem, depth)
        if result ~= cutoff then return result
    @endcode
*/

/** Figure 3.26 The algorithm for recursive best-first search.

    @code

    function RECURSIVE-BEST-FIRST-SEARCH(problem) returns a solution, or failure
        return RBFS(problem,MAKE-NODE(problem.INITIAL-STATE),∞)

    function RBFS(problem, node, f limit ) returns a solution, or failure and a new f-cost limit
        if problem.GOAL-TEST(node.STATE) then return SOLUTION(node)
        successors ←[ ]
        for each action in problem.ACTIONS(node.STATE) do
        add CHILD-NODE(problem, node, action) into successors
        if successors is empty then return failure,∞
        for each s in successors do // update f with value from previous search, if any
        s.f ←max(s.g + s.h, node.f ))
        loop do
        best ←the lowest f-value node in successors
        if best .f > f limit then return failure, best .f
        alternative ←the second-lowest f-value among successors
        result , best .f ←RBFS(problem, best , min( f limit, alternative))
        if result = failure then return result
    @endcode
*/

// 4. Beyond Classical Search

/** Figure 4.5 The simulated annealing algorithm, a version of stochastic hill climbing where some downhill moves are allowed. Downhill moves are accepted readily early in the annealing schedule and then less often as time goes on. The schedule input determines the value of the temperature T as a function of time.

    @code
    function SIMULATED-ANNEALING(problem, schedule) returns a solution state
    inputs: problem, a problem
    schedule , a mapping from time to “temperature”
    current ←MAKE-NODE(problem.INITIAL-STATE)
    for t = 1 to∞do
    T ←schedule(t )
    if T = 0 then return current
    next←a randomly selected successor of current
    ΔE ←next.VALUE – current.VALUE
    if ΔE > 0 then current ←next
    else current ←next only with probability e
    ΔE/T
    @endcode
*/

/** Figure 4.8 A genetic algorithm. The algorithm is the same as the one diagrammed in Figure 4.6, with one variation: in this more popular version, each mating of two parents produces only one offspring, not two.

    @code
    function GENETIC-ALGORITHM(population, FITNESS-FN) returns an individual
    inputs: population, a set of individuals
        FITNESS-FN, a function that measures the fitness of an individual

    repeat
        new population ←empty set
        for i = 1 to SIZE(population) do
            x ←RANDOM-SELECTION(population, FITNESS-FN)
            y ←RANDOM-SELECTION(population, FITNESS-FN)
            child ←REPRODUCE(x , y)
            if (small random probability) then child ←MUTATE(child )
            add child to new population
            population ←new population
    until some individual is fit enough, or enough time has elapsed
    return the best individual in population, according to FITNESS-FN

    function REPRODUCE(x , y) returns an individual
        inputs: x , y, parent individuals

        n←LENGTH(x ); c←random number from 1 to n
        return APPEND(SUBSTRING(x, 1, c), SUBSTRING(y, c + 1, n))
    @endcode
*/

/** Figure 4.11 An algorithm for searching AND–OR graphs generated by nondeterministic environments. It returns a conditional plan that reaches a goal state in all circumstances. (The notation [x | l] refers to the list formed by adding object x to the front of list l.)

    @code
    function AND-OR-GRAPH-SEARCH(problem) returns a conditional plan, or failure
        OR-SEARCH(problem.INITIAL-STATE, problem, [ ])

    function OR-SEARCH(state, problem, path) returns a conditional plan, or failure
        if problem.GOAL-TEST(state) then return the empty plan
        if state is on path then return failure
        for each action in problem.ACTIONS(state) do
            plan ←AND-SEARCH(RESULTS(state, action), problem, [state | path])
            if plan = failure then return [action | plan]
        return failure

    function AND-SEARCH(states, problem, path) returns a conditional plan, or failure
    for each si in states do
        plani←OR-SEARCH(si, problem, path)
        if plani = failure then return failure
    return [if s1 then plan1 else if s2 then plan2 else . . . if sn−1 then plann−1 else plann]
    @endcode
*/

/** Figure 4.21 An online search agent that uses depth-first exploration. The agent is applicable only in state spaces in which every action can be “undone” by some other action.

    @code
    function ONLINE-DFS-AGENT(s) returns an action
    inputs: s, a percept that identifies the current state
    persistent: result , a table indexed by state and action, initially empty
            untried, a table that lists, for each state, the actions not yet tried
            unbacktracked, a table that lists, for each state, the backtracks not yet tried
            s, a, the previous state and action, initially null

    if GOAL-TEST(s) then return stop
    if s is a newstate (not in untried) then untried[s]←ACTIONS(s)
    if s is not null then
        result [s, a]←s
        add s to the front of unbacktracked[s]
    if untried[s] is empty then
        if unbacktracked[s] is empty then return stop
        else a←an action b such that result [s, b] = POP(unbacktracked[s])
    else a←POP(untried[s])
    s ←s
    return a
    @endcode
*/

/** Figure 4.24 LRTA*-AGENT selects an action according to the values of neighboring states, which are updated as the agent moves about the state space.

    @code
    function LRTA*-AGENT(s) returns an action
    inputs: s, a percept that identifies the current state
    persistent: result , a table, indexed by state and action, initially empty
            H, a table of cost estimates indexed by state, initially empty
            s, a, the previous state and action, initially null

    if GOAL-TEST(s) then return stop
    if s is a new state (not in H) then H[s]←h(s)
    if s is not null
        result [s, a]←s
        H[s]← min
            b ∈ACTIONS(s)
    LRTA*-COST(s, b, result [s, b],H)
    a←an action b in ACTIONS(s) that minimizes LRTA*-COST(s, b, result [s, b],H)
    s ←s
    return a

    function LRTA*-COST(s, a, s,H) returns a cost estimate
        if s is undefined then return h(s)
        else return c(s, a, s) + H[s]
    @endcode
*/

// 5. Adversarial Search

/** Figure 5.3 An algorithm for calculating minimax decisions. It returns the action corresponding to the best possible move, that is, the move that leads to the outcome with the best utility, under the assumption that the opponent plays to minimize utility. The functions MAX-VALUE and MIN-VALUE go through the whole game tree, all the way to the leaves, to determine the backed-up value of a state. The notation argmaxa∈ S f(a) computes the element a of set S that has the maximum value of f(a).

    @code
    function MINIMAX-DECISION(state) returns an action
    return argmax
    a ∈ ACTIONS(s) MIN-VALUE(RESULT(state, a))

    function MAX-VALUE(state) returns a utility value
    if TERMINAL-TEST(state) then return UTILITY(state)
    v ←−∞
    for each a in ACTIONS(state) do
        v ←MAX(v, MIN-VALUE(RESULT(s, a)))
    return v

    function MIN-VALUE(state) returns a utility value
    if TERMINAL-TEST(state) then return UTILITY(state)
    v←∞
    for each a in ACTIONS(state) do
        v ←MIN(v, MAX-VALUE(RESULT(s, a)))
    return v

    @endcode
*/

/** Figure 5.7 The alpha–beta search algorithm. Notice that these routines are the same as the MINIMAX functions in Figure 5.3, except for the two lines in each of MIN-VALUE and MAX-VALUE that maintain α and β (and the bookkeeping to pass these parameters along).

    @code
    function ALPHA-BETA-SEARCH(state) returns an action
    v ←MAX-VALUE(state,−∞,+∞)
    return the action in ACTIONS(state) with value v

    function MAX-VALUE(state,α, β) returns a utility value
    if TERMINAL-TEST(state) then return UTILITY(state)
    v ←−∞
    for each a in ACTIONS(state) do
        v ←MAX(v, MIN-VALUE(RESULT(s,a),α, β))
        if v ≥ β then return v
        α←MAX(α, v)
    return v

    function MIN-VALUE(state,α, β) returns a utility value
    if TERMINAL-TEST(state) then return UTILITY(state)
    v ←+∞
    for each a in ACTIONS(state) do
        v ←MIN(v, MAX-VALUE(RESULT(s,a) ,α, β))
        if v ≤ α then return v
        β←MIN(β, v)
    return v
    @endcode
*/

// 6. Constraint Satisfaction Problems

/** Figure 6.3 The arc-consistency algorithm AC-3. After applying AC-3, either every arc is arc-consistent, or some variable has an empty domain, indicating that the CSP cannot be solved. The name “AC-3” was used by the algorithm’s inventor (Mackworth, 1977) because it’s the third version developed in the paper.

    @code
    function AC-3(csp) returns false if an inconsistency is found and true otherwise
    inputs: csp, a binary CSP with components (X, D, C)
    local variables: queue, a queue of arcs, initially all the arcs in csp

    while queue is not empty do
        (Xi, Xj)←REMOVE-FIRST(queue)
        if REVISE(csp, Xi, Xj ) then
            if size of Di = 0then return false
            for each Xk in Xi.NEIGHBORS - {Xj} do
                add (Xk, Xi) to queue
    return true

    function REVISE(csp, Xi, Xj ) returns true iff we revise the domain of Xi

    revised ←false
    for each x in Di do
    if no value y in Dj allows (x ,y) to satisfy the constraint between Xi and Xj then
    delete x from Di
    revised ←true
    return revised
    @endcode
*/

/** Figure 6.5 A simple backtracking algorithm for constraint satisfaction problems. The algorithm is modeled on the recursive depth-first search of Chapter 3. By varying the functions SELECT-UNASSIGNED-VARIABLE and ORDER-DOMAIN-VALUES, we can implement the general-purpose heuristics discussed in the text. The function INFERENCE can optionally be used to impose arc-, path-, or k-consistency, as desired. If a value choice leads to failure (noticed either by INFERENCE or by BACKTRACK), then value assignments (including those made by INFERENCE) are removed from the current assignment and a new value is tried.

    @code
    function BACKTRACKING-SEARCH(csp) returns a solution, or failure
        return BACKTRACK({ }, csp)

    function BACKTRACK(assignment, csp) returns a solution, or failure
        if assignment is complete then return assignment
        var ←SELECT-UNASSIGNED-VARIABLE(csp)
        for each value in ORDER-DOMAIN-VALUES(var, assignment, csp) do
            if value is consistent with assignment then
                add {var = value} to assignment
                inferences ←INFERENCE(csp, var , value)
                if inferences = failure then
                add inferences to assignment
                result ←BACKTRACK(assignment, csp)
                if result = failure then
                    return result
            remove {var = value} and inferences from assignment
        return failure
    @endcode
*/

/** Figure 6.8 The MIN-CONFLICTS algorithm for solving CSPs by local search. The initial state may be chosen randomly or by a greedy assignment process that chooses a minimal conflict value for each variable in turn. The CONFLICTS function counts the number of constraints violated by a particular value, given the rest of the current assignment.

    @codefunction MIN-CONFLICTS(csp,max steps) returns a solution or failure
    inputs: csp, a constraint satisfaction problem
            max steps, the number of steps allowed before giving up

        current ←an initial complete assignment for csp
        for i = 1 to max steps do
            if current is a solution for csp then return current
            var ←a randomly chosen conflicted variable from csp.VARIABLES
            value←the value v for var that minimizes CONFLICTS(var, v, current , csp)
            set var =value in current
        return failure
    @endcode
*/

/** Figure 6.11 The TREE-CSP-SOLVER algorithm for solving tree-structured CSPs. If the CSP has a solution, we will find it in linear time; if not, we will detect a contradiction.

    @code
    function TREE-CSP-SOLVER(csp) returns a solution, or failure
        inputs: csp, a CSP with components X, D, C

        n←number of variables in X
        assignment ←an empty assignment
        root ←any variable in X
        X ←TOPOLOGICALSORT(X, root )
        for j = n down to 2 do
            MAKE-ARC-CONSISTENT(PARENT(Xj),Xj )
            if it cannot be made consistent then return failure
        for i = 1 to n do
            assignment[Xi]←any consistent value from Di
            if there is no consistent value then return failure
        return assignment
    @endcode
*/

// 7. Logical Agents

/** Figure 7.1 A generic knowledge-based agent. Given a percept, the agent adds the percept to its knowledge base, asks the knowledge base for the best action, and tells the knowledge base that it has in fact taken that action.

    @code
    function KB-AGENT(percept ) returns an action
        persistent: KB, a knowledge base
            t , a counter, initially 0, indicating time

        TELL(KB,MAKE-PERCEPT-SENTENCE(percept , t ))
        action ←ASK(KB,MAKE-ACTION-QUERY(t ))
        TELL(KB,MAKE-ACTION-SENTENCE(action, t ))
        t ←t + 1
        return action
    @endcode
*/
/**  Figure 7.7 A BNF (Backus–Naur Form) grammar of sentences in propositional logic, along with operator precedences, from highest to lowest.
|
|  Logic Type         | Logic |
|--------------------:|:------|
| Sentence            | → AtomicSentence OR ComplexSentence
| AtomicSentence      | → True OR False OR P OR Q OR R OR . . . |
| ComplexSentence     | → ( Sentence ) OR [ Sentence ] OR ¬Sentence OR Sentence ∧ Sentence OR Sentence ∨ Sentence OR Sentence ⇒ Sentence OR Sentence ⇔ Sentence |
| OPERATOR PRECEDENCE | : ¬, ∧, ∨,⇒,⇔


*/


/** Figure 7.10 A truth-table enumeration algorithm for deciding propositional entailment. (TT stands for truth table.) PL-TRUE? returns true if a sentence holds within a model. The variable model represents a partial model—an assignment to some of the symbols. The keyword “and” is used here as a logical operation on its two arguments, returning true or false.

    @code
    function TT-ENTAILS?(KB,α) returns true or false
        inputs: KB, the knowledge base, a sentence in propositional logic
            α, the query, a sentence in propositional logic
        symbols←a list of the proposition symbols in KB and α
        return TT-CHECK-ALL(KB,α, symbols,{ })

        function TT-CHECK-ALL(KB,α, symbols ,model ) returns true or false
            if EMPTY?(symbols) then
                if PL-TRUE?(KB,model ) then return PL-TRUE?(α,model )
                else return true // when KB is false, always return true
            else do
                P ←FIRST(symbols)
                rest ←REST(symbols)
                return (TT-CHECK-ALL(KB,α, rest ,model ∪ {P = true})
                    and
                    TT-CHECK-ALL(KB,α, rest ,model ∪ {P = false }))
    @endcode
*/

/** Figure 7.11 Standard logical equivalences. The symbols α, β, and γ stand for arbitrary sentences of propositional logic.

|           Identity                      | Description. |
|:---------------------------------------:|:-------------|
|       (α ∧ β) ≡ (β ∧ α)               | Commutativity of ∧ |
|       (α ∨ β) ≡ (β ∨ α)               | Commutativity of ∨ |
| ((α ∧ β) ∧ γ) ≡ (α ∧ (β ∧ γ))        | Associativity of ∧ |
| ((α ∨ β) ∨ γ) ≡ (α ∨ (β ∨ γ))        | Associativity of ∨ |
|         ¬(¬α) ≡ α                       | Double-negation elimination |
|       (α ⇒ β) ≡ (¬β ⇒ ¬α)             | Contraposition |
|       (α ⇒ β) ≡ (¬α ∨ β)              | Implication elimination |
|       (α ⇔ β) ≡ ((α ⇒ β) ∧ (β ⇒ α))  | Biconditional elimination |
|      ¬(α ∧ β) ≡ (¬α∨ ¬β)              | De Morgan |
|      ¬(α ∨ β) ≡ (¬α∧ ¬β)              | De Morgan |
| (α ∧ (β ∨ γ)) ≡ ((α ∧ β) ∨ (α ∧ γ)) | Distributivity of ∧ over ∨ |
| (α ∨ (β ∧ γ)) ≡ ((α ∨ β) ∧ (α ∨ γ)) | Distributivity of ∨ over ∧ |
*/

/** Figure 7.12 A simple resolution algorithm for propositional logic. The function PL-RESOLVE returns the set of all possible clauses obtained by resolving its two inputs.

    @code
    function PL-RESOLUTION(KB,α) returns true or false
    inputs: KB, the knowledge base, a sentence in propositional logic
        α, the query, a sentence in propositional logic
        clauses ←the set of clauses in the CNF representation of KB ∧¬α
        new ←{}
        loop do
            for each pair of clauses Ci, Cj in clauses do
            resolvents ←PL-RESOLVE(Ci,Cj )
            if resolvents contains the empty clause then return true
            new ←new ∪ resolvents
        if new ⊆ clauses then return false
        clauses ←clauses ∪new

    @endcode
*/

/** Figure 7.15 The forward-chaining algorithm for propositional logic. The agenda keeps track of symbols known to be true but not yet “processed.” The count table keeps track of how many premises of each implication are as yet unknown. Whenever a new symbol p from the agenda is processed, the count is reduced by one for each implication in whose premise p appears (easily identified in constant time with appropriate indexing.) If a count reaches zero, all the premises of the implication are known, so its conclusion can be added to the agenda. Finally, we need to keep track of which symbols have been processed; a symbol that is already in the set of inferred symbols need not be added to the agenda again. This avoids redundant work and prevents loops caused by implications such as P ⇒ Q and Q ⇒ P.

    @code
    function PL-FC-ENTAILS?(KB, q) returns true or false
        inputs: KB, the knowledge base, a set of propositional definite clauses
                q, the query, a proposition symbol
        count ←a table, where count [c] is the number of symbols in c’s premise
        inferred ←a table,where inferred[s] is initially false for all symbols
        agenda ←a queue of symbols, initially symbols known to be true in KB
        while agenda is not empty do
            p←POP(agenda)
            if p = q then return true
            if inferred[p] = false then
                inferred[p]←true
                for each clause c in KB where p is in c.PREMISE do
                    decrement count [c]
                    if count [c] = 0 then add c.CONCLUSION to agenda
        return false
    @endcode
*/

/** Figure 7.17 The DPLL algorithm for checking satisfiability of a sentence in propositional logic. The ideas behind FIND-PURE-SYMBOL and FIND-UNIT-CLAUSE are described in the text; each returns a symbol (or null) and the truth value to assign to that symbol. Like TT-ENTAILS?, DPLL operates over partial models.

    @code
    function DPLL-SATISFIABLE?(s) returns true or false
        inputs: s, a sentence in propositional logic

        clauses ←the set of clauses in the CNF representation of s
        symbols←a list of the proposition symbols in s
        return DPLL(clauses, symbols,{ })

    function DPLL(clauses, symbols,model ) returns true or false
        if every clause in clauses is true in model then return true
        if some clause in clauses is false in model then return false
        P, value←FIND-PURE-SYMBOL(symbols, clauses,model )
        if P is non-null then return DPLL(clauses, symbols – P,model ∪ {P=value})
        P, value←FIND-UNIT-CLAUSE(clauses,model )
        if P is non-null then return DPLL(clauses, symbols – P,model ∪ {P=value})
        P ←FIRST(symbols); rest ←REST(symbols)
        return DPLL(clauses, rest ,model ∪ {P=true}) or
               DPLL(clauses, rest ,model ∪ {P=false}))
    @endcode
*/

/** Figure 7.18 The WALKSAT algorithm for checking satisfiability by randomly flipping the values of variables. Many versions of the algorithm exist.

    @code
    function WALKSAT(clauses, p,max flips) returns a satisfying model or failure
        inputs: clauses, a set of clauses in propositional logic
                p, the probability of choosing to do a “random walk” move, typically around 0.5
                max flips, number of flips allowed before giving up

        model ←a random assignment of true/false to the symbols in clauses
        for i= 1to max flips do
            if model satisfies clauses then return model
            clause←a randomly selected clause from clauses that is false in model
            with probability p flip the value in model of a randomly selected symbol from clause
            else flip whichever symbol in clause maximizes the number of satisfied clauses
        return failure
    @endcode
*/

/** Figure 7.20 A hybrid agent program for the wumpus world. It uses a propositional knowledge base to infer the state of the world, and a combination of problem-solving search and domain-specific code to decide what actions to take.

    @code
    function HYBRID-WUMPUS-AGENT(percept ) returns an action
        inputs: percept , a list, [stench,breeze,glitter ,bump,scream]
        persistent: KB, a knowledge base, initially the atemporal “wumpus physics”
                t , a counter, initially 0, indicating time
                plan, an action sequence, initially empty

        TELL(KB,MAKE-PERCEPT-SENTENCE(percept , t ))
        TELL the KB the temporal “physics” sentences for time t
        safe ←{[x , y] : ASK(KB,OKt
        x,y) = true}
        if ASK(KB,Glittert) = true then
            plan ←[Grab] + PLAN-ROUTE(current , {[1,1]}, safe) + [Climb]
        if plan is empty then
            unvisited ←{[x , y] : ASK(KB,Lt
            x,y) = false for all t ≤ t}
            plan ←PLAN-ROUTE(current, unvisited ∩safe, safe)
        if plan is empty and ASK(KB,HaveArrowt) = true then
            possible wumpus ←{[x , y] : ASK(KB,¬Wx,y) = false}
            plan ←PLAN-SHOT(current , possible wumpus, safe)
            if plan is empty then // no choice but to take a risk
            not unsafe ←{[x , y] : ASK(KB,¬ OKt
            x,y) = false}
            plan ←PLAN-ROUTE(current, unvisited ∩not unsafe, safe)
        if plan is empty then
            plan ←PLAN-ROUTE(current, {[1, 1]}, safe) + [Climb]
            action ←POP(plan)
            TELL(KB,MAKE-ACTION-SENTENCE(action, t ))
            t ←t + 1
        return action

    function PLAN-ROUTE(current,goals,allowed) returns an action sequence
        inputs: current , the agent’s current position
            goals, a set of squares; try to plan a route to one of them
            allowed, a set of squares that can form part of the route

        problem ←ROUTE-PROBLEM(current , goals,allowed)
        return A*-GRAPH-SEARCH(problem)
    @endcode
*/

/** Figure 7.22 The SATPLAN algorithm. The planning problem is translated into a CNF sentence in which the goal is asserted to hold at a fixed time step t and axioms are included for each time step up to t. If the satisfiability algorithm finds a model, then a plan is extracted by looking at those proposition symbols that refer to actions and are assigned true in the model. If no model exists, then the process is repeated with the goal moved one step later.

    @code
    function SATPLAN(init , transition, goal , T max) returns solution or failure
        inputs: init , transition, goal , constitute a description of the problem
        T max, an upper limit for plan length
        for t = 0 to T max do
        cnf ←TRANSLATE-TO-SAT(init , transition, goal , t )
        model ←SAT-SOLVER(cnf )
        if model is not null then
        return EXTRACT-SOLUTION(model )
        return failure
    @endcode
*/

/** Figure 8.3 The syntax of first-order logic with equality, specified in Backus–Naur form (see page 1060 if you are not familiar with this notation). Operator precedences are specified, from highest to lowest. The precedence of quantifiers is such that a quantifier holds over everything to the right of it.

|                 logic | Equality                            |
|----------------------:|:-----------------------------------:|
|              Sentence | → AtomicSentence OR ComplexSentence |
|        AtomicSentence | → Predicate OR Predicate(Term, . . .) OR Term = Term |
|       ComplexSentence | → ( Sentence ) OR [ Sentence ] OR ¬Sentence OR Sentence ∧ Sentence OR Sentence ∨ Sentence OR Sentence ⇒ Sentence OR Sentence ⇔ Sentence OR Quantifier Variable, . . . Sentence  |
|                  Term | → Function(Term, . . .) OR Constant OR Variable |
|            Quantifier | → ∀ OR ∃ |
|              Constant | → A OR X1 OR John OR · · · |
|              Variable | → a OR x OR s OR ··· |
|             Predicate |→ True OR False OR After OR Loves OR Raining OR ··· |
|              Function | → Mother OR LeftLeg OR · · · |
|   OPERATOR PRECEDENCE | : ¬,=, ∧, ∨,⇒,⇔ |
*/



/** Figure 9.1 The unification algorithm. The algorithm works by comparing the structures of the inputs, element by element. The substitution θ that is the argument to UNIFY is built up along the way and is used to make sure that later comparisons are consistent with bindings that were established earlier. In a compound expression such as F(A,B), the OP field picks out the function symbol F and the ARGS field picks out the argument list (A,B).

    @code
    function UNIFY(x , y, θ) returns a substitution to make x and y identical
        inputs: x , a variable, constant, list, or compound expression
                y, a variable, constant, list, or compound expression
                θ, the substitution built up so far (optional, defaults to empty)
        if θ = failure then return failure
        else if x = y then return θ
        else if VARIABLE?(x ) then return UNIFY-VAR(x , y, θ)
        else if VARIABLE?(y) then return UNIFY-VAR(y, x , θ)
        else if COMPOUND?(x ) and COMPOUND?(y) then
            return UNIFY(x.ARGS, y.ARGS, UNIFY(x.OP, y.OP, θ))
        else if LIST?(x ) and LIST?(y) then
            return UNIFY(x .REST, y.REST, UNIFY(x .FIRST, y.FIRST, θ))
        else return failure

    function UNIFY-VAR(var, x , θ) returns a substitution
        if {var/val} ∈ θ then return UNIFY(val , x , θ)
        else if {x/val} ∈ θ then return UNIFY(var, val , θ)
        else if OCCUR-CHECK?(var, x ) then return failure
        else return add {var/x } to θ
    @endcode
*/

/** Figure 9.3 A conceptually straightforward, but very inefficient, forward-chaining algorithm. On each iteration, it adds to KB all the atomic sentences that can be inferred in one step from the implication sentences and the atomic sentences already in KB. The function STANDARDIZE-VARIABLES replaces all variables in its arguments with new ones that have not been used before.

    @code
    function FOL-FC-ASK(KB,α) returns a substitution or false
        inputs: KB, the knowledge base, a set of first-order definite clauses
                α, the query, an atomic sentence

                local variables: new, the new sentences inferred on each iteration

        repeat until new is empty
            new ←{}
            for each rule in KB do
            (p1 ∧ . . . ∧ pn ⇒ q)←STANDARDIZE-VARIABLES(rule)
            for each θ such that SUBST(θ, p1 ∧ . . . ∧ pn) = SUBST(θ, p
                1 ∧ . . . ∧ p
                n)
                    for some p
                1 , . . . , p
                n in KB
                q←SUBST(θ, q)
                if q does not unify with some sentence already in KB or new then
                    add q to new
                    φ←UNIFY(q,α)
                    if φ is not fail then return φ
            add new to KB
        return false
    @endcode
*/

/** Figure 9.6 A simple backward-chaining algorithm for first-order knowledge bases.

    @code
    function FOL-BC-ASK(KB, query) returns a generator of substitutions
        return FOL-BC-OR(KB, query,{ })

    generator FOL-BC-OR(KB, goal , θ) yields a substitution
        for each rule (lhs ⇒ rhs) in FETCH-RULES-FOR-GOAL(KB, goal ) do
            (lhs, rhs)←STANDARDIZE-VARIABLES((lhs, rhs))
            for each θ in FOL-BC-AND(KB, lhs, UNIFY(rhs, goal , θ)) do
                yield θ

    generator FOL-BC-AND(KB, goals, θ) yields a substitution
        if θ = failure then return
        else if LENGTH(goals) = 0 then yield θ
        else do
            first,rest ←FIRST(goals), REST(goals)
            for each θ in FOL-BC-OR(KB, SUBST(θ, first), θ) do
                for each θ in FOL-BC-AND(KB, rest , θ) do
                    yield θ

    @endcode
*/

/** Figure 9.8 Pseudocode representing the result of compiling the Append predicate. The function NEW-VARIABLE returns a new variable, distinct from all other variables used so far. The procedure CALL(continuation) continues execution with the specified continuation.

    @code
    procedure APPEND(ax , y, az , continuation)

        trail ←GLOBAL-TRAIL-POINTER()
        if ax = [ ] and UNIFY(y, az ) then CALL(continuation)
        RESET-TRAIL(trail )
        a, x , z ←NEW-VARIABLE(), NEW-VARIABLE(), NEW-VARIABLE()
        if UNIFY(ax , [a | x ]) and UNIFY(az , [a | z ]) then APPEND(x , y, z , continuation)
    @endcode
*/

// 10. Classical Planning

/** Figure 10.9 The GRAPHPLAN algorithm. GRAPHPLAN calls EXPAND-GRAPH to add a level until either a solution is found by EXTRACT-SOLUTION, or no solution is possible.

    @code
    function GRAPHPLAN(problem) returns solution or failure

        graph ←INITIAL-PLANNING-GRAPH(problem)
        goals ←CONJUNCTS(problem.GOAL)
        nogoods ←an empty hash table
        for tl = 0 to∞do
            if goals all non-mutex in St of graph then
                solution ←EXTRACT-SOLUTION(graph, goals, NUMLEVELS(graph), nogoods)
                if solution = failure then return solution
            if graph and nogoods have both leveled off then return failure
            graph←EXPAND-GRAPH(graph, problem)
    @endcode
*/

// 11. Planning and Acting in the Real World

/** Figure 11.5 A breadth-first implementation of hierarchical forward planning search. The initial plan supplied to the algorithm is [Act]. The REFINEMENTS function returns a set of action sequences, one for each refinement of the HLA whose preconditions are satisfied by the specified state, outcome.

    @code
    function HIERARCHICAL-SEARCH(problem, hierarchy) returns a solution, or failure

        frontier ←a FIFO queue with [Act] as the only element
        loop do
            if EMPTY?( frontier) then return failure
            plan ←POP( frontier) // chooses the shallowest plan in frontier
            hla←the first HLA in plan, or null if none
            prefix,suffix ←the action subsequences before and after hla in plan
            outcome←RESULT(problem.INITIAL-STATE, prefix)
            if hla is null then // so plan is primitive and outcome is its result
                if outcome satisfies problem.GOAL then return plan
            else for each sequence in REFINEMENTS(hla, outcome, hierarchy) do
                frontier ←INSERT(APPEND(prefix , sequence, suffix), frontier )
    @endcode
*/

/** Figure 11.8 A hierarchical planning algorithm that uses angelic semantics to identify and commit to high-level plans that work while avoiding high-level plans that don’t. The predicate MAKING-PROGRESS checks to make sure that we aren’t stuck in an infinite regression of refinements. At top level, call ANGELIC-SEARCH with [Act ] as the initialPlan .

    @code
    function ANGELIC-SEARCH(problem, hierarchy, initialPlan ) returns solution or fail

        frontier ←a FIFO queue with initialPlan as the only element
        loop do
        if EMPTY?( frontier) then return fail
            plan ←POP( frontier) // chooses the shallowest node in frontier
            if REACH+(problem.INITIAL-STATE, plan) intersects problem.GOAL then
                if plan is primitive then return plan // REACH+ is exact for primitive plans
                guaranteed ←REACH−(problem.INITIAL-STATE, plan) ∩ problem.GOAL
                if guaranteed={ } and MAKING-PROGRESS(plan, initialPlan) then
                    finalState←any element of guaranteed
                return DECOMPOSE(hierarchy, problem.INITIAL-STATE, plan, finalState)
                hla←some HLA in plan
                prefix ,suffix ←the action subsequences before and after hla in plan
                for each sequence in REFINEMENTS(hla, outcome, hierarchy) do
                    frontier ←INSERT(APPEND(prefix , sequence, suffix), frontier)

    function DECOMPOSE(hierarchy, s0 , plan, sf ) returns a solution

        solution ←an empty plan
        while plan is not empty do
            action ←REMOVE-LAST(plan)
            si ←a state in REACH−(s0 , plan) such that sf ∈REACH−(si , action)
            problem ←a problem with INITIAL-STATE = si and GOAL = sf
            solution ←APPEND(ANGELIC-SEARCH(problem, hierarchy, action), solution)
            sf ←si
        return solution
    @endcode
*/

// 12. Knowledge Representation

// No functions

// 13. Quantifying Uncertainty

/** Figure 13.1 A decision-theoretic agent that selects rational actions.

    @code
    function DT-AGENT(percept ) returns an action
        persistent: belief state, probabilistic beliefs about the current state of the world
                    action, the agent’s action

        update belief state based on action and percept
        calculate outcome probabilities for actions,
            given action descriptions and current belief state
        select action with highest expected utility
            given probabilities of outcomes and utility information
        return action
    @endcode
*/

// 14. Probabilistic Learning

/** Figure 14.9 The enumeration algorithm for answering queries on Bayesian networks.

    @code
    function ENUMERATION-ASK(X, e, bn) returns a distribution over X
        inputs: X, the query variable
                e, observed values for variables E
                bn, a Bayes net with variables {X} ∪ E ∪ Y // Y = hidden variables

        Q(X)←a distribution over X, initially empty
        for each value xi of X do
            Q(xi)←ENUMERATE-ALL(bn.VARS, exi )
                where exi is e extended with X = xi
        return NORMALIZE(Q(X))

    function ENUMERATE-ALL(vars, e) returns a real number
        if EMPTY?(vars) then return 1.0
        Y ←FIRST(vars)
        if Y has value y in e
            then return P(y | parents(Y )) × ENUMERATE-ALL(REST(vars), e)
            else returny P(y | parents(Y )) × ENUMERATE-ALL(REST(vars), ey)
                where ey is e extended with Y = y
    @endcode
*/

/** Figure 14.11 The variable elimination algorithm for inference in Bayesian networks.

    @code
    function ELIMINATION-ASK(X, e, bn) returns a distribution over X
        inputs: X, the query variable
                e, observed values for variables E
                bn, a Bayesian network specifying joint distribution P(X1,... , Xn)

        factors ←[ ]
        for each var in ORDER(bn.VARS) do
            factors ←[MAKE-FACTOR(var , e)|factors]
            if var is a hidden variable then factors ←SUM-OUT(var, factors )
        return NORMALIZE(POINTWISE-PRODUCT(factors))
    @endcode
*/

/** Figure 14.13 A sampling algorithm that generates events from a Bayesian network. Each variable is sampled according to the conditional distribution given the values already sampled for the variable’s parents.

    @code
    function PRIOR-SAMPLE(bn) returns an event sampled from the prior specified by bn
        inputs: bn, a Bayesian network specifying joint distribution P(X1, . . . , Xn)

        x←an event with n elements
        foreach variable Xi in X1, . . . , Xn do
            x[i]←a random sample from P(Xi | parents(Xi))
        return x
    @endcode
*/

/** Figure 14.14 The rejection-sampling algorithm for answering queries given evidence in a Bayesian network.

    @code
    function REJECTION-SAMPLING(X, e, bn,N) returns an estimate of P(X|e)
        inputs: X, the query variable
                e, observed values for variables E
                bn, a Bayesian network
                N, the total number of samples to be generated
        local variables: N, a TArray of counts for each value of X, initially zero

        for j = 1 to N do
            x←PRIOR-SAMPLE(bn)
            if x is consistent with e then
                N[x ]←N[x ]+1 where x is the value of X in x
        return NORMALIZE(N)
    @endcode
*/

/** Figure 14.15 The likelihood-weighting algorithm for inference in Bayesian networks. In WEIGHTED-SAMPLE, each nonevidence variable is sampled according to the conditional distribution given the values already sampled for the variable’s parents, while a weight is accumulated based on the likelihood for each evidence variable.

    @code
    function LIKELIHOOD-WEIGHTING(X, e, bn,N) returns an estimate of P(X|e)
        inputs: X, the query variable
                e, observed values for variables E
                bn, a Bayesian network specifying joint distribution P(X1, . . . , Xn)
                N, the total number of samples to be generated
        local variables: W, a TArray of weighted counts for each value of X, initially zero

        for j = 1 to N do
            x,w ←WEIGHTED-SAMPLE(bn, e)
            W[x ]←W[x] + w where x is the value of X in x
        return NORMALIZE(W)

    function WEIGHTED-SAMPLE(bn, e) returns an event and a weight

        w ←1; x←an event with n elements initialized from e
        foreach variable Xi in X1, . . . , Xn do
            if Xi is an evidence variable with value xi in e
                then w ←w × P(Xi = xi | parents(Xi))
                else x[i]←a random sample from P(Xi | parents(Xi))
        return x, w
    @endcode
*/

/** Figure 14.16 The Gibbs sampling algorithm for approximate inference in Bayesian networks; this version cycles through the variables, but choosing variables at random also works.

    @code
    function GIBBS-ASK(X, e, bn,N) returns an estimate of P(X|e)
        local variables: N, a TArray of counts for each value of X, initially zero
                Z, the nonevidence variables in bn
                x, the current state of the network, initially copied from e
        initialize x with random values for the variables in Z
        for j = 1 to N do
            for each Zi in Z do
                set the value of Zi in x by sampling from P(Zi|mb(Zi))
                N[x ]←N[x] + 1 where x is the value of X in x
        return NORMALIZE(N)
    @endcode
*/

// 15. Probabilistic Reasoning over Time

/** Figure 15.4 The forward–backward algorithm for smoothing: computing posterior probabilities of a sequence of states given a sequence of observations. The FORWARD and BACKWARD operators are defined by Equations (15.5) and (15.9), respectively.

    @code
    function FORWARD-BACKWARD(ev, prior ) returns a TArray of probability distributions
        inputs: ev, a TArray of evidence values for steps 1, . . . , t
                prior , the prior distribution on the initial state, P(X0)
        local variables: fv, a TArray of forward messages for steps 0, . . . , t
                b, a representation of the backward message, initially all 1s
                sv, a TArray of smoothed estimates for steps 1, . . . , t

        fv[0]←prior
        for i= 1to t do
            fv[i]←FORWARD(fv[i − 1], ev[i])
        for i= t downto 1 do
            sv[i]←NORMALIZE(fv[i]×b)
            b←BACKWARD(b, ev[i])
        return sv
    @endcode
*/

/** Figure 15.6 An algorithm for smoothing with a fixed time lag of d steps, implemented as an online algorithm that outputs the new smoothed estimate given the observation for a new time step. Notice that the final output NORMALIZE(f×B1) is just α f×b, by Equation (15.14).

    @code
    function FIXED-LAG-SMOOTHING(et,hmm, d) returns a distribution over Xt−d
        inputs: et, the current evidence for time step t
                hmm, a hidden Markov model with S × S transition matrix T
                d, the length of the lag for smoothing
        persistent: t , the current time, initially 1
                f, the forward message P(Xt|e1:t), initially hmm.PRIOR
                B, the d-step backward transformation matrix, initially the identity matrix
                et−d:t, double-ended list of evidence from t − d to t, initially empty
        local variables: Ot−d,Ot, diagonal matrices containing the sensor model information

        add et to the end of et−d:t
        Ot←diagonal matrix containing P(et|Xt)
        if t > d then
            f←FORWARD(f, et)
            remove et−d−1 from the beginning of et−d:t
            Ot−d←diagonal matrix containing P(et−d|Xt−d)
            B←O−1
            t−dT−1BTOt
        else B←BTOt
        t ←t + 1
        if t > d then return NORMALIZE(f × B1) else return null
    @endcode
*/

/** Figure 15.17 The particle filtering algorithm implemented as a recursive update operation with state (the set of samples). Each of the sampling operations involves sampling the relevant slice variables in topological order, much as in PRIOR-SAMPLE. The WEIGHTED-SAMPLE-WITH-REPLACEMENT operation can be implemented to run in O(N) expected time. The step numbers refer to the description in the text.

    @code
    function PARTICLE-FILTERING(e,N, dbn) returns a set of samples for the next time step
        inputs: e, the new incoming evidence
                N, the number of samples to be maintained
                dbn, a DBN with prior P(X0), transition model P(X1|X0), sensor model P(E1|X1)
        persistent: S, a TArray of samples of size N, initially generated from P(X0)
        local variables: W, a TArray of weights of size N

        for i = 1 to N do
            S[i ]←sample from P(X1 | X0 = S[i ]) // step 1
            W[i ]←P(e | X1 = S[i]) // step 2
        S ←WEIGHTED-SAMPLE-WITH-REPLACEMENT(N, S,W) // step 3
        return S
    @endcode
*/

// 16. Making Simple Decisions

/** Figure 16.9 Design of a simple information-gathering agent. The agent works by repeatedly selecting the observation with the highest information value, until the cost of the next observation is greater than its expected benefit.

    @code
    function INFORMATION-GATHERING-AGENT(percept ) returns an action
        persistent: D, a decision network

        integrate percept into D
        j ←the value that maximizes VPI (Ej) / Cost (Ej )
        if VPI (Ej) > Cost (Ej )
            return REQUEST(Ej )
        else return the best action from D
    @endcode
*/

/** Figure 17.4 The value iteration algorithm for calculating utilities of states. The termination condition is from Equation (17.8).

    @code
    function VALUE-ITERATION(mdp, ) returns a utility function
        inputs: mdp, an MDP with states S, actions A(s), transition model P(s | s, a),
                rewards R(s), discount γ
                , the maximum error allowed in the utility of any state
        local variables: U, U, vectors of utilities for states in S, initially zero
                δ, the maximum change in the utility of any state in an iteration

        repeat
            U ←U; δ←0
            for each state s in S do
                U[s]←R(s) + γ max
                a∈A(s)

                s
                P(s | s, a) U[s]
            if |U[s] − U[s]| > δ then δ ←|U[s] − U[s]|
        untilδ < (1 − γ)/γ
        return U
    @endcode
*/

/** Figure 17.7 The policy iteration algorithm for calculating an optimal policy.

    @code
    function POLICY-ITERATION(mdp) returns a policy
        inputs: mdp, an MDP with states S, actions A(s), transition model P(s | s, a)
        local variables: U, a TArray of utilities for states in S, initially zero
                π, a policy TArray indexed by state, initially random

        repeat
            U ←POLICY-EVALUATION(π,U,mdp)
            unchanged?←true
            for each state s in S do
                if max
                    a ∈A(s)

                    s
                    P(s
                     | s, a) U[s
                    ] >
                    s
                    P(s
                     | s, π[s]) U[s
                    ] then do
                    π[s]←argmax
                    a ∈A(s)

                    s
                    P(s
                     | s, a) U[s
                    ]
                    unchanged?←false
        until unchanged?
        return π
    @endcode
*/

/** Figure 17.9 A high-level sketch of the value iteration algorithm for POMDPs. The REMOVE-DOMINATED-PLANS step and MAX-DIFFERENCE test are typically implemented as linear programs.

    @code
    function POMDP-VALUE-ITERATION(pomdp, ) returns a utility function
        inputs: pomdp, a POMDP with states S, actions A(s), transition model P(s | s, a),
            sensor model P(e | s), rewards R(s), discount γ
            , the maximum error allowed in the utility of any state
        local variables: U, U, sets of plans p with associated utility vectors αp

        U←a set containing just the empty plan [ ], with α[ ](s)= R(s)
        repeat
            U ←U
            U←the set of all plans consisting of an action and, for each possible next percept,
                a plan in U with utility vectors computed according to Equation (17.13)
            U←REMOVE-DOMINATED-PLANS(U)
        until MAX-DIFFERENCE(U,U) < (1 − γ)/γ
        return U
    @endcode
*/

// 18. Learning from Examples

/** Figure 18.5 The decision-tree learning algorithm. The function IMPORTANCE is described in Section 18.3.4. The function PLURALITY-VALUE selects the most common output value among a set of examples, breaking ties randomly.

    @code
    function DECISION-TREE-LEARNING(examples, attributes, parent examples) returns
        a tree

        if examples is empty then return PLURALITY-VALUE(parent examples)
        else if all examples have the same classification then return the classification
        else if attributes is empty then return PLURALITY-VALUE(examples)
        else
            A←argmaxa ∈ attributes IMPORTANCE(a, examples)
            tree←a new decision tree with root test A
            for each value vk of A do
                exs ←{e : e ∈examples and e.A = vk}
                subtree←DECISION-TREE-LEARNING(exs, attributes −A, examples)
                add a branch to tree with label (A = vk) and subtree subtree
            return tree
    @endcode
*/

/** Figure 18.8 An algorithm to select the model that has the lowest error rate on validation data by building models of increasing complexity, and choosing the one with best empirical error rate on validation data. Here errT means error rate on the training data, and errV means error rate on the validation data. Learner (size, examples) returns a hypothesis whose complexity is set by the parameter size, and which is trained on the examples. PARTITION(examples, fold, k) splits examples into two subsets: a validation set of size N/k and a training set with all the other examples. The split is different for each value of fold.

    @code
    function CROSS-VALIDATION-WRAPPER(Learner , k, examples) returns a hypothesis

        local variables: errT, an array, indexed by size, storing training-set error rates
                errV , an array, indexed by size, storing validation-set error rates

        for size = 1 to∞ do
            errT[size], errV [size]←CROSS-VALIDATION(Learner , size, k, examples)
            if errT has converged then do
                best size←the value of size with minimum errV [size]
                return Learner (best size, examples)

    function CROSS-VALIDATION(Learner , size, k, examples) returns two values:
                average training set error rate, average validation set error rate

        fold errT ←0; fold errV ←0
        for fold = 1 to k do
            training set , validation set ←PARTITION(examples, fold , k)
            h←Learner (size, training set )
            fold errT ←fold errT + ERROR-RATE(h, training set)
            fold errV ←fold errV +ERROR-RATE(h, validation set )
        return fold errT/k, fold errV /k
    @endcode
*/

/** Figure 18.11 An algorithm for learning decision lists.

    @code
    function DECISION-LIST-LEARNING(examples) returns a decision list, or failure

        if examples is empty then return the trivial decision list No
        t ←a test that matches a nonempty subset examplest of examples
            such that the members of examplest are all positive or all negative
        if there is no such t then return failure
        if the examples in examplest are positive then o←Yes else o←No
        return a decision list with initial test t and outcome o and remaining tests given by
            DECISION-LIST-LEARNING(examples − examplest)
    @endcode
*/

/** Figure 18.24 The back-propagation algorithm for learning in multilayer networks.

    @code
    function BACK-PROP-LEARNING(examples, network ) returns a neural network
        inputs: examples, a set of examples, each with input TArray x and output TArray y
                network , a multilayer network with L layers, weights wi,j , activation function g
        local variables: Δ, a TArray of errors, indexed by network node

        repeat
            for each weight wi,j in network do
                wi,j ←a small random number
            for each example (x, y) in examples do

                // Propagate the inputs forward to compute the outputs

                for each node i in the input layer do
                    ai←xi
                for  = 2 to L do
                for each node j in layer  do
                    inj ←i wi,j ai
                    aj ←g(inj)

                // Propagate deltas backward from output layer to input layer

                for each node j in the output layer do
                    Δ[j]←g(inj) × (yj − aj)
                for  = L − 1 to 1 do
                    for each node i in layer  do
                        Δ[i]←g(ini)j wi,j Δ[j]

                // Update every weight in network using deltas

                for each weight wi,j in network do
                wi,j ←wi,j + α × ai × Δ[j]
        until some stopping criterion is satisfied
        return network
    @endcode
*/

/** Figure 18.34 The ADABOOST variant of the boosting method for ensemble learning. The algorithmgenerates hypotheses by successively reweighting the training examples. The function WEIGHTED-MAJORITY generates a hypothesis that returns the output value with the highest vote from the hypotheses in h, with votes weighted by z.

    @code
    function ADABOOST(examples,L,K) returns a weighted-majority hypothesis
        inputs: examples, set of N labeled examples (x1, y1), . . . , (xN, yN)
                L, a learning algorithm
                K, the number of hypotheses in the ensemble
        local variables: w, a TArray of N example weights, initially 1/N
                h, a TArray of K hypotheses
                z, a TArray of K hypothesis weights

        for k = 1 to K do
            h[k]←L(examples,w)
            error ←0
            for j = 1 to N do
                if h[k](xj ) = yj then error ←error + w[j]
            for j = 1 to N do
                if h[k](xj) = yj then w[j]←w[j] · error/(1 − error )
            w←NORMALIZE(w)
            z[k]←log (1 − error )/error
        return WEIGHTED-MAJORITY(h, z)
    @endcode
*/

/** Figure 19.2 The current-best-hypothesis learning algorithm. It searches for a consistent hypothesis that fits all the examples and backtracks when no consistent specialization/ generalization can be found. To start the algorithm, any hypothesis can be passed in; it will be specialized or gneralized as needed.

    @code
    function CURRENT-BEST-LEARNING(examples, h) returns a hypothesis or fail
        if examples is empty then
            return h
        e←FIRST(examples)
        if e is consistent with h then
            return CURRENT-BEST-LEARNING(REST(examples), h)
        else if e is a false positive for h then
            for each h in specializations of h consistent with examples seen so far do
            h←CURRENT-BEST-LEARNING(REST(examples), h)
            if h = fail then return h
        else if e is a false negative for h then
            for each h in generalizations of h consistent with examples seen so far do
                h←CURRENT-BEST-LEARNING(REST(examples), h)
                if h = fail then return h
        return fail
    @endcode
*/

/** Figure 19.3 The version space learning algorithm. It finds a subset of V that is consistent with all the examples.

    @code
    function VERSION-SPACE-LEARNING(examples) returns a version space
        local variables: V, the version space: the set of all hypotheses

        V ←the set of all hypotheses
        for each example e in examples do
            if V is not empty then V ←VERSION-SPACE-UPDATE(V, e)
        return V

        function VERSION-SPACE-UPDATE(V, e) returns an updated version space

        V ←{h ∈V : h is consistent with e}
    @endcode
*/

/** Figure 19.8 An algorithm for finding a minimal consistent determination.

    @code
    function MINIMAL-CONSISTENT-DET(E,A) returns a set of attributes
        inputs: E, a set of examples
                A, a set of attributes, of size n

        for i = 0 to n do
            for each subset Ai of A of size i do
                if CONSISTENT-DET?(Ai,E) then return Ai

    function CONSISTENT-DET?(A,E) returns a truth value
        inputs: A, a set of attributes
                E, a set of examples
        local variables: H, a hash table

        for each example e in E do
            if some example in H has the same values as e for the attributes A
                but a different classification then return false
            store the class of e in H, indexed by the values for attributes A of the example e
        return true
    @endcode
*/

/** Figure 19.12 Sketch of the FOIL algorithm for learning sets of first-order Horn clauses from examples. NEW-LITERALS and CHOOSE-LITERAL are explained in the text.

    @code
    function FOIL(examples, target) returns a set of Horn clauses
        inputs: examples, set of examples
                target, a literal for the goal predicate
        local variables: clauses, set of clauses, initially empty

        while examples contains positive examples do
            clause←NEW-CLAUSE(examples, target)
            remove positive examples covered by clause from examples
            add clause to clauses
        return clauses

    function NEW-CLAUSE(examples, target) returns a Horn clause
        local variables: clause, a clause with target as head and an empty body
                l , a literal to be added to the clause
                extended examples, a set of examples with values for new variables

        extended examples ←examples
        while extended examples contains negative examples do
            l ←CHOOSE-LITERAL(NEW-LITERALS(clause), extended examples)
            append l to the body of clause
            extended examples ←set of examples created by applying EXTEND-EXAMPLE
                to each example in extended examples
        return clause

    function EXTEND-EXAMPLE(example, literal ) returns a set of examples

        if example satisfies literal
            then return the set of examples created by extending example with
                each possible constant value for each new variable in literal
        else return the empty set
    @endcode
*/

// 21. Reinforcement Learning

/** Figure 21.2 A passive reinforcement learning agent based on adaptive dynamic programming. The POLICY-EVALUATION function solves the fixed-policy Bellman equations, as described on page 657.

    @code
    function PASSIVE-ADP-AGENT(percept ) returns an action
        inputs: percept , a percept indicating the current state s and reward signal r 
        persistent: π, a fixed policy
                mdp, an MDP with model P, rewards R, discount γ
                U, a table of utilities, initially empty
                Nsa , a table of frequencies for state–action pairs, initially zero
                Ns|sa , a table of outcome frequencies given state–action pairs, initially zero
                s, a, the previous state and action, initially null

        if s is new then U[s]←r ; R[s]←r 
        if s is not null then
            increment Nsa [s, a] and Ns|sa [s, s, a]
            for each t such that Ns|sa [t , s, a] is nonzero do
                P(t | s, a)←Ns|sa [t , s, a] / Nsa [s, a]
        U ←POLICY-EVALUATION(π,U,mdp)
        if s.TERMINAL? then s, a←null else s, a←s, π[s]
        return a
    @endcode
*/

/** Figure 21.4 A passive reinforcement learning agent that learns utility estimates using temporal differences. The step-size function α(n) is chosen to ensure convergence, as described in the text.

    @code
    function PASSIVE-TD-AGENT(percept ) returns an action
        inputs: percept , a percept indicating the current state s and reward signal r 
        persistent: π, a fixed policy
                U, a table of utilities, initially empty
                Ns, a table of frequencies for states, initially zero
                s, a, r , the previous state, action, and reward, initially null

        if s is new then U[s]←r 
        if s is not null then
            increment Ns[s]
            U[s]←U[s] + α(Ns[s])(r + γ U[s] − U[s])
        if s.TERMINAL? then s, a, r ←null else s, a, r ←s, π[s], r 
        return a
    @endcode
*/

/** Figure 21.8 An exploratory Q-learning agent. It is an active learner that learns the value Q(s, a) of each action in each situation. It uses the same exploration function f as the exploratory ADP agent, but avoids having to learn the transition model because the Q-value of a state can be related directly to those of its neighbors.

    @code
    function Q-LEARNING-AGENT(percept ) returns an action
        inputs: percept , a percept indicating the current state s and reward signal r 
        persistent: Q, a table of action values indexed by state and action, initially zero
            Nsa , a table of frequencies for state–action pairs, initially zero
            s, a, r , the previous state, action, and reward, initially null

        if TERMINAL?(s) then Q[s,None]←r 
        if s is not null then
            increment Nsa [s, a]
            Q[s, a]←Q[s, a] + α(Nsa [s, a])(r + γ maxa Q[s, a] − Q[s, a])
        s, a, r ←s, argmaxa f(Q[s, a],Nsa [s, a]), r 
        return a
    @endcode
*/

// 22. Natural Language Processing

/** Figure 22.1 The HITS algorithm for computing hubs and authorities with respect to a query. RELEVANT-PAGES fetches the pages that match the query, and EXPAND-PAGES adds in every page that links to or is linked from one of the relevant pages. NORMALIZE divides each page’s score by the sum of the squares of all pages’ scores (separately for both the authority and hubs scores).

    @code
    function HITS(query) returns pages with hub and authority numbers
        pages ←EXPAND-PAGES(RELEVANT-PAGES(query))
        for each p in pages do
            p.AUTHORITY←1
            p.HUB←1
        repeat until convergence do
            for each p in pages do
                p.AUTHORITY←i INLINKi(p).HUB
                p.HUB←i OUTLINKi(p).AUTHORITY
            NORMALIZE(pages)
        return pages
    @endcode
*/

// 23. Natural Language for Communication

/** Figure 23.5 The CYK algorithm for parsing. Given a sequence of words, it finds the most probable derivation for the whole sequence and for each subsequence. It returns the whole table, P, in which an entry P[X, start , len] is the probability of the most probable X of length len starting at position start . If there is no X of that size at that location, the probability is 0.

    @code
    function CYK-PARSE(words, grammar ) returns P, a table of probabilities

        N ←LENGTH(words)
        M ←the number of nonterminal symbols in grammar
        P ←an array of size [M, N, N], initially all 0

        // Insert lexical rules for each word

        for i = 1 to N do
            for each rule of form (X → wordsi [p]) do
                P[X, i, 1]←p

        // Combine first and second parts of right-hand sides of rules, from short to long

        for length = 2 to N do
            for start = 1 to N − length + 1 do
                for len1 = 1 to N − 1 do
                len2 ←length − len1
                for each rule of the form (X → Y Z [p]) do
                    P[X, start , length]←MAX(P[X, start , length],
                        P[Y, start , len1 ] × P[Z, start + len1 , len2 ] × p)
        return P
    @endcode
*/

// 25. Robotics

/** Figure 25.9 A Monte Carlo localization algorithm using a range-scan sensor model with independent noise.

    @code
    function MONTE-CARLO-LOCALIZATION(a, z , N, P(X|X, v, ω), P(z|z∗), m) returns a set of samples for the next time step
    inputs: a, robot velocities v and ω
            z, range scan z1, . . . , zM
            P(X|X, v, ω), motion model
            P(z|z∗), range sensor noise model
            m, 2D map of the environment
    persistent: S, a TArray of samples of size N
    local variables: W, a TArray of weights of size N
            S, a temporary TArray of particles of size N
            W, a TArray of weights of size N

    if S is empty then // initialization phase
        for i = 1 to N do
            S[i]←sample from P(X0)
        for i = 1 to N do // update cycle
            S[i]←sample from P(X|X = S[i], v, ω)
            W[i]←1
            for j = 1 to M do
                z∗←RAYCAST(j, X = S[i], m)
                W[i]←W[i] · P(zj | z∗)
        S ←WEIGHTED-SAMPLE-WITH-REPLACEMENT(N,S,W)
    return S
    @endcode
*/

}       // namespace _
#endif  //< KABUKI_AIML_AGENT_H
