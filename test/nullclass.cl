(*
 *  A contribution from Anne Sheets (sheets@cory)
 *
 *  Tests the arithmetic operations and various other things
 *)

class A {
   method3(num : Int) : C {  -- negate
      (let x : Int in
	 {
            x <- ~num;
	    (new C).set_var(x);
	 }
      )
   }; 
};


