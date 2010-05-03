#
# databel  R class
# (C) 2009, 2010, Yurii Aulchenko, EMCR
#
# This class works with datable_cpp object using direct access 
#
# SLOTS
#
# METHODS
# initilize, new
# [      keeping class
# [<-    keeping class
# dim
# length
# dimnames
# show
#
# !cbind
# !rbind
#
# is()
# as("databel","matrix")
# as("databel","vector")
# as("matrix","databel")
#
# backingfilename
# cachesizeMb
# cachesizeMb<-
# !get_data_type
# get_dimnames
# set_dimnames<-
# connect
# disconnect
# save_as
# dimnames<-
#
#

setClass(
		Class = "databel",
		representation = representation(
				usedRowIndex = "integer",	
				usedColIndex = "integer",	
				uninames = "list", 
				backingfilename = "character",	
				cachesizeMb = "integer",	
				data = "externalptr" 
		),
		package = "DatABEL"
);


setMethod(
		f = "initialize",
		signature = "databel",
		definition = function(.Object,baseobject,cachesizeMb=64,readonly=TRUE)
		{
#			cat("----- databel ini start -----\n");
			
			if (!(is(baseobject,"character") || is(baseobject,"databel"))) 
				stop("databel initialize: baseobject should be of character (filename) or databel class");			
			if (!is.numeric(cachesizeMb)) 
				stop("databel initialize: cache size must be numeric")
			readonly <- as(readonly,"logical")
			cachesizeMb <- as(cachesizeMb,"integer")
			if (cachesizeMb<0) 
				stop(paste("databel initialize: cache size must be positive integer; now",cachesizeMb))
			
			if (is(baseobject,"character")){
#				address <- .Call("open_FileMatrix_R",
#						fname = baseobject, 
#						csize = cachesizeMb,
#						rof = readonly,
#						PACKAGE="DatABEL"); 
#				if (is(address,"null")) stop("databel initialize: can not create databel object at step 1, NULL pointer returned")
#				.Object@basedataptr <- address
#				address <-.Call("create_FilteredMatrixFromAbstractMatrix_R", 
#						.Object@basedataptr, PACKAGE="DatABEL");
#				if (is(address,"null")) stop("databel initialize: can not create databel object at step 2, NULL pointer returned")
				address <- .Call("open_FilteredMatrix_R",
						fname = baseobject, 
						csize = cachesizeMb,
						rof = readonly,
						PACKAGE="DatABEL"); 
				if (is(address,"null")) stop("databel initialize: can not create databel object at step 1, NULL pointer returned")
				.Object@data <- address
				
				nrows <- .Call("get_nobs_R",.Object@data,PACKAGE="DatABEL")
				ncols <- .Call("get_nvars_R",.Object@data,PACKAGE="DatABEL")
				.Object@usedRowIndex <- c(1:nrows)
				.Object@usedColIndex <- c(1:ncols)
				
				.Object@uninames <- uninames(.Object@data)
				
				.Object@backingfilename <- baseobject
				.Object@cachesizeMb <- cachesizeMb
				
			} else if (is(baseobject,"databel")) {
				
				for (sn in slotNames(baseobject))
					slot(.Object,sn) <- slot(baseobject,sn)
				
				address <- .Call("open_FilteredMatrix_R",
						fname = backingfilename(baseobject), 
						csize = cachesizeMb,
						rof = readonly,
						PACKAGE="DatABEL"); 
				if (is(address,"null")) stop("databel initialize: can not create databel object at step 2-1, NULL pointer returned")
				.Object@data <- address
				.Object@data <- .Call("setFilteredArea_R",.Object@data,.Object@usedColIndex,.Object@usedRowIndex);				
				
			} else {
				stop("databel initialize: unreachable statement -- baseobject should be of databel class of FV-file name");
			}
			
#			cat("----- databel ini end -------\n");
			
			return(.Object)
		}
);

# replace standard methods

