#' initiates databel_base_R object
#' 
#' initiates databel_base_R object
#'
#' @param filename name of the file containing the data
#' @param cachesizeMb what cache size (size of RAM) to be used
#' 
#' @author Yurii Aulchenko
#'

databel_base_R <- function(filename,cachesizeMb=64)
{
	new(Class="databel_base_R",filename=filename,cachesizeMb=cachesizeMb);
}

