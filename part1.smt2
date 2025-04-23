(declare-const integer1 Int)
(declare-const integer2 Int)
(declare-const integer3 Int)

(declare-fun TestFunc (Int) Int)
(declare-const res Int)

(assert (= res (TestFunc integer1 )))