setMethod(
		f = "show",
		signature = "databel",
		definition = function(object)
		{
			connected <- databel_check(object,reconnect=TRUE)
# SHOULD ACTUALLY SHOW ONLY NON-MASKED DATA
			
			cat("uninames$unique.names =",object@uninames$unique.names,"\n")
			cat("uninames$unique.rownames =",object@uninames$unique.rownames,"\n")
			cat("uninames$unique.colnames =",object@uninames$unique.colnames,"\n")
			cat("backingfilename =",object@backingfilename,"\n")
			cat("cachesizeMb =",object@cachesizeMb,"\n")
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
			
			if (!connected) {
				cat("databel show: object is not connected\n")
				return();
			}
			cat("Upper-left",toCol,"columns and ",toRow,"rows:\n")
			showout <- as(object[1:toRow,1:toCol],"matrix")
			print(showout)
		}
);

setMethod(
		f = "dim",
		signature = "databel",
		definition = function(x)
		{
			connected <- databel_check(x);
			if (!connected) stop("databel dim: databel_check failed")
			return(c(length(x@usedRowIndex),length(x@usedColIndex)))
		}
);


setMethod(
		f = "length",
		signature = "databel",
		definition = function(x)
		{
			connected <- databel_check(x)
			if (!connected) stop("databel length: databel_check failed")
			dm <- dim(x)
			return(dm[1]*dm[2])
		}
);



setMethod(
		f = "dimnames",
		signature = "databel",
		definition = function(x)
		{
			connected <- databel_check(x)
			if (!connected) stop("databel dimnames: databel_check failed")
			if (x@uninames$unique.names) {
				return(get_dimnames(x))
			} else if (x@uninames$unique.rownames) {
				return(list(get_dimnames(x)[[1]],NULL))
			} else if (x@uninames$unique.colnames) {
				return(list(NULL,get_dimnames(x)[[2]]))
			} else {
				return(NULL)
			}
		}
);

setMethod(
		f = "dimnames<-",
		signature = "databel",
		definition = function(x,value)
		{
			connected <- databel_check(x)
			if (!connected) stop("databel dimnames<-: databel_check failed")
			if (anyDuplicated(value[[1]])) stop("non-unigue names in dim [[1]] (use set_dimnames?)")
			if (anyDuplicated(value[[2]])) stop("non-unigue names in dim [[2]] (use set_dimnames?)")
			set_dimnames(x) <- value
			x@uninames <- uninames(x@data)
			return(x)
		}
);


setMethod(
		f = "[",
		signature = "databel",
		definition = function(x,i,j,drop)
		{
#			print("[ started")
			connected <- databel_check(x)
			if (!connected) stop("databel [: object is not connected")
			if (missing(drop)) drop = FALSE;
			newi <- convert_intlogcha_index_to_int(i,x,1)
			newj <- convert_intlogcha_index_to_int(j,x,2)
			out <- databel(x)
#			print(c("out dims orig are",dim(out)))
			out@usedRowIndex <- out@usedRowIndex[newi]
			out@usedColIndex <- out@usedColIndex[newj]
			out@data <- .Call("setFilteredArea_R",out@data,out@usedColIndex,out@usedRowIndex);
#			print(c("out dims after are",dim(out)))
			out@uninames <- uninames(out@data)
#			print("[ ended")
			return(out);
		}
);



setMethod(
		f = "[<-",
		signature = "databel",
		definition = function(x,i,j,value)
		{
#			print("started [<-")
			connected <- databel_check(x)
			if (!connected) stop("databel [<-: databel_check failed")
			
			newi <- convert_intlogcha_index_to_int(i,x,1)
			newj <- convert_intlogcha_index_to_int(j,x,2)
			
			if (length(value) != length(newi)*length(newj)) stop("databel [<-: dimensions of i,j,value do not match")
			
			value <- matrix(value,ncol=length(newj),nrow=length(newi))
			
			# this is an expensive way, should re-write for better performance
			kkk <- 1
			for (k in newj) {
				cur_value <- as(x[,k],"vector")
				cur_value[newi] <- value[,kkk]
#				print(c(k,":",cur_value))
				if (!.Call("write_variable_double_FileMatrix_R",
						as.integer(k),
						as.double(cur_value),
						x@data,PACKAGE="DatABEL"))
					stop("databel [<-: can not write variable")
				kkk <- kkk + 1
			}
#			print("finished [<-")
			return(x)
		}
);


# IS

setGeneric('is.databel', function(x) standardGeneric('is.databel'))

setMethod('is.databel', signature(x='databel'),
		function(x) return(TRUE))

setMethod('is.databel', definition=function(x) return(FALSE))

# setAs

as.matrix.databel <- function(x, ... ) 
{
	return(as(x,"matrix"))
}

