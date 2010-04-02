#
# databel base R class
# (C) 2009, Yurii Aulchenko, EMCR
#
# This class works with datable_cpp object using direct access 
#
# SLOTS
# bakingfilename
# chachesizeMb
# data
#
# METHODS
# initilize, new
# ![      keeping class
# [<-    keeping class
# dim
# length
# dimnames  -- fake, return NULL
# show
#
# !cbind
# !rbind
#
# is()
# as()
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
#

setClass(
		Class = "databel_base_R",
		representation(
				backingfilename = "character",	
				cachesizeMb = "integer",	
				data = "externalptr" 
		),
		package = "DatABEL"
);

setMethod(
		f = "initialize",
		signature = "databel_base_R",
		definition = function(.Object,filename,cachesizeMb)
		{
			#cat("----- databel_base_R ini start -----\n");
			
			if (missing(filename)) return(.Object);
			
			if (!is(filename,"character") || length(filename) > 1) 
				stop("file name argument is either non-character or non-scalar");
			
			if (!is.numeric(cachesizeMb)) 
				stop("cach size must be positive integer")
			if (cachesizeMb<0) 
				stop("cach size must be positive integer")
			
			.Object@backingfilename = as.character(filename)
			address <- .Call("open_FileMatrix_R",
					fname = as.character(filename), 
					csize = as.integer(cachesizeMb),
					PACKAGE="DatABEL"
			); 
			if (is.null(address)) stop("can not create object, NULL pointer returned")
			.Object@data <- address
			
			.Object@cachesizeMb <- .Call("get_cachesizeMb_R",.Object@data)
			
#			print(c("connected at pointer",.Object@data))			
			#cat("----- databel_base_R ini end -------\n");
			return(.Object)
		}
);

# replace standard methods

setMethod(
		f = "show",
		signature = "databel_base_R",
		definition = function(object)
		{
			connected <- databel_base_R_check(object,force_connected=FALSE)
			cat("backingfilename =",object@backingfilename,"\n")
			cat("cachesizeMb =",object@cachesizeMb,"\n")
			if (!connected) {
				warning("object is not connected\n")
				return()
			}
			cat("number of columns (variables) = ",ncol(object),"\n");
			cat("number of rows (observations) = ",nrow(object),"\n");
			
			toCol <- 10
			toRow <- 5
			if (ncol(object)<toCol) toCol <- ncol(object)
			if (nrow(object)<toRow) toRow <- nrow(object)
			cat("Upper-left",toCol,"columns and ",toRow,"rows:\n")
			
			out <- matrix(NA,ncol=toCol,nrow=toRow);
			tmpout <- rep(0,toCol);
			for (iii in 1:toCol) {
				tmpout <- .Call("read_variable_float_FileMatrix_R",as.integer(iii-1),object@data,PACKAGE="DatABEL")[1:toRow]
				out[,iii] <- tmpout
			}
			print(out);
		}
);

setMethod(
		f = "dim",
		signature = "databel_base_R",
		definition = function(x)
		{
			connected <- databel_base_R_check(x)
			if (connected) {
				return(c(
								.Call("get_nobs_R",x@data,PACKAGE="DatABEL"),
								.Call("get_nvars_R",x@data,PACKAGE="DatABEL")
						)
				)
			} else {
				stop("object is not connected")
			}
		}
);


setMethod(
		f = "length",
		signature = "databel_base_R",
		definition = function(x)
		{
			connected <- databel_base_R_check(x)
			if (connected) {
				dm <- dim(x)
				return(dm[1]*dm[2])
			} else {
				stop("object is not connected!")
			}
		}
);

setMethod(
		f = "dimnames",
		signature = "databel_base_R",
		definition = function(x)
		{
			connected <- databel_base_R_check(x)
			if (!connected) stop("object is not connected")
			return(NULL)
		}
);

#
# caution: may return very large objects! not to be used directly by user!
#
setMethod(
		f = "[",
		signature = "databel_base_R",
		definition = function(x,i,j,drop)
		{
			connected <- databel_base_R_check(x)
			if (!connected) stop("object is not connected")
			if (missing(drop)) drop = FALSE;
			tmp <- get_nobs_nvar_num_log(x,i,j)
			nobs <- tmp[[1]]
			nvar <- tmp[[2]]
			out <- matrix(NA,ncol=length(nvar),nrow=length(nobs));
			tmpout <- rep(0,length(nobs));
			for (iii in 1:length(nvar)) {
				tmpout <- .Call("read_variable_float_FileMatrix_R",as.integer(nvar[iii]-1),x@data,PACKAGE="DatABEL")[nobs]
				out[,iii] <- tmpout
			}
			return(out);
		}
);


