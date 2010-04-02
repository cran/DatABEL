### --- Test setup ---

if(FALSE) {
## Not really needed, but can be handy when writing tests
library("RUnit")
library("DatABEL")
}
#test.empty <- function(){}
### do not run
#stop("SKIP THIS TEST")
###

# common functions and data

data_prefix = "big_data/ERF.merlin.22.collected.ped.out.mldose"
fv_filename <- data_prefix 

databel_base_R_large_file_reads <- function(fv_data)
{
	# collect check data
	varsum_filename <- paste(data_prefix,"_varsum",sep="")
	varsum <- scan(varsum_filename)
	obssum_filename <- paste(data_prefix,"_obssum",sep="")
	obssum <- scan(obssum_filename)
	varnames_filename <- paste(data_prefix,"_varnames",sep="")
	varnames <- scan(varnames_filename,what="character")
	obsnames_filename <- paste(data_prefix,"_obsnames",sep="")
	obsnames <- scan(obsnames_filename,what="character")
	
	
	# check properties are there
	realdim <- c(length(obsnames),length(varnames))
	checkEquals(realdim,dim(fv_data))
	checkEquals(realdim[1]*realdim[2],length(fv_data))
	checkTrue(is.null(dimnames(fv_data)))
	checkEquals(fv_filename,backingfilename(fv_data))
	checkTrue(is.list(get_dimnames(fv_data)))
	checkEquals(2,length(get_dimnames(fv_data)))
	checkTrue(is.integer(cachesizeMb(fv_data)))
	
	# test that names are ok	
	checkEquals(varnames,get_dimnames(fv_data)[[2]])
	checkEquals(obsnames,get_dimnames(fv_data)[[1]])
	
	# test read all	
	accumulated_obssum <- rep(0,dim(fv_data)[1])
	
	for (i in (1:dim(fv_data)[2])) {
		checkEquals(varsum[i],sum(fv_data[,i]),tolerance=5*sqrt(.Machine$double.eps))
		accumulated_obssum <- accumulated_obssum + fv_data[,i]
	}
	checkEquals(obssum,as(accumulated_obssum,"vector"),tolerance=5*sqrt(.Machine$double.eps))

	# as ...
	nCol <- round(runif(1,min=10,max=30))
	nRow <- round(runif(1,min=10,max=30))
	rancols <- sample(1:dim(fv_data)[2],nCol)
	ranrows <- sample(1:dim(fv_data)[1],nRow)
	testmatr <- fv_data[ranrows,rancols]
	checkTrue(is.matrix(testmatr))
	checkTrue(is.numeric(testmatr))
	checkTrue(is.double(testmatr))
	checkEquals(c(nRow,nCol),dim(testmatr))
	
	
}

### --- Test functions ---

test.databel_base_R_large_file_reads <- function()
{
	
	## try to read from non-existing file
	checkException(databel_base_R("no_such_file"))
	checkException(databel_base_R("no_such_file1",128))
	checkException(databel_base_R(c("no_such_file2","aaa")))
	checkException(databel_base_R(127))
	checkException(databel_base_R(TRUE))
	
	# cann I read from base file name, *.fvi, *fvd?
	tmpname1 <- paste(fv_filename,".fvi",sep="")
	print(tmpname1)
	if (file.exists(tmpname1)) {
		fv_data <- databel_base_R(tmpname1)
	}
	tmpname2 <- paste(fv_filename,".fvd",sep="")
	print(tmpname2)
	if (file.exists(tmpname2)) {
		fv_data <- databel_base_R(tmpname2)
	}
	
	if (!(file.exists(tmpname1) && file.exists(tmpname1))) {
		warning(paste("file",tmpname1,"is missing; stopping tests"))
		return(NULL)
	}
	fv_data <- databel_base_R(fv_filename)
	
	# check if nothing works with disconnected
	disconnect(fv_data)
	checkTrue(is.null(disconnect(fv_data)))
	checkException(fv_data[,1])
	checkException(fv_data[1,])
	checkException(dim(fv_data))
	checkException(get_dimnames(fv_data))
	checkException(cachesizeMb(fv_data) <- 12)
	
	# check that some properties are still there
	checkTrue(is.integer(cachesizeMb(fv_data)) && cachesizeMb(fv_data) > 0)
	checkEquals(fv_filename,backingfilename(fv_data))
	
	connect(fv_data)
	checkTrue(is.null(connect(fv_data)))
	
	# test reading with different chache size
	databel_base_R_large_file_reads(fv_data)
	print(fv_data)
	checkException(cachesizeMb(fv_data) <- "aaa")
	checkException(cachesizeMb(fv_data) <- -1)
	cachesizeMb(fv_data) <- 0
	checkTrue(cachesizeMb(fv_data)>0)
	databel_base_R_large_file_reads(fv_data)
	cachesizeMb(fv_data) <- 4
	print(cachesizeMb(fv_data))
	checkEquals(4,cachesizeMb(fv_data))
	cachesizeMb(fv_data) <- 180
	checkEquals(180,cachesizeMb(fv_data))
	cachesizeMb(fv_data) <- 96
	checkEquals(96,cachesizeMb(fv_data))
	databel_base_R_large_file_reads(fv_data)
	
}
