#!/usr/bin/awk -f
{ 
	if( $3 ~ /^[GHIJK]/ &&  $7 ~ /^[234567]/  ) 
		print $4, $7; 
} 
