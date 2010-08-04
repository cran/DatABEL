.onLoad <- function(lib, pkg) {
	cat("DatABEL v 0.9-0 (August 04, 2010) loaded\n")
	.Call("checkNumBits");	
}