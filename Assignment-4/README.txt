--ADDURI SAI SRI DATTA
-- 190101003

Summary :
I have give two input files
		- one with correct input named input.p
		- other with error input named input_error.p.
		
One will run successfully while other gives u a error message.
Run instructions : 

Run make clean  
>> make 

Run make  
>> make 

Run parser on input file with Errors
>> ./parser input_error.p

Run parser with input file 
>> ./parser input.p

Errors handles : 
-- Syntactical Errors
-- Redeclaration of variable
-- Redefinition of variable
-- Missing declaration of variable
-- Using variable without initialisation-- Type mismatch(assigning real to integer and vice versa)
-- Using keyword as identifier
-- Unidentified character in input
-- For loop has same type in range of index
