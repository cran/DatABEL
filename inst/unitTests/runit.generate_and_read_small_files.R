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

### ---- common functions and data -----

#source("../inst/unitTests/shared_functions.R")
source(paste(path,"/shared_functions.R",sep=""))

### --- Test functions ---

#
# add NAs to the test
#

test.as.databel_base_R <- function()
{
	
	unlink("tmp*")
	
	testmatr <- make_random_matrix()
	test_fv <- as(testmatr,"databel_base_R")
	checkNumEq(testmatr,test_fv)
	disconnect(test_fv);connect(test_fv)
	checkNumEq(testmatr,test_fv)
	
	test_fv <- matrix2databel_base_R(testmatr,file="tmp",type="DOUBLE")
	checkNumEq(testmatr,test_fv)
	disconnect(test_fv);connect(test_fv)
	checkNumEq(testmatr,test_fv)
	
	# try to ini using the same name
	print(system("ls -al"))
	checkException(test_fv <- matrix2databel_base_R(testmatr,file="tmp",type="DOUBLE"))
	
	rm(test_vf);gc()
	unlink("tmp*")
	
	#define UNSIGNED_SHORT_INT 1
	testmatr <- make_random_matrix(range_data = c(0,255), type="integer")
	test_fv <- as(testmatr,"databel_base_R")
	checkNumEq(testmatr,test_fv)
	
	test_fv <- matrix2databel_base_R(testmatr,file="tmp",type="UNSIGNED_SHORT_INT")
	checkNumEq(testmatr,test_fv)
	
	rm(test_vf,testmatr);gc()
	unlink("tmp*")
	
	#define SHORT_INT          2
	testmatr <- make_random_matrix(range_data = c(-127,127), type="integer")
	test_fv <- as(testmatr,"databel_base_R")
	checkNumEq(testmatr,test_fv)
	
	test_fv <- matrix2databel_base_R(testmatr,file="tmp",type="SHORT_INT")
	checkNumEq(testmatr,test_fv)
	
	rm(test_vf,testmatr);gc()
	unlink("tmp*")
	
	
	#define UNSIGNED_INT       3
	testmatr <- make_random_matrix(range_data = c(0,100000), type="integer")
	test_fv <- as(testmatr,"databel_base_R")
	checkNumEq(testmatr,test_fv)
	
	test_fv <- matrix2databel_base_R(testmatr,file="tmp",type="UNSIGNED_INT")
	checkNumEq(testmatr,test_fv)
	
	rm(test_vf,testmatr);gc()
	unlink("tmp*")
	
	#define INT                4
	testmatr <- make_random_matrix(range_data = c(-100000,100000), type="integer")
	test_fv <- as(testmatr,"databel_base_R")
	checkNumEq(testmatr,test_fv)
	
	test_fv <- matrix2databel_base_R(testmatr,file="tmp",type="INT")
	checkNumEq(testmatr,test_fv)
	
	rm(test_vf,testmatr);gc()
	unlink("tmp*")
	
	#define FLOAT              5
	testmatr <- make_random_matrix(range_data = c(-1e16,1e16), type="double")
	test_fv <- matrix2databel_base_R(testmatr,file="tmp",type="FLOAT")
	checkNumEq(testmatr,test_fv)
	
	rm(test_vf,testmatr);gc()
	unlink("tmp*")
	
	#define DOUBLE             6
	testmatr <- make_random_matrix(range_data = c(-1e16,1e16), type="double")
	test_fv <- as(testmatr,"databel_base_R")
	checkNumEq(testmatr,test_fv)

	test_fv <- matrix2databel_base_R(testmatr,file="tmp",type="DOUBLE")
	checkNumEq(testmatr,test_fv)
	
	rm(test_vf,testmatr);gc()
	unlink("tmp*")
	
}

test.save_as <- function()
{
	testmatr <- make_random_matrix()
#	print(testmatr)
	dba <- matrix2databel_base_R(testmatr,file="tmp")
	dfa <- as(dba,"databel_filtered_R")
#	print(as.matrix(dba))
	checkNumEq(testmatr,dba)
#	print(as(dfa,"matrix"))
	checkNumEq(testmatr,as(dfa,"matrix"))
	
	dims <- dim(testmatr)
	subrows <- sample(1:dims[1],2+floor(runif(1,min=(dims[1]-3)/10,max=(dims[1]-3))))
	subcols <- sample(1:dims[2],2+floor(runif(1,min=(dims[2]-3)/10,max=(dims[2]-3))))
	sub_tm <- testmatr[subrows,subcols]
#	print(subrows)
#	print(subcols)
#	print(sub_tm)
	sub_dba <- save_as(dba,row=subrows,col=subcols,file="tmp_sub")
#	print(as.matrix(sub_dba))
#	print(as.matrix(as(sub_dba,"databel_filtered_R")))
	checkNumEq(sub_tm,sub_dba)
	
	sub_dfa <- save_as(dfa[subrows,subcols],file="tmp_dfa_sub")
#	print(sub_tm)
#	print(as(dfa[subrows,subcols],"matrix"))
#	print(as(sub_dfa,"matrix"))
	checkNumEq(sub_tm,as(sub_dfa,"matrix"))	
	
	sub_dfa <- save_as(dfa,row=subrows,col=subcols,file="tmp_dfa_sub1")
#	print(sub_tm)
#	print(as(dfa[subrows,subcols],"matrix"))
#	print(as(sub_dfa,"matrix"))
	checkNumEq(sub_tm,as(sub_dfa,"matrix"))	
	
	unlink("tmp*")
}

test.write_databel_base_R <- function()
{
###
}
