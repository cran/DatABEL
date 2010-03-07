#' initiates databel_filtered_R object
#'
#' initiates databel_filtered_R object
#'
#' @param file_or_baseobject name of the file or \link{databel_base_R-class} object
#' @param cachesizeMb cache size (amount of RAM) to be used
#' 
#' @author Yurii Aulchenko
#'

databel_filtered_R <- function(file_or_baseobject,cachesizeMb)
{
	if (is.character(file_or_baseobject)) 
	{
		if (missing(cachesizeMb)) cachesizeMb <- 64
		baseobject <- databel_base_R(file_or_baseobject,cachesizeMb)
		return(new(Class="databel_filtered_R",baseobject))
	} else if (class(file_or_baseobject) == "databel_base_R") {	
		return(new(Class="databel_filtered_R",file_or_baseobject))
	} else {
		stop("first argument should provide 'databel_base_R' object or filevector file name")
	}
}
