: IMMEDIATE last_word @ cfa 1 - 1 swap c! ;

: if ' branch0 , here 0  , ; IMMEDIATE
: else ' branch , here 0 , swap here swap ! ; IMMEDIATE
: then here swap ! ; IMMEDIATE

: rot >r swap r> swap ;
: 2dup dup >r swap dup >r swap r> r> ;
: > 2dup = rot rot < lor not ;

: repeat here ; IMMEDIATE
: until ' branch0 , , ; IMMEDIATE

: ( repeat read_char 41 - not until ; IMMEDIATE

: for
    ' swap , ' >r , ' >r ,
    here ' r> , ' r> , ' 2dup , 
    ' >r , ' >r , ' < , ' branch0 ,
    here 0 , swap
; IMMEDIATE

: endfor
    ' r> , ' lit , 1 , ' + , ' >r ,
    ' branch , , here swap ! ' r> ,
    ' drop , ' r> , ' drop ,
; IMMEDIATE
