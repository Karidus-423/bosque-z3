(declare-const integer Int)
(declare-const boolean Bool)

(declare-fun TestFunc (Int Bool) Int)
(declare-const res Int)

(assert (= res (TestFunc integer boolean)))