setMethod(
		f = "[<-",
		signature = "databel_base_R",
		definition = function(x,i,j,value)
		{
			connected <- databel_base_R_check(x)
			if (!connected) stop("object is not connected")
			tmp <- get_nobs_nvar_num_log(x,i,j)
#			print(c("TMP=",tmp))
			nobs <- tmp[[1]]
			nvar <- tmp[[2]]
			rm(tmp)
# REALLY DIRTY IMPLEMNTATION
			
#			tmp <- as(x,"matrix")
			value <- matrix(value,ncol=length(nvar),nrow=length(nobs))
			if (length(nobs) != dim(value)[1]) stop("dimention of 'i' does not match")
			if (length(nvar) != dim(value)[2]) stop("dimention of 'j' does not match")
#			if (class(try(tmp[nobs,nvar] <- value))=="try-error") stop("ERROR in assignment")
#			print("TMPmatr=")
#			print(tmp)
			kkk <- 1
			for (k in nvar) {
#				print(value)
#				print(k)
				cur_value <- x[,k]
				cur_value[nobs] <- value[,kkk]
#				cat(k," ");flush()
				if (!.Call("write_variable_double_FileMatrix_R",
						as.integer(k-1),
						as.double(cur_value),
#						as.double(tmp[,k]),
						x@data,PACKAGE="DatABEL"))
					stop("can not write variable")
				kkk <- kkk+1
#				ls()
			}
#			cat("\n")
			return(x)
		}
);



#
# new generic methods
#

# take care -- will disconnect all objects based on that one
# only for testing purposes!
setGeneric(
		name = "disconnect",
		def = function(object) {standardGeneric("disconnect");}
);

setMethod(
		f = "disconnect",
		signature = "databel_base_R",
		definition = function(object) 
		{
			connected <- databel_base_R_check(object, force_connected = FALSE)
			if (!connected) {
				warning("object is already disconnected")
				return()
			}
			tmp <- .Call("disconnect_R",object@data,PACKAGE="DatABEL");			
		}
);

#

setGeneric(
		name = "connect",
		def = function(object) {standardGeneric("connect");}
);

