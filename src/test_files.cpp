#include <stdlib.h>
#include <string>

std::string data_type_bsq = "\
declare namespace Main;\
entity Foo {\
field x: Int;\
field y: Int;\
}\
public function main(f: Foo): Int {\
let k = f.x + f.y;\
assert k > f.x;\
    return k;\
}";

std::string data_type_smt =
    ";; ; ;Template file for building SMTLIB models of Bosque code ; ; "
    "(declare - datatype @Result(par(T)((@Result - err - trgt)( @Result - err "
    "- other)(@Result - ok(@Result - value T))))) ; ; ; ; Primitive datatypes; "
    "; (declare - datatype None((none))); ; Bool is Bool(define - sort Nat() "
    "Int); ; Int is Int(define - sort BigNat() Int)(define - sort BigInt() "
    "Int)( define - sort Float() Real)(define - sort CString() String); ; "
    "String is String ; ; no content--; ; --ENUM_DECLS--; ; ; ; no content--; "
    "; --TYPEDECL_DECLS--; ; ; ; ; ; Entity datatypes; ; (declare - "
    "datatypes(;; no content--;; --SPECIAL_DECLS--;;;; no content--;; "
    "--COLLECTION_DECLS--;; (Main @Foo 0);; no content--;; "
    "--DATATYPE_DECLS--;; (@Term 0))( ;; no content--;; "
    "--SPECIAL_CONSTRUCTORS--;;;; no content--;; --COLLECTION_CONSTRUCTORS--;; "
    "((Main @Foo - mk(Main @Foo - x Int)(Main @Foo - y Int)));; no content--;; "
    "--DATATYPE_CONSTRUCTORS--;; ((@Term - mk - None);; no content--;; "
    "--TYPEDECL_TERM_CONSTRUCTORS--;;;; no content--;; "
    "--SPECIAL_TERM_CONSTRUCTORS--;;;; no content--;; "
    "--ENTITY_TERM_CONSTRUCTORS--;;;; no content--;; "
    "--DATATYPE_TERM_CONSTRUCTORS--;;))) ; ; no content--; ; "
    "--SUBTYPE_PREDICATES--; ; ; ; NLA options(declare - fun @NLA_I_mult(Int "
    "Int) Int)(declare - fun @NLA_I_div(Int Int) Int) (declare - const Int "
    "@zero Int)(declare - const Int @zero - cc - temp(@Result Int))(declare - "
    "const Int @one Int)(declare - const Int @one - cc - temp(@Result Int)) ; "
    "; no content--; ; --PRE_FUNCS--; ; (define - fun Main @main((f Main "
    "@Foo))(@Result Int)(let( (k(+(Main @Foo - x f)(Main @Foo - y "
    "f))))(ite(not(> k(Main @Foo - x f)))( as @Result - err - other(@Result "
    "Int))(@Result - ok k)))) (assert(= Int @zero - cc - temp(@Result - ok "
    "0)))( assert((_ is @Result - ok) Int @zero - cc - temp))( assert(= Int "
    "@zero(@Result - value Int @zero - cc - temp)))( assert(= Int @one - cc - "
    "temp(@Result - ok 1)))( assert((_ is @Result - ok) Int @one - cc - "
    "temp))( assert(= Int @one(@Result - value Int @one - cc - temp))) ; ; ; ; "
    "; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; Above is "
    "SMTLIB code generated from the Bosque Code; ;Below is the setup for "
    "checking for an error -- if we can trigger ; ; then error then the entire "
    "formula is satisfiable and we want get the value; ;for the argument f"
    " (declare - const f Main @Foo)(declare - const res(@Result Int))(assert(= "
    "res(Main @main f))) (assert(= res @Result - err - other)) (check - "
    "sat)(get - model) ";

std::string CString_bsq =
    "declare namespace Main; const s:CString = 'Manchester'; public function "
    "main(s: CString): CString { let t:CString = 'Manchester'; assert s !== t; "
    "return t; } ";

std::string CString_smt =
    " ;; ;;Template file for building SMTLIB models of Bosque code ;; "
    "(declare-datatype @Result (par (T) ( (@Result-err-trgt) "
    "(@Result-err-other) (@Result-ok (@Result-value T))))) (declare-datatypes "
    "( (@EList-2 2) (@EList-3 3) (@EList-4 4)) ( (par (T1 T2) ((@EList-2-mk "
    "(_1 T1) (_2 T2))) ) (par (T1 T2 T3) ((@EList-3-mk (_1 T1) (_2 T2) (_3 "
    "T3))) ) (par (T1 T2 T3 T4) ((@EList-4-mk (_1 T1) (_2 T2) (_3 T3) (_4 "
    "T4))) ))) ;; ;; Primitive datatypes ;; (declare-datatype None ((none))) "
    ";;Bool is Bool (define-sort Nat () Int) ;;Int is Int (define-sort BigNat "
    "() Int) (define-sort BigInt () Int) (define-sort Float () Real) "
    "(define-sort CString () String) ;;String is String ;;no content -- "
    ";;--ENUM_DECLS--;; ;;no content -- ;;--TYPEDECL_DECLS--;; ;; ;; Entity "
    "datatypes ;; (declare-datatypes ( ;;no content -- ;;--SPECIAL_DECLS--;; "
    ";;no content -- ;;--COLLECTION_DECLS--;; ;;no content -- "
    ";;--ENTITY_DECLS--;; ;;no content -- ;;--DATATYPE_DECLS--;; (@Term 0)) ( "
    ";;no content -- ;;--SPECIAL_CONSTRUCTORS--;; ;;no content -- "
    ";;--COLLECTION_CONSTRUCTORS--;; ;;no content -- "
    ";;--ENTITY_CONSTRUCTORS--;; ;;no content -- ;;--DATATYPE_CONSTRUCTORS--;; "
    "( (@Term-mk-None) ;;no content -- ;;--TYPEDECL_TERM_CONSTRUCTORS--;; ;;no "
    "content -- ;;--SPECIAL_TERM_CONSTRUCTORS--;; ;;no content -- "
    ";;--ENTITY_TERM_CONSTRUCTORS--;; ;;no content -- "
    ";;--DATATYPE_TERM_CONSTRUCTORS--;;))) ;;no content -- "
    ";;--SUBTYPE_PREDICATES--;; ;;NLA options (declare-fun @NLA_I_mult (Int "
    "Int) Int) (declare-fun @NLA_I_div (Int Int) Int) (declare-const Main@s "
    "CString) (declare-const Main@s-cc-temp (@Result CString)) ;;no content -- "
    ";;--PRE_FUNCS--;; (define-fun Main@main ((s CString)) (@Result CString) "
    "(let ((t  Manchester"
    ")) (ite (not (not (= s t))) (as @Result-err-other (@Result CString)) "
    "(@Result-ok t)))) (assert (= Main@s-cc-temp (@Result-ok  Manchester"
    "))) (assert (is-@Result-ok Main@s-cc-temp)) (assert (= Main@s "
    "(@Result-value Main@s-cc-temp))) "
    ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; ;;TODO: Make "
    "it String bound to ASCII characters due to CString. (declare-const a_str "
    "String) (declare-const res (@Result String)) (assert (= res (Main@main "
    "a_str))) (assert (= res @Result-err-other)) ";
