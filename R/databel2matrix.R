#' converts 'databel' to matrix
#' 
#' Converts regular R matrix to \code{\linkS4class{databel}} object. 
#' This is the procedure used by "as" converting to DatABEL objects, 
#' in which case a temporary file name is created
#' 
#' @param from 'databel' matrix
#' @param rows which rows to include
#' @param cols which columns to include
#' 
#' @return object of \code{\linkS4class{matrix}} class
#' 
#' @author Stepan Yakovenko
#' 

databel2matrix <- function(from, rows, cols) {
    newi <- convert_intlogcha_index_to_int(rows,from,1)
 	newj <- convert_intlogcha_index_to_int(cols,from,2)
    ret <- matrix(1.2345678912345, length(newi),length(newj));

    if(!.Call("assignDoubleMatrix", from@data, newi, newj, ret, as.integer(1)))
	    stop("databel [<-: can't write variable.");

    dimnames(ret) <- dimnames(from)
	return(ret)
}

