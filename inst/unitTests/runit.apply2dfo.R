### --- Test setup ---

if(FALSE) {
	## Not really needed, but can be handy when writing tests
	library("RUnit")
	library("DatABEL")
}

test.empty <- function(){}
### do not run
#stop("SKIP THIS TEST")
###

### ---- common functions and data -----

source("../inst/unitTests/shared_functions.R")

### --- Test functions ---

test.apply2dfo <- function()
{
	testmatr <- make_random_matrix(range_data = c(-10,10))
	dfo <- as(testmatr,"databel_filtered_R")

	res0 <- apply(testmatr,MAR=2,FUN=sum)
	print("res0")
	print(res0)
	pfun <- function(a) return(a)
	res1 <- apply2dfo(SNP,anFUN="sum",MAR=2,procFUN="pfun",dfodata=dfo,transpose=F)
	print(res1)
	res2 <- apply2dfo(SNP,anFUN="sum",MAR=2,procFUN="pfun",dfodata=dfo,transpose=F,outclass="databel_filtered_R",outfile="tmp")
	print(res2)
	res3 <- apply2dfo(SNP,anFUN="sum",MAR=2,procFUN="pfun",dfodata=dfo,transpose=T)
	print(res3)
	res4 <- apply2dfo(SNP,anFUN="sum",MAR=2,procFUN="pfun",dfodata=dfo,transpose=T,outclass="databel_filtered_R",outfile="tmp1")
	print(res4)
	checkEqualsNumeric(as(res0,"vector"),as(res1,"vector"),tol=5*sqrt(.Machine$double.eps))
	checkEqualsNumeric(as(res0,"vector"),as(res2,"vector"),tol=5*sqrt(.Machine$double.eps))
	checkEqualsNumeric(as(res0,"vector"),as(res3,"vector"),tol=5*sqrt(.Machine$double.eps))
	checkEqualsNumeric(as(res0,"vector"),as(res4,"vector"),tol=5*sqrt(.Machine$double.eps))
	unlink("tmp*")
	
	res0 <- apply(testmatr,MAR=1,FUN=sum)
	print("res0-1")
	print(res0)
	pfun <- function(a) return(a)
	res1 <- apply2dfo(SNP,anFUN="sum",MAR=1,procFUN="pfun",dfodata=dfo,transpose=F)
	print(res1)
	res2 <- apply2dfo(SNP,anFUN="sum",MAR=1,procFUN="pfun",dfodata=dfo,transpose=F,outclass="databel_filtered_R",outfile="tmp")
	print(res2)
	res3 <- apply2dfo(SNP,anFUN="sum",MAR=1,procFUN="pfun",dfodata=dfo,transpose=T)
	print(res3)
	res4 <- apply2dfo(SNP,anFUN="sum",MAR=1,procFUN="pfun",dfodata=dfo,transpose=T,outclass="databel_filtered_R",outfile="tmp1")
	print(res4)
	checkEqualsNumeric(as(res0,"vector"),as(res1,"vector"),tol=5*sqrt(.Machine$double.eps))
	checkEqualsNumeric(as(res0,"vector"),as(res2,"vector"),tol=5*sqrt(.Machine$double.eps))
	checkEqualsNumeric(as(res0,"vector"),as(res3,"vector"),tol=5*sqrt(.Machine$double.eps))
	checkEqualsNumeric(as(res0,"vector"),as(res4,"vector"),tol=5*sqrt(.Machine$double.eps))
	unlink("tmp*")

}