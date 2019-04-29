( n -- 0/1 )
: is_even
    2 % not ;

( n -- 0/1 )
: is_prime
	dup 2 <	
	if ( if n less than 2 )
		drop 0 
	else
		dup 2 = 
		if ( if n is 2 ) 
			drop 1 
		else 
			2 >r dup repeat
				drop
				( n ) 
				dup r@ %
				( n 0/1... )
				0 = 
				if 
					0 
				else
					( n )
					dup r@
					( n n i )
					dup *
					( n n i^2)
					( 1 - prime, -1 - next iter )
					< if 1 else -1 then		
					( n 1/-1)
					r> 1 + >r	
				then
				dup -1 = not until
		( n res )
		r> drop swap drop
		then
	then ;

( n -- addr )
: save
	cell%
	allot
	( n addr )	
	dup >r ! r> ;

( addr1, addr2 -- addr3 )	
: concat
	dup count rot
	( addr2 len2 addr1 )
	dup count rot
	( addr2 addr1 len1 len2 )
	swap dup rot
	( addr2 addr1 len1 len1 len2 )
	+ heap-alloc
	( addr2 addr1 len1 addr3 )
	>r swap r> 
	( addr2 len1 addr1 addr3 )
	dup rot
	( addr2 len1 addr3 addr3 addr1 )
	dup >r string-copy r>
	( addr2 len1 addr3 addr1)
	heap-free dup
	( addr2 len1 addr3 addr3 )
	rot +
	( addr2 addr3 addr3+offset )
	rot dup >r string-copy r>
	( addr3 addr2 )
	heap-free ; 
		 	
