(declare-fun TestFunc (Int Bool) Int)
(declare-const res Int)

(declare-const integer Int)
(declare-const boolean Bool)

(assert (= boolean true))
(assert (= res (TestFunc integer boolean)))

(check-sat)
(get-model)
