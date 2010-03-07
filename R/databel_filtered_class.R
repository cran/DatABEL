#
# this is datable_filtered_R, based on databel_base_R
#
# need to add
# dimnames() <- value
# save_as()
# !rbind/cbind
#
#

setClass(
		Class = "databel_filtered_R",
		representation = representation(
				usedRowIndex = "integer",	
				usedColIndex = "integer",	
				unique.names = "logical",		# NULL if not used
				unique.colnames = "logical",		# NULL if not used
				unique.rownames = "logical"		# NULL if not used
#				"databel_base_R" 
#				base_data = "matrix" 
		),
		contains = "databel_base_R",
		package = "DatABEL"
);


setMethod(
		f = "initialize",
		signature = "databel_filtered_R",
		definition = function(.Object,baseobject)
		{
			#cat("----- databel_filtered_R ini start -----\n");
			
			if (!(is(baseobject,"databel_base_R") || is(baseobject,"databel_filtered_R"))) 
				stop("baseobject should be of databel_base_R or databel_filtered_R class");
			for (sn in slotNames(baseobject))
				slot(.Object,sn) <- slot(baseobject,sn)

			.Object@usedRowIndex <- c(1:(dim(baseobject)[1]))
			.Object@usedColIndex <- c(1:(dim(baseobject)[2]))
			
			.Object@unique.names <- .Object@unique.colnames <- .Object@unique.rownames <- FALSE
			
			colnames <- .Call("get_all_varnames_R",.Object@data)
			rownames <- .Call("get_all_obsnames_R",.Object@data)
			if (length(unique(colnames)) == ncol(.Object)) {
				.Object@unique.colnames <- TRUE
			} else {
				warning("some column names are not unique")
			}
			if (length(unique(rownames)) == nrow(.Object)) {
				.Object@unique.rownames <- TRUE
			} else {
				warning("some row names are not unique")
			}
			.Object@unique.names <- (.Object@unique.colnames && .Object@unique.rownames)
			
			#cat("----- databel_filtered_R ini end -------\n");
			
			return(.Object)
		}
);

# replace standard methods

setMethod(
		f = "show",
		signature = "databel_filtered_R",
		definition = function(object)
		{
			connected <- databel_filtered_R_check(object,force_connected=FALSE)
# SHOULD ACTUALLY SHOW ONLY NON-MASKED DATA
			
			cat("unique.names =",object@unique.names,"\n")
			cat("unique.rownames =",object@unique.rownames,"\n")
			cat("unique.colnames =",object@unique.colnames,"\n")
			cat("backingfilename =",object@backingfilename,"\n")
			cat("cachesizeMb =",object@cachesizeMb,"\n")
			if (!connected) {
				cat("object is not connected\n")
				return();
			}
			cat("number of columns (variables) = ",ncol(object),"\n");
			cat("number of rows (observations) = ",nrow(object),"\n");
			toCol <- 10
			toRow <- 5
			if (ncol(object)<toCol) toCol <- ncol(object)
			if (nrow(object)<toRow) toRow <- nrow(object)
			cat("usedRowIndex: ")
			for (i in 1:toRow) cat(object@usedRowIndex[i]," ")
			if (toRow<dim(object)[1]) cat("...")
			cat("\n")
			cat("usedColIndex: ")
			for (i in 1:toCol) cat(object@usedColIndex[i]," ")
			if (toCol<dim(object)[2]) cat("...")
			cat("\n")
			cat("Upper-left",toCol,"columns and ",toRow,"rows:\n")
#			print(object@usedRowIndex[1:toRow])
#			print(object@usedColIndex[1:toCol])
			class(object) <- "databel_base_R"
			show(object[object@usedRowIndex[1:toRow],object@usedColIndex[1:toCol]])
		}
);

setMethod(
		f = "dim",
		signature = "databel_filtered_R",
		definition = function(x)
		{
			connected <- databel_filtered_R_check(x)
			if (connected) {
				return(c(length(x@usedRowIndex),length(x@usedColIndex)))
			} else {
				return(NULL)
			}
		}
);

setMethod(
		f = "length",
		signature = "databel_filtered_R",
		definition = function(x)
		{
			connected <- databel_filtered_R_check(x)
			if (connected) {
				dm <- dim(x)
				return(dm[1]*dm[2])
			} else {
				return(NULL)
			}
		}
);


setMethod(
		f = "dimnames",
		signature = "databel_filtered_R",
		definition = function(x)
		{
			databel_filtered_R_check(x)
			if (!x@unique.rownames && !x@unique.colnames) return(NULL)
			out <- list(NULL,NULL)
			if (x@unique.rownames) out[[1]] <- .Call("get_all_obsnames_R",x@data)[x@usedRowIndex]
			if (x@unique.colnames) out[[2]] <- .Call("get_all_varnames_R",x@data)[x@usedColIndex]
			return(out)
		}
);

setMethod(
		f = "dimnames<-",
		signature = "databel_filtered_R",
		definition = function(x,value)
		{
			class(x) <- "databel_base_R"
			set_dimnames(x) <- value 
			class(x) <- "databel_filtered_R"
			return(x)
		}
);