as.vector.databel <- function(x, ... ) 
{
	return(as(x,"vector"))
}

setAs("databel","vector",
		function(from) {
			to <- as(from,"matrix")
			return(as(to,"vector"))
		}
);

setAs("databel","matrix",
		function(from) {
			connected <- databel_check(from)
			if (!connected) stop("setAs('databel','matrix'): check_connected failed")
			dims <- dim(from)
#			print(c(dims,class(from),from@usedColIndex,from@data,from@usedRowIndex))
			to <- matrix(NA,ncol=dims[2],nrow=dims[1]);
			tmpout <- rep(0,dims[1]);
			for (iii in 1:dims[2]) {
				tmpout <- .Call("read_variable_double_FileMatrix_R",as.integer(iii),from@data,PACKAGE="DatABEL")
#				print(c(iii,tmpout))
				to[,iii] <- tmpout
			}
			dimnames(to) <- dimnames(from)
			return(to)
		}
);


setAs("matrix","databel",
		function(from) {
			if (!is.numeric(from)) stop("from must be numeric (integer or double)")
			type <- "DOUBLE"
			tmpfilename <- get_temporary_file_name();
			to <- matrix2databel(from,filename=tmpfilename,cachesizeMb=64,type=type)
			cat("coersion from 'matrix' to 'databel' of type",type,"; object connected to file",tmpfilename,"\n")
			return(to)
		}
);



### new generics

setGeneric(
		name = "get_dimnames",
		def = function(object) {standardGeneric("get_dimnames");}
);

setMethod(
		f = "get_dimnames",
		signature = "databel",
		definition = function(object) 
		{
			connected <- databel_check(object)
			if (!connected) stop("object is not connected",immediate.=TRUE)
			return(list(.Call("get_all_obsnames_R",object@data,PACKAGE="DatABEL"),.Call("get_all_varnames_R",object@data,PACKAGE="DatABEL")))
		}
);

setGeneric(
		name = "set_dimnames<-",
		def = function(x,value) {standardGeneric("set_dimnames<-");}
);


setMethod(
		f = "set_dimnames<-",
		signature = "databel",
		definition = function(x,value)
		{
			
			connected <- databel_check(x);
			if (!connected) stop("set_dimnames<-: databel_check failed")
			
			if (!is.list(value)) stop("set_dimnames<-: value is not a list")
			if (length(value)!=2) stop("set_dimnames<-: value should be a list with two vectors")
			
			if (length(value[[1]]) != dim(x)[1]) {
				if (is.null(value[[1]])) {
					value[[1]] <- as.character(c(1:dim(x)[1]))
				} else {
					stop("set_dimnames<-: dimention 1 of x and lengthof list[[1]] do not match")
				}
			}
			if (length(value[[2]]) != dim(x)[2]) {
				if (is.null(value[[2]])) {
					value[[2]] <- as.character(c(1:dim(x)[2]))
				} else {
					stop("set_dimnames<-: dimention 2 of x and lengthof list[[2]] do not match")
				}
			} 
				
			if (!is.character(value[[1]])) stop("set_dimnames<-: colnames must be characters")
			if (!is.character(value[[2]])) stop("set_dimnames<-: rownames must be characters")
			if (!is.null(value[[2]]))
				r1 <- .Call("set_all_varnames_R",x@data,as.character(value[[2]]))
			if (!is.null(value[[1]]))
				r2 <- .Call("set_all_obsnames_R",x@data,as.character(value[[1]]))
			
			x@uninames <- uninames(x@data)
			
#			if (length(unique(value[[1]]))==dim(x)[1]) 
#				x@uninames$unique.rownames <- TRUE else x@uninames$unique.rownames <- FALSE
#			if (length(unique(value[[2]]))==dim(x)[2]) 
#				x@uninames$unique.colnames <- TRUE else x@uninames$unique.colnames <- FALSE
#			if (x@uninames$unique.colnames && x@uninames$unique.rownames) 
#				x@uninames$unique.names <- TRUE else x@uninames$unique.names <- FALSE
			
			return(x)
		}
);

setGeneric(
		name = "backingfilename",
		def = function(object) {standardGeneric("backingfilename");}
);

setMethod(
		f = "backingfilename",
		signature = "databel",
		definition = function(object) 
		{
			return(object@backingfilename)
		}
);

#

