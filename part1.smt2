(declare-const integer1 Int)
(declare-const boolean1 Bool)
(declare-const bollean2 Bool)

(declare-fun TestFunc (Bool Bool Int) Int)
(declare-const res Int)

(assert (= res (TestFunc boolean1 bollean2 integer1 )))
