# databel_base_R helper functions

externalptr_is_null <- function(x) {
	if (!is(x,"externalptr")) stop("x is not 'externalptr'")
	return(.Call("externalptr_is_null",x))
}

databel_base_R_check <- function(x,force_connected=TRUE) 
{
#	if (class(x) != "databel_base_R") stop("object is not of class 'databel_base_R'")
	if (class(x@data) != "externalptr") stop("data is not of class 'externalptr'");
	if (externalptr_is_null(x@data)) {
		if (force_connected) {
			stop("object is not connected")
		} else {
#			warning("object is not connected\n") #,immediate.=TRUE)
			return(FALSE)
		}
	}
	return(TRUE);
}

get_nobs_nvar_num_log <- function(x,i,j)
{
	databel_base_R_check(x)
#	length_1 <- dim(x)[1]
#	length_2 <- dim(x)[2]
#	if (missing(i)) {i <- c(1:length_1)}
#	if (missing(j)) {j <- c(1:length_2)}

# process i
	nobs <- convert_intlogcha_index_to_int(i,x,1)
# process j
	nvar <- convert_intlogcha_index_to_int(j,x,2)

	return(list(nobs,nvar))
}

