<BODY>

<DIV id="id_1">
<H1> C Interview Notes </H1>
</DIV>
<DIV id="id_2_1">
<P class="p9 ft6"><H2> Macros , Inline function </H2></P>
<P class="p10 ft6">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*  Preprocessor asre just sibstitution patterns applied beofre compiation starts </P>
<P class="p10 ft6">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;* Functions are not preprocessed, instead they are compile </P>
<P class="p10 ft6">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Disadvantages of Macros are :
a) There is no type checking
b) Difficult to debug as they cause simple replacement.
c) Macro don’t have namespace, so a macro in one section of code can affect other section. </P>
<P class="p10 ft6">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Although inline functions are similar to macros (because the function code is expanded at the point of the call at compile time), inline functions are parsed by the compiler, whereas macros are expanded by the preprocessor. As a result, there are several important differences:
  Inline functions follow all the protocols of type safety enforced on normal functions.
  Inline functions are specified using the same syntax as any other function except that they include the inline keyword in the function declaration.
  Expressions passed as arguments to inline functions are evaluated once. In some cases, expressions passed as arguments to macros can be evaluated more than once. </P>
<P class="p10 ft6">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*  Preprocessor asre just sibstitution patterns applied beofre compiation starts </P>

</DIV>
</BODY>
