(declare-const integer1 Int)
(declare-const boolean1 Bool)
(declare-const integer2 Int)

(declare-fun TestFunc (Int Bool Int) Int)
(declare-const res Int)

(assert (= res (TestFunc integer1 boolean1 integer2 )))