setMethod(
		f = "connect",
		signature = "databel_base_R",
		definition = function(object) 
		{
			connected <- databel_base_R_check(object, force_connected = FALSE)
			if (connected) {
				warning("object already connected, nothing done") #,immediate. = TRUE)
				return();
			} else {
				oon <- slotNames(object)
				new_obj <- databel_base_R(filename = object@backingfilename, cachesizeMb = object@cachesizeMb)
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

#

setGeneric(
		name = "backingfilename",
		def = function(object) {standardGeneric("backingfilename");}
);

setMethod(
		f = "backingfilename",
		signature = "databel_base_R",
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
		signature = "databel_base_R",
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
		signature = "databel_base_R",
		definition = function(x,value) 
		{
#			cat("set_cachesizeMb not implemented yet, leaving cachesizeMb unchanged\n")
			connected <- databel_base_R_check(x)
			if (!connected) stop("object is not connected")
			if (!is.numeric(value)) stop("value must be numeric")
			value <- as.integer(value)
			if (value<0) stop("can not set cachesizeMb to <0")
			.Call("set_cachesizeMb_R",x@data,value)
			x@cachesizeMb <- .Call("get_cachesizeMb_R",x@data)
			return(x)
		}
);

#

setGeneric(
		name = "get_dimnames",
		def = function(object) {standardGeneric("get_dimnames");}
);

setMethod(
		f = "get_dimnames",
		signature = "databel_base_R",
		definition = function(object) 
		{
			connected <- databel_base_R_check(object)
			if (connected) {
				return(list(.Call("get_all_obsnames_R",object@data,PACKAGE="DatABEL"),.Call("get_all_varnames_R",object@data,PACKAGE="DatABEL")))
			} else {
				stop("object is not connected",immediate.=TRUE)
			}
		}
);

setGeneric(
		name = "set_dimnames<-",
		def = function(x,value) {standardGeneric("set_dimnames<-");}
);

setMethod(
		f = "set_dimnames<-",
		signature = "databel_base_R",
		definition = function(x,value) 
		{
			connected <- databel_base_R_check(x)
			if (connected) {
				if (!is.list(value)) stop("value is not a list")
				if (length(value)!=2) stop("value should be a list with two vectors")
				if (length(value[[1]]) != dim(x)[1]) stop("dimention 1 of x and lengthof list[[1]] do not match")
				if (length(value[[2]]) != dim(x)[2]) stop("dimention 2 of x and lengthof list[[2]] do not match")
				if (!is.character(value[[1]])) stop("colnames must be characters")
				if (!is.character(value[[2]])) stop("rownames must be characters")
#				print("I AM HERE 0")
				if (!is.null(value[[2]]))
					r1 <- .Call("set_all_varnames_R",x@data,as.character(value[[2]]))
				# for some reason it does not work without PRINTs here
#				print("I AM HERE 1")
				if (!is.null(value[[1]]))
					r2 <- .Call("set_all_obsnames_R",x@data,as.character(value[[1]]))
#				print("I AM HERE 2")
			} else {
				stop("object is not connected",immediate.=TRUE)
			}
			return(x)
		}
);

setGeneric(
		name = "save_as",
		def = function(x,rows,cols,file,cachesizeMb=64) {standardGeneric("save_as");}
);

setMethod(
		f = "save_as",
		signature = "databel_base_R",
		definition = function(x,rows,cols,file,cachesizeMb=64) 
		{
			if (!is.character(file)) stop("file arrgument should be character")
			tmp <- get_nobs_nvar_num_log(x,rows,cols)
			nobs <- tmp[[1]]
			nvar <- tmp[[2]]
			rm(tmp)
			intpar <- as.integer(c(length(nvar),length(nobs),(nvar-1),(nobs-1)))
			if (!
					.Call("save_R",file,intpar,x@data)
					) stop("can not save_as()")
			return(databel_base_R(file,cachesizeMb))
		}
);

# IS

setGeneric('is.databel_base_R', function(x) standardGeneric('is.databel_base_R'))

setMethod('is.databel_base_R', signature(x='databel_base_R'),
		function(x) return(TRUE))

setMethod('is.databel_base_R', definition=function(x) return(FALSE))

# setAs

as.matrix.databel_base_R <- function(x, ... ) 
{
	return(as(x,"matrix"))
}

setAs("databel_base_R","matrix",
		function(from) {
			connected <- databel_base_R_check(from)
			if (!connected) stop("object is not connected")
			dims <- dim(from)
			out <- matrix(NA,ncol=dims[2],nrow=dims[1]);
			tmpout <- rep(0,dims[1]);
			for (iii in 1:dims[2]) {
				tmpout <- .Call("read_variable_float_FileMatrix_R",as.integer(iii-1),from@data,PACKAGE="DatABEL")
				out[,iii] <- tmpout
			}
			return(out);
		}
);

as.vector.databel_base_R <- function(x, mode) 
{
	out <- as(x,"vector")
	if (!missing(mode))
		out <- as(out,mode)
	out
}
setAs("databel_base_R","vector",
		function(from) {
			to <- as(from,"matrix")
			to <- as.vector(to)
			to
		}
);

setAs("matrix","databel_base_R",
		function(from) {
			# need only double
#			if (is.integer(from)) {
#				type <- "INT"
#				if ( all(from>=0) && max(from)<=255) type <- "UNSIGNED_SHORT_INT"
#				if ( all(from>=0) && max(from)>255) type <- "UNSIGNED_INT"
#				if (!all(from>=0) && min(from)>=(-127) && max(from)<=127) type <- "SHORT_INT"
#			} else if (is.double(from)) {
#				type <- "DOUBLE"
#			} else stop("from must be numeric (integer or double)")
# BEGIN all types are converted to DOUBLE			
			if (!is.numeric(from)) stop("from must be numeric (integer or double)")
			type <- "DOUBLE"
# END all types are converted to DOUBLE			
			tmpfilename <- get_temporary_file_name();
			to <- matrix2databel_base_R(from,tmpfilename,64,type)
			cat("coersion from 'matrix' to 'databel_base_R' of type",type,"; object connected to file",tmpfilename,"\n")
			return(to)
		}
);


#setGeneric(
#		name = "as.databel_base_R",
#		def = function(from, ... ) 
#		{standardGeneric("as.databel_base_R");}
#);
#
#setMethod(
#		f = "as.databel_base_R",
#		signature = "matrix",
#		definition = function(from, ...) #type, ... )
#		{
##			if (missing(type))
#			return(as(from,"databel_base_R"))
##			else {
##				tmpfilename <- temporary_filename();
##				to <- matrix2databel_base_R(from,tmpfilename,64,type)
##				cat("coersion from 'matrix' to 'databel_base_R' of type",type,"; object connected to file",tmpfilename,"\n")
##				return(to)
##			}
#		}
#);


