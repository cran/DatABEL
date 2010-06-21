.onLoad <- function(lib, pkg) {
	cat("DatABEL v 0.1-6 (June 21, 2010) loaded\n")
	.Call("checkNumBits");	
}