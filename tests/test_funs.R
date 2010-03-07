#
# internal test functions
#


test_ID_and_DIFF <- function(data,object)
{
	precis <- 1e-7
	if (!identical(data,object)) {
		cat("data and objects are NOT identical\n")
		diff <- try(any(abs((data-object)/data)>precis))
		if (class(diff) == "try-error") {
			cat("can not generate difference\n")
			return(FALSE)
		}
		if (!diff) {
			cat("abs difference is <=",precis,"\n")
			return(TRUE)
		} else {
			cat("abs difference is >",precis,"\n")
			return(FALSE)
		}
		stop("reached unreachable statement!")
	} else {
		cat("data and object are identical\n")
		return(TRUE);
	} 
	stop("reached unreachable statement!")
}
testobject <- function(data,object,FUN,...) {
	if (missing(FUN)) {	
		out <- test_ID_and_DIFF(data,object)
		return(out)
	} else {
		out_data <- try(FUN(data, ... ))
		if (class(out_data) == "try-error") {
			cat("can not apply FUN to data\n")
			return(FALSE)
		}
		out_object <- try(FUN(object, ... ))
		if (class(out_object) == "try-error") {
			cat("can not apply FUN to object\n")
			return(FALSE)
		}
		out <- test_ID_and_DIFF(out_data,out_object)
		return(out)
	}
	stop("reached unreachable statement!")
}
