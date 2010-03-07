#' converts matrix to 'databel_base_R'
#' 
#' Converts regular R matrix to \code{\linkS4class{databel_base_R-class}} object. 
#' This is the procedure used by "as" converting to DatABEL objects, 
#' in which case a temporary file name is created
#' 
#' @param from R matrix
#' @param filename which FILEVECTOR BASE file name to use
#' @param cachesizeMb cache size to be used when accessing the object
#' @param type type of data to use for storage ("DOUBLE", "FLOAT", "INT", 
#' "UNSIGNED_INT", "UNSIGNED_SHORT_INT", "SHORT_INT")
#' 
#' @return object of class \code{\linkS4class{databel_base_R-class}}
#' 
#' @author Yurii Aulchenko
#' 

matrix2databel_base_R <- function(from, filename, cachesizeMb = 64, type = "DOUBLE") {
# a bit dirty
	make_empty_fvf(as.character(filename),nvar=dim(from)[2],nobs=dim(from)[1],type=type)
	to <- databel_base_R(as.character(filename),as.integer(cachesizeMb))
	to[] <- from
	matrix_dimnames <- dimnames(from)
	if (!is.null(matrix_dimnames)) set_dimnames(to) <- matrix_dimnames
	to
}