setGeneric(
		name = "cachesizeMb",
		def = function(object) {standardGeneric("cachesizeMb");}
);

setMethod(
		f = "cachesizeMb",
		signature = "databel",
		definition = function(object) 
		{
			return(object@cachesizeMb)
		}
);

#

setGeneric(
		name = "cachesizeMb<-",
		def = function(x,value) {standardGeneric("cachesizeMb<-");}
);

setMethod(
		f = "cachesizeMb<-",
		signature = "databel",
		definition = function(x,value) 
		{
#			cat("set_cachesizeMb not implemented yet, leaving cachesizeMb unchanged\n")
			connected <- databel_check(x)
			if (!connected) stop("cachesizeMb<-: databel_check failed")
			if (!is.numeric(value)) stop("value must be numeric")
			value <- as.integer(value)
			if (value<0) stop("can not set cachesizeMb to <0")
			.Call("set_cachesizeMb_R",x@data,value)
			x@cachesizeMb <- .Call("get_cachesizeMb_R",x@data)
			return(x)
		}
);

setGeneric(
		name = "save_as",
		def = function(x,rows,cols,file,type="databel",cachesizeMb=64,readonly=TRUE) {standardGeneric("save_as");}
);


setMethod(
		f = "save_as",
		signature = "databel",
		definition = function(x,rows,cols,file,cachesizeMb=64,readonly=TRUE) 
		{
			#allowd_types <- c("databel","text")
			if (!is.character(file)) stop("databel save_as: file argument should be character")
			if (!missing(rows)) {
				newi <- convert_intlogcha_index_to_int(rows,x,1)
			} else {
				newi <- 1:dim(x)[1]
			}
			if (!missing(cols)) {
				newj <- convert_intlogcha_index_to_int(cols,x,2)
			} else {
				newj <- 1:dim(x)[2]
			}
			
			### check order!!!
#			print(newj)
#			print(newi)
			intpar <- as.integer(c(length(newj),length(newi),(newj-1),(newi-1)))
#			print(intpar)
			if (!.Call("save_R",file,intpar,x@data)) 
				stop("can not save_as(): save_R failed")
			newobj <- databel(file,cachesizeMb=cachesizeMb,readonly=readonly)
			return(newobj)
		}
);

setGeneric(
		name = "connect",
		def = function(object,readonly=TRUE) {standardGeneric("connect");}
);

setMethod(
		f = "connect",
		signature = "databel",
		definition = function(object,readonly=TRUE) 
		{
#			print("connect for databel started")
			connected <- databel_check(object, reconnect = FALSE, stop_on_error = FALSE, quiet = TRUE)
			if (connected) {
#				print("connect for databel finished")
				warning("object already connected, nothing done") #,immediate. = TRUE)
				return();
			} else {
				new_obj <- databel(backingfilename(object), cachesizeMb=cachesizeMb(object),readonly=readonly)
				new_obj@usedColIndex <- object@usedColIndex
				new_obj@usedRowIndex <- object@usedRowIndex
				res <- .Call("setFilteredArea_R",new_obj@data,object@usedColIndex,object@usedRowIndex);
#				print("after setFilteredArea_R in connect")
#				print("before eval.parent(sub...")
				eval.parent(substitute(object <- new_obj));
#				print("after eval.parent(sub...")
			}
#			print("connect for databel finished")
		}
);

setGeneric(
		name = "disconnect",
		def = function(object) {standardGeneric("disconnect");}
);



setMethod(
		f = "disconnect",
		signature = "databel",
		definition = function(object)
		{
			connected <- databel_check(object, reconnect = FALSE, stop_on_error = FALSE)
			if (!connected) {
				warning("object is already disconnected")
				return()
			}
			tmp <- .Call("disconnectFiltered_R",object@data,PACKAGE="DatABEL");
		}
);

setGeneric(
		name = "setReadOnly<-",
		def = function(x,value) {standardGeneric("setReadOnly<-");}
);


setMethod(
		f = "setReadOnly<-",
		signature = "databel",
		definition = function(x,value)
		{
			connected <- databel_check(x)
			if (!connected) stop("databel setReadOnly: object is not connected")
			res <- .Call("setReadOnly_R",x@data,as(value,"logical"),PACKAGE="DatABEL");
			if (!res) stop(paste("databel setReadOnly: can not set ReadOnly flag to",as(value,"logical")))
		}
);