setMethod(
		f = "[",
		signature = "databel_filtered_R",
		definition = function(x,i,j,drop)
		{
			if (missing(drop)) drop = FALSE;
#			if (!missing(i)) if (class(i) == "character") 
			newi <- convert_intlogcha_index_to_int(i,x,1)
			newj <- convert_intlogcha_index_to_int(j,x,2)
			out <- x
			out@usedRowIndex <- x@usedRowIndex[newi]
			out@usedColIndex <- x@usedColIndex[newj]
			return(out);
		}
);


setMethod(
		f = "[<-",
		signature = "databel_filtered_R",
		definition = function(x,i,j,value)
		{
			newi <- convert_intlogcha_index_to_int(i,x,1)
			newj <- convert_intlogcha_index_to_int(j,x,2)
			base_i <- x@usedRowIndex[newi]
			base_j <- x@usedColIndex[newj]
			
			#if (!missing(i)) print(c("in dfR [<-, i",i))
			#if (!missing(j)) print(c("in dfR [<-, j",j))
			#print(c("in dfR [<-, newi",newi))
			#print(c("in dfR [<-, newj",newj))
			#print(c("in dfR [<-, base_i",base_i))
			#print(c("in dfR [<-, base_j",base_j))
			
			class(x) <- "databel_base_R"
			x[base_i,base_j] <- value 
			class(x) <- "databel_filtered_R"

			return(x)
		}
);


# IS

setGeneric('is.databel_filtered_R', function(x) standardGeneric('is.databel_filtered_R'))

setMethod('is.databel_filtered_R', signature(x='databel_filtered_R'),
		function(x) return(TRUE))

setMethod('is.databel_filtered_R', definition=function(x) return(FALSE))

# setAs

as.matrix.databel_filtered_R <- function(x, ... ) 
{
	return(as(x,"matrix"))
}

as.vector.databel_filtered_R <- function(x, ... ) 
{
	return(as(x,"vector"))
}

setAs("databel_filtered_R","vector",
        function(from) {
            to <- as(from,"matrix")
            return(as(to,"vector"))
        }
);

setAs("databel_filtered_R","matrix",
		function(from) {
			databel_filtered_R_check(from)

			if (!from@unique.rownames && !from@unique.colnames) dmn <- NULL else dmn <- list(NULL,NULL)
			if (from@unique.rownames) dmn[[1]] <- .Call("get_all_obsnames_R",from@data)[from@usedRowIndex]
			if (from@unique.colnames) dmn[[2]] <- .Call("get_all_varnames_R",from@data)[from@usedColIndex]
			
			class(from) <- "databel_base_R"
			to <- as(from[from@usedRowIndex,from@usedColIndex],"matrix")
			dimnames(to) <- dmn
			return(to)
		}
);

setAs("databel_filtered_R","databel_base_R",
		function(from) {
#			connected <- databel_filtered_R_check(from)
#			if (!connected) stop("object not connected")
#			to <- databel_base_R(backingfilename(from))
			to <- from
			class(to) <- "databel_base_R"
			return(to)
		}
);


setAs("matrix","databel_filtered_R",
		function(from) {
			to <- as(from,"databel_base_R")
			to <- as(to,"databel_filtered_R")
			return(to)
		}
);

setAs("databel_base_R","databel_filtered_R",
		function(from) {
			connected <- databel_filtered_R_check(from)
			if (!connected) stop("object not connected")
			to <- databel_filtered_R(from)
			return(to)
		}
);



### new generics

#setGeneric(
#		name = "save_as",
#		def = function(x,rows,cols,file,cachesizeMb) {standardGeneric("save_as");}
#);

setMethod(
		f = "save_as",
		signature = "databel_filtered_R",
		definition = function(x,rows,cols,file,cachesizeMb=64) 
		{
			if (!is.character(file)) stop("file arrgument should be character")
			newi <- convert_intlogcha_index_to_int(rows,x,1)
			newj <- convert_intlogcha_index_to_int(cols,x,2)
			nobs <- x@usedRowIndex[newi]
			nvar <- x@usedColIndex[newj]
#			intpar <- as.integer(c(length(nvar),length(nobs),nvar,nobs))
#			res <- .Call("save_R",file,intpar,x@data)
#			if (!res) stop("can not save_as()")
			newobj <- save_as(as(x,"databel_base_R"),nobs,nvar,file,cachesizeMb=cachesizeMb)
			return(databel_filtered_R(newobj,cachesizeMb))
		}
);

#
# a method from databel_base_R should work, but does not!

setMethod(
		f = "connect",
		signature = "databel_filtered_R",
		definition = function(object) 
		{
			connected <- databel_filtered_R_check(object, force_connected = FALSE)
			if (connected) {
				warning("object already connected, nothing done") #,immediate. = TRUE)
				return();
			} else {
				oon <- slotNames(object)
				new_obj <- databel_filtered_R(backingfilename(object), cachesizeMb(object))
				non <- slotNames(new_obj)
				missnames <- oon[!(oon %in% non)]
				new_obj <- as(new_obj,class(object))
				for (repn in missnames) {
					slot(new_obj,repn) <- slot(object,repn)
				}
				eval.parent(substitute(object <- new_obj));
			}
		}
);
